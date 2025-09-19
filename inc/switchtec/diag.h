/*
 * Microsemi Switchtec(tm) PCIe Management Library
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

#ifndef LIBSWITCHTEC_DIAG_H
#define LIBSWITCHTEC_DIAG_H

#include <stdint.h>

/**
 * @file
 * @brief Diagnostic structures
 */

struct switchtec_diag_rcvr_obj_dump_in {
	uint8_t port_id;
	uint8_t lane_id;
	uint16_t resvd;
};

struct switchtec_diag_rcvr_obj_dump_out {
	uint8_t port_id;
	uint8_t lane_id;
	uint8_t ctle;
	uint8_t target_amplitude;
	uint8_t speculative_dfe;
	int8_t dynamic_dfe[7];
};

enum {
	DIAG_PORT_EQ_STATUS_OP_PER_PORT = 0,
	DIAG_PORT_EQ_STATUS_OP_PER_LANE = 1,
};

enum {
	SWTEC_EOM_4P_CAPTURE = 0,
	SWTEC_EOM_6P_CAPTURE = 1,
};

struct switchtec_diag_port_eq_status_in {
	uint8_t sub_cmd;
	uint8_t op_type;
	uint8_t port_id;
	uint8_t lane_id;
};

struct switchtec_diag_port_eq_status_in2 {
	uint8_t sub_cmd;
	uint8_t port_id;
	uint8_t lane_id;
	uint8_t resvd;
};

struct switchtec_diag_port_eq_status_out {
	uint8_t sub_cmd;
	uint8_t op_type;
	uint8_t port_id;
	uint8_t lane_id;

	struct {
		uint8_t pre;
		uint8_t post;
	} cursors[16];
};

struct switchtec_diag_port_eq_table_out {
	uint8_t sub_cmd;
	uint8_t port_id;
	uint8_t lane_id;
	uint8_t step_cnt;
	struct {
		uint8_t pre_cursor;
		uint8_t post_cursor;
		uint8_t fom;
		uint8_t pre_cursor_up;
		uint8_t post_cursor_up;
		uint8_t error_status;
		uint8_t active_status;
		uint8_t speed;
	} steps[126];
};

struct switchtec_diag_port_eq_tx_fslf_out {
	uint8_t sub_cmd;
	uint8_t port_id;
	uint8_t lane_id;
	uint8_t fs;
	uint8_t lf;
	uint8_t resvd[3];
};

struct switchtec_diag_ext_recv_obj_dump_in {
	uint8_t sub_cmd;
	uint8_t port_id;
	uint8_t lane_id;
	uint8_t resvd;
};

struct switchtec_diag_ext_dump_coeff_prev_in {
	uint8_t sub_cmd;
	uint8_t op_type;
	uint8_t port_id;
	uint8_t lane_id;
};

struct switchtec_diag_rcvr_ext_out {
	uint8_t port_id;
	uint8_t lane_id;
	uint16_t ctle2_rx_mode;
	uint8_t dtclk_9;
	uint8_t dtclk_8_6;
	uint8_t dtclk_5;
};

struct switchtec_diag_refclk_ctl_in {
	uint8_t sub_cmd;
	uint8_t stack_id;
};

enum switchtec_diag_loopback_type {
	DIAG_LOOPBACK_RX_TO_TX = 0,
	DIAG_LOOPBACK_TX_TO_RX = 1,
};

struct switchtec_diag_loopback_in {
	uint8_t sub_cmd;
	uint8_t port_id;
	uint8_t enable;
	uint8_t type;
};

struct switchtec_diag_loopback_out {
	uint8_t port_id;
	uint8_t enabled;
	uint8_t type;
	uint8_t resvdd;
};

struct switchtec_diag_loopback_ltssm_in {
	uint8_t sub_cmd;
	uint8_t port_id;
	uint8_t enable;
	uint8_t speed;
};

struct switchtec_diag_loopback_ltssm_out {
	uint8_t port_id;
	uint8_t enabled;
	uint8_t speed;
	uint8_t resvd;
};

struct switchtec_diag_pat_gen_in {
	uint8_t sub_cmd;
	uint8_t port_id;
	uint8_t pattern_type;
	uint8_t lane_id;
};

struct switchtec_diag_pat_gen_inject {
	uint8_t sub_cmd;
	uint8_t port_id;
	uint16_t resvd;
	uint32_t err_cnt;
};

struct switchtec_diag_pat_gen_out {
	uint8_t port_id;
	uint8_t pattern_type;
	uint16_t resvd;
	uint32_t err_cnt_lo;
	uint32_t err_cnt_hi;
};

#pragma pack (push,1)
struct switchtec_diag_port_eye_start {
	uint8_t sub_cmd;
	uint8_t resvd1[3];
	/*defines the lane ID to run */
	uint8_t lane_id;
	/*Target BER type */
	uint8_t target_ber_type;
	/*Defines the maximum numbers of errors*/
	uint8_t max_allowed_ber_errors;
	/*Defines the confidence Level*/
	uint8_t confidence_lvl;
	/*defines the horizontal step size*/
	int8_t x_step;
	/*defines the vertical step size*/
	int8_t y_step;
	/*Defines the vertical sweep at x1 for six point capture*/
	int8_t v_sweep_at_x1;
	/*Defines the vertical sweep at x2 for six point capture*/
	int8_t v_sweep_at_x2;
	/*Defines the Time in recovery before link down value for EOM*/
	uint32_t eom_time_in_recov_in_us;
	/*defines the error method 0-parity error method, 1-l2r error method*/
	uint8_t eom_error_method;
	uint8_t reserved[3];
};

struct switchtec_diag_port_eye_cmd {
	uint8_t sub_cmd;
	uint8_t data_mode;
	uint8_t resvd;
	uint8_t status;
};

struct switchtec_diag_port_eye_fetch {
	uint8_t sub_cmd;
	uint8_t data_mode;
	uint8_t resvd1;
	uint8_t status;
	int32_t eye_left;
	int32_t eye_right;
	int32_t eye_top_x1;
	int32_t eye_bottom_x1;
	int32_t eye_top_x2;
	int32_t eye_bottom_x2;
};

struct switchtec_diag_port_6p_eye_data {
	int32_t eye_left;
	int32_t eye_right;
	int32_t eye_top_x1;
	int32_t eye_bottom_x1;
	int32_t eye_top_x2;
	int32_t eye_bottom_x2;
};

struct switchtec_diag_port_eye_data {
	int32_t eye_left;
	int32_t eye_right;
	int32_t eye_top_x1;
	int32_t eye_bottom_x1;
};
#pragma pack(pop)

struct switchtec_diag_cross_hair_in {
	uint8_t sub_cmd;
	uint8_t lane_id;
	uint8_t all_lanes;
	uint8_t num_lanes;
};

struct switchtec_diag_cross_hair_get {
	uint8_t lane_id;
	uint8_t state;

	union {
		struct {
			int8_t byte0;
			int8_t byte1;
			int16_t word0;
			int16_t word1;
			int16_t word2;
			int16_t word3;
		};
		struct {
			uint8_t prev_state;
			uint8_t _byte1;
			int16_t x_pos;
			int16_t y_pos;
		};
		struct {
			int8_t eye_left_lim;
			int8_t eye_right_lim;
			int16_t eye_bot_left_lim;
			int16_t eye_bot_right_lim;
			int16_t eye_top_left_lim;
			int16_t eye_top_right_lim;
		};
	};
};

typedef enum {
    /* command error code for Start, fetch and cancel */
    EOM_SUCCESS,                  /* EOM general result, means no error */

    /* Error codes for EOM Start sub command */
    EOM_FAILED_TO_START,          /* EOM Failed to Start*/
    EOM_INVALID_PARAMETER,        /* EOM parameter error */
    EOM_PREV_CMD_NOT_FINISHED,    /* EOM cmd error, the previous cmd has not finished */

    /* Error codes for EOM Fetch Sub command */
    EOM_IN_PROGRESS,              /* EOM in progress, but no output data yet */
    EOM_NOT_RUNNING,              /*EOM Not running*/

    /* Error codes for EOM Cancel sub command*/
    EOM_FAILED_TO_CANCEL,         /*EOM Failed to cancel*/

    /* error codes for EOM Running state machine*/
    EOM_SPEED_IS_NOT_GEN3_OR_4,   /* EOM Speed is not Gen3 or 4*/
    EOM_MARGIN_CODE_OUT_OF_RANGE, /* EOM Margin code out of range*/
    EOM_WAITING_FOR_HARDWARE,     /* EOM waiting for hardware*/

    /*Invalid error code*/
    EOM_ERROR = 0xFF,
} mrpc_stat_eom_enum;

#endif
/**@}*/
