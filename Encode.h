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

    Dict() {
        dict = (DictElem*)calloc(DICT_INIT_COUNT, sizeof(DictElem));
        size = 0;
    };

    ~Dict() {
        if (dict != nullptr)
            free(dict);
    };

    DictElem* dict;
    size_t size;
};

void InsertToDict(Dict* dict, const char* elem, int code);
int  CheckElemInDict(Dict* dict, char* elem);

void LZWEncodeData(const char* data, FILE* output);
