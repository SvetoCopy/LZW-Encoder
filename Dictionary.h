#pragma once
#include "assert.h"
#include "string.h"
#include "iostream"
#include "stdlib.h"
#include "limits"

typedef unsigned short Code_t;

const int DICT_INIT_COUNT = 65536;

const Code_t UNFOUND = 65536;

struct DictElem {
    char* key;
    Code_t code;
};

struct Dict {

    Dict();
    ~Dict();

    DictElem* dict;
    size_t size;
};

Code_t GetCodeFromTable(Dict* dict, char* elem);

void InsertToDict(Dict* dict, const char* elem, Code_t code);
bool inTable(Dict* dict, Code_t code);

char* GetStrFromTable(Dict* dict, Code_t code);

void PrintDict(Dict* dict);
void FillStartDict(Dict* dict);

