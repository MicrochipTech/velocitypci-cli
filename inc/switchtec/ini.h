/*
 * Microchip VelocityPCI PCIe Management Library
 * Copyright (c) 2025, Microchip Corporation
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

#ifndef LIBSWITCHTEC_INI_H
#define LIBSWITCHTEC_INI_H

#include <stdio.h>
#include <string.h>

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

typedef unsigned long DWORD_PTR, *PDWORD_PTR;

#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))

#define FALSE               0
#define TRUE                1

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
 
typedef enum tagXMODE {
     XMODE_WRITE_BYTE =	0x00,
     XMODE_SET_BITS,
     XMODE_CLEAR_BITS,
     XMODE_PATCH_CODE		
} XMODE;
 
enum INI_STATE{
     NONE,
     SECTION,
     NAME,
     VALUE,
     COMMENT
};
 
int switchtec_convert_ini2bin(FILE *szINIFileName, char *pclog, unsigned char *m_pBinConfig, int *binlength);
 
#endif // LIBSWITCHTEC_INI_H