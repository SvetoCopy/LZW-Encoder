#define _CRT_SECURE_NO_WARNINGS
#include "LZW.h"

DictElem ConcatenateStrWithChar(char* str, size_t str_size, char sym) {
    
    assert(str != nullptr);

    char* new_str = (char*)calloc(str_size + 2, sizeof(char));

    if (new_str == nullptr) {
        assert(!"calloc error");
    }

    memcpy(new_str, str, str_size);

    new_str[str_size] = sym;
    new_str[str_size + 1] = '\0';

    DictElem res = {};

    res.key      = new_str;
    res.key_size = str_size + 1;

    return res;
}

long GetFileSize(FILE* file) {

    assert(file != nullptr);

    long size;
    long current_pos;

    current_pos = ftell(file);

    fseek(file, 0, SEEK_END);
    size = ftell(file);

    fseek(file, current_pos, SEEK_SET);

    return size;
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

    while (code != END_CODE) {

        if (code == CLEAR_CODE) {

            dict.size = 0;

            FillStartDict(&dict);
            fill_code = dict.size;
            
            ReadBuffer(code);

            if (code == END_CODE) break;   

            DictElem DictElem_from_code = GetStrFromTable(&dict, code);
            fwrite(DictElem_from_code.key, sizeof(char), DictElem_from_code.key_size, output);

            old_code = code;
        }
        else {

            if (inTable(&dict, code)) {

                DictElem DictElem_from_code = GetStrFromTable(&dict, code);
                fwrite(DictElem_from_code.key, sizeof(char), DictElem_from_code.key_size, output);

                DictElem DictElem_from_old_code = GetStrFromTable(&dict, old_code);
                DictElem new_DictElem           = ConcatenateStrWithChar(DictElem_from_old_code.key,
                                                                         DictElem_from_old_code.key_size,
                                                                         DictElem_from_code.key[0]);

                InsertToDict(&dict, new_DictElem.key, new_DictElem.key_size, fill_code);
                fill_code++;

                free(new_DictElem.key);
                old_code = code;
            }
            else {

                DictElem DictElem_from_old_code = GetStrFromTable(&dict, old_code);

                DictElem new_DictElem = ConcatenateStrWithChar(DictElem_from_old_code.key,
                                                               DictElem_from_old_code.key_size,
                                                               DictElem_from_old_code.key[0]);

                fwrite(new_DictElem.key, sizeof(char), new_DictElem.key_size, output);

                InsertToDict(&dict, new_DictElem.key, new_DictElem.key_size, fill_code);

                free(new_DictElem.key);
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
    PrintDict(&dict);

    char*  buff     = (char*)calloc(MAX_BUFF_SIZE, sizeof(char));
    size_t buff_len = 0;

    Code_t elem_code = 0;
    WriteCode(CLEAR_CODE);

    Code_t code = dict.size;

    long file_size = GetFileSize(source);
    long i = 0;

    while (i < file_size) {

        AddToBufferChar(data[i]);

        Code_t elem_code = GetCodeFromTable(&dict, buff, buff_len);

        if (elem_code == UNFOUND) {

            buff[buff_len - 1] = '\0';
            buff_len--;

            Code_t write_code = GetCodeFromTable(&dict, buff, buff_len);

            if (write_code == UNFOUND) {
                assert(!"something went wrong");
            }

            WriteCode(write_code);

            AddToBufferChar(data[i]);

            if (code > MAX_CODE || dict.size > MAX_DICT_SIZE) {

                dict.size = 0;   // clear dict

                WriteCode(CLEAR_CODE);
                FillStartDict(&dict);

                code = dict.size;

                buff_len = 0;
                
                continue;
            }

            InsertToDict(&dict, buff, buff_len, code);
            code++;

            FillBufferWithChar(data[i]);
        }

        i++;
    }

    WriteCode(GetCodeFromTable(&dict, buff, buff_len));
    WriteCode(END_CODE);

    free(buff);

}