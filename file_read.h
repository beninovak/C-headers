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
    uint64_t size;
    char* start;
} _file_line;

typedef struct {
    uint64_t line_count;
    _file_line* lines;
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
uint64_t file_read_get_lines(FILE* fptr) {
    if (fptr == NULL) return -1;
    rewind(fptr);

    char buffer[BUFFER_SIZE];
    uint64_t lines = 0;

    while(1) {
        size_t bytes = fread(buffer, 1, BUFFER_SIZE, fptr);
        if (ferror(fptr)) {
            return -1;
        }

        for(uint32_t i = 0; i < bytes; i++) {
            if (buffer[i] == NEWLINE_CHAR) {
                lines++;
            }
        }

        if (feof(fptr)) {
            break;
        }
    }
    
    rewind(fptr);

    return lines > 0 ? lines : 1; // If no new line character exists, there is only one line.
}

_file* file_read(FILE* fptr) {
    if (fptr == NULL) return NULL;
    uint64_t line_count = file_read_get_lines(fptr);
    if (line_count == -1) return NULL;
    _file* file = (_file*)calloc(1, sizeof(uint64_t) + sizeof(_file_line*));
    
    if (file == NULL) return NULL;

    file->line_count = line_count;
    file->lines = (_file_line*)calloc(line_count, sizeof(_file_line));

    char* line = NULL;
    size_t size = 0;
    ssize_t line_size;

    uint32_t count = 0;
    while((line_size = getline(&line, &size, fptr)) != -1) {
        if (line == NULL || strcmp(line, "") == 0 || strlen(line) <= 0) continue;

        if (count < line_count) {
            file->lines[count].size = (uint64_t)line_size;
            file->lines[count].start = (char*)calloc(line_size, sizeof(char));
            if (file->lines[count].start != NULL) {
                memcpy(file->lines[count].start, line, line_size);
            }
        }
        free(line);
        line = NULL; // Just in case
        count++;
    }
    free(line);
    line = NULL;

    return file;
}

void file_free(_file* file) {
    for(uint32_t i = 0; i < file->line_count; i++) {
        if (file->lines[i].start != NULL) {
            free(file->lines[i].start);
        }
    }
    if (file->lines != NULL) {
        free(file->lines);
        file->lines = NULL;
    }
    if (file != NULL) {
        free(file);
        file = NULL;
    }
}
