#pragma once

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint32_t size;
    char** tokens;
} _tokens;


_tokens* _strings_tokenize(char* _str, char separator) {
    int length = strlen(_str);

    int token_count = 0;
    for(int i = 0; i < length; i++) {
        if (_str[i] == separator) {
            token_count++;

            // Ignore multiple separator characters in a row.
            int j = i + 1;
            while(1) {
                if (_str[j] == separator) {
                    i++;
                    j++;
                } else {
                    break;
                }
            }
        }
    }
    token_count++; // One more token than separators.
    
    _tokens* toks = (_tokens*)calloc(1, sizeof(_tokens*));
    toks->size = token_count;
    toks->tokens = (char**)calloc(token_count, sizeof(char*));

    char* token;
    for(int i = 0; i < token_count; i++) {
        if (i == 0) {
            token = strtok(_str, &separator);
        } else {
            token = strtok(NULL, &separator);
        }

        toks->tokens[i] = (char*)calloc(strlen(token), sizeof(char));
        strcpy(toks->tokens[i], token);
    }
    return toks;
}
