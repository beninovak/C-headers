// #include <stdlib.h>
#include <stdio.h>
#include <time.h>
//include "old_read.h"
#include "read_file.h"
#include "_strings.h"

int main() {
    clock_t start_time = clock();

    FILE* fptr = fopen("text_1kb.txt", "rb");
    //long num_lines = file_read_get_lines(fptr);

    //printf("Number of lines: %ld\n", num_lines);

    // _file* file = file_read(fptr);
    // printf("Line count: %u", file->line_count);
    //
    // for(int i = 0; i < file->line_count; i++) {
    //     printf("%u - %s\n", file->lines[i]->size, file->lines[i]->start);
    // }
    
    char test[50] = "28,13,89,98,96,16,49";
    _tokens* tokens = _strings_tokenize(test, ',');

    printf("Size: %d -> ", tokens->size);
    for(int i = 0; i < tokens->size; i++) {
        printf("%s(%lu) ", tokens->tokens[i], strlen(tokens->tokens[i]));
    }
    printf("\n");

    _tokens_sort(tokens, 1);
    printf("Size: %d -> ", tokens->size);
    for(int i = 0; i < tokens->size; i++) {
        printf("%s(%lu) ", tokens->tokens[i], strlen(tokens->tokens[i]));
    }
    printf("\n");
    double total_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %f\n", total_time);
    return 0;
}
