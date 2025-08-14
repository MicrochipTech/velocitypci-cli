/*
 * Microsemi Switchtec(tm) PCIe Management Command Line Interface
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

#ifdef __linux__

#include "commands.h"
#include "argconfig.h"
#include "suffix.h"
#include "progress.h"
#include "gui.h"
#include "common.h"
#include "progress.h"

#include "config.h"

#include <switchtec/switchtec.h>
#include <switchtec/utils.h>
#include <switchtec/mfg.h>
#include <switchtec/ini.h>
#include <switchtec/endian.h>

#include <locale.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <lib/switchtec_priv.h>

#define PCI100X_DEV_FAM     0

#define PING_SPI_CLK_SEL_MASK       0x30000
#define PING_SPI_CLK_SEL_OFFSET     16
#define PING_SPI_BUS_RATE_MASK      0xFFFF

static const struct argconfig_choice recovery_mode_choices[] = {
	{"I2C", SWITCHTEC_BL2_RECOVERY_I2C, "I2C"},
	{"XMODEM", SWITCHTEC_BL2_RECOVERY_XMODEM, "XModem"},
	{"BOTH", SWITCHTEC_BL2_RECOVERY_I2C_AND_XMODEM,
		"both I2C and XModem (default)"},
	{}
};

static const struct argconfig_choice secure_state_choices[] = {
	{"INITIALIZED_UNSECURED", SWITCHTEC_INITIALIZED_UNSECURED,
		"unsecured state"},
	{"INITIALIZED_SECURED", SWITCHTEC_INITIALIZED_SECURED,
		"secured state"},
	{}
};

static const char* parse_recovery_reason(enum stmfd_rcvry_reason phase_id)
{
    const char * reason_str[] = {   "Unknown Phase",
                                    "BL1 - Strap Assertion",
                                    "BL1 - Firmware Assertion",
                                    "BL2 - Strap Assertion",
                                    "BL2 - Firmware Assertion",
                                    "BL2 - Image Execute",
                                    "Main FW" };
    uint8_t reason_idx = 0;

    if ((phase_id >= STMFD_RCVRY_BL1_STRAP) && (phase_id <= STMFD_RCVRY_MAINFW))
    {
        reason_idx = phase_id;
    }

    return reason_str[reason_idx];
}

/**
 * @brief Parses SPI frequency parameters got from ping command and calculates 
 * the clock rate.
 *
 * This function takes the SPI parameters and computes the corresponding
 * SPI clock rate. The result is returned from the function through pointer clk_rate.
 *
 * @param[in] spi_params The SPI parameters used to calculate the clock rate.
 * @param[out] clk_rate Pointer to a float where the calculated clock rate will be stored.
 *
 * @return Returns true if the parsing was successful, false otherwise.
 *
 * @note This function assumes that spi_params contains valid data.
 */
static bool spi_frequency_parser(uint32_t spi_params, float * clk_rate)
{
    float dcsu_lookup[] = {400.0, 501.0, 550.0, 600.0};
    int bus_rate_max[] = {7, 9, 10, 11};
    float dcsu_value = 0;
    int clk_sel = 0;
    int bus_rate = 0;

    clk_sel = (spi_params & PING_SPI_CLK_SEL_MASK) >> PING_SPI_CLK_SEL_OFFSET;

    bus_rate = (spi_params & PING_SPI_BUS_RATE_MASK);
    if (bus_rate > bus_rate_max[clk_sel])
    {
            return false;
    }

    dcsu_value = dcsu_lookup[clk_sel];

    *clk_rate = dcsu_value / (2 * (bus_rate + 1));

    return true;
}

#define CMD_DESC_PING "ping device and get current boot phase"

static int ping(int argc, char **argv)
{
	int ret;
	static struct {
		struct switchtec_dev *dev;
		bool extended_cmd;
	} cfg = {
	.extended_cmd = false};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"extended_cmd", 'x', "", CFG_NONE, &cfg.extended_cmd, no_argument,
		"print extended ping command information"},
		{NULL}
	};
	struct ping_dev_info ping_info;
	char version_str[12];
	float clk_rate;

	argconfig_parse(argc, argv, CMD_DESC_PING, opts, &cfg, sizeof(cfg));

	ret = switchtec_get_device_info(cfg.dev, NULL, NULL, NULL, &ping_info);
	if (ret) {
		switchtec_perror("mfg ping");
		return ret;
	}
	printf("Mfg Ping: \tSUCCESS\n");

	if (ping_info.ping_rev != 0)
	{
        version_to_string(ping_info.fw_version, version_str, sizeof(version_str));

        printf("FW Version: \t%s\n", version_str);
        printf("Device family: \t%s\n", (ping_info.dev_family == PCI100X_DEV_FAM) ? "PCI100x" : "Unknown");

		if (true == spi_frequency_parser(ping_info.spi_freq, &clk_rate))
		{
			printf("SPI Frequency: \t%.2f MHz\n", clk_rate);
		}
		else
		{
			printf("SPI Frequency: \tUnknown [SPI_CLK_SEL: %x, SPI_CLK_RATE: %x]\n", \
                            (ping_info.spi_freq & PING_SPI_CLK_SEL_MASK) >> PING_SPI_CLK_SEL_OFFSET,
                            (ping_info.spi_freq & PING_SPI_BUS_RATE_MASK));
		}

		printf("Ping entry: \t%s\n\n", parse_recovery_reason(ping_info.rcvry_entry_reason));
		printf("Strap status\n-------------\n");
		printf("SPI QUAD MODE:\t\t\t%s\n",          PCI100X_STRAP_STAT(ping_info.strap_stat , SPI_QUAD_MODE));
		printf("INLINE ECC MODE:\t\t%s\n",          PCI100X_STRAP_STAT(ping_info.strap_stat , INLINE_ECC_MODE));
		if (true == cfg.extended_cmd)
		{
			printf("SPI 3B/4B MODE:\t\t\t%s\n",         PCI100X_STRAP_STAT(ping_info.strap_stat , SPI_3B_4B_MODE));
			printf("XIP MODE:\t\t\t%s\n",               PCI100X_STRAP_STAT(ping_info.strap_stat , XIP_MODE));
		}
		printf("WATCHDOG:\t\t\t%s\n",               PCI100X_STRAP_STAT(ping_info.strap_stat , WATCHDOG_EN));
		printf("TWI RECOVERY ADDRESS BIT0:\t%s\n",  PCI100X_STRAP_STAT(ping_info.strap_stat , TWI_RCVRY_ADDR0));
		printf("TWI RECOVERY ADDRESS BIT1:\t%s\n",  PCI100X_STRAP_STAT(ping_info.strap_stat , TWI_RCVRY_ADDR1));
		printf("SECURE BOOT DRY RUN:\t\t%s\n",      PCI100X_STRAP_STAT(ping_info.strap_stat , SEC_BOOT_DRY_RUN));
		printf("PE MODE:\t\t\t%s\n",                PCI100X_STRAP_STAT(ping_info.strap_stat , PE_MODE_EN));
		printf("MEMBIST:\t\t\t%s\n",                PCI100X_STRAP_STAT(ping_info.strap_stat , MEMBIST_EN));
		printf("MEMBIST CPU RESET:\t\t%s\n",        PCI100X_STRAP_STAT(ping_info.strap_stat , MEMBIST_CPU_RESET));
	}

	return 0;
}

static const char* program_status_to_string(enum switchtec_otp_program_status s)
{
	switch(s) {
	case SWITCHTEC_OTP_PROGRAMMABLE:
		return "R/W (Programmable)";
	case SWITCHTEC_OTP_UNPROGRAMMABLE:
		return "R/O (Unprogrammable)";
	default:
		return "Unknown";
	}
}

static void print_security_config(struct switchtec_security_cfg_state *state,
				  struct switchtec_security_cfg_otp_region *otp)
{
	int key_idx;
	int i;

	printf("\nBasic Secure Settings %s\n",
		state->basic_setting_valid? "(Valid)":"(Invalid)");

	printf("\tSecure State: \t\t\t");
	switch(state->secure_state) {
	case SWITCHTEC_UNINITIALIZED_UNSECURED:
		printf("UNINITIALIZED_UNSECURED\n");
		break;
	case SWITCHTEC_INITIALIZED_UNSECURED:
		printf("INITIALIZED_UNSECURED\n");
		break;
	case SWITCHTEC_INITIALIZED_SECURED:
		printf("INITIALIZED_SECURED\n");
		break;
	default:
		printf("Unsupported State\n");
		break;
	}

	printf("\tSPI Clock Rate [In OTP]: \t%.2f MHz\n", state->spi_clk_rate);

	printf("\tI2C Recovery TMO: \t\t%d Second(s)\n",
		state->i2c_recovery_tmo);
	printf("\tI2C Port: \t\t\t%d\n", state->i2c_port);
	printf("\tI2C Address (7-bits): \t\t0x%02x\n", state->i2c_addr);
	printf("\tI2C Command Map: \t\t0x%08x\n\n", state->i2c_cmd_map);

	printf("Exponent Hex Data %s: \t\t0x%08x\n",
		state->basic_setting_valid? "(Valid)":"(Invalid)",
		state->public_key_exponent);

	printf("KMSK Entry Number %s: \t\t%d\n",
		state->public_key_num_valid? "(Valid)":"(Invalid)",
		state->public_key_num);

	if (state->public_key_ver)
		printf("Current KMSK index %s: \t\t%d\n",
			state->public_key_ver_valid? "(Valid)":"(Invalid)",
			state->public_key_ver);
	else
		printf("Current KMSK index %s: \t\tNot Set\n",
			state->public_key_ver_valid? "(Valid)":"(Invalid)");

	for(key_idx = 0; key_idx < state->public_key_num; key_idx++) {
		printf("KMSK Entry %d:  ", key_idx + 1);
		for(i = 0; i < SWITCHTEC_KMSK_LEN; i++)
				printf("%02x", state->public_key[key_idx][i]);
		printf("\n");
	}

	if (otp) {
		printf("\nOTP Region Program Status\n");
		printf("\tBasic Secure Settings %s%s\n",
		       otp->basic_valid? "(Valid):  ": "(Invalid):",
		       program_status_to_string(otp->basic));
		printf("\tMixed Version %s\t%s\n",
		       otp->mixed_ver_valid? "(Valid):  ": "(Invalid):",
		       program_status_to_string(otp->mixed_ver));
		printf("\tMain FW Version %s\t%s\n",
		       otp->main_fw_ver_valid? "(Valid):  ": "(Invalid):",
		       program_status_to_string(otp->main_fw_ver));
		printf("\tSecure Unlock Version %s%s\n",
		       otp->sec_unlock_ver_valid? "(Valid):  ": "(Invalid):",
		       program_status_to_string(otp->sec_unlock_ver));
		for (i = 0; i < 4; i++) {
			printf("\tKMSK%d %s\t\t%s\n", i,
			       otp->kmsk_valid[i]? "(Valid):  ": "(Invalid):",
			       program_status_to_string(otp->kmsk[i]));
		}
	}
}

static void print_security_cfg_set(struct switchtec_security_cfg_set *set)
{
	printf("\nBasic Secure Settings\n");

	printf("\tSPI Clock Rate: \t\t%.2f MHz\n", set->spi_clk_rate);

	printf("\tI2C Recovery TMO: \t\t%d Second(s)\n",
		set->i2c_recovery_tmo);

	printf("\tI2C Port: \t\t\t%d\n", set->i2c_port);
	printf("\tI2C Address (7-bits): \t\t0x%02x\n", set->i2c_addr);
	printf("\tI2C Command Map: \t\t0x%08x\n", set->i2c_cmd_map);

	printf("Exponent Hex Data: \t\t\t0x%08x\n", set->public_key_exponent);
}

#define CMD_DESC_VERSIONS_GET "display chip serial number, customer ID, secure versions of Key manifest, BL2, Main FW and SUV"

static int secure_version_get(int argc, char **argv)
{
	int ret;
	struct switchtec_sn_ver_info sn_info = {};

	static struct {
		struct switchtec_dev *dev;
	} cfg = {};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_VERSIONS_GET, opts, &cfg, sizeof(cfg));

	ret = switchtec_sn_ver_get(cfg.dev, &sn_info);
	if (ret) {
		switchtec_perror("mfg versions-get");
		return ret;
	}
	printf("Chip Serial Number: \t\t\t0x%08x\n", sn_info.chip_serial);
	printf("Key Manifest Secure Version: \t\t0x%08x\n", sn_info.ver_km);
	printf("BL2 Secure Version: \t\t\t0x%08x\n", sn_info.ver_bl2);
	printf("Main Secure Version: \t\t\t0x%08x\n", sn_info.ver_main);
	printf("Secure Unlock Version: \t\t\t0x%08x\n", sn_info.ver_sec_unlock);
	printf("Customer ID: \t\t\t\t0x%04x\n", sn_info.customer_id);

	return 0;

}

#define CMD_DESC_INFO "display security settings"

static int info(int argc, char **argv)
{
	int ret;
	enum switchtec_boot_phase phase_id;

	struct switchtec_sn_ver_info sn_info = {};

	static struct {
		struct switchtec_dev *dev;
		int verbose;
	} cfg = {};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"verbose", 'v', "", CFG_NONE, &cfg.verbose, no_argument,
		 "print additional chip information"},
		{NULL}};

	struct switchtec_security_cfg_state state;

	argconfig_parse(argc, argv, CMD_DESC_INFO, opts, &cfg, sizeof(cfg));

	ret = switchtec_security_config_get(cfg.dev, &state);
	if (ret) {
		switchtec_perror("mfg info");
		return ret;
	}

	phase_id = switchtec_boot_phase(cfg.dev);
	printf("Current Boot Phase: \t\t\t%s\n",
	       switchtec_phase_id_str(phase_id));

	ret = switchtec_sn_ver_get(cfg.dev, &sn_info);
	if (ret) {
		switchtec_perror("mfg info");
		return ret;
	}
	printf("Chip Serial: \t\t\t\t0x%08x\n", sn_info.chip_serial);
	printf("Key Manifest Secure Version: \t\t0x%08x\n", sn_info.ver_km);
	printf("BL2 Secure Version: \t\t\t0x%08x\n", sn_info.ver_bl2);
	printf("Main Secure Version: \t\t\t0x%08x\n", sn_info.ver_main);
	printf("Secure Unlock Version: \t\t\t0x%08x\n", sn_info.ver_sec_unlock);
	printf("Customer ID: \t\t\t\t0x%04x\n", sn_info.customer_id);

	if (phase_id == SWITCHTEC_BOOT_PHASE_BL2) {
		printf("\nOther secure settings are only shown in the BL1 or Main Firmware phase.\n\n");
		return 0;
	}

	if (cfg.verbose)  {
		if (!state.otp_valid) {
			print_security_config(&state, NULL);
			fprintf(stderr,
				"\nAdditional (verbose) chip info is not available on this chip!\n\n");
		} else if (switchtec_gen(cfg.dev) == SWITCHTEC_GEN4 &&
			   phase_id != SWITCHTEC_BOOT_PHASE_FW) {
			print_security_config(&state, NULL);
			fprintf(stderr,
				"\nAdditional (verbose) chip info is only available in the Main Firmware phase!\n\n");
		} else {
			print_security_config(&state, &state.otp);
		}

		return 0;
	}

	print_security_config(&state, NULL);

	return 0;
}

#define CMD_DESC_MAILBOX "retrieve mailbox logs"

static int mailbox(int argc, char **argv)
{
	int ret;

	static struct {
		struct switchtec_dev *dev;
		int out_fd;
		const char *out_filename;
	} cfg = {};
	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"filename", .cfg_type=CFG_FD_WR, .value_addr=&cfg.out_fd,
		  .argument_type=optional_positional,
		  .force_default="switchtec_mailbox.log",
		  .help="file to log mailbox data"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_MAILBOX, opts, &cfg, sizeof(cfg));

	ret = switchtec_mailbox_to_file(cfg.dev, cfg.out_fd);
	if (ret) {
		switchtec_perror("mfg mailbox");
		close(cfg.out_fd);
		return ret;
	}

	close(cfg.out_fd);

	fprintf(stderr, "\nLog saved to %s.\n", cfg.out_filename);

	return 0;
}

static void print_image_list(struct switchtec_active_index *idx)
{
	printf("IMAGE\t\tINDEX\n");
	printf("Key Manifest\t%d\n", idx->keyman);
	printf("BL2\t\t%d\n", idx->bl2);
	printf("Config\t\t%d\n", idx->config);
	printf("Firmware\t%d\n", idx->firmware);
}

#define CMD_DESC_IMAGE_LIST "display active image list (BL1 only)"

static int image_list(int argc, char **argv)
{
	int ret;
	struct switchtec_active_index index;

	static struct {
		struct switchtec_dev *dev;
	} cfg = {};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_IMAGE_LIST, opts, &cfg, sizeof(cfg));

	if (switchtec_boot_phase(cfg.dev) != SWITCHTEC_BOOT_PHASE_BL1) {
		fprintf(stderr, "This command is only available in BL1!\n");
		return -1;
	}

	ret = switchtec_active_image_index_get(cfg.dev, &index);
	if (ret) {
		switchtec_perror("image list");
		return ret;
	}

	print_image_list(&index);

	return 0;
}

#define CMD_DESC_IMAGE_SELECT "select active image index (BL1 only)"

static int image_select(int argc, char **argv)
{
	int ret;
	struct switchtec_active_index index;

	static struct {
		struct switchtec_dev *dev;
		unsigned char bl2;
		unsigned char firmware;
		unsigned char config;
		unsigned char keyman;
	} cfg = {
		.bl2 = SWITCHTEC_ACTIVE_INDEX_NOT_SET,
		.firmware = SWITCHTEC_ACTIVE_INDEX_NOT_SET,
		.config = SWITCHTEC_ACTIVE_INDEX_NOT_SET,
		.keyman = SWITCHTEC_ACTIVE_INDEX_NOT_SET
	};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"bl2", 'b', "", CFG_BYTE, &cfg.bl2,
			required_argument, "active image index for BL2"},
		{"firmware", 'm', "", CFG_BYTE, &cfg.firmware,
			required_argument, "active image index for FIRMWARE"},
		{"config", 'c', "", CFG_BYTE, &cfg.config,
			required_argument, "active image index for CONFIG"},
		{"keyman", 'k', "", CFG_BYTE, &cfg.keyman, required_argument,
			"active image index for KEY MANIFEST"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_IMAGE_SELECT, opts, &cfg, sizeof(cfg));

	if (cfg.bl2 == SWITCHTEC_ACTIVE_INDEX_NOT_SET &&
	    cfg.firmware == SWITCHTEC_ACTIVE_INDEX_NOT_SET &&
	    cfg.config == SWITCHTEC_ACTIVE_INDEX_NOT_SET &&
	    cfg.keyman == SWITCHTEC_ACTIVE_INDEX_NOT_SET) {
		fprintf(stderr,
			"One of BL2, Config, Key Manifest or Firmware indices must be set in this command!\n");
		return -1;
	}

	if (switchtec_boot_phase(cfg.dev) != SWITCHTEC_BOOT_PHASE_BL1) {
		fprintf(stderr,
			"This command is only available in BL1!\n");
		return -2;
	}

	if (cfg.bl2 > 1 && cfg.bl2 != SWITCHTEC_ACTIVE_INDEX_NOT_SET) {
		fprintf(stderr, "Active index of BL2 must be within 0-1!\n");
		return -3;
	}
	index.bl2 = cfg.bl2;

	if (cfg.firmware > 1 &&
	    cfg.firmware != SWITCHTEC_ACTIVE_INDEX_NOT_SET) {
		fprintf(stderr,
			"Active index of FIRMWARE must be within 0-1!\n");
		return -4;
	}
	index.firmware = cfg.firmware;

	if (cfg.config > 1 && cfg.config != SWITCHTEC_ACTIVE_INDEX_NOT_SET) {
		fprintf(stderr,
			"Active index of CONFIG must be within 0-1!\n");
		return -5;
	}
	index.config = cfg.config;

	if (cfg.keyman > 1 && cfg.keyman != SWITCHTEC_ACTIVE_INDEX_NOT_SET) {
		fprintf(stderr,
			"Active index of KEY MANIFEST must be within 0-1!\n");
		return -6;
	}
	index.keyman = cfg.keyman;

	ret = switchtec_active_image_index_set(cfg.dev, &index);
	if (ret) {
		switchtec_perror("image select");
		return ret;
	}

	return ret;
}

#define CMD_DESC_BOOT_RESUME "resume device boot process (BL1 and BL2 only)"

static int boot_resume(int argc, char **argv)
{
	const char *desc = CMD_DESC_BOOT_RESUME "\n\n"
			   "A normal device boot process includes the BL1, "
			   "BL2 and Main Firmware boot phases. In the case "
			   "when the boot process is paused at the BL1 or BL2 phase "
			   "(due to boot failure or BOOT_RECOVERY PIN[0:1] "
			   "being set to LOW), sending this command requests "
			   "the device to try resuming a normal boot process.\n\n"
			   "NOTE: if your system does not support hotplug, "
			   "your device might not be immediately accessible "
			   "after a normal boot process. In this case, be sure "
			   "to reboot your system after sending this command.";
	int ret;

	static struct {
		struct switchtec_dev *dev;
		int assume_yes;
	} cfg = {};
	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"yes", 'y', "", CFG_NONE, &cfg.assume_yes, no_argument,
		 "assume yes when prompted"},
		{NULL}
	};

	argconfig_parse(argc, argv, desc, opts, &cfg, sizeof(cfg));

	if (switchtec_boot_phase(cfg.dev) == SWITCHTEC_BOOT_PHASE_FW) {
		fprintf(stderr,
			"This command is only available in BL1 or BL2!\n");
		return -1;
	}

	if (!cfg.assume_yes)
		fprintf(stderr,
			"WARNING: if your system does not support hotplug,\n"
			"your device might not be immediately accessible\n"
			"after a normal boot process. In this case, be sure\n"
			"to reboot your system after sending this command.\n\n");

	ret = ask_if_sure(cfg.assume_yes);
	if (ret)
		return ret;

	ret = switchtec_boot_resume(cfg.dev);
	if (ret) {
		switchtec_perror("mfg boot-resume");
		return ret;
	}

	return 0;
}

#define CMD_DESC_FW_TRANSFER "transfer a firmware image to device (BL1 only)"

static int fw_transfer(int argc, char **argv)
{
	const char *desc = CMD_DESC_FW_TRANSFER "\n\n"
			   "This command only supports transferring a firmware "
			   "image when the device is in the BL1 boot phase. Use "
			   "'fw-execute' after this command to excute the "
			   "transferred image. Note that the image is stored "
			   "in device RAM and is lost after device reboot.\n\n"
			   "To update an image in the BL2 or MAIN boot phase, use "
			   "the 'fw-update' command instead.\n\n"
			   BOOT_PHASE_HELP_TEXT;
	int ret;
	enum switchtec_fw_type type;

	static struct {
		struct switchtec_dev *dev;
		FILE *fimg;
		const char *img_filename;
		int assume_yes;
		int force;
	} cfg = {};
	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"img_file", .cfg_type=CFG_FILE_R, .value_addr=&cfg.fimg,
			.argument_type=required_positional,
			.help="firmware image file to transfer"},
		{"yes", 'y', "", CFG_NONE, &cfg.assume_yes, no_argument,
			"assume yes when prompted"},
		{"force", 'f', "", CFG_NONE, &cfg.force, no_argument,
			"force interrupting an existing fw-update command "
			"in case firmware is stuck in a busy state"},
		{NULL}
	};

	argconfig_parse(argc, argv, desc, opts, &cfg, sizeof(cfg));

	if (switchtec_boot_phase(cfg.dev) != SWITCHTEC_BOOT_PHASE_BL1) {
		fprintf(stderr,
			"This command is only available in the BL1 boot phase!\n");
		fprintf(stderr,
			"Use 'fw-update' instead to update an image in other boot phases.\n");
		return -1;
	}

	printf("Writing the following firmware image to %s:\n",
		switchtec_name(cfg.dev));

	type = check_and_print_fw_image(fileno(cfg.fimg), cfg.img_filename);

	if (type != SWITCHTEC_FW_TYPE_BL2) {
		fprintf(stderr,
			"This command only supports transferring a BL2 image.\n");
		return -2;
	}

	ret = ask_if_sure(cfg.assume_yes);
	if (ret) {
		fclose(cfg.fimg);
		return ret;
	}

	progress_start();
	ret = switchtec_fw_write_file(cfg.dev, cfg.fimg, 1, cfg.force,
				      progress_update);
	fclose(cfg.fimg);

	if (ret) {
		printf("\n");
		switchtec_fw_perror("mfg fw-transfer", ret);
		return -3;
	}

	progress_finish(0);
	printf("\n");

	return 0;
}

#define CMD_DESC_FW_EXECUTE "execute previously transferred firmware image (BL1 only)"

static int fw_execute(int argc, char **argv)
{
	const char *desc = CMD_DESC_FW_EXECUTE "\n\n"
			   "This command is only supported when the device is "
			   "in the BL1 boot phase. The firmware image must have "
			   "been transferred using the 'fw-transfer' command. "
			   "After firmware initializes, it listens for activity from "
			   "I2C, UART (XModem), or both interfaces for input. "
			   "Once activity is detected from an interface, "
			   "firmware falls into recovery mode on that interface. "
			   "The interface to listen on can be specified using "
			   "the 'bl2_recovery_mode' option. \n\n"
			   "To activate an image in the BL2 or MAIN boot "
			   "phase, use the 'fw-toggle' command instead.\n\n"
			   BOOT_PHASE_HELP_TEXT;
	int ret;

	static struct {
		struct switchtec_dev *dev;
		int assume_yes;
		enum switchtec_bl2_recovery_mode bl2_rec_mode;
	} cfg = {
		.bl2_rec_mode = SWITCHTEC_BL2_RECOVERY_I2C_AND_XMODEM
	};
	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"yes", 'y', "", CFG_NONE, &cfg.assume_yes, no_argument,
			"assume yes when prompted"},
		{"bl2_recovery_mode", 'm', "MODE",
			CFG_CHOICES, &cfg.bl2_rec_mode,
			required_argument, "BL2 recovery mode",
			.choices = recovery_mode_choices},
		{NULL}
	};

	argconfig_parse(argc, argv, desc, opts, &cfg, sizeof(cfg));

	if (switchtec_boot_phase(cfg.dev) != SWITCHTEC_BOOT_PHASE_BL1) {
		fprintf(stderr,
			"This command is only available in the BL1 phase!\n");
		return -2;
	}

	if (!cfg.assume_yes)
		printf("This command will execute the previously transferred image.\n");
	ret = ask_if_sure(cfg.assume_yes);
	if (ret) {
		return ret;
	}

	ret = switchtec_fw_exec(cfg.dev, cfg.bl2_rec_mode);
	if (ret) {
		switchtec_fw_perror("mfg fw-execute", ret);
		return ret;
	}

	return 0;
}

#define CMD_DESC_STATE_SET "set device secure state (BL1 and Main Firmware only)"

static int state_set(int argc, char **argv)
{
	int ret;
	struct switchtec_security_cfg_state state = {};

	const char *desc = CMD_DESC_STATE_SET "\n\n"
			   "This command can only be used when the device "
			   "secure state is UNINITIALIZED_UNSECURED.\n\n"
			   "NOTE - A device can be in one of these "
			   "three secure states: \n"
			   "UNINITIALIZED_UNSECURED: this is the default state "
			   "when the chip is shipped. All security-related settings "
			   "are 'uninitialized', and the chip is in the 'unsecured' "
			   "state. \n"
			   "INITIALIZED_UNSECURED: this is the state when "
			   "security-related settings are 'initialized', and "
			   "the chip is set to the 'unsecured' state. \n"
			   "INITIALIZED_SECURED: this is the state when "
			   "security-related settings are 'initialized', and "
			   "the chip is set to the 'secured' state. \n\n"
			   "Use 'config-set' or other mfg commands to "
			   "initialize security settings when the chip is in "
			   "the UNINITIALIZED_UNSECURED state, then use this "
			   "command to switch the chip to the INITIALIZED_SECURED "
			   "or INITIALIZED_UNSECURED state. \n\n"
			   "WARNING: ONCE THE CHIP STATE IS SUCCESSFULLY SET, "
			   "IT CAN NO LONGER BE CHANGED. USE CAUTION WHEN ISSUING "
			   "THIS COMMAND.";

	static struct {
		struct switchtec_dev *dev;
		enum switchtec_secure_state state;
		int assume_yes;
	} cfg = {
		.state = SWITCHTEC_SECURE_STATE_UNKNOWN,
	};
	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"state", 't', "state",
			CFG_CHOICES, &cfg.state,
			required_argument, "secure state",
			.choices=secure_state_choices},
		{"yes", 'y', "", CFG_NONE, &cfg.assume_yes, no_argument,
		 "assume yes when prompted"},
		{NULL}
	};

	argconfig_parse(argc, argv, desc, opts, &cfg, sizeof(cfg));

	if (cfg.state == SWITCHTEC_SECURE_STATE_UNKNOWN) {
		fprintf(stderr,
			"Secure state must be set in this command!\n");
		return -1;
	}

	if (switchtec_boot_phase(cfg.dev) == SWITCHTEC_BOOT_PHASE_BL2) {
		fprintf(stderr,
			"This command is only available in BL1 or Main Firmware!\n");
		return -2;
	}

	ret = switchtec_security_config_get(cfg.dev, &state);
	if (ret) {
		switchtec_perror("mfg state-set");
		return ret;
	}
	if (state.secure_state != SWITCHTEC_UNINITIALIZED_UNSECURED) {
		fprintf(stderr,
			"This command is only valid when secure state is UNINITIALIZED_UNSECURED!\n");
		return -3;
	}

	print_security_config(&state, NULL);

	if (!cfg.assume_yes) {
		fprintf(stderr,
			"\nWARNING: This operation makes changes to the device OTP memory and is IRREVERSIBLE!\n");

		ret = ask_if_sure(cfg.assume_yes);
		if (ret)
			return -4;
	}

	ret = switchtec_secure_state_set(cfg.dev, cfg.state);
	if (ret) {
		switchtec_perror("mfg state-set");
		return ret;
	}

	return 0;
}

#define CMD_DESC_DIE_TRACE_PROG "program die trace into OTP"

static int die_trace_prog(int argc, char **argv)
{
	int ret = 0;
	struct otp_die_trace otp_die_trace_str;

	static struct {
		struct switchtec_dev *dev;
		FILE *die_trace_fimg;
		char *die_trace_file;
		int assume_yes;
	} cfg = {};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"die_trace", 'd', "die_trace",
			.cfg_type=CFG_FILE_R,
			.value_addr=&cfg.die_trace_fimg,
			.argument_type=required_argument,
			.help="Die Trace file.bin\n" \
			"Should contain Die Trace settings\n"},
		{"yes", 'y', "", CFG_NONE, &cfg.assume_yes, no_argument,
			"assume yes when prompted"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_DIE_TRACE_PROG, opts, &cfg, sizeof(cfg));
	
	if (cfg.die_trace_file == NULL)
	{
		fprintf(stderr,
			"Die Trace bin file must be supplied in this command!\n");
		return -1;
	}

	fseek(cfg.die_trace_fimg, 0, SEEK_END);
	if (ftell(cfg.die_trace_fimg) < OTP_DIE_TRACE_LENGTH) {
		fprintf(stderr, "\nSize of Die Trace file %s is invalid!\n",
			cfg.die_trace_file);

		ret = -1;
	}
	fseek(cfg.die_trace_fimg, 0, SEEK_SET);

	if (cfg.die_trace_file) {
		ret = switchtec_read_die_trace_file(cfg.die_trace_fimg, &otp_die_trace_str);
		fclose(cfg.die_trace_fimg);
	
		if (ret) {
			fprintf(stderr, "Invalid Die Trace Bin file %s!\n",
				cfg.die_trace_file);
			return -6;
		}
	}
	
	if (!cfg.assume_yes)
	fprintf(stderr,
		"\nWARNING: This operation makes changes to the device OTP memory and is IRREVERSIBLE!\n");
	ret = ask_if_sure(cfg.assume_yes);
	if (ret)
		return ret;

	ret = switchtec_otp_die_trace_prog(cfg.dev, otp_die_trace_str.die_trace);
	if(ret)
		switchtec_perror("mfg otp-die-trace-prog");
	else
		printf("Die Trace programmed successfully into OTP\n");
	return ret;
}

#define CMD_DESC_CHIP_SERIAL_PROG "program chip serial number into OTP"

static int chip_serial_prog(int argc, char **argv)
{
	int ret;

	static struct {
		struct switchtec_dev *dev;
		uint32_t chip_serial_num;
	} cfg = {
		.chip_serial_num = 0
	};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"chip_serial_num", 'c', "chip_serial_num",
			.cfg_type=CFG_LONG,
			.value_addr=&cfg.chip_serial_num,
			.argument_type=required_argument,
			.help="Enter 4-byte Chip Serial Number\n"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_CHIP_SERIAL_PROG, opts, &cfg, sizeof(cfg));

	if (cfg.chip_serial_num == 0)
	{
		fprintf(stderr,
			"Chip Serial Number must be supplied in this command!\n");
		return -1;
	}

	ret = switchtec_chip_serial_num_prog(cfg.dev, cfg.chip_serial_num);
	if(ret)
		switchtec_perror("mfg otp-chip-serial-prog");
	else
		printf("Chip Serial Number programmed successfully into OTP\n");
	return ret;
}

#define CMD_DESC_SKU_INFO_SET "write SKU info settings into OTP"

static int sku_set(int argc, char **argv)
{
	struct otp_sku_info_set sku_info_set_str = {};
	int ret;

	static struct {
		struct switchtec_dev *dev;
		uint8_t block_num;
		uint8_t sku_info;
	} cfg = {
		.block_num = 0,
		.sku_info = 0
	};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"block_num", 'b', "block_num",
			.cfg_type=CFG_BYTE,
			.value_addr=&cfg.block_num,
			.argument_type=required_argument,
			.help="Block Number\n" \
			"Valid Range : 0 - 3\n"},
		{"sku_info", 's', "sku_info",
			.cfg_type=CFG_BYTE,
			.value_addr=&cfg.sku_info,
			.argument_type=required_argument,
			.help="Set SKU Info\n" \
			"Valid Range : 0 - 4\n"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_SKU_INFO_SET, opts, &cfg, sizeof(cfg));

	if(cfg.sku_info < 0 || cfg.sku_info > 4)
	{
		printf("\nInvalid SKU Info!\n");
		return -1;
	}

	sku_info_set_str.block_num = cfg.block_num;
	sku_info_set_str.sku_info = cfg.sku_info;
	ret = switchtec_otp_sku_info_set(cfg.dev, &sku_info_set_str);
	if(ret)
		switchtec_perror("mfg otp-sku-info-set");
	return ret;
}

#define CMD_DESC_SKU_INFO_GET "read SKU info settings from OTP"

static int sku_get(int argc, char **argv)
{
	struct otp_sku_info_get sku_info_get_str = {};
	int ret;

	static struct {
		struct switchtec_dev *dev;
	} cfg = {};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_SKU_INFO_GET, opts, &cfg, sizeof(cfg));

	ret = switchtec_otp_sku_info_get(cfg.dev, &sku_info_get_str);
	if (ret)
	{
		switchtec_perror("mfg otp-sku-info-get");
		return ret;
	}
	
	printf("SKU Info:\n");
	for(int blk = 0; blk < SKU_NO_OF_BLOCKS; blk++)
	{
		if((sku_info_get_str.valid_flags & (1 << blk)) != 0)
			printf("Block %d = %x\n", blk, sku_info_get_str.sku_info[blk]);
		else
			printf("Block %d = Not Set\n", blk);
	}
	printf("Write Protected Status\n");
	printf("%x\n", sku_info_get_str.write_protected);

	return ret;
}

#define CMD_DESC_OTP_REGS_WRITE_PROT "write protect OTP Regions"

static int otp_write_prot(int argc, char **argv)
{
	int ret;

	static struct {
		struct switchtec_dev *dev;
		bool sku_info;
		bool otp_config;
		int assume_yes;
	} cfg = {};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"sku_info", 's', "sku_info",
			.cfg_type=CFG_BOOL,
			.value_addr=&cfg.sku_info,
			.argument_type=required_argument,
			.help="Enter 1 to Write Protect SKU Info Region\n"
		},
		{"otp_config", 'c', "otp_config",
			.cfg_type=CFG_BOOL,
			.value_addr=&cfg.otp_config,
			.argument_type=required_argument,
			.help="Enter 1 to Write Protect OTP Config Region\n"
		},
		{"yes", 'y', "", CFG_NONE, &cfg.assume_yes, no_argument,
		 "assume yes when prompted"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_OTP_REGS_WRITE_PROT, opts, &cfg, sizeof(cfg));

	if (!cfg.assume_yes)
		fprintf(stderr,
			"\nWARNING: This operation makes changes to the device OTP memory and is IRREVERSIBLE!\n");
	ret = ask_if_sure(cfg.assume_yes);
	if (ret)
		return ret;

	ret = switchtec_otp_regs_write_protect(cfg.dev, cfg.sku_info, cfg.otp_config);
	if (ret)
		switchtec_perror("mfg otp-write-prot");
	return ret;
}


#define CMD_DESC_SJTAG_MODE_SET "write SJTAG mode into OTP"

static int sjtag_mode_set(int argc, char **argv)
{
	int ret;

	static struct {
		struct switchtec_dev *dev;
		uint8_t sjtag_mode;
	} cfg = {
		.sjtag_mode = 0
	};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"sjtag_mode", 'm', "sjtag_mode",
			.cfg_type=CFG_BYTE,
			.value_addr=&cfg.sjtag_mode,
			.argument_type=required_argument,
			.help="Specify SJTAG Mode\n" \
			"Supported SJTAG Modes are\n" \
			"* 0 - Open\n" \
			"* 1 - Secure Mode 1\n" \
			"* 2 - Secure Mode 2\n" \
			"* 3 - Closed\n"},
		{NULL}
	};
	
	argconfig_parse(argc, argv, CMD_DESC_SJTAG_MODE_SET, opts, &cfg, sizeof(cfg));

	ret = switchtec_otp_sjtag_mode_set(cfg.dev, cfg.sjtag_mode);
	if(ret)
		switchtec_perror("mfg otp-sjtag-mode-set");
	return ret;
}

#define CMD_DESC_SJTAG_UUID_SET "write SJTAG UUID into OTP"

static int sjtag_uuid_set(int argc, char **argv)
{
	int ret;
	struct switchtec_uuid uuid;

	static struct {
		struct switchtec_dev *dev;
		FILE *uuid_fimg;
		char *uuid_file;
		int assume_yes;
	} cfg = {};

const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,

		{"uuid_file", 'u', .cfg_type=CFG_FILE_R,
			.value_addr=&cfg.uuid_fimg,
			.argument_type=required_argument,
			.help="UUID Bin File(.bin)\n" \
			"Should contain 10 bytes of SJTAG UUID to be set\n"},

		{"yes", 'y', "", CFG_NONE, &cfg.assume_yes, no_argument,
		 "assume yes when prompted"},
		{NULL}
	};
	
	argconfig_parse(argc, argv, CMD_DESC_SJTAG_UUID_SET, opts, &cfg, sizeof(cfg));

	if (cfg.uuid_file == NULL)
	{
		fprintf(stderr,
			"SJTAG UUID bin file must be supplied in this command!\n");
		return -1;
	}

	if (cfg.uuid_file) {
		ret = switchtec_read_uuid_file(cfg.uuid_fimg, &uuid);
		fclose(cfg.uuid_fimg);

		if (ret) {
			fprintf(stderr, "Invalid UUID file %s!\n",
				cfg.uuid_file);
			return -6;
		}
	}
	
	if (!cfg.assume_yes)
		fprintf(stderr,
			"\nWARNING: This operation makes changes to the device OTP memory and is IRREVERSIBLE!\n");
	ret = ask_if_sure(cfg.assume_yes);
	if (ret)
		return ret;

	ret = switchtec_otp_sjtag_uuid_set(cfg.dev, uuid.uuid_data);
	if(ret)
		switchtec_perror("mfg otp-sjtag-uuid-set");
	return ret;


}

#define CMD_DESC_SJTAG_KEY_SET "write SJTAG key into OTP"

static int sjtag_key_set(int argc, char **argv)
{
	int ret;
	struct otp_sjtag_key sjtag_key;

	static struct {
		struct switchtec_dev *dev;
		FILE *sjtag_key_fimg;
		char *sjtag_key_file;
		int assume_yes;
	} cfg = {};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"sjtag_key_file", 'k', "sjtag_key_file",
			.cfg_type=CFG_FILE_R,
			.value_addr=&cfg.sjtag_key_fimg,
			.argument_type=required_argument,
			.help="SJTAG Key Bin File(.bin)\n" \
			"Should contain 16 Bytes of SJTAG Key to be set\n"},
		{"yes", 'y', "", CFG_NONE, &cfg.assume_yes, no_argument,
			"assume yes when prompted"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_SJTAG_KEY_SET, opts, &cfg, sizeof(cfg));

	if (cfg.sjtag_key_file == NULL)
	{
		fprintf(stderr,
			"SJTAG Key bin file must be supplied in this command!\n");
		return -1;
	}

	if (cfg.sjtag_key_file) {
		ret = switchtec_read_sjtag_key_file(cfg.sjtag_key_fimg, &sjtag_key);
		fclose(cfg.sjtag_key_fimg);

		if (ret) {
			fprintf(stderr, "Invalid SJTAG Key file %s!\n",
				cfg.sjtag_key_file);
			return -6;
		}
	}

	if (!cfg.assume_yes)
	fprintf(stderr,
		"\nWARNING: This operation makes changes to the device OTP memory and is IRREVERSIBLE!\n");
	ret = ask_if_sure(cfg.assume_yes);
	if (ret)
		return ret;

	ret = switchtec_otp_sjtag_key_set(cfg.dev, sjtag_key.sjtag_key);
	if(ret)
		switchtec_perror("mfg otp-sjtag-key-set");
	return ret;
}

#define CMD_DESC_SJTAG_KEY_VERIFY "verify given SJTAG key and that of written in OTP"

static int sjtag_key_verify(int argc, char **argv)
{
	int ret;
	uint8_t is_verified = 0;
	struct otp_sjtag_key sjtag_key;

	static struct {
		struct switchtec_dev *dev;
		FILE *sjtag_key_fimg;
		char *sjtag_key_file;
	} cfg = {};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"sjtag_key_file", 'k', "sjtag_key_file",
			.cfg_type=CFG_FILE_R,
			.value_addr=&cfg.sjtag_key_fimg,
			.argument_type=required_argument,
			.help="SJTAG Key Bin File(.bin)\n" \
			"Should contain the SJTAG Key to be verified with that in OTP\n"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_SJTAG_KEY_VERIFY, opts, &cfg, sizeof(cfg));

	if (cfg.sjtag_key_file == NULL)
	{
		fprintf(stderr,
			"SJTAG Key bin file must be supplied in this command!\n");
		return -1;
	}

	if (cfg.sjtag_key_file) {
		ret = switchtec_read_sjtag_key_file(cfg.sjtag_key_fimg, &sjtag_key);
		fclose(cfg.sjtag_key_fimg);

		if (ret) {
			fprintf(stderr, "Invalid SJTAG Key file %s!\n",
				cfg.sjtag_key_file);
			return -6;
		}
	}

	ret = switchtec_otp_sjtag_key_verify(cfg.dev, sjtag_key.sjtag_key, &is_verified);
	if(ret)
		switchtec_perror("mfg otp-sjtag-key-verify");
	else
	{
		if(is_verified)
			printf("\nGiven SJTAG key is same as that of written in OTP\n");
		else
			printf("\nGiven SJTAG key is not same as that of written in OTP\n");
	}

	return ret;
}

#define CMD_DESC_SJTAG_KEY_LOCK "lock SJTAG key access"

static int sjtag_key_lock(int argc, char **argv)
{
	int ret;
	static struct {
		struct switchtec_dev *dev;
		int assume_yes;
	} cfg = {};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"yes", 'y', "", CFG_NONE, &cfg.assume_yes, no_argument,
		"assume yes when prompted"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_SJTAG_KEY_LOCK, opts, &cfg, sizeof(cfg));

	if (!cfg.assume_yes)
	fprintf(stderr,
		"\nWARNING: This operation makes changes to the device OTP memory and is IRREVERSIBLE!\n");
	ret = ask_if_sure(cfg.assume_yes);
	if (ret)
		return ret;

	ret = switchtec_otp_sjtag_key_lock(cfg.dev);
	if(ret)
		switchtec_perror("mfg otp-sjtag-key-lock");
	return ret;
}

#define CMD_DESC_SELF_TEST_POLICY_SET "set self test policy for cryptoblock"

static int selftest_pol_set(int argc, char **argv)
{
	int ret;
	struct otp_self_test_policy otp_self_test_policy_str;

	static struct {
		struct switchtec_dev *dev;
		FILE *self_test_fimg;
		char *self_test_file;
		int assume_yes;
	} cfg = {};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"self_test_policy", 's', "self_test_policy",
			.cfg_type=CFG_FILE_R,
			.value_addr=&cfg.self_test_fimg,
			.argument_type=required_argument,
			.help="Self Test Policy Bin File(.bin)\n" \
			"Should contain Self Test Policy settings\n"},
		{"yes", 'y', "", CFG_NONE, &cfg.assume_yes, no_argument,
			"assume yes when prompted"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_SELF_TEST_POLICY_SET, opts, &cfg, sizeof(cfg));

	if (cfg.self_test_file == NULL)
	{
		fprintf(stderr,
			"Self Test Policy bin must be supplied in this command!\n");
		return -1;
	}

	if (cfg.self_test_file) {
		ret = switchtec_read_self_test_policy_file(cfg.self_test_fimg, &otp_self_test_policy_str);
		fclose(cfg.self_test_fimg);

		if (ret) {
			fprintf(stderr, "Invalid Self Test Policy Key file %s!\n",
				cfg.self_test_file);
			return -6;
		}
	}

	if (!cfg.assume_yes)
	fprintf(stderr,
		"\nWARNING: This operation makes changes to the device OTP memory and is IRREVERSIBLE!\n");
	ret = ask_if_sure(cfg.assume_yes);
	if (ret)
		return ret;

	ret = switchtec_otp_self_test_policy_set(cfg.dev, otp_self_test_policy_str.otp_self_test_policy);
	if(ret)
		switchtec_perror("mfg otp-self-test-policy-set");
	else
		printf("Self Test Policy programmed successfully into OTP\n");
	return ret;
}

#define CMD_DESC_BIT_BYTE_PROG_MODE_SET "set bit or byte programming mode for OTP"

static int otp_prog_mode_set(int argc, char **argv)
{
	int ret;
	static struct {
		struct switchtec_dev *dev;
		unsigned char bit_byte_mode;
	} cfg = {
		.bit_byte_mode = 1 //Default to Byte mode
	};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"bit_or_byte_mode", 'm', "bit_or_byte_mode",
			.cfg_type=CFG_BYTE,
			.value_addr=&cfg.bit_byte_mode,
			.argument_type=required_argument,
			.help="Selects OTP bit or byte mode\n" \
			"* 0 - Enable Bit Mode\n" \
			"* 1 - Enable Byte Mode\n"},
		{NULL}
	};
	argconfig_parse(argc, argv, CMD_DESC_BIT_BYTE_PROG_MODE_SET, opts, &cfg, sizeof(cfg));

	ret = switchtec_otp_bit_byte_prog_mode_set(cfg.dev, cfg.bit_byte_mode);
	if(ret)
		switchtec_perror("mfg otp-bit-byte-prog-mode-set");
	return ret;
}

#define CMD_DESC_SMART_PROG_MODE_SET "set smart programming mode for OTP"

static int otp_smprg_set(int argc, char **argv)
{
	int ret;
	static struct {
		struct switchtec_dev *dev;
		unsigned char smart_prog_mode;
	} cfg = {
		.smart_prog_mode = 0
	};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"otp_smart_prog_mode", 'm', "otp_smart_prog_mode",
			.cfg_type=CFG_BYTE,
			.value_addr=&cfg.smart_prog_mode,
			.argument_type=required_argument,
			.help="OTP Smart Programming Mode\n" \
			"* 0 - Disable Smart Programming Mode\n" \
			"* 1 - Enable Smart Programming Mode\n"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_SMART_PROG_MODE_SET, opts, &cfg, sizeof(cfg));

	ret = switchtec_otp_smart_prog_mode_set(cfg.dev, cfg.smart_prog_mode);
	if(ret)
		switchtec_perror("mfg otp-smart-prog-mode-set");
	return ret;
}

#define CMD_DESC_MODE_STATUS_GET "get status about programming modes of OTP"

static int otp_mode_status_get(int argc, char **argv)
{
	int ret, i;

	struct otp_mode_status otp_mode_status_str = {};

	static struct {
		struct switchtec_dev *dev;
	} cfg = {};
	
	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_MODE_STATUS_GET, opts, &cfg, sizeof(cfg));

	ret = switchtec_otp_mode_status_get(cfg.dev, &otp_mode_status_str);
	if(ret)
		switchtec_perror("mfg otp-mode-status-get");
	else
	{
		printf("Self Test Policy : ");
		for(i = 0; i < OTP_SELF_TEST_POLICY_LENGTH; i++)
		{
			printf("%x ", otp_mode_status_str.otp_self_test_policy.otp_self_test_policy[i]);
		}
		
		printf("\nBit Byte Mode : ");
		if(otp_mode_status_str.bit_byte_mode)
			printf("Byte Mode\n");
		else
			printf("Bit Mode\n");
		
		printf("Smart Programming Mode : ");
		if(otp_mode_status_str.smart_prog_mode)
			printf("On\n");
		else
			printf("Off\n");

		printf("Die Trace : ");

		for(i = 0; i < OTP_DIE_TRACE_LENGTH; i++)
		{
			printf("%x ", otp_mode_status_str.otp_die_trace.die_trace[i]);
		}
		printf("\n");

	}
	return ret;
}

#define CMD_DESC_CUST_ID_PROG "program given customer id in OTP"

static int otp_cust_id_prog(int argc, char **argv)
{
	int ret;

	static struct {
		struct switchtec_dev *dev;
		unsigned short cust_id;
	} cfg = {
		.cust_id = 0
	};
	
	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"customer_id", 'c', "customer_id",
			.cfg_type=CFG_SHORT,
			.value_addr=&cfg.cust_id,
			.argument_type=required_argument,
			.help="Customer ID\n"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_CUST_ID_PROG, opts, &cfg, sizeof(cfg));

	if (cfg.cust_id == 0)
	{
		fprintf(stderr,
			"Customer Id must be supplied in this command!\n");
		return -1;
	}

	ret = switchtec_custid_prog(cfg.dev, cfg.cust_id);
	if(ret)
		switchtec_perror("mfg otp-custid-prog");
	else
	{
		printf("Customer ID Programmed Successfully!\n");
	}
	return ret;
}

#define CMD_DESC_OTP_CONFIG_READ "read config data from OTP from specified address and length"

static int otp_config_read(int argc, char **argv)
{
	int ret;
	static struct {
		struct switchtec_dev *dev;
		int out_fd;
		const char *out_filename;
		unsigned short config_start_address;
		unsigned short config_length;
	} cfg = {};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"start_address", 's', "Config Start address",
			.cfg_type=CFG_SHORT,
			.value_addr=&cfg.config_start_address,
			.argument_type=required_argument,
			.help="Start Address from the OTP to read Config data\n Config region starts from 0x1800\n"},
		{"len", 'l', "Config Length",
			.cfg_type=CFG_SHORT,
			.value_addr=&cfg.config_length,
			.argument_type=required_argument,
			.help="Length in bytes to read Config data from OTP\n"},
		{"bin_file", .cfg_type=CFG_FD_WR, .value_addr=&cfg.out_fd,
		  .argument_type=optional_positional,
		  .force_default="otp_config_read.bin",
		  .help="OTP Config Read"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_SMART_PROG_MODE_SET, opts, &cfg, sizeof(cfg));
	if((cfg.config_start_address < OTP_CONFIG_START || cfg.config_start_address > OTP_MAX_SIZE))
	{
		switchtec_perror("Config Start Address is out of range");
		return -1;
	}

	if((cfg.config_start_address + cfg.config_length - 1) > OTP_MAX_SIZE)
	{
		switchtec_perror("Config Length is out of range");
		return -1;
	}

	ret = switchtec_otp_config_read(cfg.dev, cfg.config_start_address, cfg.config_length, cfg.out_fd);
	if(ret)
		switchtec_perror("mfg otp-config-read");
	else
		printf("OTP Config Dump Successful and dumped into %s\n", cfg.out_filename);
	return ret;
}

#define CMD_DESC_OTP_CONFIG_PROG "program the config data into OTP"

static int otp_config_prog(int argc, char **argv)
{
	int ret;
	struct otp_config_data config_data;
	unsigned short config_len;

	static struct {
		struct switchtec_dev *dev;
		bool is_firmware_halt;
		FILE *config_data_fimg;
		char *config_data_file;
		int assume_yes;
	} cfg = {
		.is_firmware_halt = false,
	};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"is_firmware_halt", 'h', "is_firmware_halt",
			.cfg_type=CFG_BOOL,
			.value_addr=&cfg.is_firmware_halt,
			.argument_type=required_argument,
			.help="Enter 1 to Halt the firmware in while(1)\n"
		},
		{"config_data_bin", 'f', "otp_config_data_bin",
			.cfg_type=CFG_FILE_R,
			.value_addr=&cfg.config_data_fimg,
			.argument_type=required_argument,
			.help="OTP Config Data Bin File(.bin)\n" \
			"Should contain Config data to be written into OTP\n"},
		{"yes", 'y', "", CFG_NONE, &cfg.assume_yes, no_argument,
			"assume yes when prompted"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_OTP_CONFIG_PROG, opts, &cfg, sizeof(cfg));

	if (cfg.config_data_file == NULL)
	{
		fprintf(stderr,
			"Config Data bin file must be supplied in this command!\n");
		return -1;
	}

	/*Move file point at the end of file.*/
	fseek(cfg.config_data_fimg, 0, SEEK_END);

	/*Get the actual size.*/
	config_len = ftell(cfg.config_data_fimg);

	fseek(cfg.config_data_fimg, 0, SEEK_SET);

	if(config_len > OTP_CONFIG_DATA_MAX_LEN)
	{
		switchtec_perror("Length of Config data should be less than or equal to 1020 bytes");
	}

	if (cfg.config_data_file) {
		ret = switchtec_read_otp_config_data_file(cfg.config_data_fimg, &config_data, config_len);
		fclose(cfg.config_data_fimg);

		if (ret) {
			fprintf(stderr, "Invalid OTP Config Bin file %s!\n",
				cfg.config_data_file);
			return -6;
		}
	}

	if (!cfg.assume_yes)
	fprintf(stderr,
		"\nWARNING: This operation makes changes to the device OTP memory and is IRREVERSIBLE!\n \
		   If is_firmware_halt argument is set, then the firmware will hang in infinite loop.\n \
		   OTP Config programming Completion and Status will be deduced from corresponding GPIOs\n");
	ret = ask_if_sure(cfg.assume_yes);
	if (ret)
		return ret;

	ret = switchtec_otp_config_prog(cfg.dev, config_len, config_data.config_data, cfg.is_firmware_halt);
	if(ret)
		switchtec_perror("mfg otp-record-prog");
	else
	{
		printf("\nOTP Config Programming successful\n");
	}

	return ret;
}

#define CMD_DESC_CONFIG_SET "set device security settings (BL1 and Main Firmware only)"

static int config_set(int argc, char **argv)
{
	int ret;
	struct switchtec_security_cfg_state state = {};
	struct switchtec_security_cfg_set settings = {};

	static struct {
		struct switchtec_dev *dev;
		FILE *setting_fimg;
		char *setting_file;
		int assume_yes;
	} cfg = {};
	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"setting_file", .cfg_type=CFG_FILE_R,
			.value_addr=&cfg.setting_fimg,
			.argument_type=required_positional,
			.help="security setting file"},
		{"yes", 'y', "", CFG_NONE, &cfg.assume_yes, no_argument,
			"assume yes when prompted"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_CONFIG_SET, opts, &cfg, sizeof(cfg));

	if (switchtec_boot_phase(cfg.dev) == SWITCHTEC_BOOT_PHASE_BL2) {
		fprintf(stderr,
			"This command is only available in BL1 or Main Firmware!\n");
		return -1;
	}

	ret = switchtec_security_config_get(cfg.dev, &state);
	if (ret) {
		switchtec_perror("mfg config-set");
		return ret;
	}
	if (state.secure_state != SWITCHTEC_UNINITIALIZED_UNSECURED) {
		fprintf(stderr,
			"This command is only available when secure state is UNINITIALIZED_UNSECURED!\n");
		return -2;
	}

	ret = switchtec_read_sec_cfg_file(cfg.dev, cfg.setting_fimg,
					  &settings);
	fclose(cfg.setting_fimg);
	if (ret == -EBADF) {
		fprintf(stderr, "Invalid secure setting file: %s!\n",
			cfg.setting_file);
		return -3;
	} else if (ret == -ENODEV) {
		fprintf(stderr, "The security setting file is for a different generation of Switchtec device!\n");
		return -5;
	} else if (ret) {
		switchtec_perror("mfg config-set");
	}

	printf("Writing the below settings to device: \n");
	print_security_cfg_set(&settings);

	if (!cfg.assume_yes)
		fprintf(stderr,
			"\nWARNING: This operation makes changes to the device OTP memory and is IRREVERSIBLE!\n");
	ret = ask_if_sure(cfg.assume_yes);
	if (ret)
		return -4;

	ret = switchtec_security_config_set(cfg.dev, &settings);
	if (ret) {
		switchtec_perror("mfg config-set");
		return ret;
	}

	return 0;
}

#define CMD_DESC_KMSK_ENTRY_ADD "add a KSMK entry (BL1 and Main Firmware only)"

#if HAVE_LIBCRYPTO
static int kmsk_entry_add(int argc, char **argv)
{
	int i;
	int ret;
	struct switchtec_kmsk kmsk;
	struct switchtec_pubkey pubk;
	struct switchtec_signature sig;
	struct switchtec_security_cfg_state state = {};

	const char *desc = CMD_DESC_KMSK_ENTRY_ADD "\n\n"
			   "KMSK stands for Key Manifest Secure Key. It is a "
			   "key used to verify the Key Manifest partition, which "
			   "contains keys used to verify all other partitions.\n";
	static struct {
		struct switchtec_dev *dev;
		FILE *pubk_fimg;
		char *pubk_file;
		FILE *sig_fimg;
		char *sig_file;
		FILE *kmsk_fimg;
		char *kmsk_file;
		int assume_yes;
	} cfg = {};
	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"pub_key_file", 'p', .cfg_type=CFG_FILE_R,
			.value_addr=&cfg.pubk_fimg,
			.argument_type=required_argument,
			.help="public key file"},
		{"signature_file", 's', .cfg_type=CFG_FILE_R,
			.value_addr=&cfg.sig_fimg,
			.argument_type=required_argument,
			.help="signature file"},
		{"kmsk_entry_file", 'k', .cfg_type=CFG_FILE_R,
			.value_addr=&cfg.kmsk_fimg,
			.argument_type=required_argument,
			.help="KMSK entry file"},
		{"yes", 'y', "", CFG_NONE, &cfg.assume_yes, no_argument,
		 "assume yes when prompted"},
		{NULL}
	};

	argconfig_parse(argc, argv, desc, opts, &cfg, sizeof(cfg));

	if (cfg.kmsk_file == NULL) {
		fprintf(stderr,
			"KSMK entry file must be set in this command!\n");
		return -1;
	}

	if (switchtec_boot_phase(cfg.dev) == SWITCHTEC_BOOT_PHASE_BL2) {
		fprintf(stderr,
			"This command is only available in BL1 or Main Firmware!\n");
		return -2;
	}

	ret = switchtec_security_config_get(cfg.dev, &state);
	if (ret) {
		switchtec_perror("mfg ksmk-entry-add");
		return ret;
	}
	if (state.secure_state == SWITCHTEC_INITIALIZED_UNSECURED) {
		fprintf(stderr,
			"This command is only valid when secure state is not INITIALIZED_UNSECURED!\n");
		return -3;
	}

	ret = switchtec_read_kmsk_file(cfg.kmsk_fimg, &kmsk);
	fclose(cfg.kmsk_fimg);
	if (ret) {
		fprintf(stderr, "Invalid KMSK file %s!\n", cfg.kmsk_file);
		return -4;
	}

	if (switchtec_security_state_has_kmsk(&state, &kmsk)) {
		if (!cfg.assume_yes)
			fprintf(stderr,
				"WARNING: the specified KMSK entry already exists on the device.\n"
				"Writing duplicate KMSK entries could make your device unbootable!\n");
		ret = ask_if_sure(cfg.assume_yes);
		if (ret)
			return ret;
	}

	if (state.secure_state == SWITCHTEC_INITIALIZED_SECURED &&
	   cfg.pubk_file == NULL) {
		fprintf(stderr,
			"Public key file must be specified when secure state is INITIALIZED_SECURED!\n");
		return -4;
	}

	if (cfg.pubk_file) {
		ret = switchtec_read_pubk_file(cfg.pubk_fimg, &pubk);
		fclose(cfg.pubk_fimg);

		if (ret) {
			fprintf(stderr, "Invalid public key file %s!\n",
				cfg.pubk_file);
			return -5;
		}
	}

	if (state.secure_state == SWITCHTEC_INITIALIZED_SECURED &&
	   cfg.sig_file == NULL) {
		fprintf(stderr,
			"Signature file must be specified when secure state is INITIALIZED_SECURED!\n");
		return -5;
	}

	if (cfg.sig_file) {
		ret = switchtec_read_signature_file(cfg.sig_fimg, &sig);
		fclose(cfg.sig_fimg);

		if (ret) {
			fprintf(stderr, "Invalid signature file %s!\n",
				cfg.sig_file);
			return -6;
		}
	}

	printf("Adding the following KMSK entry to device:\n");
	for(i = 0; i < SWITCHTEC_KMSK_LEN; i++)
		printf("%02x", kmsk.kmsk[i]);
	printf("\n");

	if (!cfg.assume_yes)
		fprintf(stderr,
			"\nWARNING: This operation makes changes to the device OTP memory and is IRREVERSIBLE!\n");
	ret = ask_if_sure(cfg.assume_yes);
	if (ret)
		return -7;

	if (state.secure_state == SWITCHTEC_INITIALIZED_SECURED) {
		ret = switchtec_kmsk_set(cfg.dev, &pubk, &sig, &kmsk);

	}
	else {
		ret = switchtec_kmsk_set(cfg.dev, NULL,	NULL, &kmsk);
	}

	if (ret)
		switchtec_perror("mfg kmsk-entry-add");

	return ret;
}
#endif

#define CMD_DESC_DEBUG_UNLOCK "unlock firmware debug features"

#if HAVE_LIBCRYPTO
static int debug_unlock(int argc, char **argv)
{
	int ret;
	struct switchtec_pubkey pubk;
	struct switchtec_signature sig;

	const char *desc = CMD_DESC_DEBUG_UNLOCK "\n\n"
			   "This command unlocks the EJTAG port, Command Line "
			   "Interface (CLI), MRPC command and Global Address "
			   "Space (GAS) registers.";
	static struct {
		struct switchtec_dev *dev;
		FILE *pubkey_fimg;
		char *pubkey_file;
		unsigned long unlock_version;
		unsigned long serial;
		FILE *sig_fimg;
		char *sig_file;
	} cfg = {
		.unlock_version = 0xffff,
	};
	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"pub_key", 'p', .cfg_type=CFG_FILE_R,
			.value_addr=&cfg.pubkey_fimg,
			.argument_type=required_argument,
			.help="public key file"},
		{"serial_number", 'n', .cfg_type=CFG_LONG,
			.value_addr=&cfg.serial,
			.argument_type=required_argument,
			.help="device serial number"},
		{"unlock_version", 'v', .cfg_type=CFG_LONG,
			.value_addr=&cfg.unlock_version,
			.argument_type=required_argument,
			.help="unlock version"},
		{"signature_file", 's', .cfg_type=CFG_FILE_R,
			.value_addr=&cfg.sig_fimg,
			.argument_type=required_argument,
			.help="signature file"},
		{NULL}
	};

	argconfig_parse(argc, argv, desc, opts, &cfg, sizeof(cfg));

	if (cfg.serial == 0) {
		fprintf(stderr,
			"Serial number must be set for this command!\n");
		return -1;
	}

	if (cfg.unlock_version == 0xffff) {
		fprintf(stderr,
			"Unlock version must be set for this command!\n");
		return -1;
	}

	if (cfg.pubkey_file == NULL) {
		fprintf(stderr,
			"Public key file must be set for this command!\n");
		return -1;
	}

	if (cfg.sig_file == NULL) {
		fprintf(stderr,
			"Signature file must be set for this command!\n");
		return -1;
	}

	ret = switchtec_read_pubk_file(cfg.pubkey_fimg, &pubk);
	fclose(cfg.pubkey_fimg);

	if (ret) {
		fprintf(stderr, "Invalid public key file %s!\n",
			cfg.pubkey_file);
		return -2;
	}

	ret = switchtec_read_signature_file(cfg.sig_fimg, &sig);
	fclose(cfg.sig_fimg);

	if (ret) {
		fprintf(stderr, "Invalid signature file %s!\n",
			cfg.sig_file);
		return -3;
	}

	ret = switchtec_dbg_unlock(cfg.dev, cfg.serial, cfg.unlock_version,
				   &pubk, &sig);
	if (ret)
		switchtec_perror("mfg dbg-unlock");

	return ret;
}
#endif

#define CMD_DESC_DEBUG_LOCK_UPDATE "update debug feature secure unlock version"

#if HAVE_LIBCRYPTO
static int debug_lock_update(int argc, char **argv)
{
	int ret;
	struct switchtec_pubkey pubk;
	struct switchtec_signature sig;

	static struct {
		struct switchtec_dev *dev;
		FILE *pubkey_fimg;
		char *pubkey_file;
		unsigned long unlock_version;
		unsigned long serial;
		FILE *sig_fimg;
		char *sig_file;
		unsigned int assume_yes;
	} cfg = {
		.unlock_version = 0xffff,
	};
	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"pub_key", 'p', .cfg_type=CFG_FILE_R,
			.value_addr=&cfg.pubkey_fimg,
			.argument_type=required_argument,
			.help="public key file"},
		{"serial_number", 'n', .cfg_type=CFG_LONG,
			.value_addr=&cfg.serial,
			.argument_type=required_argument,
			.help="device serial number"},
		{"new_unlock_version", 'v', .cfg_type=CFG_LONG,
			.value_addr=&cfg.unlock_version,
			.argument_type=required_argument,
			.help="new unlock version"},
		{"signature_file", 's', .cfg_type=CFG_FILE_R,
			.value_addr=&cfg.sig_fimg,
			.argument_type=required_argument,
			.help="signature file"},
		{"yes", 'y', "", CFG_NONE, &cfg.assume_yes, no_argument,
			"assume yes when prompted"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_DEBUG_LOCK_UPDATE, opts, &cfg, sizeof(cfg));

	if (cfg.serial == 0) {
		fprintf(stderr,
			"Serial number must be set for this command!\n");
		return -1;
	}

	if (cfg.unlock_version == 0xffff) {
		fprintf(stderr,
			"Unlock version must be set for this command!\n");
		return -1;
	}

	if (cfg.pubkey_file == NULL) {
		fprintf(stderr,
			"Public key file must be set for this command!\n");
		return -1;
	}

	if (cfg.sig_file == NULL) {
		fprintf(stderr,
			"Signature file must be set for this command!\n");
		return -1;
	}

	ret = switchtec_read_pubk_file(cfg.pubkey_fimg, &pubk);
	fclose(cfg.pubkey_fimg);
	if (ret) {
		printf("Invalid public key file %s!\n",
			cfg.pubkey_file);
		return -2;
	}

	ret = switchtec_read_signature_file(cfg.sig_fimg, &sig);
	fclose(cfg.sig_fimg);
	if (ret) {
		printf("Invalid signature file %s!\n",
			cfg.sig_file);
		return -3;
	}

	fprintf(stderr,
		"WARNING: This operation makes changes to the device OTP memory and is IRREVERSIBLE!\n");
	ret = ask_if_sure(cfg.assume_yes);
	if (ret)
		return -4;

	ret = switchtec_dbg_unlock_version_update(cfg.dev, cfg.serial,
						  cfg.unlock_version,
						  &pubk, &sig);
	if (ret)
		switchtec_perror("dbg-lock-update");

	return ret;
}
#endif

#if !HAVE_LIBCRYPTO
static int no_openssl(int argc, char **argv)
{
	fprintf(stderr,
		"This command is only available when the OpenSSL development library is installed.\n"
		"Try installing the OpenSSL development library in your system and rebuild this\n"
		"program by running 'configure' and then 'make'.\n");
	return -1;
}

#define kmsk_entry_add		no_openssl
#define debug_unlock		no_openssl
#define debug_lock_update	no_openssl

#endif

#define CMD_DESC_DEBUG_TOKEN "generate debug unlock token file for the device"
static int debug_unlock_token(int argc, char **argv)
{
	int ret;

	struct switchtec_sn_ver_info sn_info = {};
	struct {
		uint32_t id;
		uint32_t serial;
		uint32_t version;
	} token;

	const char *desc = CMD_DESC_DEBUG_TOKEN "\n\n"
			   "Use the generated token file on your security "
			   "management system to generate the signature file "
			   "required for command 'mfg debug-unlock'";

	struct {
		struct switchtec_dev *dev;
		int out_fd;
		const char *out_filename;
	} cfg = {};
	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"token_file", .cfg_type=CFG_FD_WR, .value_addr=&cfg.out_fd,
		  .argument_type=optional_positional,
		  .force_default="debug.tkn",
		  .help="debug unlock token file"},
		{NULL}};

	argconfig_parse(argc, argv, desc, opts, &cfg, sizeof(cfg));

	ret = switchtec_sn_ver_get(cfg.dev, &sn_info);
	if (ret) {
		switchtec_perror("mfg debug unlock token");
		return ret;
	}

	token.id = htole32(1);
	token.serial = htole32(sn_info.chip_serial);
	token.version = htole32(sn_info.ver_sec_unlock);

	ret = write(cfg.out_fd, &token, sizeof(token));
	if(ret <= 0) {
		switchtec_perror("mfg debug unlock token");
		return ret;
	}

	fprintf(stderr, "\nToken data saved to %s\n", cfg.out_filename);
	close(cfg.out_fd);

	return 0;
}

#define CMD_DESC_INI2BIN "generate bin from ini file"
static int otp_ini2bin(int argc, char **argv)
{
	int ret;
	int binlength;
	char szstr[1024];
	unsigned char bin[10*1024];

	struct {
		struct switchtec_dev *dev;
		FILE *ini_file;
		int out_fd;
		const char *out_filename;
	} cfg = {};

	const struct argconfig_options opts[] = {
		{"ini_file", 'i', .cfg_type=CFG_FILE_R,
			.value_addr=&cfg.ini_file,
			.argument_type=required_argument,
			.help="ini file as an input"},
		{"bin_file", .cfg_type=CFG_FD_WR, .value_addr=&cfg.out_fd,
		  .argument_type=optional_positional,
		  .force_default="otp.bin",
		  .help="OTP Bin file"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_INI2BIN, opts, &cfg, sizeof(cfg));

	ret = switchtec_convert_ini2bin(cfg.ini_file, szstr, bin, &binlength);

	if(ret <= 0) {
		printf("%s", szstr);
		return ret;
	}

	ret = write(cfg.out_fd, &bin, binlength);

	if (!ret) {
		switchtec_perror("otp_ini2bin");
		return ret;
	}

	fprintf(stderr, "\n Success: Bin file saved to %s\n", cfg.out_filename);
	close(cfg.out_fd);

	return 0;
}

#define CMD_DESC_SJTAG_STS_GET "read SJTAG unlock status & mode"
#define CMD_DESC_SJTAG_UNLOCK "SJTAG unlock command"
/*******************************************************************************
* FUNCTION sjtag_unlock()
*_______________________________________________________________________________
*
* DESCRIPTION:
*   This function initiates the SJTAG Unlock sequence
*
* INPUTS:
*   argc    -
*   argv    -
*
* OUTPUTS:
*   None
*
* RETURNS:
*   Returns error code from sub function
*
********************************************************************************/
static int sjtag_unlock(int argc, char **argv)
{
    struct sjtag_status_get status = {};
	struct sjtag_idcode_get sjtag_unlock = {};
    struct sjtag_uuid uuid = {};
	struct sjtag_nonce nonce = {};
    struct switchtec_sn_ver_info sn_info = {};
    uint8_t sjtag_hr[SJTAG_HR_LEN];
	struct sjtag_debug_token debug_token;
	FILE *debug_token_bin_file;
	char dir_path[1024];

    int ret;
    static struct {
        struct switchtec_dev *dev;
		FILE *sjtag_debug_token;
		const char *sjtag_debug_token_file;
        bool verbose;
        bool force_hr;
    } cfg = {
    .verbose = false,
    .force_hr = false};

    const struct argconfig_options opts[] = {
        DEVICE_OPTION_MFG,
		{"sjtag_debug_token_file",
		.cfg_type=CFG_FILE_R,
		.value_addr=&cfg.sjtag_debug_token,
		.argument_type=optional_positional,
		.help="Optional Argument. If not provided, the Debug Token will be generated\n"\
		"SJTAG Debug Token File(.bin)\n" \
		"Generated by the HSM Server\n"},
        {"verbose", 'v', "", CFG_NONE, &cfg.verbose, no_argument,
        "print additional sjtag-unlock information"},
        {"force_hr", 'f', "", CFG_NONE, &cfg.force_hr, no_argument,
        "send HR for all SJTAG mode"},
        {NULL}
    };

    do
    {
        argconfig_parse(argc, argv, CMD_DESC_SJTAG_STS_GET, opts, &cfg, sizeof(cfg));

        ret = switchtec_sjtag_status_get(cfg.dev, &status);
        if (ret)
		{
			switchtec_perror("Failed to retrive the SJTAG Status from the device");
			ret = -1;
			break;
		}

        if(true == cfg.verbose)
        {
            printf("SJTAG Mode: %x\n", (status.data & 0x03));
        }

        /* For Open mode - Unlock status(status.data[2]) should 1 */
        if ((((status.data & 0x03) == 0x00) || ((status.data & 0x04) == 0x04)) && (false == cfg.force_hr))
        {
            printf("SJTAG is in Open State\n");
        }
        else if ((((status.data & 0x03) == 0x03) && ((status.data & 0x04) == 0x00)) && (false == cfg.force_hr))
        {
            printf("SJTAG is in Closed State\n");
        }
        else
        {
            if (cfg.sjtag_debug_token_file)
			{
                ret = switchtec_read_sjtag_debug_token_file(cfg.sjtag_debug_token, &debug_token);
                fclose(cfg.sjtag_debug_token);
                if (ret)
				{
                    switchtec_perror("Invalid SJTAG Debug Token File");
                    ret = -1;
                    break;
                }
            }
            else
            {
                ret = switchtec_sjtag_get_uuid_idcode(cfg.dev, uuid.uuid, sjtag_unlock.idcode);
                if (ret)
				{
					switchtec_perror("Failed to retrive the UUID/IDCODE from the device");
                    ret = -1;
					break;
				}

                ret = switchtec_sn_ver_get(cfg.dev, &sn_info);
				if (ret)
				{
					switchtec_perror("Failed to retrive the SUV from the device");
					ret = -1;
					break;
				}

                ret = sjtag_debug_token_gen(sjtag_unlock.idcode, uuid.uuid, (uint8_t *)&sn_info.ver_sec_unlock, debug_token.debug_token, cfg.verbose);
                if(ret)
                {
                    ret = -1;
                    break;
                }

                debug_token_bin_file = fopen("sjtag_debug_token.bin", "wb");
                if (NULL == debug_token_bin_file)
				{
					switchtec_perror("Error opening file");
					ret = -1;
					break;
                }

                fwrite(debug_token.debug_token, sizeof(uint8_t), SJTAG_DEBUG_TOKEN_LEN, debug_token_bin_file);
                fclose(debug_token_bin_file);
				getcwd(dir_path, sizeof(dir_path));
				printf("Generated SJTAG Debug Token Path: %s/sjtag_debug_token.bin\n", dir_path);
            }

            ret = switchtec_sjtag_get_nonce(cfg.dev, &nonce);
            if (ret)
			{
				switchtec_perror("Failed to retrive the Nonce from the device");
				ret = -1;
				break;
			}

            sjtag_hr_calc(debug_token.debug_token, nonce.nonce, sjtag_hr, cfg.verbose);

            ret = switchtec_sjtag_hr_send(cfg.dev, sjtag_hr);
            if (ret)
            {
                switchtec_perror("mfg sjtag-hr-set");
                printf("SJTAG Unlocking Failed!\n");
            }
            else
            {
                printf("SJTAG Unlocked!\n");
            }
        }
    }while(false);
    return ret;
} /* sjtag_unlock() */

static const struct cmd commands[] = {
	CMD(ping, CMD_DESC_PING),
	CMD(info, CMD_DESC_INFO),
	CMD(secure_version_get, CMD_DESC_VERSIONS_GET),
	CMD(mailbox, CMD_DESC_MAILBOX),
	CMD(image_list, CMD_DESC_IMAGE_LIST),
	CMD(image_select, CMD_DESC_IMAGE_SELECT),
	CMD(fw_transfer, CMD_DESC_FW_TRANSFER),
	CMD(fw_execute, CMD_DESC_FW_EXECUTE),
	CMD(boot_resume, CMD_DESC_BOOT_RESUME),
	CMD(state_set, CMD_DESC_STATE_SET),
	CMD(config_set, CMD_DESC_CONFIG_SET),
	CMD(kmsk_entry_add, CMD_DESC_KMSK_ENTRY_ADD),
	CMD(debug_unlock, CMD_DESC_DEBUG_UNLOCK),
	CMD(debug_lock_update, CMD_DESC_DEBUG_LOCK_UPDATE),
	CMD(debug_unlock_token, CMD_DESC_DEBUG_TOKEN),
	CMD(die_trace_prog, CMD_DESC_DIE_TRACE_PROG),
	CMD(chip_serial_prog, CMD_DESC_CHIP_SERIAL_PROG),
	CMD(sku_set, CMD_DESC_SKU_INFO_SET),
	CMD(sku_get, CMD_DESC_SKU_INFO_GET),
	CMD(otp_write_prot, CMD_DESC_OTP_REGS_WRITE_PROT),
	CMD(sjtag_mode_set, CMD_DESC_SJTAG_MODE_SET),
	CMD(sjtag_uuid_set, CMD_DESC_SJTAG_UUID_SET),
	CMD(sjtag_key_set, CMD_DESC_SJTAG_KEY_SET),
	CMD(sjtag_key_verify, CMD_DESC_SJTAG_KEY_VERIFY),
	CMD(sjtag_key_lock, CMD_DESC_SJTAG_KEY_LOCK),
	CMD(selftest_pol_set, CMD_DESC_SELF_TEST_POLICY_SET),
	CMD(otp_prog_mode_set, CMD_DESC_BIT_BYTE_PROG_MODE_SET),
	CMD(otp_smprg_set, CMD_DESC_SMART_PROG_MODE_SET),
	CMD(otp_mode_status_get, CMD_DESC_MODE_STATUS_GET),
	CMD(otp_cust_id_prog, CMD_DESC_CUST_ID_PROG),
	CMD(otp_config_read, CMD_DESC_OTP_CONFIG_READ),
	CMD(otp_config_prog, CMD_DESC_OTP_CONFIG_PROG),
	CMD(sjtag_unlock, CMD_DESC_SJTAG_UNLOCK),
	CMD(otp_ini2bin, CMD_DESC_INI2BIN),
	{}
};

static struct subcommand subcmd = {
	.name = "mfg",
	.cmds = commands,
	.desc = "Manufacturing Process Commands",
	.long_desc = "These commands control and manage"
		  " mfg settings.",
};

REGISTER_SUBCMD(subcmd);

#endif /* __linux__ */
