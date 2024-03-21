#pragma once        
#include "assert.h"
#include "string.h"
#include "iostream"
#include "stdlib.h"
#include "limits"
                                      
const unsigned short CLEAR_CODE = 65535;
const unsigned short END_CODE   = 65534;

const int   DICT_INIT_COUNT = 65536;
const int   MAX_DICT_SIZE   = 65536;
const int   MAX_FILE_SIZE   = 90182;

const unsigned short UNFOUND = 65536;

struct DictElem {
    char* key;
    unsigned short code;
};

struct Dict {

    Dict();
    ~Dict();

    DictElem* dict;
    size_t size;
};

void LZWEncodeData(FILE* source, FILE* output);
void LZWDecodeData(FILE* source, FILE* output);