#pragma once
#include "assert.h"
#include "string.h"
#include "iostream"

const int DICT_INIT_COUNT = 10;

const int UNFOUND = -1;

struct DictElem {
    char* key;
    int code;
};

struct Dict {

    Dict() {
        dict = new DictElem[DICT_INIT_COUNT];
    };

    ~Dict() {
        delete[] dict;
    };

    DictElem* dict;
    size_t size;
};

void InsertToDict(Dict* dict, char* elem);
int  CheckElemInDict(Dict* dict, char* elem)

char* ZLVEncodeData(char* data);