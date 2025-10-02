// #include <stdlib.h>
#include <stdio.h>
#include <time.h>
// #include "file_read.h"
#include "file_read_multi.h"
#include "_strings.h"

int main() {
    clock_t start_time = clock();

    // FILE* fptr = fopen("geometry_for_ocelots.txt", "r");
    // long num_lines = file_read_get_lines(fptr);

    // printf("Sizeof ssize_t: %lu\n", sizeof(ssize_t));
    // printf("Sizeof uint32_t: %lu\n", sizeof(uint32_t));
    // printf("Sizeof uint64_t: %lu\n", sizeof(uint64_t));
    // printf("Number of lines: %ld\n", num_lines);

    _file* file = file_read("text_1kb.txt");
    // fclose(fptr);
    printf("\nLine count: %lu\n", file->line_count);

    for(int i = 0; i < file->line_count; i++) {
        printf("%lu - %s\n", file->lines[i].size, file->lines[i].start);
    }
    
    printf("\n");
    file_free(file);
    // _tokens_free(tokens);
    double total_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %f\n\n", total_time);
    return 0;
}
