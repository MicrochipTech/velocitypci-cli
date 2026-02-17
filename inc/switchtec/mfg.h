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

#ifndef LIBSWITCHTEC_MFG_H
#define LIBSWITCHTEC_MFG_H

/* Bit value for strap configurations in PCI100x */
#define SPI_QUAD_MODE           (0U)
#define INLINE_ECC_MODE         (1U)
#define SPI_3B_4B_MODE          (8U)
#define XIP_MODE                (9U)
#define WATCHDOG_EN             (10U)
#define TWI_RCVRY_ADDR0         (12U)
#define TWI_RCVRY_ADDR1         (13U)
#define SEC_BOOT_DRY_RUN        (14U)
#define PE_MODE_EN              (15U)
#define MEMBIST_EN              (16U)
#define MEMBIST_CPU_RESET       (17U)

#define PCI100X_STRAP_STAT(strap, bit) ((((strap >> bit) & 0x01) == 0x01) ? "Enabled" : "Disabled")

#define SWITCHTEC_MB_LOG_LEN	32

#define SWITCHTEC_PUB_KEY_LEN	512
#define SWITCHTEC_SIG_LEN	512
#define SWITCHTEC_KMSK_LEN	64
#define SWITCHTEC_KMSK_NUM	4

#define SWITCHTEC_SECURITY_SPI_RATE_MAX_NUM	16

#define OTP_SJTAG_UUID_LENGTH 10
#define OTP_SJTAG_KEY_LENGTH 16
#define OTP_SELF_TEST_POLICY_LENGTH 4
#define OTP_DIE_TRACE_LENGTH 16
#define OTP_CONFIG_DATA_MAX_LEN 1020

#define OTP_CONFIG_START		0x1800
#define OTP_MAX_SIZE			0x3FFF

#define SKU_NO_OF_BLOCKS		4

#define SJTAG_SERVER_HEADER_LEN (4)
#define SJTAG_IDCODE_LEN		(4)
#define SJTAG_HR_LEN 			(32)
#define SJTAG_DEBUG_TOKEN_LEN	(32)
#define SJTAG_HMAC_MSG_LEN		(22)
#define SJTAG_UUID_LEN			(10)
#define SJTAG_UNLOCK_STR_LEN	(18)
#define SUV_LEN				    (4)
#define SJTAG_NONCE_LEN			(32)
#define SJTAG_SHA256_MSG_LEN	(SJTAG_DEBUG_TOKEN_LEN + SJTAG_NONCE_LEN)

/*SJTAG Server Header index */
#define SJTAG_SERVER_CMD_INDEX          (0)
#define SJTAG_SERVER_CMD_RESP_INDICATOR_INDEX   (1)
#define SJTAG_SERVER_RESPONSE_STATUS_INDEX      (2)

/* SJTAG Server Header command masks */
#define SJTAG_SERVER_CMD_MSK            (0x00)
#define SJTAG_SERVER_RESP_MSK           (0x40)

/* SJTAG Server Header Commands*/
#define SJTAG_SERVER_CMD_CHAL_RESP      (0x01)

/* Responses from the SJTAG Server */
#define SERVER_STATUS_SUCCESS           (0x00)
#define SERVER_STATUS_INVALID_SJTAGID   (0x02)
#define SERVER_STATUS_INVALID_HEADER    (0x01)
#define SERVER_STATUS_DIGEST_COMPUTE_FAIL (0x03)

#define MAX_LENGTH 256

#define KEY_BUF_SIZE		(16*1024)
#define CONFIG_SECTION_NAME	"OTPGEN_CONFIG"


#define KEY_XDATA_WRITE		"XWRE_"
#define KEY_XDATA_SET		"XSET_"
#define KEY_XDATA_CLEAR		"XCLR_"
#define KEY_PATCH_CODE      "CODE_"

#define KEY_ID_SIZE			5
#define SET_XDATA_ADDRESS			0x80

#define	TV_TAG_SKIP_CMD_MASK		0x80
#define	TV_TAG_SKIP_LEN_MASK_		0x7F
#define	TV_TAG_DATA_LEN_MASK_		0x7F

#define CUSTOM_COMMAND				0xFE
#define STOP						0xFF
#define SET_XMODE_NONE				0x55

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;

#define FALSE               0
#define TRUE                1

struct switchtec_sn_ver_info {
	uint32_t chip_serial;
	uint32_t ver_km;
	uint32_t ver_bl2;
	uint32_t ver_main;
	uint32_t ver_sec_unlock;
	uint16_t customer_id;
};
enum switchtec_debug_mode {
	SWITCHTEC_DEBUG_MODE_ENABLED,
	SWITCHTEC_DEBUG_MODE_DISABLED_BUT_ENABLE_ALLOWED,
	SWITCHTEC_DEBUG_MODE_DISABLED,
	SWITCHTEC_DEBUG_MODE_DISABLED_EXT
};

enum switchtec_secure_state {
	SWITCHTEC_UNINITIALIZED_UNSECURED,
	SWITCHTEC_INITIALIZED_UNSECURED,
	SWITCHTEC_INITIALIZED_SECURED,
	SWITCHTEC_SECURE_STATE_UNKNOWN = 0xff,
};

/**
 * @brief Flag which indicates if an OTP region is programmable or not
 */
enum switchtec_otp_program_status {
	SWITCHTEC_OTP_PROGRAMMABLE = 0,
	SWITCHTEC_OTP_UNPROGRAMMABLE = 1,
};

struct switchtec_security_cfg_otp_region {
	bool basic_valid;
	bool mixed_ver_valid;
	bool main_fw_ver_valid;
	bool sec_unlock_ver_valid;
	bool kmsk_valid[4];
	enum switchtec_otp_program_status basic;
	enum switchtec_otp_program_status mixed_ver;
	enum switchtec_otp_program_status main_fw_ver;
	enum switchtec_otp_program_status sec_unlock_ver;
	enum switchtec_otp_program_status kmsk[4];
};

struct switchtec_security_cfg_state {
	uint8_t basic_setting_valid;
	uint8_t public_key_num_valid;
	uint8_t public_key_ver_valid;
	uint8_t kmsk_valid;

	enum switchtec_secure_state secure_state;

	float spi_clk_rate;
	uint32_t i2c_recovery_tmo;
	uint32_t i2c_port;
	uint32_t i2c_addr;
	uint32_t i2c_cmd_map;
	uint32_t public_key_exponent;
	uint32_t public_key_num;
	uint32_t public_key_ver;

	uint8_t public_key[SWITCHTEC_KMSK_NUM][SWITCHTEC_KMSK_LEN];

	bool otp_valid;
	struct switchtec_security_cfg_otp_region otp;
};

struct switchtec_security_cfg_set {
	float spi_clk_rate;
	uint32_t i2c_recovery_tmo;
	uint32_t i2c_port;
	uint32_t i2c_addr;
	uint32_t i2c_cmd_map;
	uint32_t public_key_exponent;
};

enum switchtec_active_index_id {
	SWITCHTEC_ACTIVE_INDEX_0 = 0,
	SWITCHTEC_ACTIVE_INDEX_1 = 1,
	SWITCHTEC_ACTIVE_INDEX_NOT_SET = 0xfe
};

struct switchtec_active_index {
	enum switchtec_active_index_id bl2;
	enum switchtec_active_index_id firmware;
	enum switchtec_active_index_id config;
	enum switchtec_active_index_id keyman;
};

enum switchtec_bl2_recovery_mode {
	SWITCHTEC_BL2_RECOVERY_I2C = 1,
	SWITCHTEC_BL2_RECOVERY_XMODEM = 2,
	SWITCHTEC_BL2_RECOVERY_I2C_AND_XMODEM = 3
};

struct switchtec_kmsk {
	uint8_t kmsk[SWITCHTEC_KMSK_LEN];
};

struct switchtec_pubkey {
	uint8_t pubkey[SWITCHTEC_PUB_KEY_LEN];
	uint32_t pubkey_exp;
};

struct otp_sku_info_set {
	uint8_t block_num;
	uint8_t sku_info;
};

struct otp_sku_info_get {
	uint8_t sku_info[SKU_NO_OF_BLOCKS];
	uint8_t write_protected:1;
	uint8_t valid_flags:SKU_NO_OF_BLOCKS;
	uint8_t rsvd:3;
};

struct sjtag_idcode_get {
	uint8_t idcode[SJTAG_IDCODE_LEN];
};

struct sjtag_status_get {
	uint32_t data;
};

struct sjtag_uuid {
	uint8_t uuid[SJTAG_UUID_LEN];
};

struct sjtag_nonce {
	uint8_t nonce[SJTAG_NONCE_LEN];
};

struct sjtag_debug_token{
	uint8_t debug_token[SJTAG_DEBUG_TOKEN_LEN];
};

struct switchtec_uuid{
	uint8_t uuid_data[OTP_SJTAG_UUID_LENGTH];
};

struct otp_sjtag_key{
	uint8_t sjtag_key[OTP_SJTAG_KEY_LENGTH];
};

struct otp_config_data{
	uint8_t config_data[OTP_CONFIG_DATA_MAX_LEN];
};

struct otp_self_test_policy{
	uint8_t otp_self_test_policy[OTP_SELF_TEST_POLICY_LENGTH];
};

struct otp_die_trace {
	uint8_t die_trace[OTP_DIE_TRACE_LENGTH];
};

#pragma pack(push,1)
struct otp_mode_status{
		struct otp_self_test_policy otp_self_test_policy;
		uint32_t bit_byte_mode:1;
		uint32_t smart_prog_mode:1;
		uint32_t rsvd:30;
		struct otp_die_trace otp_die_trace;
};
#pragma pack(pop)

struct switchtec_signature{
	uint8_t signature[SWITCHTEC_SIG_LEN];
};

struct switchtec_security_spi_avail_rate {
	int num_rates;
	float rates[SWITCHTEC_SECURITY_SPI_RATE_MAX_NUM];
};
int switchtec_sn_ver_get(struct switchtec_dev *dev,
			 struct switchtec_sn_ver_info *info);
int switchtec_security_config_get(struct switchtec_dev *dev,
			          struct switchtec_security_cfg_state *state);
int switchtec_security_spi_avail_rate_get(struct switchtec_dev *dev,
		struct switchtec_security_spi_avail_rate *rates);
int switchtec_security_config_set(struct switchtec_dev *dev,
				  struct switchtec_security_cfg_set *setting);
int switchtec_mailbox_to_file(struct switchtec_dev *dev, int fd);
int switchtec_active_image_index_get(struct switchtec_dev *dev,
				     struct switchtec_active_index *index);
int switchtec_active_image_index_set(struct switchtec_dev *dev,
				     struct switchtec_active_index *index);
int switchtec_fw_exec(struct switchtec_dev *dev,
		      enum switchtec_bl2_recovery_mode recovery_mode);
int switchtec_boot_resume(struct switchtec_dev *dev);
int switchtec_kmsk_set(struct switchtec_dev *dev,
		       struct switchtec_pubkey *public_key,
		       struct switchtec_signature *signature,
		       struct switchtec_kmsk *kmsk);
int switchtec_secure_state_set(struct switchtec_dev *dev,
			       enum switchtec_secure_state state);
int switchtec_dbg_unlock(struct switchtec_dev *dev, uint32_t serial,
			 uint32_t ver_sec_unlock,
			 struct switchtec_pubkey *public_key,
			 struct switchtec_signature *signature);
int switchtec_dbg_unlock_version_update(struct switchtec_dev *dev,
					uint32_t serial,
					uint32_t ver_sec_unlock,
					struct switchtec_pubkey *public_key,
			 		struct switchtec_signature *signature);
int switchtec_read_sec_cfg_file(struct switchtec_dev *dev,
				FILE *setting_file,
				struct switchtec_security_cfg_set *set);
int switchtec_read_pubk_file(FILE *pubk_file, struct switchtec_pubkey *pubk);
int switchtec_read_kmsk_file(FILE *kmsk_file, struct switchtec_kmsk *kmsk);
int switchtec_read_signature_file(FILE *sig_file,
				  struct switchtec_signature *sigature);
int switchtec_read_uuid_file(FILE *uuid_file,
				  struct switchtec_uuid *uuid_data);
int switchtec_read_sjtag_key_file(FILE *sjtag_file,
				  struct otp_sjtag_key *sjtag_key);
int switchtec_read_self_test_policy_file(FILE *self_test_file,
				  struct otp_self_test_policy *otp_self_test_policy);
int switchtec_read_die_trace_file(FILE *die_trace_file,
				  struct otp_die_trace *otp_die_trace_ptr);
int
switchtec_security_state_has_kmsk(struct switchtec_security_cfg_state *state,
				  struct switchtec_kmsk *kmsk);

int switchtec_otp_sku_info_set(struct switchtec_dev *dev,
				struct otp_sku_info_set *sku_info_set_str);
int switchtec_otp_regs_write_protect(struct switchtec_dev *dev,
				bool sku_info, bool otp_config);
int switchtec_chip_serial_num_prog(struct switchtec_dev *dev,
				uint32_t chip_serial_num);
int switchtec_otp_sku_info_get(struct switchtec_dev *dev,
				struct otp_sku_info_get *sku_info_get_str);

int switchtec_otp_sjtag_mode_set(struct switchtec_dev *dev,
										uint8_t sjtag_mode);
int switchtec_otp_sjtag_uuid_set(struct switchtec_dev *dev,
										uint8_t *sjtag_uuid);
int switchtec_otp_sjtag_key_set(struct switchtec_dev *dev,
										uint8_t *sjtag_key);
int switchtec_otp_sjtag_key_verify(struct switchtec_dev *dev,
										uint8_t *sjtag_key, uint8_t *is_verified);
int switchtec_otp_sjtag_key_lock(struct switchtec_dev *dev);
int switchtec_otp_self_test_policy_set(struct switchtec_dev *dev, uint8_t *self_test_policy);
int switchtec_otp_die_trace_prog(struct switchtec_dev *dev, uint8_t *die_trace);
int switchtec_otp_bit_byte_prog_mode_set(struct switchtec_dev *dev, bool bit_byte_mode);
int switchtec_otp_smart_prog_mode_set(struct switchtec_dev *dev,bool smart_prog_mode);
int switchtec_otp_mode_status_get(struct switchtec_dev *dev, struct otp_mode_status *otp_mode_status_str);
int switchtec_custid_prog(struct switchtec_dev *dev, uint16_t cust_id);
int switchtec_otp_config_prog(struct switchtec_dev *dev,
							  uint16_t config_len,
							  uint8_t *config_data,
							  bool is_firmware_halt);
int switchtec_otp_config_read(struct switchtec_dev *dev,
							  uint16_t config_start_address,
							  int32_t config_length,
							  int out_fd);
int switchtec_read_otp_config_data_file(FILE *config_file,
				  struct otp_config_data *config_data,
				  unsigned short config_len);
int switchtec_sjtag_status_get(struct switchtec_dev *dev,
							   struct sjtag_status_get *sjtag_sts_get);

int switchtec_sjtag_get_uuid_idcode(struct switchtec_dev *dev,
									uint8_t *sjtag_uuid_ptr,
									uint8_t *sjtag_idcode);

int switchtec_sjtag_get_nonce(struct switchtec_dev *dev,
                                struct sjtag_nonce *sjtag_nonce_ptr);

int switchtec_sjtag_hr_send(struct switchtec_dev *dev,
										uint8_t *sjtag_hr);

int switchtec_read_sjtag_debug_token_file(FILE *debug_token_file, struct sjtag_debug_token *debug_token);
void sjtag_hr_calc(uint8_t *sjtag_debug_token, uint8_t *sjtag_nonce, uint8_t *digest, bool verbose);
int sjtag_debug_token_gen(uint8_t *idcode_ptr, uint8_t *uuid_ptr, uint8_t *suv_ptr, uint8_t *sjtag_debug_token, const char *server_ip, uint16_t server_port, bool verbose);
void sjtag_hr_compute(uint8_t *sjtag_uuid, uint8_t *sjtag_suv, uint8_t *sjtag_nonce, uint8_t *sjtag_hr);



#endif // LIBSWITCHTEC_MFG_H
