/*
 * Microsemi Switchtec(tm) PCIe Management Library
 * Copyright (c) 2019, Microsemi Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

/**
 * @file
 * @brief Switchtec core library functions for mfg operations
 */

/**
 * @defgroup mfg Manufacturing Functions
 * @brief Manufacturing-related API functions
 *
 * These are functions used during manufacturing process. These
 * includes functions that configure device security settings and
 * recover device from boot failures.
 *
 * Some of these functions modify device One-Time-Programming (OTP) memory,
 * so they should be used with great caution, and you should really
 * know what you are doing when calling these functions. FAILURE TO DO SO
 * COULD MAKE YOUR DEVICE UNBOOTABLE!!
 *
 * @{
 */

#include "switchtec_priv.h"
#include "switchtec/switchtec.h"
#include "switchtec/mfg.h"
#include "switchtec/errors.h"
#include "switchtec/endian.h"
#include "switchtec/mrpc.h"
#include "switchtec/errors.h"
#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#ifndef _WIN32
#include <arpa/inet.h>
#endif
#include <iconv.h>

#include "lib/crc.h"
#include "config.h"

#ifdef __linux__

#if HAVE_LIBCRYPTO
#include <openssl/pem.h>
#endif
#define SWITCHTEC_ACTV_IMG_ID_KMAN		1
#define SWITCHTEC_ACTV_IMG_ID_BL2		2
#define SWITCHTEC_ACTV_IMG_ID_CFG		3
#define SWITCHTEC_ACTV_IMG_ID_FW		4

#define SWITCHTEC_MB_MAX_ENTRIES		16
#define SWITCHTEC_ACTV_IDX_MAX_ENTRIES		32
#define SWITCHTEC_ACTV_IDX_SET_ENTRIES		4

#define SWITCHTEC_CLK_RATE_BITSHIFT		2
#define SWITCHTEC_CLK_RATE_BITMASK		0x0f
#define SWITCHTEC_RC_TMO_BITSHIFT		6
#define SWITCHTEC_RC_TMO_BITMASK		0x0f
#define SWITCHTEC_I2C_PORT_BITSHIFT		10
#define SWITCHTEC_I2C_PORT_BITMASK		0x0f
#define SWITCHTEC_I2C_ADDR_BITSHIFT		14
#define SWITCHTEC_I2C_ADDR_BITSHIFT_GEN5	23
#define SWITCHTEC_I2C_ADDR_BITMASK		0x7f
#define SWITCHTEC_CMD_MAP_BITSHIFT		21
#define SWITCHTEC_CMD_MAP_BITSHIFT_GEN5		30
#define SWITCHTEC_CMD_MAP_BITMASK		0xffff
#define SWITCHTEC_CMD_MAP_BITMASK_GEN5		0x3fff

#define SWITCHTEC_JTAG_LOCK_AFT_RST_BITMASK	0x40
#define SWITCHTEC_JTAG_LOCK_AFT_BL1_BITMASK	0x80
#define SWITCHTEC_JTAG_UNLOCK_BL1_BITMASK	0x0100
#define SWITCHTEC_JTAG_UNLOCK_AFT_BL1_BITMASK	0x0200

static int switchtec_mfg_cmd(struct switchtec_dev *dev, uint32_t cmd,
			     const void *payload, size_t payload_len,
			     void *resp, size_t resp_len);

#if (HAVE_LIBCRYPTO && !HAVE_DECL_RSA_GET0_KEY)
/**
 * openssl1.0 or older versions don't have this function, so copy
 * the code from openssl1.1 here
 *
 * @param r [in] Pointer to the RSA structure
 * @param n [out] Pointer to the BIGNUM structure for the modulus
 * @param e [out] Pointer to the BIGNUM structure for the public exponent
 * @param d [out] Pointer to the BIGNUM structure for the private exponent
 * @return void
 */
static void RSA_get0_key(const RSA *r, const BIGNUM **n,
			 const BIGNUM **e, const BIGNUM **d)
{
	if (n != NULL)
		*n = r->n;
	if (e != NULL)
		*e = r->e;
	if (d != NULL)
		*d = r->d;
}
#endif

/**
 * @brief Get I2C operands based on the Switchtec generation.
 *
 * @param[in] gen The Switchtec generation.
 * @param[out] addr_shift Pointer to store the address shift value.
 * @param[out] map_shift Pointer to store the map shift value.
 * @param[out] map_mask Pointer to store the map mask value.
 * @return void
 */
static void get_i2c_operands(enum switchtec_gen gen, uint32_t *addr_shift,
			     uint32_t *map_shift, uint32_t *map_mask)
{
	if (gen > SWITCHTEC_GEN4) {
		*addr_shift = SWITCHTEC_I2C_ADDR_BITSHIFT_GEN5;
		*map_shift = SWITCHTEC_CMD_MAP_BITSHIFT_GEN5;
		*map_mask = SWITCHTEC_CMD_MAP_BITMASK_GEN5;
	} else {
		*addr_shift = SWITCHTEC_I2C_ADDR_BITSHIFT;
		*map_shift = SWITCHTEC_CMD_MAP_BITSHIFT;
		*map_mask = SWITCHTEC_CMD_MAP_BITMASK;
	}
}

static float spi_clk_rate_float[] = {
	25, 150, 137.5, 125.25, 100, 66.67, 50, 40, 33.33, 28.58
};

static float spi_clk_hi_rate_float[] = {
	120, 80, 60, 48, 40, 34, 30, 26.67, 24, 21.82
};

#pragma pack(push, 1)
typedef struct {
	uint64_t cfg;
	uint32_t public_key_exponent;
}cfg_stmfd;
#pragma pack(pop)

struct get_cfgs_reply {
	uint32_t valid;
	uint32_t rsvd1;
	cfg_stmfd cfg_stmfd;
	uint8_t rsvd2;
	uint8_t public_key_num;
	uint8_t public_key_ver;
	uint8_t spi_core_clk_high;
	uint8_t public_key[SWITCHTEC_KMSK_NUM][SWITCHTEC_KMSK_LEN];
	uint8_t rsvd4[32];
};

/**
 * @brief Retrieve configuration settings from the device.
 *
 * @param[in] dev Pointer to the switchtec device structure.
 * @param[out] cfgs Pointer to the structure where configuration settings will be stored.
 * @param[out] otp_valid Pointer to an integer that will be set to true if OTP is valid, false otherwise.
 *
 * @return 0 on success, or a negative error code on failure.
 */
static int get_configs(struct switchtec_dev *dev,
		       struct get_cfgs_reply *cfgs,
		       int *otp_valid)
{
	uint8_t subcmd = 0;
	int ret;

	if (switchtec_gen(dev) == SWITCHTEC_GEN5) {
		subcmd = 1;
		ret = switchtec_mfg_cmd(dev,
					MRPC_SECURITY_CONFIG_GET_GEN5,
					&subcmd, sizeof(subcmd),
					cfgs, sizeof(struct get_cfgs_reply));
		if (!ret)
			*otp_valid = true;
	} else {
		ret = switchtec_mfg_cmd(dev, MRPC_SECURITY_CONFIG_GET_EXT,
					&subcmd, sizeof(subcmd),
					cfgs, sizeof(struct get_cfgs_reply));
		if (ret && ERRNO_MRPC(errno) != ERR_CMD_INVALID)
			return ret;

		if (!ret) {
			*otp_valid = true;
			return ret;
		}

		*otp_valid = false;
		ret = switchtec_mfg_cmd(dev, MRPC_SECURITY_CONFIG_GET,
					NULL, 0, cfgs,
					sizeof(struct get_cfgs_reply));
	}

	return ret;
}

/**
 * @brief Get the available SPI rates for the Switchtec device.
 *
 * @param[in] dev Pointer to the Switchtec device structure.
 * @param[out] rates Pointer to the structure to store the available SPI rates.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int switchtec_security_spi_avail_rate_get(struct switchtec_dev *dev,
		struct switchtec_security_spi_avail_rate *rates)
{
	int ret;
	struct get_cfgs_reply reply;
	int otp_valid;

	ret = get_configs(dev, &reply, &otp_valid);
	if (ret)
		return ret;

	rates->num_rates = 10;
	if (reply.spi_core_clk_high)
		memcpy(rates->rates, spi_clk_hi_rate_float,
		       sizeof(spi_clk_hi_rate_float));
	else
		memcpy(rates->rates, spi_clk_rate_float,
		       sizeof(spi_clk_rate_float));

	return 0;
}

/**
 * @brief Parse OTP settings from flags.
 *
 * @param[in,out] otp   Pointer to the OTP region structure to be populated.
 * @param[in]     flags Flags containing the OTP settings.
 * @return void
 */
static void parse_otp_settings(struct switchtec_security_cfg_otp_region *otp,
			       uint32_t flags)
{
	otp->basic_valid = !!(flags & BIT(7));
	otp->basic = !!(flags & BIT(8));
	otp->mixed_ver_valid = !!(flags & BIT(9));
	otp->mixed_ver = !!(flags & BIT(10));
	otp->main_fw_ver_valid = !!(flags & BIT(11));
	otp->main_fw_ver = !!(flags & BIT(12));
	otp->sec_unlock_ver_valid = !!(flags & BIT(13));
	otp->sec_unlock_ver = !!(flags & BIT(14));
	otp->kmsk_valid[0] = !!(flags & BIT(15));
	otp->kmsk[0] = !!(flags & BIT(16));
	otp->kmsk_valid[1] = !!(flags & BIT(17));
	otp->kmsk[1] = !!(flags & BIT(18));
	otp->kmsk_valid[2] = !!(flags & BIT(19));
	otp->kmsk[2] = !!(flags & BIT(20));
	otp->kmsk_valid[3] = !!(flags & BIT(21));
	otp->kmsk[3] = !!(flags & BIT(22));
}

/**
 * @brief Get secure boot configurations
 * @param[in]  dev   Switchtec device handle
 * @param[out] state Current secure boot settings
 * @return 0 on success, error code on failure
 */
int switchtec_security_config_get(struct switchtec_dev *dev,
				  struct switchtec_security_cfg_state *state)
{
	int ret;
	uint32_t addr_shift;
	uint32_t map_shift;
	uint32_t map_mask;
	int spi_clk;
	struct get_cfgs_reply reply;
	int otp_valid;

	ret = get_configs(dev, &reply, &otp_valid);
	if (ret)
		return ret;

	reply.valid = le32toh(reply.valid);
	reply.cfg_stmfd.cfg = le64toh(reply.cfg_stmfd.cfg);
	reply.cfg_stmfd.public_key_exponent = le32toh(reply.cfg_stmfd.public_key_exponent);

	state->basic_setting_valid = !!(reply.valid & 0x01);
	state->public_key_num_valid = !!(reply.valid & 0x02);
	state->public_key_ver_valid = !!(reply.valid & 0x04);
	state->kmsk_valid = !!(reply.valid & 0x78);

	state->otp_valid = otp_valid;
	if (otp_valid)
		parse_otp_settings(&state->otp, reply.valid);

	state->secure_state = (reply.cfg_stmfd.cfg) & 0x03;

	spi_clk = (reply.cfg_stmfd.cfg >> SWITCHTEC_CLK_RATE_BITSHIFT) & 0x0f;

	/* 0 - OTP_SEC_MODE_UNINITIALIZED_UNSECURED - Set the SPI clock freq to default */
	if(0 == state->secure_state)
	{
		if (spi_clk == 0) {
			if (switchtec_gen(dev) == SWITCHTEC_GEN5)
				spi_clk = 9;
		}
	}

	if (reply.spi_core_clk_high)
		state->spi_clk_rate = spi_clk_hi_rate_float[spi_clk - 1];
	else
		state->spi_clk_rate = spi_clk_rate_float[spi_clk];

	state->i2c_recovery_tmo =
		(reply.cfg_stmfd.cfg >> SWITCHTEC_RC_TMO_BITSHIFT) & 0x0f;
	state->i2c_port = (reply.cfg_stmfd.cfg >> SWITCHTEC_I2C_PORT_BITSHIFT) & 0xf;

	get_i2c_operands(switchtec_gen(dev), &addr_shift, &map_shift,
			 &map_mask);
	state->i2c_addr =
		(reply.cfg_stmfd.cfg >> addr_shift) & SWITCHTEC_I2C_ADDR_BITMASK;
	state->i2c_cmd_map = (reply.cfg_stmfd.cfg >> map_shift) & map_mask;

	state->public_key_exponent = reply.cfg_stmfd.public_key_exponent;
	state->public_key_num = reply.public_key_num;
	state->public_key_ver = reply.public_key_ver;
	memcpy(state->public_key, reply.public_key,
	       SWITCHTEC_KMSK_NUM * SWITCHTEC_KMSK_LEN);

	return 0;
}

/**
 * @brief Retrieve mailbox entries
 * @param[in]  dev	Switchtec device handle
 * @param[in]  fd	File handle to write the log data
 * @return 0 on success, error code on failure
 */
int switchtec_mailbox_to_file(struct switchtec_dev *dev, int fd)
{
	int ret;
	int num_to_read = htole32(SWITCHTEC_MB_MAX_ENTRIES);
	struct mb_reply {
		uint8_t num_returned;
		uint8_t num_remaining;
		uint8_t rsvd[2];
		uint8_t data[SWITCHTEC_MB_MAX_ENTRIES *
			     SWITCHTEC_MB_LOG_LEN];
	} reply;

	do {
		ret = switchtec_mfg_cmd(dev, MRPC_MAILBOX_GET, &num_to_read,
					sizeof(int), &reply,  sizeof(reply));
		if (ret)
			return ret;

		reply.num_remaining = le32toh(reply.num_remaining);
		reply.num_returned = le32toh(reply.num_returned);

		ret = write(fd, reply.data,
			    (reply.num_returned) * SWITCHTEC_MB_LOG_LEN);
		if (ret < 0)
			return ret;
	} while (reply.num_remaining > 0);

	return 0;
}

/**
 * @brief Converts SPI clock rate from float to an integer index.
 *
 * This function takes a floating-point SPI clock rate and converts it to an integer index
 * based on predefined clock rate arrays. The function supports both high rate and normal rate
 * conversions.
 *
 * @param clk_float [in] The SPI clock rate as a float.
 * @param hi_rate [in] A flag indicating whether to use the high rate clock array.
 * @return The index corresponding to the clock rate, or -1 if no match is found.
 */
static int convert_spi_clk_rate(float clk_float, int hi_rate)
{
	int i;
	int spi_rate_max_val = 10;
	float *p;

	if (hi_rate)
	{
		p = spi_clk_hi_rate_float;
	}
	else
	{
		p = spi_clk_rate_float;
	}

	for (i = 0; i < spi_rate_max_val; i++)
	{
		if ((clk_float < p[i] + 0.1) && (clk_float > p[i] - 0.1))
		{
			if (hi_rate)
			{
				return i + 1;
			}
			else
			{
				return i;
			}
		}
	}

	return -1;
}

/**
 * @brief Set secure settings
 * @param[in]  dev	Switchtec device handle
 * @param[out] setting	Secure boot settings
 * @return 0 on success, error code on failure
 */
int switchtec_security_config_set(struct switchtec_dev *dev,
				  struct switchtec_security_cfg_set *setting)
{
	int ret;
	struct setting_data {
		uint64_t cfg;
		uint32_t pub_key_exponent;
		uint8_t rsvd[4];
	} sd;
	struct get_cfgs_reply reply;
	uint64_t ldata = 0;
	uint32_t addr_shift;
	uint32_t map_shift;
	uint32_t map_mask;
	int spi_clk;
	uint8_t cmd_buf[20] = {};
	int otp_valid;

	ret = get_configs(dev, &reply, &otp_valid);
	if (ret)
		return ret;

	memset(&sd, 0, sizeof(sd));

	spi_clk = convert_spi_clk_rate(setting->spi_clk_rate,
				       reply.spi_core_clk_high);
	if (spi_clk < 0) {
		errno = EINVAL;
		return -1;
	}

	sd.cfg |= (spi_clk & SWITCHTEC_CLK_RATE_BITMASK) <<
			SWITCHTEC_CLK_RATE_BITSHIFT;

	sd.cfg |= (setting->i2c_recovery_tmo & SWITCHTEC_RC_TMO_BITMASK) <<
			SWITCHTEC_RC_TMO_BITSHIFT;
	sd.cfg |= (setting->i2c_port & SWITCHTEC_I2C_PORT_BITMASK) <<
			SWITCHTEC_I2C_PORT_BITSHIFT;

	get_i2c_operands(switchtec_gen(dev), &addr_shift, &map_shift,
			 &map_mask);
	sd.cfg |= (setting->i2c_addr & SWITCHTEC_I2C_ADDR_BITMASK) <<
			addr_shift;

	ldata = setting->i2c_cmd_map & map_mask;
	ldata <<= map_shift;
	sd.cfg |= ldata;

	sd.cfg = htole64(sd.cfg);

	sd.pub_key_exponent = htole32(setting->public_key_exponent);

	if (switchtec_gen(dev) == SWITCHTEC_GEN4) {
		ret = switchtec_mfg_cmd(dev, MRPC_SECURITY_CONFIG_SET,
					&sd, sizeof(sd), NULL, 0);
	} else {
		cmd_buf[0] = 1;
		memcpy(cmd_buf + 4, &sd, sizeof(sd));
		ret = switchtec_mfg_cmd(dev, MRPC_SECURITY_CONFIG_SET_GEN5,
					cmd_buf, sizeof(cmd_buf), NULL, 0);
	}
	return ret;
}

/**
 * @brief Set the SJTAG mode for the Switchtec device.
 *
 * @param[in] dev Pointer to the Switchtec device structure.
 * @param[in] sjtag_mode The SJTAG mode to set.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int switchtec_otp_sjtag_mode_set(struct switchtec_dev *dev,
										uint8_t sjtag_mode)
{
	struct sjtag_mode_cmd {
		uint8_t subcmd;
		uint8_t sjtag_mode:2;
	} cmd = {};

	cmd.subcmd = MRPC_OTP_SJTAG_MODE_SET;
	cmd.sjtag_mode = sjtag_mode;

	return switchtec_mfg_cmd(dev, MRPC_OTP_SJTAG_INFO, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Send SJTAG host response to the device.
 *
 * @param[in] dev Pointer to the switchtec device structure.
 * @param[in] sjtag_hr Pointer to the SJTAG host response data.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int switchtec_sjtag_hr_send(struct switchtec_dev *dev,
										uint8_t *sjtag_hr)
{
	struct sjtag_uuid_cmd {
		uint8_t subcmd;
		uint8_t reserved[3];
		uint8_t sjtag_hr[SJTAG_HR_LEN];
	} cmd = {};

	cmd.subcmd = MRPC_SJTAG_SET_HOST_RESPONSE;
	
	memcpy((void *)cmd.sjtag_hr, (void *)sjtag_hr, SJTAG_HR_LEN);

	return switchtec_mfg_cmd(dev, MRPC_SJTAG_UNLOCK, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Set the SJTAG UUID for a Switchtec device.
 *
 * @param[in] dev Pointer to the Switchtec device structure.
 * @param[in] sjtag_uuid Pointer to the SJTAG UUID to be set.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int switchtec_otp_sjtag_uuid_set(struct switchtec_dev *dev,
										uint8_t *sjtag_uuid)
{
	struct sjtag_uuid_cmd {
		uint8_t subcmd;
		uint8_t sjtag_uuid[OTP_SJTAG_UUID_LENGTH];
	} cmd = {};

	cmd.subcmd = MRPC_OTP_SJTAG_UUID_SET;
	
	memcpy((void *)cmd.sjtag_uuid, (void *)sjtag_uuid, OTP_SJTAG_UUID_LENGTH);

	return switchtec_mfg_cmd(dev, MRPC_OTP_SJTAG_INFO, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Set the SJTAG key in the OTP memory of the Switchtec device.
 *
 * @param dev [in] Pointer to the Switchtec device structure.
 * @param sjtag_key [in] Pointer to the SJTAG key to be set.
 *
 * @return int Status of the operation.
 */
int switchtec_otp_sjtag_key_set(struct switchtec_dev *dev,
										uint8_t *sjtag_key)
{
	struct sjtag_key_cmd {
		uint8_t subcmd;
		uint8_t sjtag_key[OTP_SJTAG_KEY_LENGTH];
	} cmd = {};

	cmd.subcmd = MRPC_OTP_SJTAG_KEY_SET;
	
	memcpy((void *)cmd.sjtag_key, (void *)sjtag_key, OTP_SJTAG_KEY_LENGTH);

	return switchtec_mfg_cmd(dev, MRPC_OTP_SJTAG_INFO, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Programs the OTP configuration on the Switchtec device.
 *
 * @param dev [in] Pointer to the Switchtec device structure.
 * @param config_len [in] Length of the configuration data.
 * @param config_data [in] Pointer to the configuration data.
 * @param is_firmware_halt [in] Boolean flag indicating if the firmware should halt.
 *
 * @return int Status of the operation.
 */

int switchtec_otp_config_prog(struct switchtec_dev *dev,
							  uint16_t config_len,
							  uint8_t *config_data,
							  bool is_firmware_halt)
{
	#pragma pack(push, 1)
	struct otp_config_cmd {
		uint8_t subcmd;
		uint16_t config_len:10;
		uint16_t reserved:6;
		uint8_t is_firmware_halt:1;
		uint8_t reserved1:7;
		uint8_t config_data[OTP_CONFIG_DATA_MAX_LEN];
	} cmd = {};
	#pragma pack(pop)

	cmd.subcmd = MRPC_OTP_CONFIG_PROG;
	cmd.config_len = config_len;
	cmd.is_firmware_halt = is_firmware_halt;
	
	memcpy((void *)cmd.config_data, (void *)config_data, OTP_CONFIG_DATA_MAX_LEN);

	return switchtec_mfg_cmd(dev, MRPC_OTP_PROG_GEN_HDLR, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Reads OTP configuration data from the device.
 *
 * @param[in] dev Pointer to the switchtec device structure.
 * @param[in] config_start_address The starting address of the configuration data to read.
 * @param[in] config_length The length of the configuration data to read.
 * @param[in] out_fd The file descriptor to write the configuration data to.
 *
 * @return 0 on success, -1 on failure.
 */

int switchtec_otp_config_read(struct switchtec_dev *dev,
							  uint16_t config_start_address,
							  int32_t config_length,
							  int out_fd)
{
	int ret = 0;
	
	#pragma pack(push, 1)
	struct otp_config_cmd {
		uint8_t subcmd;
		uint16_t config_start_address:14;
		uint16_t config_length:10;
	} cmd = {};
	#pragma pack(pop)

	struct otp_config_read_reply {
		uint8_t config_data[OTP_CONFIG_DATA_MAX_LEN];
	} reply;

	cmd.subcmd = MRPC_OTP_CONFIG_READ;

	cmd.config_start_address = config_start_address;
	while(config_length > 0)
	{
		cmd.config_length = (OTP_CONFIG_DATA_MAX_LEN < config_length) ? OTP_CONFIG_DATA_MAX_LEN : config_length;
		ret = switchtec_mfg_cmd(dev, MRPC_OTP_CONTROL_STATUS_GEN_HDLR, &cmd,
					sizeof(cmd), &reply, sizeof(reply));
		if(ret)
		{
			return -1;
		}

		ret = write(out_fd, reply.config_data, cmd.config_length);
		if(ret < 0)
		{
			return ret;
		}
		cmd.config_start_address += OTP_CONFIG_DATA_MAX_LEN;
		config_length -= OTP_CONFIG_DATA_MAX_LEN;
	}
	close(out_fd);

	return 0;
}

/**
 * @brief Verify the SJTAG key in the OTP memory.
 *
 * @param[in] dev Pointer to the switchtec device structure.
 * @param[in] sjtag_key Pointer to the SJTAG key to be verified.
 * @param[out] is_verified Pointer to a variable that will be set to 1 if the key is verified, 0 otherwise.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int switchtec_otp_sjtag_key_verify(struct switchtec_dev *dev,
										uint8_t *sjtag_key, uint8_t *is_verified)
{
	int ret;
	struct sjtag_key_cmd {
		uint8_t subcmd;
		uint8_t sjtag_key[OTP_SJTAG_KEY_LENGTH];
	} cmd = {};

	uint8_t reply = 0;

	cmd.subcmd = MRPC_OTP_SJTAG_KEY_VERIFY;
	
	memcpy((void *)cmd.sjtag_key, (void *)sjtag_key, OTP_SJTAG_KEY_LENGTH);

	ret = switchtec_mfg_cmd(dev, MRPC_OTP_SJTAG_INFO, &cmd,
				 sizeof(cmd), &reply, sizeof(uint8_t));
	
	if(ret)
		return ret;
		
	memcpy((void *)is_verified, (void *)&reply, sizeof(uint8_t));
	return 0;
}

/**
 * @brief Locks the SJTAG key on the specified Switchtec device.
 *
 * @param[in] dev Pointer to the Switchtec device structure.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int switchtec_otp_sjtag_key_lock(struct switchtec_dev *dev)
{
	struct sjtag_key_cmd {
		uint8_t subcmd;
	} cmd = {};

	cmd.subcmd = MRPC_OTP_SJTAG_KEY_LOCK;

	return switchtec_mfg_cmd(dev, MRPC_OTP_SJTAG_INFO, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Program the chip serial number for a Switchtec device.
 *
 * @param dev [in] Pointer to the Switchtec device structure.
 * @param chip_serial_num [in] The chip serial number to be programmed.
 *
 * @return int Status of the operation.
 */
int switchtec_chip_serial_num_prog(struct switchtec_dev *dev,
				uint32_t chip_serial_num)
{
	struct sku_info_cmd {
		uint8_t subcmd;
		uint32_t chip_serial_num;
	} cmd = {};

	cmd.subcmd = MRPC_OTP_PROG_CHIP_NUM;
	cmd.chip_serial_num = chip_serial_num;

	return switchtec_mfg_cmd(dev, MRPC_OTP_PROG_GEN_HDLR, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Set the SKU information in the OTP memory of the Switchtec device.
 *
 * @param[in] dev Pointer to the Switchtec device structure.
 * @param[in] sku_info_set_str Pointer to the structure containing SKU information to be set.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int switchtec_otp_sku_info_set(struct switchtec_dev *dev,
				struct otp_sku_info_set *sku_info_set_str)
{
	struct sku_info_cmd {
		uint8_t subcmd;
		uint8_t block_num:2;
		uint8_t reserved:6;
		uint8_t sku_info:7;
		uint8_t reserved1:1;
	} cmd = {};

	cmd.subcmd = MRPC_OTP_SKU_INFO_SET;
	cmd.block_num = sku_info_set_str->block_num;
	cmd.sku_info = sku_info_set_str->sku_info;

	return switchtec_mfg_cmd(dev, MRPC_OTP_PROG_GEN_HDLR, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Retrieve the UUID and ID code from the switchtec device.
 *
 * @param[in] dev Pointer to the switchtec device structure.
 * @param[out] sjtag_uuid_ptr Pointer to the structure where the UUID will be stored.
 * @param[out] sjtag_idcode Pointer to the structure where the ID code will be stored.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int switchtec_sjtag_get_uuid_idcode(struct switchtec_dev *dev,
									uint8_t *sjtag_uuid_ptr,
									uint8_t *sjtag_idcode)
{
	int ret;
	struct sjtag_uuid_idcode {
		uint8_t uuid[10];
        uint8_t reserved[2];
        uint8_t idcode[4];
	} reply = {};

	uint8_t sub_cmd = MRPC_SJTAG_GET_UUID_IDCODE;

	ret = switchtec_mfg_cmd(dev, MRPC_SJTAG_UNLOCK, &sub_cmd,
				 sizeof(sub_cmd), &reply, sizeof(reply));
	
	if (ret)
		return ret;

    memcpy((void *)sjtag_uuid_ptr, (void *)reply.uuid, SJTAG_UUID_LEN);
    memcpy((void *)sjtag_idcode, (void *)reply.idcode, SJTAG_IDCODE_LEN);

	return 0;
}

/**
 * @brief Retrieve the SJTAG nonce from the device.
 *
 * @param[in] dev Pointer to the switchtec device structure.
 * @param[out] sjtag_nonce_ptr Pointer to the structure where the nonce will be stored.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int switchtec_sjtag_get_nonce(struct switchtec_dev *dev,
				struct sjtag_nonce *sjtag_nonce_ptr)
{
	int ret;
	struct sjtag_nonce {
		uint32_t data[8];
	} reply = {};

	uint8_t sub_cmd = MRPC_SJTAG_GET_NONCE;

	ret = switchtec_mfg_cmd(dev, MRPC_SJTAG_UNLOCK, &sub_cmd,
				 sizeof(sub_cmd), &reply, sizeof(reply));

	if (ret)
		return ret;

	memcpy((void *)sjtag_nonce_ptr, (void *)&reply, sizeof(reply));

	return 0;
}


/**
 * @brief Get the SJTAG status from the switchtec device.
 *
 * @param[in] dev Pointer to the switchtec device structure.
 * @param[out] sjtag_sts_get Pointer to the structure where the SJTAG status will be stored.
 *
 * @return 0 on success, or a negative error code on failure.
 */

int switchtec_sjtag_status_get(struct switchtec_dev *dev,
				struct sjtag_status_get *sjtag_sts_get)
{
	int ret;
	struct stag_sts_cmd {
		uint32_t data;
	} reply = {};

	uint8_t sub_cmd = MRPC_SJTAG_GET_STATUS;

	ret = switchtec_mfg_cmd(dev, MRPC_SJTAG_UNLOCK, &sub_cmd,
				 sizeof(sub_cmd), &reply, sizeof(reply));
	
	if (ret)
		return ret;

	memcpy((void *)sjtag_sts_get, (void *)&reply, sizeof(reply));
	return 0;
}

/*******************************************************************************
* FUNCTION sjtag_hr_calc()
*_______________________________________________________________________________
*
* DESCRIPTION:
*   This function calculates the SJTAG Host Response
*
* @param[in] sjtag_debug_token - Pointer to Debug Token
* @param[in] sjtag_nonce - Pointer to Nonce
* @param[in] verbose - verbose enable/disable
* @param[out] digest - SHA256 Digest
*
* @return void
*
********************************************************************************/
void sjtag_hr_calc(uint8_t *sjtag_debug_token, uint8_t *sjtag_nonce, uint8_t *digest, bool verbose)
{
	uint8_t sjtag_sha256_msg[SJTAG_SHA256_MSG_LEN] = {0};
    SHA256_CTX sha256_algo;

    if(true == verbose)
    {
        printf("Nonce: ");
        for(int i = 0; i < SJTAG_NONCE_LEN; i++)
        {
            printf("%x", sjtag_nonce[31 - i]);
        }
        printf("\n");
    }

	(void)memcpy((void*)(sjtag_sha256_msg), (void*)sjtag_debug_token, SJTAG_DEBUG_TOKEN_LEN);
	(void)memcpy((void*)(&sjtag_sha256_msg[SJTAG_DEBUG_TOKEN_LEN]), (void*)sjtag_nonce, SJTAG_NONCE_LEN);

	SHA256_Init(&sha256_algo);
    SHA256_Update(&sha256_algo, sjtag_sha256_msg, SJTAG_SHA256_MSG_LEN);
    SHA256_Final(digest, &sha256_algo);
    if(true == verbose)
    {
        printf("Host Response: ");
        for(int i = 0; i < SJTAG_HR_LEN; i++)
        {
            printf("%x", digest[i]);
        }
        printf("\n");
    }
}

/**
 * @brief This function generates the command header to send to the server.
 *
 * @param[in,out] cmd_header  Pointer to the command header array to be filled.
 * @param[in] cmd             Command to encode in the header.
 *
 * @return void
 */
void sjtag_encode_cmd_header(uint8_t *cmd_header, uint8_t cmd)
{
    cmd_header[SJTAG_SERVER_CMD_INDEX] = cmd;
    cmd_header[SJTAG_SERVER_CMD_RESP_INDICATOR_INDEX] = SJTAG_SERVER_CMD_MSK;
    cmd_header[2] = 0;
    cmd_header[3] = 0;
}

/*******************************************************************************
* FUNCTION sjtag_resp_is_success()
*_______________________________________________________________________________
*
* @brief
*   This function checks for Debug Token generation success from the Response header
*   received from the server.
*
* @param resp_header [in] - Response header
*
* @return
*   Returns 0 on Success and -1 on Failure
*
********************************************************************************/
int sjtag_resp_is_success(uint8_t *resp_header)
{
    int ret = -1;

    if((SJTAG_SERVER_CMD_CHAL_RESP == resp_header[SJTAG_SERVER_CMD_INDEX]) && (SJTAG_SERVER_RESP_MSK == resp_header[SJTAG_SERVER_CMD_RESP_INDICATOR_INDEX]))
    {
        if(SERVER_STATUS_SUCCESS == resp_header[SJTAG_SERVER_RESPONSE_STATUS_INDEX])
        {
            ret = 0;
        }
        else if(SERVER_STATUS_INVALID_SJTAGID == resp_header[SJTAG_SERVER_RESPONSE_STATUS_INDEX])
        {
            printf("Error in generating debug token generation due to invalid IDCODE!\n");
        }
        else if(SERVER_STATUS_INVALID_HEADER == resp_header[SJTAG_SERVER_RESPONSE_STATUS_INDEX])
        {
            printf("Error in generating debug token generation due to invalid header!\n");
        }
        else if(SERVER_STATUS_DIGEST_COMPUTE_FAIL == resp_header[SJTAG_SERVER_RESPONSE_STATUS_INDEX])
        {
            printf("Exception in generating the digest!\n");
        }
        else
        {
            printf("Invalid response header from SJTAG server!\n");
        }
    }
    return ret;
}

/**
 * @brief This Generates the Debug Token
 *
 * @param idcode_ptr [in] Pointer to IDCODE
 * @param uuid_ptr [in] Pointer to UUID
 * @param suv_ptr [in] Pointer to SUV
 * @param sjtag_debug_token [out] Debug Token
 * @param verbose [in] verbose enable/disable
 *
 * @return Returns 0 on Success and -1 on Failure
 */
int sjtag_debug_token_gen(uint8_t *idcode_ptr, uint8_t *uuid_ptr, uint8_t *suv_ptr, uint8_t *sjtag_debug_token, bool verbose)
{
    int ret = 0;
    int socket_server;
    struct sockaddr_in server_address;
    uint8_t sjtag_hmac_msg[SJTAG_HMAC_MSG_LEN] = {0};
    uint8_t sjtag_idcode_send[SJTAG_IDCODE_LEN] = {0};
    const uint8_t unlock_str[SJTAG_UNLOCK_STR_LEN] = {'S','T','M','F','D','_','S','J','T','A','G','_','U','N','L','O','C','K'};
    uint8_t cmd_header[SJTAG_SERVER_HEADER_LEN];
    uint8_t resp_header[SJTAG_SERVER_HEADER_LEN];

    do{
        /* Create a socket communication */
        socket_server = socket(AF_INET, SOCK_STREAM, 0);
        if(socket_server < 0)
        {
            printf("Unable to create socket communication!\n");
            ret = -1;
            break;
        }

        /* Set port and IP */
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(SJTAG_SERVER_PORT);
        server_address.sin_addr.s_addr = inet_addr(SJTAG_SERVER_IP);

        /* Send a connection request to Plugin server */
        if(connect(socket_server, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
        {
            printf("Unable to connect with Plugin server!\n");
            ret = -1;
            break;
        }
        printf("Connected with Plugin server successfully!\n");

        sjtag_encode_cmd_header(cmd_header, SJTAG_SERVER_CMD_CHAL_RESP);
	    /* Send the server header */
        if(send(socket_server, cmd_header, SJTAG_SERVER_HEADER_LEN, 0) < 0)
        {
            printf("Unable to sent the Command Header to the Plugin server!\n");
            ret = -1;
            break;
        }

        if(true == verbose)
            printf("IDCODE: ");

        for(int i=0; i<SJTAG_IDCODE_LEN; i++)
        {
            sjtag_idcode_send[i] = idcode_ptr[3 - i];
            if(true == verbose)
            {
                printf("%x", sjtag_idcode_send[i]);
            }
        }
        if(true == verbose)
            printf("\n");

        /* Send the IDCODE to the server */
        if(send(socket_server, sjtag_idcode_send, SJTAG_IDCODE_LEN, 0) < 0)
        {
            printf("Unable to sent the IDCODE to the Plugin server!\n");
            ret = -1;
            break;
        }

        if(true == verbose)
		{
			printf("UUID: ");
			for(int i=0; i<SJTAG_UUID_LEN; i++)
			{
				printf("%x", uuid_ptr[i]);
			}
			printf("\n");
		}

        /* Send the UUID to the server */
        if(send(socket_server, uuid_ptr, SJTAG_UUID_LEN, 0) < 0)
        {
            printf("Unable to send the UUID to the Plugin server!\n");
            ret = -1;
            break;
        }

        (void)memcpy((void*)sjtag_hmac_msg, (void*)suv_ptr, SUV_LEN);
        (void)memcpy((void*)(&sjtag_hmac_msg[SUV_LEN]), (void*)unlock_str, SJTAG_UNLOCK_STR_LEN);

        if(true == verbose)
        {
            printf("SUV: ");
            for(int i = 0; i < SUV_LEN; i++)
            {
                printf("%x", suv_ptr[3 - i]);
            }
            printf("\n");
         }

        /* Send the HMAC message to the server */
        if(send(socket_server, sjtag_hmac_msg, SJTAG_HMAC_MSG_LEN, 0) < 0)
        {
            printf("Unable to send the HMAC message to the Plugin server!\n");
            ret = -1;
            break;
        }

        /* Receive the response header from the Plugin server */
        if(recv(socket_server, resp_header, SJTAG_SERVER_HEADER_LEN, 0) < 0)
        {
            printf("Error while receiving response header from the Plugin server!\n");
            ret = -1;
            break;
        }

        ret = sjtag_resp_is_success(resp_header);
        if(0 == ret)
        {
            /* Receive the Debug Token from the Plugin server */
            if(recv(socket_server, sjtag_debug_token, SJTAG_DEBUG_TOKEN_LEN, 0) < 0)
            {
                printf("Error while receiving Debug Token from the Plugin server!\n");
                ret = -1;
                break;
            }
            if(true == verbose)
            {
                printf("Debug Token: ");
                for(int i = 0; i < SJTAG_DEBUG_TOKEN_LEN; i++)
                {
                    printf("%x", sjtag_debug_token[i]);
                }
                printf("\n");
            }
        }
    }while(false);

    /* Close the socket communication to the Plugin server */
    close(socket_server);

    return ret;
}
/**
 * @brief This function fetches the Debug Token from a bin file.
 *
 * @param[in] debug_token_file Pointer to the .bin file.
 * @param[out] debug_token Pointer to the structure where the Debug Token will be stored.
 *
 * @return 0 on success, or -EBADF on failure.
 */
int switchtec_read_sjtag_debug_token_file(FILE *debug_token_file,
				  struct sjtag_debug_token *debug_token)
{
	ssize_t rlen;

	rlen = fread(debug_token->debug_token, 1, SJTAG_DEBUG_TOKEN_LEN, debug_token_file);

	if (rlen < SJTAG_DEBUG_TOKEN_LEN)
		return -EBADF;

	return 0;
}

/**
 * @brief Write Protect given choice of OTP Regions
 * @param[in]  dev	Switchtec device handle
 * @param[in]  sku_info		flag to write protect sku info
 * @param[in]  otp_config	flag to write protect otp_config
 * @return 0 on success, error code on failure
 */
int switchtec_otp_regs_write_protect(struct switchtec_dev *dev,
				bool sku_info, bool otp_config)
{
	#pragma pack(push, 1)
	struct otp_write_prot_cmd {
		uint8_t subcmd;
		bool sku_info:1;
		bool otp_config:1;
	} cmd={};
	#pragma pack(pop)

	cmd.subcmd = MRPC_OTP_REGS_WRITE_PROT;
	cmd.sku_info = sku_info;
	cmd.otp_config = otp_config;

	return switchtec_mfg_cmd(dev, MRPC_OTP_PROG_GEN_HDLR, &cmd,
                 sizeof(cmd), NULL, 0);
}

/**
 * @brief Get SKU Information and Write Protected Status
 * @param[in]  dev	Switchtec device handle
 * @param[out]  sku_info_get_str	Pointer to structure that contains sku information
 * @return 0 on success, error code on failure
 */
int switchtec_otp_sku_info_get(struct switchtec_dev *dev,
				struct otp_sku_info_get *sku_info_get_str)
{
	uint8_t sub_cmd = MRPC_OTP_SKU_INFO_GET;
	return switchtec_mfg_cmd(dev, MRPC_OTP_CONTROL_STATUS_GEN_HDLR, &sub_cmd,
				 sizeof(sub_cmd), sku_info_get_str, sizeof(struct otp_sku_info_get));
}

/**
 * @brief Program Self Test Policy into OTP
 * @param[in]  dev	Switchtec device handle
 * @param[in]  self_test_policy	Pointer to array that contains self test policy
 * @return 0 on success, error code on failure
 */
int switchtec_otp_self_test_policy_set(struct switchtec_dev *dev,
							uint8_t *self_test_policy)
{
	struct self_test_policy_cmd {
		uint8_t subcmd;
		uint8_t self_test_policy[OTP_SELF_TEST_POLICY_LENGTH];
	} cmd = {};

	cmd.subcmd = MRPC_OTP_SELF_TEST_POLICY_SET;
	
	memcpy((void *)cmd.self_test_policy, (void *)self_test_policy, OTP_SELF_TEST_POLICY_LENGTH);

	return switchtec_mfg_cmd(dev, MRPC_OTP_PROG_GEN_HDLR, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Progrom Die Trace into OTP
 * @param[in]  dev	Switchtec device handle
 * @param[in]  die_trace	Pointer to array that contains die_trace
 * @return 0 on success, error code on failure
 */
int switchtec_otp_die_trace_prog(struct switchtec_dev *dev,
							uint8_t *die_trace)
{
	struct otp_die_trace_cmd {
		uint32_t subcmd;
		uint8_t die_trace[OTP_DIE_TRACE_LENGTH];
	} cmd = {};

	cmd.subcmd = MRPC_OTP_PROG_DIE_TRACE;
	
	memcpy((void *)cmd.die_trace, (void *)die_trace, OTP_DIE_TRACE_LENGTH);

	return switchtec_mfg_cmd(dev, MRPC_OTP_PROG_GEN_HDLR, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Set Bit or Byte Mode for OTP
 * @param[in]  dev	Switchtec device handle
 * @param[in]  bit_byte_mode	Flag to indicate Bit or Byte mode
 * @return 0 on success, error code on failure
 */
int switchtec_otp_bit_byte_prog_mode_set(struct switchtec_dev *dev,
							bool bit_byte_mode)
{
	
	struct self_test_policy_cmd {
		uint8_t subcmd;
		bool bit_byte_mode;
	} cmd = {};

	cmd.subcmd = MRPC_OTP_BIT_BYTE_MODE_SET;
	cmd.bit_byte_mode = bit_byte_mode;
	return switchtec_mfg_cmd(dev, MRPC_OTP_CONTROL_STATUS_GEN_HDLR, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Set Smart Programming mode for OTP
 * @param[in]  dev	Switchtec device handle
 * @param[in]  smart_prog_mode	Flag to smart programming mode for OTP
 * @return 0 on success, error code on failure
 */
int switchtec_otp_smart_prog_mode_set(struct switchtec_dev *dev,
							bool smart_prog_mode)
{
	struct self_test_policy_cmd {
		uint8_t subcmd;
		bool smart_prog_mode;
	} cmd = {};

	cmd.subcmd = MRPC_OTP_SMART_PROG_MODE_SET;
	cmd.smart_prog_mode = smart_prog_mode;

	return switchtec_mfg_cmd(dev, MRPC_OTP_CONTROL_STATUS_GEN_HDLR, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Get Status of Programming Modes in OTP
 * @param[in]  dev	Switchtec device handle
 * @param[out] otp_mode_status_str	pointer to structure otp_mode_status
 * @return 0 on success, error code on failure
 */
int switchtec_otp_mode_status_get(struct switchtec_dev *dev, struct otp_mode_status *otp_mode_status_str)
{
	uint8_t subcmd = MRPC_OTP_MODE_STATUS_GET;
	return switchtec_mfg_cmd(dev, MRPC_OTP_CONTROL_STATUS_GEN_HDLR, &subcmd,
				 sizeof(subcmd), otp_mode_status_str, sizeof(struct otp_mode_status));
}

/**
 * @brief Program given customer id into OTP
 * @param[in]  dev	Switchtec device handle
 * @param[out] cust_id	Customer ID to be programmed
 * @return 0 on success, error code on failure
 */
int switchtec_custid_prog(struct switchtec_dev *dev, uint16_t cust_id)
{
	#pragma pack(push, 1)
	struct cust_id_cmd {
		uint8_t subcmd;
		uint16_t cust_id;
	} cmd = {};
	#pragma pack(pop)

	cmd.subcmd = MRPC_OTP_CUSTID_PROG;
	cmd.cust_id = cust_id;

	return switchtec_mfg_cmd(dev, MRPC_OTP_PROG_GEN_HDLR, &cmd,
				 sizeof(cmd), NULL, 0);
}
/**
 * @brief Get active image index
 * @param[in]  dev	Switchtec device handle
 * @param[out] index	Active images indices
 * @return 0 on success, error code on failure
 */
int switchtec_active_image_index_get(struct switchtec_dev *dev,
				     struct switchtec_active_index *index)
{
	int ret;
	struct active_indices {
		uint8_t index[SWITCHTEC_ACTV_IDX_MAX_ENTRIES];
	} reply;

	ret = switchtec_mfg_cmd(dev, MRPC_ACT_IMG_IDX_GET, NULL,
				0, &reply, sizeof(reply));
	if (ret)
		return ret;

	index->keyman = reply.index[SWITCHTEC_ACTV_IMG_ID_KMAN];
	index->bl2 = reply.index[SWITCHTEC_ACTV_IMG_ID_BL2];
	index->config = reply.index[SWITCHTEC_ACTV_IMG_ID_CFG];
	index->firmware = reply.index[SWITCHTEC_ACTV_IMG_ID_FW];

	return 0;
}

/**
 * @brief Set active image index
 * @param[in]  dev	Switchtec device handle
 * @param[in] index	Active image indices
 * @return 0 on success, error code on failure
 */
int switchtec_active_image_index_set(struct switchtec_dev *dev,
				     struct switchtec_active_index *index)
{
	int ret;
	int i = 0;
	struct active_idx {
		uint32_t count;
		struct entry {
			uint8_t image_id;
			uint8_t index;
		} idx[SWITCHTEC_ACTV_IDX_SET_ENTRIES];
	} set;

	memset(&set, 0, sizeof(set));

	if (index->keyman != SWITCHTEC_ACTIVE_INDEX_NOT_SET) {
		set.idx[i].image_id = SWITCHTEC_ACTV_IMG_ID_KMAN;
		set.idx[i].index = index->keyman;
		i++;
	}

	if (index->bl2 != SWITCHTEC_ACTIVE_INDEX_NOT_SET) {
		set.idx[i].image_id = SWITCHTEC_ACTV_IMG_ID_BL2;
		set.idx[i].index = index->bl2;
		i++;
	}

	if (index->config != SWITCHTEC_ACTIVE_INDEX_NOT_SET) {
		set.idx[i].image_id =  SWITCHTEC_ACTV_IMG_ID_CFG;
		set.idx[i].index = index->config;
		i++;
	}

	if (index->firmware != SWITCHTEC_ACTIVE_INDEX_NOT_SET) {
		set.idx[i].image_id = SWITCHTEC_ACTV_IMG_ID_FW;
		set.idx[i].index = index->firmware;
		i++;
	}

	if (i == 0)
		return 0;

	set.count = htole32(i);

	ret = switchtec_mfg_cmd(dev, MRPC_ACT_IMG_IDX_SET, &set,
				sizeof(set), NULL, 0);
	return ret;
}

/**
 * @brief Execute the transferred firmware
 * @param[in]  dev		Switchtec device handle
 * @param[in]  recovery_mode	Recovery mode in case of a boot failure
 * @return 0 on success, error code on failure
 */
int switchtec_fw_exec(struct switchtec_dev *dev,
		      enum switchtec_bl2_recovery_mode recovery_mode)
{
	struct fw_exec_struct {
		uint8_t subcmd;
		uint8_t recovery_mode;
		uint8_t rsvd[2];
	} cmd;

	memset(&cmd, 0, sizeof(cmd));
	cmd.subcmd = MRPC_FW_TX_EXEC;
	cmd.recovery_mode = recovery_mode;

	return switchtec_mfg_cmd(dev, MRPC_FW_TX, &cmd, sizeof(cmd), NULL, 0);
}

/**
 * @brief Resume device boot.
 *        Note that after calling this function, the current
 *        'dev' pointer is no longer valid. Before making further
 *        calls to switchtec library functions, be sure to close
 *        this pointer and get a new one by calling switchtec_open().
 *        Also be sure to check the return value of switchtec_open()
 *        for error, as the device might not be immediately
 *        accessible after normal boot process.
 * @param[in] dev Switchtec device handle
 * @return 0 on success, error code on failure
 */
int switchtec_boot_resume(struct switchtec_dev *dev)
{
	return switchtec_mfg_cmd(dev, MRPC_BOOTUP_RESUME, NULL, 0,
				 NULL, 0);
}

/**
 * @brief Set device secure state
 * @param[in]  dev	Switchtec device handle
 * @param[in]  state	Secure state
 * @return 0 on success, error code on failure
 */
int switchtec_secure_state_set(struct switchtec_dev *dev,
			       enum switchtec_secure_state state)
{
	uint32_t data;

	if ((state != SWITCHTEC_INITIALIZED_UNSECURED)
	   && (state != SWITCHTEC_INITIALIZED_SECURED)) {
		return ERR_PARAM_INVALID;
	}
	data = htole32(state);

	return switchtec_mfg_cmd(dev, MRPC_SECURE_STATE_SET, &data,
				 sizeof(data), NULL, 0);
}

/**
 * @brief Send the public key to unlock the device.
 *
 * @param dev [in] Pointer to the switchtec device structure.
 * @param public_key [in] Pointer to the structure containing the public key.
 *
 * @return 0 on success, error code on failure
 */
static int dbg_unlock_send_pubkey(struct switchtec_dev *dev,
				  struct switchtec_pubkey *public_key)
{
	struct public_key_cmd {
		uint8_t subcmd;
		uint8_t rsvd[3];
		uint8_t pub_key[SWITCHTEC_PUB_KEY_LEN];
		uint32_t pub_key_exp;
	} cmd = {};

	cmd.subcmd = MRPC_DBG_UNLOCK_PKEY;
	memcpy(cmd.pub_key, public_key->pubkey, SWITCHTEC_PUB_KEY_LEN);
	cmd.pub_key_exp = htole32(public_key->pubkey_exp);

	return switchtec_mfg_cmd(dev, MRPC_DBG_UNLOCK, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Unlock firmware debug features
 * @param[in]  dev		Switchtec device handle
 * @param[in]  serial		Device serial number
 * @param[in]  ver_sec_unlock	Secure unlock version
 * @param[in]  public_key	public key data
 * @param[in]  signature	Signature of data sent
 * @return 0 on success, error code on failure
 */
int switchtec_dbg_unlock(struct switchtec_dev *dev, uint32_t serial,
			 uint32_t ver_sec_unlock,
			 struct switchtec_pubkey *public_key,
			 struct switchtec_signature *signature)
{
	int ret;
	struct unlock_cmd {
		uint8_t subcmd;
		uint8_t rsvd[3];
		uint32_t serial;
		uint32_t unlock_ver;
		uint8_t signature[SWITCHTEC_SIG_LEN];
	} cmd = {};

	ret = dbg_unlock_send_pubkey(dev, public_key);
	if (ret)
		return ret;

	cmd.subcmd = MRPC_DBG_UNLOCK_DATA;
	cmd.serial = htole32(serial);
	cmd.unlock_ver = htole32(ver_sec_unlock);
	memcpy(cmd.signature, signature->signature, SWITCHTEC_SIG_LEN);

	return switchtec_mfg_cmd(dev, MRPC_DBG_UNLOCK, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Update firmware debug secure unlock version number
 * @param[in]  dev		Switchtec device handle
 * @param[in]  serial		Device serial number
 * @param[in]  ver_sec_unlock	New secure unlock version
 * @param[in]  public_key	public key data
 * @param[in]  signature	Signature of data sent
 * @return 0 on success, error code on failure
 */
int switchtec_dbg_unlock_version_update(struct switchtec_dev *dev,
					uint32_t serial,
					uint32_t ver_sec_unlock,
					struct switchtec_pubkey *public_key,
			 		struct switchtec_signature *signature)
{
	int ret;
	struct update_cmd {
		uint8_t subcmd;
		uint8_t rsvd[3];
		uint32_t serial;
		uint32_t unlock_ver;
		uint8_t signature[SWITCHTEC_SIG_LEN];
	} cmd = {};

	ret = dbg_unlock_send_pubkey(dev, public_key);
	if (ret)
		return ret;

	cmd.subcmd = MRPC_DBG_UNLOCK_UPDATE;
	cmd.serial = htole32(serial);
	cmd.unlock_ver = htole32(ver_sec_unlock);
	memcpy(cmd.signature, signature->signature, SWITCHTEC_SIG_LEN);

	return switchtec_mfg_cmd(dev, MRPC_DBG_UNLOCK, &cmd, sizeof(cmd),
				 NULL, 0);
}

/**
 * @brief Read security settings from config file
 * @param[in]  dev		Switchtec device handle
 * @param[in]  setting_file	Security setting file
 * @param[out] set		Security settings
 * @return 0 on success, error code on failure
 */
int switchtec_read_sec_cfg_file(struct switchtec_dev *dev,
				FILE *setting_file,
				struct switchtec_security_cfg_set *set)
{
	ssize_t rlen;
	char magic[4] = {'S', 'S', 'F', 'F'};
	uint32_t crc;
	struct setting_file_header {
		uint8_t magic[4];
		uint32_t version;
		uint8_t hw_gen;
		uint8_t rsvd[3];
		uint32_t crc;
	};
	struct setting_file_data {
		uint64_t cfg;
		uint32_t pub_key_exponent;
		uint8_t rsvd[36];
	};
	struct setting_file {
		struct setting_file_header header;
		struct setting_file_data data;
	} file_data;
	struct get_cfgs_reply reply;
	uint32_t addr_shift;
	uint32_t map_shift;
	uint32_t map_mask;
	enum switchtec_gen gen;
	int spi_clk;
	int ret;
	int otp_valid;

	ret = get_configs(dev, &reply, &otp_valid);
	if (ret)
		return ret;

	rlen = fread(&file_data, 1, sizeof(file_data), setting_file);

	if (rlen < sizeof(file_data))
		return -EBADF;

	if (memcmp(file_data.header.magic, magic, sizeof(magic)))
		return -EBADF;

	crc = crc32((uint8_t*)&file_data.data,
			sizeof(file_data.data), 0, 1, 1);
	if (crc != le32toh(file_data.header.crc))
		return -EBADF;
	switch (file_data.header.hw_gen) {
	case 0:
		gen = SWITCHTEC_GEN4;
		break;
	case 1:
		gen = SWITCHTEC_GEN5;
		break;
	default:
		return -EBADF;
	}

	if (gen != switchtec_gen(dev))
		return -ENODEV;

	memset(set, 0, sizeof(struct switchtec_security_cfg_set));
	file_data.data.cfg = le64toh(file_data.data.cfg);

	spi_clk = (file_data.data.cfg >> SWITCHTEC_CLK_RATE_BITSHIFT) &
		SWITCHTEC_CLK_RATE_BITMASK;
	if (reply.spi_core_clk_high)
		set->spi_clk_rate = spi_clk_hi_rate_float[spi_clk - 1];
	else
		set->spi_clk_rate = spi_clk_rate_float[spi_clk];

	set->i2c_recovery_tmo =
		(file_data.data.cfg >> SWITCHTEC_RC_TMO_BITSHIFT) &
		SWITCHTEC_RC_TMO_BITMASK;
	set->i2c_port =
		(file_data.data.cfg >> SWITCHTEC_I2C_PORT_BITSHIFT) &
		SWITCHTEC_I2C_PORT_BITMASK;

	get_i2c_operands(gen, &addr_shift, &map_shift, &map_mask);
	set->i2c_addr =
		(file_data.data.cfg >> addr_shift) &
		SWITCHTEC_I2C_ADDR_BITMASK;
	set->i2c_cmd_map = (file_data.data.cfg >> map_shift) & map_mask;

	set->public_key_exponent = le32toh(file_data.data.pub_key_exponent);

	return 0;
}

/**
 * @brief Set and send the public key to the device.
 *
 * @param[in] dev Pointer to the switchtec device structure.
 * @param[in] public_key Pointer to the structure containing the public key.
 *
 * @return 0 on success, or a negative error code on failure.
 */
static int kmsk_set_send_pubkey(struct switchtec_dev *dev,
				struct switchtec_pubkey *public_key)
{
	struct kmsk_pubk_cmd {
		uint8_t subcmd;
		uint8_t reserved[3];
		uint8_t pub_key[SWITCHTEC_PUB_KEY_LEN];
		uint32_t pub_key_exponent;
	} cmd = {};

	cmd.subcmd = MRPC_KMSK_ENTRY_SET_PKEY;
	memcpy(cmd.pub_key, public_key->pubkey,
	       SWITCHTEC_PUB_KEY_LEN);
	cmd.pub_key_exponent = htole32(public_key->pubkey_exp);

	return switchtec_mfg_cmd(dev, MRPC_KMSK_ENTRY_SET, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Set the KMSK send signature.
 *
 * @param[in] dev Pointer to the switchtec device structure.
 * @param[in] signature Pointer to the switchtec signature structure.
 *
 * @return 0 on success, or a negative error code on failure.
 */
static int kmsk_set_send_signature(struct switchtec_dev *dev,
				   struct switchtec_signature *signature)
{
	struct kmsk_signature_cmd {
		uint8_t subcmd;
		uint8_t reserved[3];
		uint8_t signature[SWITCHTEC_SIG_LEN];
	} cmd = {};

	cmd.subcmd = MRPC_KMSK_ENTRY_SET_SIG;
	memcpy(cmd.signature, signature->signature,
	       SWITCHTEC_SIG_LEN);

	return switchtec_mfg_cmd(dev, MRPC_KMSK_ENTRY_SET, &cmd,
				 sizeof(cmd), NULL, 0);
}

/**
 * @brief Set the KMSK (Key Management Security Key) for the device.
 *
 * @param[in] dev Pointer to the switchtec device structure.
 * @param[in] kmsk Pointer to the switchtec KMSK structure containing the key.
 *
 * @return 0 on success, or a negative error code on failure.
 */
static int kmsk_set_send_kmsk(struct switchtec_dev *dev,
			      struct switchtec_kmsk *kmsk)
{
	struct kmsk_kmsk_cmd {
		uint8_t subcmd;
		uint8_t num_entries;
		uint8_t reserved[2];
		uint8_t kmsk[SWITCHTEC_KMSK_LEN];
	} cmd = {};

	cmd.subcmd = MRPC_KMSK_ENTRY_SET_KMSK;
	cmd.num_entries = 1;
	memcpy(cmd.kmsk, kmsk->kmsk, SWITCHTEC_KMSK_LEN);

	return switchtec_mfg_cmd(dev, MRPC_KMSK_ENTRY_SET, &cmd, sizeof(cmd),
				 NULL, 0);
}

/**
 * @brief Set KMSK entry
 * 	  KMSK stands for Key Manifest Secure Key.
 * 	  It is a key used to verify Key Manifest
 * 	  partition, which contains keys to verify
 * 	  all other partitions.
 * @param[in]  dev		Switchtec device handle
 * @param[in]  public_key	Public key
 * @param[in]  signature	Signature
 * @param[in]  kmsk		KMSK entry data
 * @return 0 on success, error code on failure
 */
int switchtec_kmsk_set(struct switchtec_dev *dev,
		       struct switchtec_pubkey *public_key,
		       struct switchtec_signature *signature,
		       struct switchtec_kmsk *kmsk)
{
	int ret;

	if (public_key) {
		ret = kmsk_set_send_pubkey(dev, public_key);
		if (ret)
			return ret;
	}

	if (signature) {
		ret = kmsk_set_send_signature(dev, signature);
		if (ret)
			return ret;
	}

	return kmsk_set_send_kmsk(dev, kmsk);
}

#if HAVE_LIBCRYPTO
/**
 * @brief Read public key from public key file
 * @param[in]  pubk_file Public key file
 * @param[out] pubk	 Public key
 * @return 0 on success, error code on failure
 */
int switchtec_read_pubk_file(FILE *pubk_file, struct switchtec_pubkey *pubk)
{
	RSA *RSAKey = NULL;
	const BIGNUM *modulus_bn;
	const BIGNUM *exponent_bn;
	uint32_t exponent_tmp = 0;

	RSAKey = PEM_read_RSA_PUBKEY(pubk_file, NULL, NULL, NULL);
	if (RSAKey == NULL) {
		fseek(pubk_file, 0L, SEEK_SET);
		RSAKey = PEM_read_RSAPrivateKey(pubk_file, NULL, NULL, NULL);
		if (RSAKey == NULL)
			return -1;
	}

	RSA_get0_key(RSAKey, &modulus_bn, &exponent_bn, NULL);

	BN_bn2bin(modulus_bn, pubk->pubkey);
	BN_bn2bin(exponent_bn, (uint8_t *)&exponent_tmp);

	pubk->pubkey_exp = be32toh(exponent_tmp);
	RSA_free(RSAKey);

	return 0;
}
#endif


/**
 * @brief Read KMSK data from KMSK file
 * @param[in]  kmsk_file KMSK file
 * @param[out] kmsk      KMSK entry data
 * @return 0 on success, error code on failure
 */
int switchtec_read_kmsk_file(FILE *kmsk_file, struct switchtec_kmsk *kmsk)
{
	ssize_t rlen;
	struct kmsk_struct {
		uint8_t magic[4];
		uint32_t version;
		uint32_t reserved;
		uint32_t crc32;
		uint8_t kmsk[SWITCHTEC_KMSK_LEN];
	} data;

	char magic[4] = {'K', 'M', 'S', 'K'};
	uint32_t crc;

	rlen = fread(&data, 1, sizeof(data), kmsk_file);

	if (rlen < sizeof(data))
		return -EBADF;

	if (memcmp(data.magic, magic, sizeof(magic)))
		return -EBADF;

	crc = crc32(data.kmsk, SWITCHTEC_KMSK_LEN, 0, 1, 1);
	if (crc != le32toh(data.crc32))
		return -EBADF;

	memcpy(kmsk->kmsk, data.kmsk, SWITCHTEC_KMSK_LEN);

	return 0;
}

/**
 * @brief Read signature data from signature file
 * @param[in]  sig_file  Signature file
 * @param[out] signature Signature data
 * @return 0 on success, error code on failure
 */
int switchtec_read_signature_file(FILE *sig_file,
				  struct switchtec_signature *signature)
{
	ssize_t rlen;

	rlen = fread(signature->signature, 1, SWITCHTEC_SIG_LEN, sig_file);

	if (rlen < SWITCHTEC_SIG_LEN)
		return -EBADF;

	return 0;
}
/**
 * @brief Read UUID data from UUID file
 * @param[in]  uuid_file  UUID file
 * @param[out] uuid       UUID data
 * @return 0 on success, error code on failure
 */
int switchtec_read_uuid_file(FILE *uuid_file,
				  struct switchtec_uuid *uuid)
{
	ssize_t rlen;

	rlen = fread(uuid->uuid_data, 1, OTP_SJTAG_UUID_LENGTH, uuid_file);

	if (rlen < OTP_SJTAG_UUID_LENGTH)
		return -EBADF;

	return 0;
}

/**
 * @brief Read SJTAG key data from SJTAG file
 * @param[in]  sjtag_file  SJTAG file
 * @param[out] sjtag_key   SJTAG key data
 * @return 0 on success, error code on failure
 */
int switchtec_read_sjtag_key_file(FILE *sjtag_file,
				  struct otp_sjtag_key *sjtag_key)
{
	ssize_t rlen;

	rlen = fread(sjtag_key->sjtag_key, 1, OTP_SJTAG_KEY_LENGTH, sjtag_file);

	if (rlen < OTP_SJTAG_KEY_LENGTH)
		return -EBADF;

	return 0;
}

/**
 * @brief Read OTP Config data from bin file
 * @param[in]  config_file  OTP Config File
 * @param[out] config_data  Array of Config data
 * @param[in]  config_len   Length of the config data
 * @return 0 on success, error code on failure
 */
int switchtec_read_otp_config_data_file(FILE *config_file,
				  struct otp_config_data *config_data,
				  unsigned short config_len)
{
	ssize_t rlen;
	rlen = fread((void *)config_data->config_data, 1, config_len, config_file);

	if (rlen < config_len)
		return -EBADF;
		
	return 0;
}

/**
 * @brief Read Self Test Policy from bin file
 * @param[in]  self_test_file  Self test policy file
 * @param[out] otp_self_test_policy Self Test Policy Structure
 * @return 0 on success, error code on failure
 */
int switchtec_read_self_test_policy_file(FILE *self_test_file,
				  struct otp_self_test_policy *otp_self_test_policy)
{
	ssize_t rlen;

	rlen = fread(otp_self_test_policy->otp_self_test_policy, 1, OTP_SELF_TEST_POLICY_LENGTH, self_test_file);

	if (rlen < OTP_SELF_TEST_POLICY_LENGTH)
		return -EBADF;

	return 0;
}

/**
 * @brief Read Die Trace from die trace bin file
 * @param[in]  die_trace_file  Die Trace file
 * @param[out] otp_die_trace_ptr Pointer to otp_die_trace structure
 * @return 0 on success, error code on failure
 */
int switchtec_read_die_trace_file(FILE *die_trace_file,
				  struct otp_die_trace *otp_die_trace_ptr)
{
	ssize_t rlen;

	rlen = fread(otp_die_trace_ptr->die_trace, 1, OTP_DIE_TRACE_LENGTH, die_trace_file);

	if (rlen < OTP_DIE_TRACE_LENGTH)
		return -EBADF;

	return 0;
}


/**
 * @brief Check if secure config already has a KMSK entry
 * 	  KMSK stands for Key Manifest Secure Key.
 * 	  It is a key used to verify Key Manifest
 * 	  partition, which contains keys used to
 * 	  verify all other partitions.
 * @param[in]  state  Secure config
 * @param[out] kmsk   KMSK entry to check for
 * @return 0 on success, error code on failure
 */
int switchtec_security_state_has_kmsk(struct switchtec_security_cfg_state *state,
				  struct switchtec_kmsk *kmsk)
{
	int key_idx;

	for(key_idx = 0; key_idx < state->public_key_num; key_idx++) {
		if (memcmp(state->public_key[key_idx], kmsk->kmsk,
			   SWITCHTEC_KMSK_LEN) == 0)
			return 1;
	}

	return 0;
}

#endif /* __linux__ */

/**
 * @brief Execute a manufacturing command on the Switchtec device.
 *
 * @param[in] dev Pointer to the Switchtec device structure.
 * @param[in] cmd Command to be executed.
 * @param[in] payload Pointer to the payload data.
 * @param[in] payload_len Length of the payload data.
 * @param[out] resp Pointer to the response buffer.
 * @param[in] resp_len Length of the response buffer.
 *
 * @return 0 on success, -1 on failure with errno set.
 */
static int switchtec_mfg_cmd(struct switchtec_dev *dev, uint32_t cmd,
			     const void *payload, size_t payload_len,
			     void *resp, size_t resp_len)
{
	if (dev->ops->flags & SWITCHTEC_OPS_FLAG_NO_MFG) {
		errno = ERR_UART_NOT_SUPPORTED | SWITCHTEC_ERRNO_MRPC_FLAG_BIT;
		return -1;
	}
	return switchtec_cmd(dev, cmd, payload, payload_len,
			     resp, resp_len);
}

/**
 * @brief Get serial number and security version
 * @param[in]  dev	Switchtec device handle
 * @param[out] info	Serial number and security version info
 * @return 0 on success, error code on failure
 */
int switchtec_sn_ver_get(struct switchtec_dev *dev,
			 struct switchtec_sn_ver_info *info)
{
	int ret;

	ret = switchtec_mfg_cmd(dev, MRPC_SN_VER_GET, NULL, 0, info,
			sizeof(struct switchtec_sn_ver_info));
	if (ret)
		return ret;

	info->chip_serial = le32toh(info->chip_serial);
	info->ver_bl2 = le32toh(info->ver_bl2);
	info->ver_km = le32toh(info->ver_km);
	info->ver_main = le32toh(info->ver_main);
	info->ver_sec_unlock = le32toh(info->ver_sec_unlock);
	info->customer_id = le16toh(info->customer_id);

	return 0;
}


/**@}*/
