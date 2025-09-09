#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <stdio.h>

enum logger_mode_type {
    LOGGER_MODE_DEBUG = 0,
    LOGGER_MODE_INFO  = 1,
    LOGGER_MODE_WARNING = 2,
    LOGGER_MODE_ERROR = 3
};

enum logger_output_type {
    EXTERNAL_STREAM = 0,
    OWNED_FILE = 1
};

const char* logger_mode_string(const logger_mode_type type);
void logger_time_string(char *buff, size_t n);
const char* logger_color_on(const logger_mode_type mode);

void logger_initialize_stream(FILE *stream); /* NULL => stderr */
int  logger_initialize_file(const char *path); 
void logger_close();


void logger_log_message(logger_mode_type mode,
                        const char *file, int line,
                        const char *format, ...);

#define LOGGER_DEBUG(...)   logger_log_message(LOGGER_MODE_DEBUG,   __FILE__, __LINE__, __VA_ARGS__)
#define LOGGER_INFO(...)    logger_log_message(LOGGER_MODE_INFO,    __FILE__, __LINE__, __VA_ARGS__)
#define LOGGER_WARNING(...) logger_log_message(LOGGER_MODE_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define LOGGER_ERROR(...)   logger_log_message(LOGGER_MODE_ERROR,   __FILE__, __LINE__, __VA_ARGS__)


#endif