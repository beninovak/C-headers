#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

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
#endif

typedef struct {
    uint32_t size;
    char* start;
} _file_line;

typedef struct {
    uint32_t line_count;
    _file_line** lines;
} _file;

/*
 *  This function returns the number of lines in a file,
 *  as detected with the OS specific new line character.
 *  
 *  Input: FILE* fptr of file.
 *  Output: 
 *      - If fptr is NULL, returns -1
 *      - If fptr is NOT NULL, returns the number of lines.
 */
uint32_t file_read_get_lines(FILE* fptr) {
    if (fptr == NULL) return -1;
    rewind(fptr);

    char buffer[BUFFER_SIZE];
    uint32_t lines = 0;

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

    uint32_t line_count = file_read_get_lines(fptr);
    _file* file = (_file*)calloc(1, sizeof(_file*));
    file->line_count = file_read_get_lines(fptr);
    file->lines = (_file_line**)calloc(line_count, sizeof(_file_line*));

    printf("%p\n", file->lines);
    char* line = NULL;
    size_t size = 0;
    ssize_t line_size;

    int count = 0;
    while((line_size = getline(&line, &size, fptr)) != -1) {
        if (count < line_count) {
            file->lines[count] = (_file_line*)calloc(1, sizeof(uint32_t) + sizeof(char*));
            file->lines[count]->size = line_size;
            file->lines[count]->start = (char*)calloc(line_size, sizeof(char));
            memcpy(file->lines[count]->start, line, line_size);
        }
        count++;
    }

    return file;
}
