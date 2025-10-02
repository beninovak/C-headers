#pragma once
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

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

typedef struct {
    char* file_name;
    uint32_t start_line;
    uint32_t line_count;
    uint32_t start_index;
    uint32_t byte_count;
} _file_arg;

uint32_t file_read_line_count;

_file* __file;

uint32_t new_lines_size = 32;
uint32_t* new_line_positions;

uint64_t file_read_get_lines(char* file_name) {
    // if (fptr == NULL) return -1;
    // rewind(fptr);
    FILE* fptr = fopen(file_name, "r");

    char buffer[BUFFER_SIZE];
    uint64_t lines = 0;

    new_line_positions = (uint32_t*)calloc(new_lines_size, sizeof(uint32_t));
    int count = 0;
    while(1) {
        size_t bytes = fread(buffer, 1, BUFFER_SIZE, fptr);
        if (ferror(fptr)) {
            return -1;
        }

        for(uint32_t i = 0; i < bytes; i++) {
            if (buffer[i] == NEWLINE_CHAR) {
                if (lines > new_lines_size) {
                    printf("\n MORE LINES \n");
                    new_lines_size *= 2;
                    new_line_positions = (uint32_t*)realloc((void*)new_line_positions, new_lines_size * sizeof(uint32_t));
                }

                new_line_positions[lines] = (uint32_t)((count * BUFFER_SIZE) + i);
                lines++;
            }
        }

        if (feof(fptr)) {
            break;
        }

        count++;
    }
    
    for(int i = 0; i < lines; i++) {
        printf("\n%u", new_line_positions[i]);
    }
    // free(new_line_positions);
    rewind(fptr);
    fclose(fptr);

    return lines > 0 ? lines : 1; // If no new line character exists, there is only one line.
}

void* file_read_partial(void* args) { // args is of type _file_arg*
    // Check if thread wants to read more lines than are left -> if total read lines exceeds file_read_line_coun

    _file_arg* arg_internal = (_file_arg*)args;

    FILE* fptr = fopen(arg_internal->file_name, "r");

    char* line = NULL;
    size_t size = 0; // use this with 'count' to restrict which line to read from?
    ssize_t line_size;

    // TODO - Need to set fseek() to the start index
    fseek(fptr, arg_internal->byte_count, arg_internal->start_index);
    uint32_t count = arg_internal->start_line;
    while((line_size = getline(&line, &size, fptr) != -1)) {
        if (line == NULL || strcmp(line, "") == 0 || strlen(line) <= 0) continue;

        printf("\n%s\n", line);
        if (count < arg_internal->line_count) {
            __file->lines[count].size = (uint64_t)line_size;
            __file->lines[count].start = (char*)calloc(line_size, sizeof(char));
            if (__file->lines[count].start != NULL) {
                memcpy(__file->lines[count].start, line, line_size);
            }
        }
        free(line);
        line = NULL; // Just in case
        count++;
    }
    free(line);
    line = NULL;
    fclose(fptr);
    return NULL; 
}

_file* file_read(char* file_name) {
    // if (fptr == NULL) return NULL;
    file_read_line_count = file_read_get_lines(file_name);
    if (file_read_line_count == -1) return NULL;

    __file = (_file*)calloc(1, sizeof(uint64_t) + sizeof(_file_line*));
    if (__file == NULL) return NULL;

    __file->line_count = file_read_line_count;
    __file->lines = (_file_line*)calloc(file_read_line_count, sizeof(_file_line));

    const uint8_t thread_count = file_read_line_count < 16 ? file_read_line_count : 16; // Check if there is a better way to get a safe amount
    uint8_t       created_threads = 0;
    pthread_t     threads[thread_count];
    _file_arg     thread_args[thread_count];
    int           thread_result;
    uint32_t      lines_per_thread = (uint32_t)(file_read_line_count / thread_count);
    uint32_t      remainder_lines = 0;

    if (file_read_line_count % thread_count != 0) {
        remainder_lines = file_read_line_count % thread_count;
    }

    uint32_t start_index;
    for (uint8_t i = 0; i < thread_count; i++) {
        start_index = (i == 0 ? 0 : new_line_positions[(i * lines_per_thread) - 1] + 1);
        thread_args[i] = (_file_arg) { 
            .file_name = file_name, 
            .start_line = i * lines_per_thread,
            .line_count = lines_per_thread,
            .start_index = start_index,
            .byte_count = new_line_positions[((i + 1) * lines_per_thread) - 1] - start_index //  Diff of end-start indices
        };

        if (i == thread_count - 1 && remainder_lines != 0) {
            FILE* fptr = fopen(file_name, "r");
            fseek(fptr, 0L, SEEK_END);
            uint64_t file_size = ftell(fptr);
            rewind(fptr);
            fclose(fptr);
            thread_args[i].line_count = lines_per_thread + remainder_lines;
            thread_args[i].byte_count = file_size - new_line_positions[(i * lines_per_thread) - 1];
        }

        printf("\n\n\nThread %d:\n", i + 1);
        printf("Start line: %d\nLine count: %d\nStart index: %d\nByte count: %d\n", thread_args[i].start_line, thread_args[i].line_count, thread_args[i].start_index, thread_args[i].byte_count);
        printf("\n\n");

        thread_result = pthread_create(&threads[i], NULL, file_read_partial, (void*)&thread_args[i]);
        if (thread_result != 0) {
            perror("pthread_create() failed");
            continue;
        }
        created_threads++;
    }

    // TODO - Skip threads that didn't open or didn't open properly ( thread_result != 0)
    for (uint8_t i = 0; i < created_threads; i++) {
        pthread_join(threads[i], NULL); // Check return value for errors
    }
        
    return __file;
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
