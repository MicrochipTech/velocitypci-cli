/*
 * Microchip Velocity PCIe Management Library
 * Copyright (c) 2025, Microchip Technology
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
 * @brief Switchtec core library functions for ini operations
 */

/**
 * @defgroup ini Ini2bin Utility Functions
 * @brief ini2bin API functions
 *
 * These are functions used for converting INI file to BIN for OTP configuration record
 *
 * @{
 */

#ifdef __linux__

#include "switchtec/ini.h"

void handle_section_state(char current_char, char *current_section, int *position) {
    current_section[(*position)++] = current_char;
}

void handle_name_state(char current_char, char *current_name, int *position) {
    current_name[(*position)++] = current_char;
}

void handle_value_state(char current_char, char *returned_string, int *position, int *valueStartWithDoubleQuote) {
    if (*position == 0 && current_char == '"') {
        *valueStartWithDoubleQuote = 1;
    }
    returned_string[(*position)++] = current_char;
}

void finalize_section_state(char *current_section, int *position) {
    current_section[(*position)++] = '\0';
}

void finalize_name_state(char *current_name, int *position) {
    current_name[(*position)++] = '\0';
}

void finalize_value_state(char *returned_string, int *position, int *valueStartWithDoubleQuote) {
    if (*position > 0 && returned_string[*position - 1] == '"') {
        if (*valueStartWithDoubleQuote != 0) {
            (*position)--;
            for (int copy = 0; copy < *position - 1; copy++) {
                returned_string[copy] = returned_string[copy + 1];
            }
            (*position)--;
        }
    }
    returned_string[(*position)++] = '\0';
}

void handle_state_transition(char current_char, enum INI_STATE *state, enum INI_STATE *new_state, char *current_section, char *current_name, char *returned_string, int *position, int *valueStartWithDoubleQuote, int nSize)
{
    switch (current_char) 
    {
        case '[':
            if (*state == NONE)
            {
                *new_state = SECTION;
            }
            break;
        case ']':
            if (*state == SECTION)
            {
                *new_state = NONE;
            }
            break;
        case ';':
            *new_state = COMMENT;
            break;
        case '\r':
        case '\n':
            *new_state = NONE;
            break;
        case '=':
            if (*state == NAME)
            {
                *new_state = VALUE;
            }
            break;
        default:
            switch (*state)
            {
                case SECTION:
                    handle_section_state(current_char, current_section, position);
                    break;
                case NAME:
                    handle_name_state(current_char, current_name, position);
                    break;
                case VALUE:
                    handle_value_state(current_char, returned_string, position, valueStartWithDoubleQuote);
                    break;
                case NONE:
                    *position = 0;
                    *state = NAME;
                    *new_state = NAME;
                    handle_name_state(current_char, current_name, position);
                    break;
                default:
                    break;
            }
            switch (*state)
            {
                case VALUE:
                    if (*position > nSize - 2)
                    {
                        *position = nSize - 2;
                    }
                    break;
                default:
                    if (*position > MAX_LENGTH - 2)
                    {
                        *position = MAX_LENGTH - 2;
                    }
                    break;
            }
            break;
    }
}

int get_key_list(const char *section, const char *name, const char *defaultValue, char *KeyList, int nSize, FILE *fp) {
    char current_section[MAX_LENGTH];
    char current_name[MAX_LENGTH];
    char returned_string[KEY_BUF_SIZE];
    if (!fp)
    {
        perror("error");
        strncpy(returned_string, defaultValue, nSize);
        returned_string[nSize - 1] = '\0';
        return strnlen(returned_string, nSize - 1);
    }
    enum INI_STATE state = NONE;
    enum INI_STATE new_state = NONE;

    int valueStartWithDoubleQuote = 0;

    char current_char;
    int position = 0;
    int pos = 0;

    fseek(fp, 0, SEEK_END); // seek to end of file
    int size = ftell(fp);  // get current file pointer
    fseek(fp, 0, SEEK_SET);
    while (size--)
    {
        size_t readed = fread(&current_char, 1, 1, fp);
        if (readed == 1)
        {
            handle_state_transition(current_char, &state, &new_state, current_section, current_name, returned_string, &position, &valueStartWithDoubleQuote, nSize);
        }
        if (new_state != state)
        {
            switch (state)
            {
                case SECTION:
                    finalize_section_state(current_section, &position);
                    break;
                case NAME:
                    finalize_name_state(current_name, &position);
                    break;
                case VALUE:
                    finalize_value_state(returned_string, &position, &valueStartWithDoubleQuote);

                    if (position > 0 && returned_string[position - 1] == '"') {
            
                        if (valueStartWithDoubleQuote != 0) {
                            position--;
                            for (int copy = 0; copy < position - 1; copy++)
                            {
                                returned_string[copy] = returned_string[copy + 1];
                            }
                            position--;
                        }
                    }
                    returned_string[position++] = '\0';
    
                    if(strncmp(current_section, section, MAX_LENGTH) == 0)
                    {
                        if(strncmp(current_name, name, MAX_LENGTH) == 0) {
                            memcpy(&KeyList[0], returned_string, strlen(returned_string));
                            pos = pos + strlen(returned_string);
                            KeyList[pos++] = '\0';
                            
                            return strnlen(returned_string, nSize - 1);
                        }
                        else if (strncmp(name, "NULL", MAX_LENGTH) == 0) {
                            memcpy(&KeyList[pos], current_name, strlen(current_name));
                            pos = pos + strlen(current_name);
    
                            KeyList[pos++] = '\0';
                            KeyList[pos++] = '=';
    
                            memcpy(&KeyList[pos++], returned_string, strlen(returned_string));
                            KeyList[pos++] = '\0';
    
                            state = new_state;
                            continue;
                        }
                        else
                        {
                            memcpy(KeyList, defaultValue, nSize);
                            KeyList[nSize - 1] = '\0';
    
                            state = new_state;
                            continue;
                        }
                    }
                    break;
                default:
                    break;
            }
            position = 0;
            valueStartWithDoubleQuote = 0;
            state = new_state;
        }
    }

    return strnlen(returned_string, nSize - 1);
}

BOOL check_valid_address(BYTE byCurrXMode, BOOL bIsDwordAddress, DWORD dwCurrAddress)
{
    if ((byCurrXMode == XMODE_CLEAR_BITS) || (byCurrXMode == XMODE_SET_BITS) ||
        (byCurrXMode == XMODE_WRITE_BYTE))
    {
        //Check for valid address
        if ((TRUE == bIsDwordAddress) && (dwCurrAddress < 0xFFFF))
        {
            return FALSE;
        }
        else if ((FALSE == bIsDwordAddress) && (dwCurrAddress > 0xFFFF))
        {
            return FALSE;
        }
    }
    return TRUE;
}

void handle_address_change(DWORD dwCurrAddress, DWORD dwPrevAddress, BOOL *bValidDataOffset, BYTE *byDataLength, WORD *wDataLengthOffset, BYTE *m_pBinConfig, WORD *m_wDataOffset, DWORD nTemp)
{
    WORD wTemp = (dwCurrAddress - dwPrevAddress - 1);

    if (*bValidDataOffset)
    {
        // Complete the data record before switching/skipping address
        m_pBinConfig[*wDataLengthOffset] = *byDataLength;
        *bValidDataOffset = FALSE;
    }

    if ((dwCurrAddress < dwPrevAddress) || (wTemp > 0x7D)) {
        // Set new xdata address
        m_pBinConfig[(*m_wDataOffset)++] = SET_XDATA_ADDRESS;
        if (nTemp < 0xFFFF) {
            m_pBinConfig[(*m_wDataOffset)++] = HIBYTE(LOWORD(dwCurrAddress));
            m_pBinConfig[(*m_wDataOffset)++] = LOBYTE(LOWORD(dwCurrAddress));
        } else {
            m_pBinConfig[(*m_wDataOffset)++] = HIBYTE(HIWORD(dwCurrAddress));
            m_pBinConfig[(*m_wDataOffset)++] = LOBYTE(HIWORD(dwCurrAddress));
            m_pBinConfig[(*m_wDataOffset)++] = HIBYTE(LOWORD(dwCurrAddress));
            m_pBinConfig[(*m_wDataOffset)++] = LOBYTE(LOWORD(dwCurrAddress));
        }
    } else {
        // Skip xdata write for wTemp length
        m_pBinConfig[(*m_wDataOffset)++] = (0x80 | LOBYTE(wTemp));
    }
}

void handle_xdata_mode_switch(BYTE byCurrXMode, BYTE *byPrevXMode, BOOL *bValidDataOffset, BYTE *byDataLength, WORD *wDataLengthOffset, BYTE *m_pBinConfig, WORD *m_wDataOffset) {
	// Update the xdata mode only if the current mode is not same of the previous mode
    if (byCurrXMode != *byPrevXMode)
    {
        if (*bValidDataOffset)
        {
            // Complete the data record before switching XDATA mode
            m_pBinConfig[*wDataLengthOffset] = *byDataLength;
            *bValidDataOffset = FALSE;
        }
        // Switch to new xdata write/set/clear mode
        m_pBinConfig[(*m_wDataOffset)++] = CUSTOM_COMMAND;
        m_pBinConfig[(*m_wDataOffset)++] = byCurrXMode;

        *byPrevXMode = byCurrXMode;
    }
}

BOOL process_key_entry(char *pszToken, char *szKeyEntry, char *szSeparators, BYTE *m_pBinConfig, WORD *m_wDataOffset, DWORD *dwPrevAddress, BYTE *byDataLength, WORD *wDataLengthOffset, BYTE *abyBinBuffer, BOOL *bValidDataOffset)
{

    unsigned int nTemp;
    BYTE byData;
    DWORD dwActualSize;
    ssize_t rlen;

    // Initialize data offset if not valid
    if (!(*bValidDataOffset))
    {
        *bValidDataOffset = TRUE;
        *wDataLengthOffset = (*m_wDataOffset)++;
        *byDataLength = 0;
    }

    // Check if the key entry points to a .bin file
    if (strstr(szKeyEntry, ".bin"))
    {
        FILE *fp = fopen(szKeyEntry, "rb");
        if (!fp) {
            return FALSE; // Failed to open file
        }

        // Move file pointer to the end to get the file size
        fseek(fp, 0, SEEK_END);
        dwActualSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        // Load data bytes from the binary file
        rlen = fread(&abyBinBuffer[0], 1, dwActualSize, fp);
        fclose(fp);

        if (rlen < dwActualSize)
        {
            return FALSE; // Failed to read the entire file
        }

        // Process the binary data
        (*dwPrevAddress)--;
        for (int k = 0; k < dwActualSize; k++)
        {
            (*byDataLength)++;
            (*dwPrevAddress)++;
            m_pBinConfig[(*m_wDataOffset)++] = abyBinBuffer[k];

            // Handle data length exceeding 0x7F bytes
            if (0x7F == *byDataLength) {
                m_pBinConfig[*wDataLengthOffset] = *byDataLength;
                *wDataLengthOffset = (*m_wDataOffset)++;
                *byDataLength = 0;
            }
        }
    } 
    else
    {
        // Process the key entry as a list of data values
        (*dwPrevAddress)--;
        pszToken = strtok(szKeyEntry, szSeparators);
        while (pszToken != NULL) {
            (*byDataLength)++;

            sscanf(pszToken, "%x", &nTemp);
            byData = nTemp;

            (*dwPrevAddress)++;
            m_pBinConfig[(*m_wDataOffset)++] = byData;

            // Handle data length exceeding 0x7F bytes
            if (0x7F == *byDataLength) {
                m_pBinConfig[*wDataLengthOffset] = *byDataLength;
                *wDataLengthOffset = (*m_wDataOffset)++;
                *byDataLength = 0;
            }

            pszToken = strtok(NULL, szSeparators);
        }
    }

    return TRUE;
}

void handle_stop_opcode(BOOL bValidDataOffset, BYTE byDataLength, BYTE *m_pBinConfig, WORD wDataLengthOffset, WORD *m_wDataOffset)
{
    if (bValidDataOffset)
    {
        if (0 == byDataLength)
        {
            m_pBinConfig[wDataLengthOffset] = STOP;
        }
        else
        {
            m_pBinConfig[wDataLengthOffset] = byDataLength;
            m_pBinConfig[(*m_wDataOffset)++] = STOP;
        }
    }
    else
    {
        m_pBinConfig[(*m_wDataOffset)++] = STOP;
    }
}

/**
 * @brief Converts an INI configuration file to a binary format.
 *
 * This function reads an INI configuration file, processes its key-value pairs,
 * and generates a binary configuration buffer. It supports various modes such as
 * writing bytes, setting bits, clearing bits, and patching code. The function
 * ensures valid addressing and handles mode switches appropriately.
 *
 * @param szINIFileName Pointer to the INI file to be converted.
 * @param pclog Pointer to a log buffer for storing debug or error messages.
 * @param m_pBinConfig Pointer to the output binary configuration buffer.
 * @param binlength Pointer to an integer where the length of the binary buffer will be stored.
 * @return TRUE (non-zero) if the conversion is successful, FALSE (zero) otherwise.
 */
int switchtec_convert_ini2bin(FILE *szINIFileName, char *pclog ,BYTE *m_pBinConfig, int *binlength)
{
    char *pKeyName;
    char szKeyList[KEY_BUF_SIZE];
    WORD m_wDataOffset = 0;
    char szKeyEntry[KEY_BUF_SIZE];
	// xdata mode is added to the binary only on byPrevXMode != byCurrXMode.
	// So, initializing the Previous xdata mode to SET_XMODE_NONE
    BYTE byPrevXMode = SET_XMODE_NONE;
    BYTE byCurrXMode = SET_XMODE_NONE;
    unsigned int nTemp = 0;
	// This fix is required for appending - 0x80(SET_XDATA_ADDRESS) in binary due to the following condition,
	// if ((dwCurrAddress < dwPrevAddress) || (wTemp > 0x7D))
	// SO, dwCurrAddress, dwPrevAddress need to be initialied to 0Xffffffff
    DWORD dwCurrAddress = 0xFFFFFFFF, dwPrevAddress = 0xFFFFFFFF;
    BYTE byPatchCount = 0;
    BOOL bValidDataOffset = FALSE;
    WORD wDataLengthOffset = 0;
    BYTE byDataLength = 0;
    char *pszToken = NULL;
    char szSeparators[] = " ,\t";
    BYTE byData;
    BYTE abyBinBuffer[10 * 1024];
    BOOL bIsDwordAddress = TRUE;
    BOOL ret = 0;

    get_key_list(CONFIG_SECTION_NAME, "NULL", "EMPTY", szKeyList, KEY_BUF_SIZE, szINIFileName);
    
    if (!strcmp(szKeyList, "EMPTY"))
    {
        return FALSE;
    }
    pKeyName = szKeyList;

    while (strcmp(pKeyName, ""))
    {
        get_key_list(CONFIG_SECTION_NAME, pKeyName, "EMPTY", szKeyEntry, KEY_BUF_SIZE, szINIFileName);

        if (!strcmp(szKeyEntry, "") || !strcmp(szKeyEntry, "EMPTY"))
        {
            // Empty key Move to next Key Entry
            pKeyName += (strlen(pKeyName) + 1);
            continue;
        }
        // Parse key entry
        if (!strncmp(pKeyName, KEY_XDATA_WRITE, KEY_ID_SIZE))
        {
            byCurrXMode = XMODE_WRITE_BYTE;
            sscanf(pKeyName, KEY_XDATA_WRITE"%x", &nTemp);
            dwCurrAddress = nTemp;
        }
        else if (!strncmp(pKeyName, KEY_XDATA_SET, KEY_ID_SIZE))
        {
            byCurrXMode = XMODE_SET_BITS;
            sscanf(pKeyName, KEY_XDATA_SET"%x", &nTemp);
            dwCurrAddress = nTemp;
        }
        else if (!strncmp(pKeyName, KEY_XDATA_CLEAR, KEY_ID_SIZE))
        {
            byCurrXMode = XMODE_CLEAR_BITS;
            sscanf(pKeyName, KEY_XDATA_CLEAR"%x", &nTemp);
            dwCurrAddress = nTemp;
        }
        else if (!strncmp(pKeyName, KEY_PATCH_CODE, KEY_ID_SIZE))
        {
            byCurrXMode = XMODE_PATCH_CODE;
            sscanf(pKeyName, KEY_PATCH_CODE"%x", &nTemp);

            //Check for valid address
            if ((TRUE == bIsDwordAddress) && (nTemp < 0xFFFF))
            {
                return FALSE;
            }
            else if ((FALSE == bIsDwordAddress) && (nTemp > 0xFFFF))
            {
                return FALSE;
            }
            dwCurrAddress = nTemp;
            byPatchCount++;
            if (bValidDataOffset)
            {
                // Complete the data record before switching/skipping address
                m_pBinConfig[wDataLengthOffset] = byDataLength;
                bValidDataOffset = FALSE;
            }
            m_pBinConfig[m_wDataOffset++] = CUSTOM_COMMAND;
            m_pBinConfig[m_wDataOffset++] = byCurrXMode;


            //32 Bit - 16 Bit Addressing check
            if (nTemp < 0xFFFF)
            {
                m_pBinConfig[m_wDataOffset++] = HIBYTE(LOWORD(dwCurrAddress));
                m_pBinConfig[m_wDataOffset++] = LOBYTE(LOWORD(dwCurrAddress));
            }
            else
            {
                m_pBinConfig[m_wDataOffset++] = HIBYTE(HIWORD(dwCurrAddress));
                m_pBinConfig[m_wDataOffset++] = LOBYTE(HIWORD(dwCurrAddress));
                m_pBinConfig[m_wDataOffset++] = HIBYTE(LOWORD(dwCurrAddress));
                m_pBinConfig[m_wDataOffset++] = LOBYTE(LOWORD(dwCurrAddress));
            }
            pszToken = strtok(szKeyEntry, szSeparators);

            sscanf(pszToken, "%x", &nTemp);
            byData = nTemp;

            m_pBinConfig[m_wDataOffset++] = byData;
            pszToken = strtok(NULL, szSeparators);

            pKeyName += (strlen(pKeyName) + 1);
            //Fix for JIRA-1644	
            byPrevXMode = byCurrXMode;
            continue;
        }
        else
        {
            return FALSE;
        }

        ret = check_valid_address(byCurrXMode, bIsDwordAddress, dwCurrAddress);
        if(!ret)
        {
            return ret;
        }

        if (dwCurrAddress != (dwPrevAddress + 1))
        {
            handle_address_change(dwCurrAddress, dwPrevAddress, &bValidDataOffset, &byDataLength, &wDataLengthOffset, m_pBinConfig, &m_wDataOffset, nTemp);
        }
        dwPrevAddress = dwCurrAddress;

        handle_xdata_mode_switch(byCurrXMode, &byPrevXMode, &bValidDataOffset, &byDataLength, &wDataLengthOffset, m_pBinConfig, &m_wDataOffset);
        
        if (!process_key_entry(pszToken, szKeyEntry, szSeparators, m_pBinConfig, &m_wDataOffset, &dwPrevAddress, &byDataLength, &wDataLengthOffset, abyBinBuffer, &bValidDataOffset))
        {
            return FALSE;
        }

        // Move to next Key Entry
        pKeyName += (strlen(pKeyName) + 1);
    }
    if (m_wDataOffset <= 1)
    {
        return FALSE;
    }

    handle_stop_opcode(bValidDataOffset, byDataLength, m_pBinConfig, wDataLengthOffset, &m_wDataOffset);

    // copy length
    *binlength = m_wDataOffset;

    return TRUE;
}

#endif
