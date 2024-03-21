#include "Dictionary.h"

Dict::Dict() {
    dict = (DictElem*)calloc(DICT_INIT_COUNT, sizeof(DictElem));
    size = 0;
};

Dict::~Dict() {
    if (dict != nullptr) {
        free(dict);
        size = 0;
    }
};

void InsertToDict(Dict* dict, const char* elem, Code_t code) {

    assert(dict != nullptr);
    assert(elem != nullptr);

    dict->dict[dict->size].key = _strdup(elem);
    dict->dict[dict->size].code = code;

    dict->size++;
}

Code_t GetCodeFromTable(Dict* dict, char* elem) {

    assert(dict != nullptr);
    assert(elem != nullptr);

    for (int i = 0; i < dict->size; i++) {
        if (strcmp(dict->dict[i].key, elem) == 0) {
            return dict->dict[i].code;
        }
    }

    return UNFOUND;
}

bool inTable(Dict* dict, Code_t code) {

    assert(dict != nullptr);

    for (int i = 0; i < dict->size; i++) {
        if (dict->dict[i].code == code) {
            return true;
        }
    }

    return false;
}

char* GetStrFromTable(Dict* dict, Code_t code) {

    assert(dict != nullptr);

    for (int i = 0; i < dict->size; i++) {
        if (dict->dict[i].code == code) {
            return dict->dict[i].key;
        }
    }

    return nullptr;
}

void PrintDict(Dict* dict) {

    assert(dict != nullptr);

    for (int i = 0; i < dict->size; i++) {

        assert(dict->dict[i].key != nullptr);

        printf("%s - %hu\n", dict->dict[i].key, dict->dict[i].code);
    }
}

void FillStartDict(Dict* dict) {

    assert(dict != nullptr);

    char data[2] = "";

    for (int i = CHAR_MIN; i < CHAR_MAX; i++) {

        data[0] = (char)i;
        InsertToDict(dict, data, i - CHAR_MIN);
    }
}
