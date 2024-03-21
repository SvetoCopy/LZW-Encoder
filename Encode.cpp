#define _CRT_SECURE_NO_WARNINGS
#include "Encode.h"

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

void InsertToDict(Dict* dict, const char* elem, unsigned short code) {

    assert(dict != nullptr);
    assert(elem != nullptr);

    dict->dict[dict->size].key  = _strdup(elem);
    dict->dict[dict->size].code = code;

    dict->size++;
}

unsigned short GetCodeFromTable(Dict* dict, char* elem) {

    assert(dict != nullptr);
    assert(elem != nullptr);

    for (int i = 0; i < dict->size; i++) {
        if (strcmp(dict->dict[i].key, elem) == 0) {
            return dict->dict[i].code;
        }
    }

    return UNFOUND;
}

bool inTable(Dict* dict, unsigned short code) {

    assert(dict != nullptr);

    for (int i = 0; i < dict->size; i++) {
        if (dict->dict[i].code == code) {
            return true;
        }
    }

    return false;
}

char* GetStrFromTable(Dict* dict, unsigned short code) {

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

void LZWDecodeData(FILE* source, FILE* output) {

    assert(source != nullptr);
    assert(output != nullptr);

    Dict dict = Dict();

    
    unsigned short read_buff[MAX_FILE_SIZE] = {};
    fread(&read_buff, sizeof(unsigned short), MAX_FILE_SIZE, source);

    unsigned short code = read_buff[0];

    unsigned short old_code = 0;
    unsigned short fill_code = dict.size;
    int k = 1;

    while (code != END_CODE ) {

        if (code == CLEAR_CODE) {

            dict.size = 0;
            FillStartDict(&dict);
            fill_code = dict.size;
            
            code = read_buff[k];
            k++;

            if (code == END_CODE) break;   

            fprintf(output, "%s", GetStrFromTable(&dict, code));
            old_code = code;
        }

        else {

            if (inTable(&dict, code)) {

                fprintf(output, "%s", GetStrFromTable(&dict, code));

                char* str_from_code = GetStrFromTable(&dict, code);
                char* str_from_old_code = GetStrFromTable(&dict, old_code);
                char* new_str = (char*)calloc(strlen(str_from_old_code) + 2, sizeof(char));

                strcpy(new_str, str_from_old_code);
                strncat(new_str, str_from_code, 1);

                InsertToDict(&dict, new_str, fill_code);
                fill_code++;

                free(new_str);
                old_code = code;
            }

            else {
                char* str_from_old_code = GetStrFromTable(&dict, old_code);

                char str_firstchar[2] = "";
                str_firstchar[0] = str_from_old_code[0];

                char* strcat_codes = (char*)calloc(strlen(str_firstchar) + strlen(str_from_old_code), sizeof(char));

                strcpy(strcat_codes, str_from_old_code);
                strcat(strcat_codes, str_firstchar);

                fprintf(output, "%s", strcat_codes);
                InsertToDict(&dict, strcat_codes, fill_code);
                fill_code++;

                old_code = code;
            }
        }
        code = read_buff[k];
        k++;
    }
}

void LZWEncodeData(FILE* source, FILE* output) {

    assert(source != nullptr);
    assert(output != nullptr);

    char data[MAX_FILE_SIZE] = "";
    fread(data, sizeof(char), MAX_FILE_SIZE, source);

    Dict dict = Dict();
    FillStartDict(&dict);


    char* buff = (char*)calloc(MAX_BUFF_SIZE, sizeof(char));
    size_t buff_len = 0;

    unsigned short elem_code = 0;
    WriteCode(CLEAR_CODE);

    unsigned short code = dict.size;

    int i = 0;

    while (data[i] != EOF) { 

        buff[buff_len++] = data[i];
        buff[buff_len] = '\0';

        unsigned short elem_code = GetCodeFromTable(&dict, buff);

        if (elem_code == UNFOUND) {

            buff[buff_len - 1] = '\0';
            buff_len--;

            WriteCode(GetCodeFromTable(&dict, buff));

            buff[buff_len++] = data[i];
            buff[buff_len] = '\0';

            if (dict.size >= MAX_DICT_SIZE) {

                dict.size = 0;   // clear dict

                WriteCode(CLEAR_CODE);
                FillStartDict(&dict);

                buff_len = 0;

                buff[buff_len++] = data[i];
                code = dict.size;
            }

            InsertToDict(&dict, buff, code);
            code++;

            buff_len = 1;
            buff[0] = data[i];
            buff[buff_len] = '\0';
        }

        i++;
    }

    WriteCode(GetCodeFromTable(&dict, buff));
    WriteCode(END_CODE);

    delete[] buff;
}