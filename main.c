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

    _file* file = file_read("geometry_for_ocelots.txt");
    // fclose(fptr);
    // printf("Line count: %lu\n", file->line_count);

    // for(int i = 0; i < file->file_read_line_count; i++) {
    //     printf("%lu - %s\n", file->lines[i].size, file->lines[i].start);
    // }
    
    printf("\n");
    double total_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    file_free(file);
    printf("\nTime elapsed: %f\n\n", total_time);
    // _tokens_free(tokens);
    return 0;
}
