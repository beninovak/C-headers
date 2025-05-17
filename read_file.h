#pragma once
#include <cstdio>

typedef struct {
    int length;
    char* start;
} _file_line;

typedef struct {
    int line_count;
    _file_line* lines;
} _file;

int file_read_get_lines(FILE* fptr) {
    if (fptr == NULL) return -1;

    rewind(fptr);

    char ch;
    int lines = 0;

    while(!feof(fptr)) {
        ch = fgetc(fptr);

        if (ch == '\n') {
            lines++;
        }
    }
    rewind(fptr);

    return lines;
}

_file* file_read(FILE* fptr) {
    if (fptr == NULL) return NULL;
    int line_count = file_read_get_lines(fptr);
    _file_line* lines;


    _file file = {
        .line_count = file_read_get_lines(fptr),
    };
}
