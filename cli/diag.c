/*
 * Microsemi Switchtec(tm) PCIe Management Command Line Interface
 * Copyright (c) 2021, Microsemi Corporation
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

#include "config.h"
#include "commands.h"
#include "argconfig.h"
#include "common.h"
#include "progress.h"
#include "graph.h"

#include <switchtec/switchtec.h>
#include <switchtec/utils.h>

#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct diag_common_cfg {
	struct switchtec_dev *dev;
	struct switchtec_status port;
	enum switchtec_diag_end end;
	enum switchtec_diag_link link;
	int port_id;
	int far_end;
	int prev;
};

#define DEFAULT_DIAG_COMMON_CFG {	\
	.port_id = -1,			\
}

#define PORT_OPTION {							\
	"port", 'p', "PORT_ID", CFG_NONNEGATIVE, &cfg.port_id,		\
	required_argument, "physical port ID to dump data for",		\
}
#define FAR_END_OPTION {						\
	"far-end", 'f', "", CFG_NONE, &cfg.far_end, no_argument,	\
	"get the far-end coefficients instead of the local ones",	\
}
#define PREV_OPTION {							\
	"prev", 'P', "", CFG_NONE, &cfg.prev, no_argument,		\
	"return the data for the previous link",			\
}

static int get_port(struct switchtec_dev *dev, int port_id,
		    struct switchtec_status *port)
{
	struct switchtec_status *status;
	int i, ports;

	ports = switchtec_status(dev, &status);
	if (ports < 0) {
		switchtec_perror("status");
		return ports;
	}

	for (i = 0; i < ports; i++) {
		if (status[i].port.phys_id == port_id ||
		    (port_id == -1 && status[i].port.upstream)) {
			*port = status[i];
			switchtec_status_free(status, ports);
			return 0;
		}
	}

	fprintf(stderr, "Invalid physical port id: %d\n", port_id);
	switchtec_status_free(status, ports);
	return -1;
}

static int diag_parse_common_cfg(int argc, char **argv, const char *desc,
				 struct diag_common_cfg *cfg,
				 const struct argconfig_options *opts)
{
	int ret;

	argconfig_parse(argc, argv, desc, opts, cfg, sizeof(*cfg));

	ret = get_port(cfg->dev, cfg->port_id, &cfg->port);
	if (ret)
		return ret;

	cfg->port_id = cfg->port.port.phys_id;

	if (cfg->far_end)
		cfg->end = SWITCHTEC_DIAG_FAR_END;
	else
		cfg->end = SWITCHTEC_DIAG_LOCAL;

	if (cfg->prev)
		cfg->link = SWITCHTEC_DIAG_LINK_PREVIOUS;
	else
		cfg->link = SWITCHTEC_DIAG_LINK_CURRENT;

	return 0;
}

#define CMD_DESC_LTSSM_LOG "Display LTSSM log"
static int ltssm_log(int argc, char **argv) {
	struct diag_common_cfg cfg = DEFAULT_DIAG_COMMON_CFG;

	const struct argconfig_options opts[] = {
		DEVICE_OPTION, PORT_OPTION, {}
	};

	struct switchtec_diag_ltssm_log output[128];
	int ret;
	int port;
	int log_count = 128;
	int i;

	ret = diag_parse_common_cfg(argc, argv, CMD_DESC_LTSSM_LOG,
				    &cfg, opts);
	if (ret)
		return ret;

	if (switchtec_is_gen3(cfg.dev)) {
		fprintf (stderr,
			 "This command is not supported on Gen3 devices\n");
		return 0;
	}
	port = cfg.port_id;
	ret = switchtec_diag_ltssm_log(cfg.dev, port, &log_count, output);
	if (ret) {
		switchtec_perror("ltssm_log");
		return ret;
	}

	printf("LTSSM Log for Physical Port %d (autowrap ON)\n\n", port);
	printf("Idx\tDelta Time\tPCIe Rate\tState\n");
	for(i = 0; i < log_count ; i++) {
		printf("%3d\t", i);
		printf("%09x\t", output[i].timestamp);
		printf("%.1fG\t\t", output[i].link_rate);
		printf("%s\n", switchtec_ltssm_str(output[i].link_state, 1));
	}

	return ret;
}

enum output_format {
	FMT_CSV,
	FMT_TEXT,
	FMT_CURSES,
};

static const struct argconfig_choice output_fmt_choices[] = {
#if defined(HAVE_LIBCURSES) || defined(HAVE_LIBNCURSES)
	#define FMT_DEFAULT FMT_CURSES
	#define FMT_DEFAULT_STR "curses"
	{"curses", FMT_CURSES, "Display data in a curses scrollable window"},
#else
	#define FMT_DEFAULT FMT_TEXT
	#define FMT_DEFAULT_STR "text"
#endif
	{"text", FMT_TEXT, "Display data in a simplified text format"},
	{"csv", FMT_CSV, "Raw Data in CSV format"},
};

int calculate_w2h_score(struct switchtec_diag_port_6p_eye_data *data)
{
	unsigned int width, height_x1, height_x2, w2h_score;

	width = abs(data->eye_left) + abs(data->eye_right);
	height_x1 = abs(data->eye_top_x1) + abs(data->eye_bottom_x2);
	height_x2 = abs(data->eye_top_x2) + abs(data->eye_bottom_x2);

	w2h_score = width * (height_x1 + height_x2);

	return w2h_score;	
}

int eye_observe_dev(struct switchtec_dev *dev, unsigned int error_threshold,
			       int lane_id, int *gen, unsigned int eye_capture_type)
{
	int ret;
	struct switchtec_diag_port_eye_data data_out;
	struct switchtec_diag_port_6p_eye_data data_6p_out;
	int eye_data[4];
	int eye_6p_data[6];
	unsigned int w2h_score;

	if (eye_capture_type == SWTEC_EOM_4P_CAPTURE) {
		ret = switchtec_diag_eye_start(dev, lane_id, error_threshold);
		if (ret) {
			switchtec_perror("eye_start");
		}

		ret = switchtec_diag_eye_fetch(dev, &data_out);
		memcpy(&eye_data[0], &data_out, sizeof(struct switchtec_diag_port_eye_data));
	
		if (!ret) {
			eye_plot_graph(eye_data, false, 0);
		}
	}
	else {
		ret = switchtec_diag_eye_6p_start(dev, lane_id, error_threshold);
		if (ret) {
			switchtec_perror("eye_start");
		}

		ret = switchtec_diag_eye_6p_data_fetch(dev, &data_6p_out);
		memcpy(&eye_6p_data[0], &data_6p_out, sizeof(struct switchtec_diag_port_6p_eye_data));

		if (!ret) {
			w2h_score = calculate_w2h_score(&data_6p_out);
			printf("W2H Score for Lane %d is %d\n", lane_id, w2h_score);
			eye_plot_graph(eye_6p_data, true, w2h_score);
		}
	}
	
	return 0;
}

#define CMD_DESC_EYE_W2H "Capture w2h Score"

static int w2h(int argc, char **argv)
{
	int ret, gen;

	static struct {
          struct switchtec_dev *dev;
          int fmt;
          int lane_id;
          unsigned int error_threshold;
          int t_step, v_step;
  	} cfg = {
          .fmt = FMT_DEFAULT,
          .lane_id = 0,
          .t_step = 1,
          .error_threshold = 4,
          .v_step = 1,
  	};
  	const struct argconfig_options opts[] = {
		DEVICE_OPTION,
		{"format", 'f', "FMT", CFG_CHOICES, &cfg.fmt, required_argument,
		"output format (default: " FMT_DEFAULT_STR ")",
		.choices=output_fmt_choices},
		{"lane", 'l', "LANE_ID", CFG_NONNEGATIVE, &cfg.lane_id,
		required_argument, "lane id within the port to observe"},
		{"error threshold", 't',"ERROR THRESHOLD", CFG_NONNEGATIVE, &cfg.error_threshold,
		optional_argument, "Maximum allowed errors"},
		{"t-step", 's', "NUM", CFG_NONNEGATIVE, &cfg.t_step,
		required_argument, "time step (default 1)"},
		{"v-step", 'S', "NUM", CFG_NONNEGATIVE, &cfg.v_step,
		required_argument, "voltage step (default: 5)"},
		{NULL}};

	argconfig_parse(argc, argv, CMD_DESC_EYE_W2H, opts, &cfg,
			sizeof(cfg));

	ret = eye_observe_dev(cfg.dev, cfg.error_threshold, cfg.lane_id, &gen, SWTEC_EOM_6P_CAPTURE);

	return ret;
}

#define CMD_DESC_EYE "Capture eye data"

static int eye(int argc, char **argv)
{
	int ret, gen;

	static struct {
          struct switchtec_dev *dev;
          int fmt;
          int lane_id;
	  unsigned int error_threshold;
	  int t_step, v_step;
  } cfg = {
          .fmt = FMT_DEFAULT,
          .lane_id = 0,
          .t_step = 1,
	  .error_threshold = 4,
          .v_step = 1,
  };
  const struct argconfig_options opts[] = {
	DEVICE_OPTION,
	{"format", 'f', "FMT", CFG_CHOICES, &cfg.fmt, required_argument,
	"output format (default: " FMT_DEFAULT_STR ")",
	.choices=output_fmt_choices},
	{"lane", 'l', "LANE_ID", CFG_NONNEGATIVE, &cfg.lane_id,
	required_argument, "lane id within the port to observe"},
	{"error threshold", 't',"ERROR THRESHOLD", CFG_NONNEGATIVE, &cfg.error_threshold,
	optional_argument, "Maximum allowed errors"},
	{"t-step", 's', "NUM", CFG_NONNEGATIVE, &cfg.t_step,
	required_argument, "time step (default 1)"},
	{"v-step", 'S', "NUM", CFG_NONNEGATIVE, &cfg.v_step,
	required_argument, "voltage step (default: 5)"},
	{NULL}};

	argconfig_parse(argc, argv, CMD_DESC_EYE, opts, &cfg,
                       sizeof(cfg));

	ret = eye_observe_dev(cfg.dev, cfg.error_threshold, cfg.lane_id, &gen, SWTEC_EOM_4P_CAPTURE);

	return ret;
}

static const struct argconfig_choice loopback_ltssm_speeds[] = {
	{"GEN1", SWITCHTEC_DIAG_LTSSM_GEN1, "GEN1 LTSSM Speed"},
	{"GEN2", SWITCHTEC_DIAG_LTSSM_GEN2, "GEN2 LTSSM Speed"},
	{"GEN3", SWITCHTEC_DIAG_LTSSM_GEN3, "GEN3 LTSSM Speed"},
	{"GEN4", SWITCHTEC_DIAG_LTSSM_GEN4, "GEN4 LTSSM Speed"},
	{}
};

static int print_loopback_mode(struct switchtec_dev *dev, int port_id)
{
	enum switchtec_diag_ltssm_speed speed;
	const struct argconfig_choice *s;
	int ret, b = 0, enable;
	const char *speed_str;
	char buf[100];

	ret = switchtec_diag_loopback_get(dev, port_id, &enable, &speed);
	if (ret) {
		switchtec_perror("loopback_get");
		return -1;
	}

	if (!enable)
		b += snprintf(&buf[b], sizeof(buf) - b, "DISABLED, ");
	if (enable & SWITCHTEC_DIAG_LOOPBACK_RX_TO_TX)
		b += snprintf(&buf[b], sizeof(buf) - b, "RX->TX, ");
	if (enable & SWITCHTEC_DIAG_LOOPBACK_TX_TO_RX)
		b += snprintf(&buf[b], sizeof(buf) - b, "TX->RX, ");
	if (enable & SWITCHTEC_DIAG_LOOPBACK_LTSSM)
		b += snprintf(&buf[b], sizeof(buf) - b, "LTSSM, ");

	/* Drop trailing comma */
	buf[b - 2] = '\0';

	speed_str = "";
	if (enable & SWITCHTEC_DIAG_LOOPBACK_LTSSM) {
		for (s = loopback_ltssm_speeds; s->name; s++) {
			if (s->value == speed) {
				speed_str = s->name;
				break;
			}
		}
	}

	printf("Port: %d    %-30s %s\n", port_id, buf, speed_str);

	return 0;
}

#define CMD_DESC_LOOPBACK "Enable Loopback on specified ports"

static int loopback(int argc, char **argv)
{
	int ret, enable = 0;

	struct {
		struct switchtec_dev *dev;
		struct switchtec_status port;
		int port_id;
		int disable;
		int enable_tx_to_rx;
		int enable_rx_to_tx;
		int enable_ltssm;
		int speed;
	} cfg = {
		.port_id = -1,
		.speed = SWITCHTEC_DIAG_LTSSM_GEN4,
	};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION,
		{"port", 'p', "PORT_ID", CFG_NONNEGATIVE, &cfg.port_id,
		 required_argument, "physical port ID to set/get loopback for"},
		{"disable", 'd', "", CFG_NONE, &cfg.disable, no_argument,
		 "Disable all loopback modes"},
		{"ltssm", 'l', "", CFG_NONE, &cfg.enable_ltssm, no_argument,
		 "Enable LTSSM loopback mode"},
		{"rx-to-tx", 'r', "", CFG_NONE, &cfg.enable_rx_to_tx, no_argument,
		 "Enable RX->TX loopback mode"},
		{"tx-to-rx", 't', "", CFG_NONE, &cfg.enable_tx_to_rx, no_argument,
		 "Enable TX->RX loopback mode"},
		{"speed", 's', "GEN", CFG_CHOICES, &cfg.speed, required_argument,
		 "LTSSM Speed (if enabling the LTSSM loopback mode), default: GEN4",
		 .choices = loopback_ltssm_speeds},
		{NULL}};

	argconfig_parse(argc, argv, CMD_DESC_LOOPBACK, opts, &cfg, sizeof(cfg));

	if (cfg.port_id < 0) {
		fprintf(stderr, "Must specify -p / --port_id\n");
		return -1;
	}

	if (cfg.disable && (cfg.enable_rx_to_tx || cfg.enable_tx_to_rx ||
			    cfg.enable_ltssm)) {
		fprintf(stderr,
			"Must not specify -d / --disable with an enable flag\n");
		return -1;
	}

	ret = get_port(cfg.dev, cfg.port_id, &cfg.port);
	if (ret)
		return ret;

	if (cfg.disable || cfg.enable_rx_to_tx || cfg.enable_tx_to_rx ||
	    cfg.enable_ltssm) {
		if (cfg.enable_rx_to_tx)
			enable |= SWITCHTEC_DIAG_LOOPBACK_RX_TO_TX;
		if (cfg.enable_tx_to_rx)
			enable |= SWITCHTEC_DIAG_LOOPBACK_TX_TO_RX;
		if (cfg.enable_ltssm)
			enable |= SWITCHTEC_DIAG_LOOPBACK_LTSSM;

		ret = switchtec_diag_loopback_set(cfg.dev, cfg.port_id,
						  enable, cfg.speed);
		if (ret) {
			switchtec_perror("loopback_set");
			return -1;
		}
	}

	return print_loopback_mode(cfg.dev, cfg.port_id);
}

static const struct argconfig_choice pattern_types[] = {
	{"PRBS7",   SWITCHTEC_DIAG_PATTERN_PRBS_7,  "PRBS 7"},
	{"PRBS11",  SWITCHTEC_DIAG_PATTERN_PRBS_11, "PRBS 11"},
	{"PRBS23",  SWITCHTEC_DIAG_PATTERN_PRBS_23, "PRBS 23"},
	{"PRBS31",  SWITCHTEC_DIAG_PATTERN_PRBS_31, "PRBS 31"},
	{"PRBS9",   SWITCHTEC_DIAG_PATTERN_PRBS_9,  "PRBS 9"},
	{"PRBS15",  SWITCHTEC_DIAG_PATTERN_PRBS_15, "PRBS 15"},
	{}
};

static const char *pattern_to_str(enum switchtec_diag_pattern type)
{
	const struct argconfig_choice *s;

	for (s = pattern_types; s->name; s++) {
		if (s->value == type)
			return s->name;
	}

	return "UNKNOWN";
}

static int print_pattern_mode(struct switchtec_dev *dev,
		struct switchtec_status *port, int port_id)
{
	enum switchtec_diag_pattern gen_pat, mon_pat;
	unsigned long long err_cnt;
	int ret, lane_id;

	ret = switchtec_diag_pattern_gen_get(dev, port_id, &gen_pat);
	if (ret) {
		switchtec_perror("pattern_gen_get");
		return -1;
	}

	ret = switchtec_diag_pattern_mon_get(dev, port_id, 0, &mon_pat, &err_cnt);
	if (ret) {
		switchtec_perror("pattern_mon_get");
		return -1;
	}

	printf("Port: %d\n", port_id);
	if (gen_pat == SWITCHTEC_DIAG_PATTERN_PRBS_DISABLED)
		printf("  Generator: Disabled\n");
	else
		printf("  Generator: %s\n", pattern_to_str(gen_pat));

	if (mon_pat == SWITCHTEC_DIAG_PATTERN_PRBS_DISABLED) {
		printf("  Monitor: Disabled\n");
	} else {
		printf("  Monitor: %-20s\n", pattern_to_str(mon_pat));
		printf("    Lane %-2d    Errors: 0x%llx\n", 0, err_cnt);
		for (lane_id = 1; lane_id < port->cfg_lnk_width; lane_id++) {
			ret = switchtec_diag_pattern_mon_get(dev, port_id,
					lane_id, NULL, &err_cnt);
			printf("    Lane %-2d    Errors: 0x%llx\n", lane_id,
			       err_cnt);
			if (ret) {
				switchtec_perror("pattern_mon_get");
				return -1;
			}
		}
	}

	return 0;
}

#define CMD_DESC_PATTERN "Enable pattern generation and monitor"

static int pattern(int argc, char **argv)
{
	int ret;

	struct {
		struct switchtec_dev *dev;
		struct switchtec_status port;
		int port_id;
		int disable;
		int generate;
		int monitor;
		int pattern;
		int inject_errs;
	} cfg = {
		.port_id = -1,
		.pattern = SWITCHTEC_DIAG_PATTERN_PRBS_31,
	};

	const struct argconfig_options opts[] = {
		DEVICE_OPTION,
		{"port", 'p', "PORT_ID", CFG_NONNEGATIVE, &cfg.port_id,
		 required_argument, "physical port ID to set/get loopback for"},
		{"disable", 'd', "", CFG_NONE, &cfg.disable, no_argument,
		 "Disable all generators and monitors"},
		{"inject", 'i', "NUM", CFG_NONNEGATIVE, &cfg.inject_errs,
		 required_argument,
		 "Inject the specified number of errors into all lanes of the TX port"},
		{"generate", 'g', "", CFG_NONE, &cfg.generate, no_argument,
		 "Enable Pattern Generator on specified port"},
		{"monitor", 'm', "", CFG_NONE, &cfg.monitor, no_argument,
		 "Enable Pattern Monitor on specified port"},
		{"pattern", 't', "PATTERN", CFG_CHOICES, &cfg.pattern,
		 required_argument,
		 "pattern to generate or monitor for (default: PRBS31)",
		 .choices = pattern_types},
		{NULL}};

	argconfig_parse(argc, argv, CMD_DESC_PATTERN, opts, &cfg, sizeof(cfg));

	if (cfg.port_id < 0) {
		fprintf(stderr, "Must specify -p / --port_id\n");
		return -1;
	}

	if (cfg.disable && (cfg.generate || cfg.monitor)) {
		fprintf(stderr,
			"Must not specify -d / --disable with an enable flag\n");
		return -1;
	}

	ret = get_port(cfg.dev, cfg.port_id, &cfg.port);
	if (ret)
		return ret;

	if (cfg.disable) {
		cfg.generate = 1;
		cfg.monitor = 1;
		cfg.pattern = SWITCHTEC_DIAG_PATTERN_PRBS_DISABLED;
	}

	if (cfg.monitor) {
		ret = switchtec_diag_pattern_mon_set(cfg.dev, cfg.port_id,
						     cfg.pattern);
		if (ret) {
			switchtec_perror("pattern_mon_set");
			return -1;
		}
	}

	if (cfg.generate) {
		ret = switchtec_diag_pattern_gen_set(cfg.dev, cfg.port_id,
						     cfg.pattern);
		if (ret) {
			switchtec_perror("pattern_gen_set");
			return -1;
		}
	}

	if (cfg.inject_errs > 1000) {
		fprintf(stderr, "Too many errors to inject. --inject / -i must be less than 1000\n");
		return -1;
	}

	if (cfg.inject_errs) {
		ret = switchtec_diag_pattern_inject(cfg.dev, cfg.port_id,
						    cfg.inject_errs);
		if (ret) {
			switchtec_perror("pattern_inject");
			return -1;
		}
		printf("Injected %d errors\n", cfg.inject_errs);
		return 0;
	}

	return print_pattern_mode(cfg.dev, &cfg.port, cfg.port_id);
}

#define CMD_DESC_LIST_MRPC "List permissible MRPC commands"

static int list_mrpc(int argc, char **argv)
{
	struct switchtec_mrpc table[MRPC_MAX_ID];
	int i, ret;

	static struct {
		struct switchtec_dev *dev;
		int all;
	} cfg = {};
	const struct argconfig_options opts[] = {
		DEVICE_OPTION,
		{"all", 'a', "", CFG_NONE, &cfg.all, no_argument,
		 "print all MRPC commands, including ones that are unknown"},
		{NULL}};

	argconfig_parse(argc, argv, CMD_DESC_LIST_MRPC, opts, &cfg,
			sizeof(cfg));
	ret = switchtec_diag_perm_table(cfg.dev, table);
	if (ret) {
		switchtec_perror("perm_table");
		return -1;
	}

	for (i = 0; i < MRPC_MAX_ID; i++) {
		if (!table[i].tag)
			continue;
		if (!cfg.all && table[i].reserved)
			continue;

		printf("  0x%03x  %-25s  %s\n", i, table[i].tag,
		       table[i].desc);
	}

	return 0;
}

#define CMD_DESC_PORT_EQ_TXCOEFF "Dump port equalization coefficients"

static int port_eq_txcoeff(int argc, char **argv)
{
	struct diag_common_cfg cfg = DEFAULT_DIAG_COMMON_CFG;
	struct switchtec_port_eq_coeff coeff;
	int i, ret;

	const struct argconfig_options opts[] = {
		DEVICE_OPTION, FAR_END_OPTION, PORT_OPTION, PREV_OPTION, {}
	};

	ret = diag_parse_common_cfg(argc, argv, CMD_DESC_PORT_EQ_TXCOEFF,
				    &cfg, opts);
	if (ret)
		return ret;

	ret = switchtec_diag_port_eq_tx_coeff(cfg.dev, cfg.port_id, cfg.end,
					      cfg.link, &coeff);
	if (ret) {
		switchtec_perror("port_eq_coeff");
		return -1;
	}

	printf("%s TX Coefficients for physical port %d %s\n\n",
	       cfg.far_end ? "Far End" : "Local", cfg.port_id,
	       cfg.prev ? "(Previous Link-Up)" : "");
	printf("Lane  Pre-Cursor  Post-Cursor\n");

	for (i = 0; i < coeff.lane_cnt; i++) {
		printf("%4d  %7d      %8d\n", i, coeff.cursors[i].pre,
		       coeff.cursors[i].post);
	}

	return 0;
}

#define CMD_DESC_PORT_EQ_TXFSLF "Dump FS/LF output data"

static int port_eq_txfslf(int argc, char **argv)
{
	struct diag_common_cfg cfg = DEFAULT_DIAG_COMMON_CFG;
	struct switchtec_port_eq_tx_fslf data;
	int i, ret;

	const struct argconfig_options opts[] = {
		DEVICE_OPTION, FAR_END_OPTION, PORT_OPTION, PREV_OPTION, {}
	};

	ret = diag_parse_common_cfg(argc, argv, CMD_DESC_PORT_EQ_TXFSLF,
				    &cfg, opts);
	if (ret)
		return ret;

	printf("%s Equalization FS/LF data for physical port %d %s\n\n",
	       cfg.far_end ? "Far End" : "Local", cfg.port_id,
	       cfg.prev ? "(Previous Link-Up)" : "");
	printf("Lane    FS    LF\n");

	for (i = 0; i < cfg.port.neg_lnk_width; i++) {
		ret = switchtec_diag_port_eq_tx_fslf(cfg.dev, cfg.port_id, i,
				cfg.end, cfg.link, &data);
		if (ret) {
			switchtec_perror("port_eq_fs_ls");
			return -1;
		}

		printf("%4d  %4d  %4d\n", i, data.fs, data.lf);
	}

	return 0;
}

#define CMD_DESC_PORT_EQ_TXTABLE "Dump far end port equalization table"

static int port_eq_txtable(int argc, char **argv)
{
	struct diag_common_cfg cfg = DEFAULT_DIAG_COMMON_CFG;
	struct switchtec_port_eq_table table;
	int i, ret;

	const struct argconfig_options opts[] = {
		DEVICE_OPTION, PORT_OPTION, PREV_OPTION, {}
	};

	ret = diag_parse_common_cfg(argc, argv, CMD_DESC_PORT_EQ_TXTABLE,
				    &cfg, opts);
	if (ret)
		return ret;

	ret = switchtec_diag_port_eq_tx_table(cfg.dev, cfg.port_id,
					      cfg.link, &table);
	if (ret) {
		switchtec_perror("port_eq_table");
		return -1;
	}

	printf("Far End TX Equalization Table for physical port %d, lane %d %s\n\n",
	       cfg.port_id, table.lane_id, cfg.prev ? "(Previous Link-Up)" : "");
	printf("Step  Pre-Cursor  Post-Cursor  FOM  Pre-Up  Post-Up  Error  Active  Speed\n");

	for (i = 0; i < table.step_cnt; i++) {
		printf("%4d  %10d  %11d  %3d  %6d  %7d  %5d  %6d  %5d\n",
		       i, table.steps[i].pre_cursor, table.steps[i].post_cursor,
		       table.steps[i].fom, table.steps[i].pre_cursor_up,
		       table.steps[i].post_cursor_up, table.steps[i].error_status,
		       table.steps[i].active_status, table.steps[i].speed);
	}

	return 0;
}

#define CMD_DESC_RCVR_OBJ "Dump analog RX coefficients/adaptation objects"

static int rcvr_obj(int argc, char **argv)
{
	struct diag_common_cfg cfg = DEFAULT_DIAG_COMMON_CFG;
	struct switchtec_rcvr_obj obj;
	int i, j, ret;

	const struct argconfig_options opts[] = {
		DEVICE_OPTION, PORT_OPTION, PREV_OPTION, {}
	};

	ret = diag_parse_common_cfg(argc, argv, CMD_DESC_RCVR_OBJ,
				    &cfg, opts);
	if (ret)
		return ret;

	printf("Coefficients for physical port %d %s\n\n", cfg.port_id,
	       cfg.prev ? "(Previous Link-Up)" : "");
	printf("Lane  CTLE  Tgt-Amp  Spec-DFE  DFE0 DFE1 DFE2 DFE3 DFE4 DFE5 DFE6\n");

	for (i = 0; i < cfg.port.neg_lnk_width; i++) {
		ret = switchtec_diag_rcvr_obj(cfg.dev, cfg.port_id, i,
					      cfg.link, &obj);
		if (ret) {
			switchtec_perror("rcvr_obj");
			return -1;
		}

		printf("%4d  %4d  %6d   %7d   ", i, obj.ctle,
		       obj.target_amplitude, obj.speculative_dfe);
		for (j = 0; j < ARRAY_SIZE(obj.dynamic_dfe); j++)
			printf("%4d ", obj.dynamic_dfe[j]);
		printf("\n");
	}

	return 0;
}

#define CMD_DESC_RCVR_EXTENDED "Dump RX mode and DTCLK"

static int rcvr_extended(int argc, char **argv)
{
	struct diag_common_cfg cfg = DEFAULT_DIAG_COMMON_CFG;
	struct switchtec_rcvr_ext ext;
	int i, ret;

	const struct argconfig_options opts[] = {
		DEVICE_OPTION, PORT_OPTION, PREV_OPTION, {}
	};

	ret = diag_parse_common_cfg(argc, argv, CMD_DESC_RCVR_EXTENDED,
				    &cfg, opts);
	if (ret)
		return ret;

	printf("Mode and DTCLCK for physical port %d %s\n\n",
	       cfg.port_id, cfg.prev ? "(Previous Link-Up)" : "");
	printf("Lane      MODE   DTCLK_5  DTCLK_8_6  DTCLK_9\n");

	for (i = 0; i < cfg.port.neg_lnk_width; i++) {
		ret = switchtec_diag_rcvr_ext(cfg.dev, cfg.port_id, i,
					      cfg.link, &ext);
		if (ret) {
			switchtec_perror("rx_mode");
			return -1;
		}

		printf("%4d  %#8x  %7d  %9d  %7d\n", i, ext.ctle2_rx_mode,
		       ext.dtclk_5, ext.dtclk_8_6, ext.dtclk_9);
	}

	return 0;
}

static const struct cmd commands[] = {
	CMD(w2h,		CMD_DESC_EYE_W2H),
	CMD(eye,		CMD_DESC_EYE),
	CMD(list_mrpc,		CMD_DESC_LIST_MRPC),
	CMD(loopback,		CMD_DESC_LOOPBACK),
	CMD(pattern,		CMD_DESC_PATTERN),
	CMD(port_eq_txcoeff,	CMD_DESC_PORT_EQ_TXCOEFF),
	CMD(port_eq_txfslf,	CMD_DESC_PORT_EQ_TXFSLF),
	CMD(port_eq_txtable,	CMD_DESC_PORT_EQ_TXTABLE),
	CMD(rcvr_extended,	CMD_DESC_RCVR_EXTENDED),
	CMD(rcvr_obj,		CMD_DESC_RCVR_OBJ),
	CMD(ltssm_log,		CMD_DESC_LTSSM_LOG),
	{}
};

static struct subcommand subcmd = {
	.name = "diag",
	.cmds = commands,
	.desc = "Diagnostic Information",
	.long_desc = "These functions provide diagnostic information from "
		"the switch",
};

REGISTER_SUBCMD(subcmd);
