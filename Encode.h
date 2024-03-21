#pragma once        
#include "Dictionary.h"                                 

#define WriteCode(code)                                         \
        elem_code = code;                                       \
        fwrite(&elem_code, sizeof(Code_t), 1, output);          \

#define FillBufferWithChar(sym)                                 \
        buff_len = 1;                                           \
        buff[0] = sym;                                          \
        buff[buff_len] = '\0';                              

#define AddToBufferChar(sym)                                    \
        buff[buff_len++] = sym;                                 \
        buff[buff_len] = '\0';  

#define ReadBuffer(code)                                        \
        code = read_buff[k];                                    \
        k++;                                                    

const Code_t CLEAR_CODE = 65535;
const Code_t END_CODE   = 65534;

const int   MAX_DICT_SIZE   = 65536;
const int   MAX_FILE_SIZE   = 500182;
const int   MAX_BUFF_SIZE   = 900000;

enum WorkMode {
    ENCODE = 1,
    DECODE = 2
};

void LZWEncodeData(FILE* source, FILE* output);
void LZWDecodeData(FILE* source, FILE* output);