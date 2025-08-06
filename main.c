// #include <stdlib.h>
#include <stdio.h>
#include <time.h>
//include "old_read.h"
#include "file_read.h"
#include "_strings.h"

int main() {
    clock_t start_time = clock();

    FILE* fptr = fopen("text_1kb.txt", "rb");
    long num_lines = file_read_get_lines(fptr);

    printf("Sizeof ssize_t: %lu\n", sizeof(ssize_t));
    printf("Sizeof uint32_t: %lu\n", sizeof(uint32_t));
    printf("Sizeof uint64_t: %lu\n", sizeof(uint64_t));
    printf("Number of lines: %ld\n", num_lines);

    _file* file = file_read(fptr);
    fclose(fptr);
    printf("Line count: %lu\n", file->line_count);

    // for(int i = 0; i < file->line_count; i++) {
    //     printf("%u - %s\n", file->lines[i]->size, file->lines[i]->start);
    // }
    //
    // char test[50] = "28,13,89";
    // _tokens* tokens = _string_tokenize(test, ',');
    //
    // printf("Size: %d -> ", tokens->size);
    // for(int i = 0; i < tokens->size; i++) {
    //     printf("%s(%lu) ", tokens->tokens[i], strlen(tokens->tokens[i]));
    // }
    // printf("\n");
    //
    // _tokens_sort(tokens, 1);
    // printf("Size: %d -> ", tokens->size);
    // for(int i = 0; i < tokens->size; i++) {
    //     printf("%s(%lu) ", tokens->tokens[i], strlen(tokens->tokens[i]));
    // }
    printf("\n");
    double total_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    file_free(file);
    printf("\nTime elapsed: %f\n\n", total_time);
    // _tokens_free(tokens);
    return 0;
}
