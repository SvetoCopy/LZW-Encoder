#include "Encode.h"

void InsertToDict(Dict* dict, char* elem) {

    assert(dict != nullptr);
    assert(elem != nullptr);

    dict->dict[dict->size].key  = elem;
    dict->dict[dict->size].code = dict->size;

    dict->size++;
}

int CheckElemInDict(Dict* dict, char* elem) {

    assert(dict != nullptr);
    assert(elem != nullptr);

    for (int i = 0; i < dict->size; i++) {
        if (strcmp(dict->dict[i].key, elem) == 0) {
            return dict->dict[i].code;
        }
    }

    return UNFOUND;
}

char* ZLVEncodeData(char* data, int data_len) {

    assert(data != nullptr);

    Dict dict = Dict();

    char*  buff     = new char[20];
    size_t buff_len = 0;

    for (int i = 0; i < data_len; i++) {

        buff[buff_len] = data[i];
        buff_len++;

        int elem_code = CheckElemInDict(&dict, buff);
    }
}