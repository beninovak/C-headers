#include <stdio.h>
#include <time.h>
//include "old_read.h"
#include "read_file.h"

int main() {
    clock_t start_time = clock();

    FILE* fptr = fopen("text_1kb.txt", "rb");
    long num_lines = file_read_get_lines(fptr);

    printf("Number of lines: %ld", num_lines);

    




    double total_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %f\n", total_time);
    return 0;
}
