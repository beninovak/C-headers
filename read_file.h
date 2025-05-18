#pragma once
#include <stdio.h>

//#define BUFFER_SIZE 16777216 -> TOO BIG
#define BUFFER_SIZE 65536 // Kinda arbitrary ngl...

#ifdef _WIN32
#define NEWLINE_CHAR '\r'

#elif __linux__
#define NEWLINE_CHAR '\n'

#elif __APPLE__
#define NEWLINE_CHAR '\n'

#else
#error "OS not supported!"
#endif // _WIN32

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

    char buffer[BUFFER_SIZE];
    int lines = 0;

    while(1) {
        size_t bytes = fread(buffer, 1, BUFFER_SIZE, fptr);
        if (ferror(fptr)) {
            return -1;
        }

        for(int i = 0; i < bytes; i++) {
            if (buffer[i] == NEWLINE_CHAR) {
                lines++;
            }
        }

        if (feof(fptr)) {
            break;
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
