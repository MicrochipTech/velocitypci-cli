/*
 * Microsemi Switchtec(tm) PCIe Management Command Line Interface
 * Copyright (c) 2017, Microsemi Corporation
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

#include <cli/commands.h>
#include <cli/argconfig.h>
#include <cli/common.h>

#include <switchtec/switchtec.h>
#include <switchtec/portable.h>
#include <switchtec/fabric.h>
#include <switchtec/utils.h>
#include <tests/common/ut_handler.h>
#include <lib/switchtec_priv.h>


#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <errno.h>
#include <ctype.h>
#include <inttypes.h>
#include <stddef.h>

#define CMD_DESC_UART_UT_HANDLER "UART unit testing command"
#define CMD_DESC_TWI_UT_HANDLER "TWI unit testing command"

struct switchtec_uart{
	struct switchtec_dev dev;
	int fd;
};

#define to_switchtec_uart(d) \
	((struct switchtec_uart *) \
	 ((char *)(d) - offsetof(struct switchtec_uart, dev)))

#define UART_MAX_WRITE_BYTES			100
#define UART_MAX_READ_BYTES			1024
#define RETRY_NUM				3
#define SWITCHTEC_UART_BAUDRATE			(B230400)

static int send_cmd(int fd, const char *fmt, int write_bytes, ...)
{
	int ret;
	int i;
	int cnt;
	char cmd[1024];
	uint8_t *write_data;
	uint32_t write_crc;
	va_list argp;

	va_start(argp, write_bytes);

	if (write_bytes) {
		write_data = va_arg(argp, uint8_t *);
		write_crc = va_arg(argp, uint32_t);
	}

	cnt = vsnprintf(cmd, sizeof(cmd), fmt, argp);

	if (write_bytes) {
		for (i = 0; i< write_bytes; i++) {
			cnt += snprintf(cmd + cnt, sizeof(cmd) - cnt,
				       "%02x", write_data[write_bytes - 1 - i]);
		}

		cnt += snprintf(cmd + cnt, sizeof(cmd) - cnt,
				" 0x%x\r", write_crc);
	}

	va_end(argp);

	ret = write(fd, cmd, cnt);
	if (ret < 0)
		return ret;

	if (ret != cnt) {
		errno = EIO;
		return -errno;
	}

	return 0;
}

static int read_resp_line(int fd, char *str)
{
	int ret;
	int cnt = 0;

	while(1) {
		ret = read(fd, str + cnt, sizeof(str));
		if (ret <= 0)
			return ret;

		cnt += ret;
		str[cnt] = '\0';
		
		/* Prompt "0x12345678:1234>" */
		if (strrchr(str, ':') + 5 == strrchr(str, '>'))
			return 0;
	}

	return -1;
}

int cli_control(struct switchtec_dev *dev, const char *str)
{
	int ret;
	char rtn[1024];
	struct switchtec_uart *udev = to_switchtec_uart(dev);

	ret = send_cmd(udev->fd, str, 0);
	if (ret)
		return ret;

	ret =  read_resp_line(udev->fd, rtn);
	printf("%s", rtn);
	if (ret)
		return ret;

	return 0;
}

struct test_cmds {
	char test_string[512];
};

static int read_input_file(FILE *ip_file, struct test_cmds *test_list, int *test_count)
{
	char line[512];
	int i = 0;

	while (fgets(line, sizeof(line), ip_file)) {
		/* ignore comments */
		if (line[0] == '#')
			continue;

		/* strip any newline characters */
		line[strcspn(line, "\r\n")] = '\0';

		strcpy (test_list[i].test_string , strdup(line));

		i++;
	}

	*test_count = i;

	return 0;
}

static int cmn_cmd(int argc, char **argv, struct test_cmds *cmds, int *count)
{
         int ret = 0;

         static struct {
                 struct switchtec_dev *dev;
                 char *uart_cmd;
		 FILE *input_file;
         } cfg = {
                 .uart_cmd = NULL,
		 .input_file = NULL,
         };

         const struct argconfig_options opts[] = {
                 DEVICE_OPTION,
                 {"uart_cmd", 'c', "UART_CMD", CFG_STRING, &cfg.uart_cmd,
                  optional_argument, .require_in_usage = 1,
                  .help = "Uart command"},
		 {"ut_file", 'C', "FILE", CFG_FILE_R, &cfg.input_file,
		 required_argument,
		 "Unit testing commands in file"},
                 {NULL}
         };

         argconfig_parse(argc, argv, CMD_DESC_UART_UT_HANDLER, opts, &cfg, sizeof(cfg));

	 if (cfg.uart_cmd == NULL) {

	 	if (cfg.input_file == NULL){
			 printf ("file name not given\n");
			 return -1;
		}

		 ret = read_input_file(cfg.input_file, cmds, count);

		 for (int i = 0; i < *count; i++){
                	printf("Starting Test command: %s\n", cmds[i].test_string);
                 	strcat(cmds[i].test_string, "\r");
                 	ret = cli_control(cfg.dev, cmds[i].test_string);
		 }
         }
	 else {
		printf("%s",cfg.uart_cmd);

	 	strcat(cfg.uart_cmd, "\r");
	 
	 	ret = cli_control(cfg.dev, cfg.uart_cmd);
	 }

         return ret;
}

static int uart(int argc, char **argv)
{
	int ret = 0;
	int cnt = 0;
	struct test_cmds cmds[512];

	ret = cmn_cmd(argc, argv, cmds, &cnt);

	return ret;
}

#define CMD_DESC_I2C_CMD "I2C MRPC command for Unit test"

static int twi(int argc, char **argv)
{
	uint8_t *payload = NULL;
	uint8_t *temp = NULL;
	uint8_t payload_len;
	
	uint32_t response;
	int ret;
	static struct {
		struct switchtec_dev *dev;
		uint32_t i2c_cmd;
		uint32_t test_id;
		uint8_t param_len;
	} cfg = {
            	.i2c_cmd = 0,
				.test_id = 0,
				.param_len = 0,
         	};
	const struct argconfig_options opts[] = {
		DEVICE_OPTION_MFG,
		{"i2c_cmd", 'c', "I2C_CMD", CFG_LONG, &cfg.i2c_cmd,
        required_argument, .require_in_usage = 1, .help = "i2c command"},
		{"test_id", 't', "TEST_ID", CFG_LONG, &cfg.test_id,
        required_argument, .require_in_usage = 1, .help = "test id"},
		{"param_len", 'l', "PARAM_LEN", CFG_BYTE, &cfg.param_len,
        optional_argument, .require_in_usage = 1, .help = "param_len"},
		{NULL}
	};

	argconfig_parse(argc, argv, CMD_DESC_I2C_CMD, opts, &cfg, sizeof(cfg));

	if(cfg.param_len == 0)
		payload_len = 4;	/* Test ID */
	else
		payload_len = 5 + cfg.param_len; /* Test ID(4) + Parameter Length(1) + Number of Parameters */

	payload = (uint8_t *)malloc((sizeof(uint8_t) * payload_len));

	if(!payload)
		return -1;

	temp = (uint8_t *)&(cfg.test_id);
	payload[0] = temp[0];
	payload[1] = temp[1];
	payload[2] = temp[2];
	payload[3] = temp[3];

	if(cfg.param_len != 0)
	{
		payload[4] = cfg.param_len;

		for(uint8_t i = 0; i < cfg.param_len; i++)
		{
			payload[5+i] = atoi((argv[5+i]));
		}
	}
	
	ret = switchtec_cmd(cfg.dev, cfg.i2c_cmd, payload,
			    payload_len, &response, sizeof(response));

	if (ret) {
		switchtec_perror("I2C command error");
		return ret;
	}

	printf("I2C command :0x%X, Test ID : %d, Response : %s \n", cfg.i2c_cmd, cfg.test_id, response?"FAIL":"PASS");

	return 0;
}

static const struct cmd commands[] = {
	CMD(uart, CMD_DESC_UART_UT_HANDLER),
	CMD(twi, CMD_DESC_TWI_UT_HANDLER),
	{},
};

static struct subcommand subcmd = {
	.name = "ut",
	.cmds = commands,
	.desc = "unit testing commands",
	.long_desc = "These functions provide diagnostic information from "
		"the switch",
};

REGISTER_SUBCMD(subcmd);



#endif//__linux__
