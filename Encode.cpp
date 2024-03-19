#include "Encode.h"

void InsertToDict(Dict* dict, const char* elem, int code) {

    assert(dict != nullptr);
    assert(elem != nullptr);

    dict->dict[dict->size].key  = _strdup(elem);
    dict->dict[dict->size].code = code;

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

void PrintDict(Dict* dict) {

    assert(dict != nullptr);

    for (int i = 0; i < dict->size; i++) {
        printf("%s - %d\n", dict->dict[i].key, dict->dict[i].code);
    }
}

void FillStartDict(Dict* dict, const char* data) {

    assert(dict != nullptr);
    assert(data != nullptr);

    char* change_data = _strdup(data);
    size_t i = 0;

    while (data[i] != '\0') {

        // doing char to string like: ka -> k\0 
        char old_val = change_data[i + 1];
        change_data[i + 1] = '\0';

        if (CheckElemInDict(dict, &(change_data[i])) == UNFOUND)
            InsertToDict(dict, &(change_data[i]), dict->size);
        
        // return old val: k\0 -> ka
        change_data[i + 1] = old_val;

        i++;
    }

    free(change_data);
}

void LZWEncodeData(const char* data, FILE* output) {

    assert(data != nullptr);

    Dict dict = Dict();

    FillStartDict(&dict, data);

    char*  buff     = new char[20];
    size_t buff_len = 0;

    buff[buff_len++] = data[0];

    int code = dict.size;

    for (int i = 1; data[i] != '\0'; i++) {
            
        buff[buff_len++] = data[i];
        buff[buff_len] = '\0';

        int elem_code = CheckElemInDict(&dict, buff);

        if (elem_code == UNFOUND) {

            InsertToDict(&dict, buff, code);

            buff[buff_len - 1] = '\0';

            fputc(CheckElemInDict(&dict, buff), output);

            code++;

            buff_len = 1;
            buff[0] = data[i];
            buff[buff_len] = '\0';
        }
    }

    fputc(CheckElemInDict(&dict, buff), output);
    PrintDict(&dict);

    delete[] buff;
}