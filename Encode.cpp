#define _CRT_SECURE_NO_WARNINGS
#include "Encode.h"

char* ConcatenateStrWithChar(char* str, char sym) {
    
    assert(str != nullptr);

    size_t str_len = strlen(str);

    char* new_str = (char*)calloc(str_len + 2, sizeof(char));

    if (new_str == nullptr) {
        assert(!"calloc error");
    }

    strcpy(new_str, str);

    new_str[str_len] = sym;
    new_str[str_len + 1] = '\0';

    return new_str;
}

void LZWDecodeData(FILE* source, FILE* output) {

    assert(source != nullptr);
    assert(output != nullptr);

    Dict dict = Dict();
    
    Code_t read_buff[MAX_FILE_SIZE] = {};
    fread(&read_buff, sizeof(Code_t), MAX_FILE_SIZE, source);

    Code_t code = read_buff[0];

    Code_t old_code  = 0;
    Code_t fill_code = dict.size;
    int k = 1;

    while (code != END_CODE ) {

        if (code == CLEAR_CODE) {

            dict.size = 0;

            FillStartDict(&dict);
            fill_code = dict.size;
            
            ReadBuffer(code);

            if (code == END_CODE) break;   

            fprintf(output, "%s", GetStrFromTable(&dict, code));
            old_code = code;
        }
        else {

            if (inTable(&dict, code)) {

                fprintf(output, "%s", GetStrFromTable(&dict, code));

                char* str_from_code     = GetStrFromTable(&dict, code);
                char* str_from_old_code = GetStrFromTable(&dict, old_code);
                char* new_str           = ConcatenateStrWithChar(str_from_old_code, str_from_code[0]);

                InsertToDict(&dict, new_str, fill_code);
                fill_code++;

                free(new_str);
                old_code = code;
            }
            else {

                char* str_from_old_code = GetStrFromTable(&dict, old_code);
                char* new_str = ConcatenateStrWithChar(str_from_old_code, str_from_old_code[0]);

                fprintf(output, "%s", new_str);
                InsertToDict(&dict, new_str, fill_code);

                fill_code++;
                old_code = code;
            }
        }

        ReadBuffer(code);
    }
}

void LZWEncodeData(FILE* source, FILE* output) {

    assert(source != nullptr);
    assert(output != nullptr);

    char data[MAX_FILE_SIZE] = "";
    fread(data, sizeof(char), MAX_FILE_SIZE, source);

    Dict dict = Dict();
    FillStartDict(&dict);

    char*  buff     = (char*)calloc(MAX_BUFF_SIZE, sizeof(char));
    size_t buff_len = 0;

    Code_t elem_code = 0;
    WriteCode(CLEAR_CODE);

    Code_t code = dict.size;

    int i = 0;

    while (data[i] != EOF) { 

        AddToBufferChar(data[i]);

        Code_t elem_code = GetCodeFromTable(&dict, buff);

        if (elem_code == UNFOUND) {

            buff[buff_len - 1] = '\0';
            buff_len--;

            WriteCode(GetCodeFromTable(&dict, buff));

            AddToBufferChar(data[i]);

            if (dict.size >= MAX_DICT_SIZE) {

                dict.size = 0;   // clear dict

                WriteCode(CLEAR_CODE);
                FillStartDict(&dict);

                buff_len = 0;
                AddToBufferChar(data[i]);

                code = dict.size;
            }

            InsertToDict(&dict, buff, code);
            code++;

            FillBufferWithChar(data[i]);
        }

        i++;
    }

    WriteCode(GetCodeFromTable(&dict, buff));
    WriteCode(END_CODE);

    delete[] buff;
}