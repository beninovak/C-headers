#pragma once

#include <errno.h>
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


int _tokens_comparator_ascending(const void* tok1, const void* tok2) {
    return strcmp(*(const char**)tok1, *(const char**)tok2);
}

int _tokens_comparator_descending(const void* tok1, const void* tok2) {
    return -1 * strcmp(*(const char**)tok1, *(const char**)tok2);
}

void _tokens_sort(_tokens* toks, int order) {
    if (order != 1 && order != -1) {
        errno = 22;
        perror("Invalid order value. order argument must be 1 (ascending) or -1(descending)!");
        return exit(0);
    }

    if (order == 1) {
        qsort(toks->tokens, toks->size, sizeof(char*), _tokens_comparator_ascending); 
    } else {
        qsort(toks->tokens, toks->size, sizeof(char*), _tokens_comparator_descending); 
    }
}

void _tokens_free(_tokens* tokens) {
    for(int i = 0; i < tokens->size; i++) {
        free(tokens->tokens[i]);
    }
    free(tokens->tokens);
    free(tokens);
}   
