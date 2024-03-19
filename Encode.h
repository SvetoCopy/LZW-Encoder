#pragma once
#include "assert.h"
#include "string.h"
#include "iostream"
#include "Tx/TXLib.h"
#include "stdlib.h"

const int DICT_INIT_COUNT = 1600;
const int MAX_FILE_SIZE   = 10000;

const int UNFOUND = -1;

struct DictElem {
    char* key;
    int code;
};

struct Dict {

    Dict();
    ~Dict();

    DictElem* dict;
    size_t size;
};

void LZWEncodeData(FILE* source, FILE* output);