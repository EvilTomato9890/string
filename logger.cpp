#include "logger.h"
#include <stdarg.h>
#include <time.h>
#include "asserts.h"

#define CYAN   "\033[36m"
#define BLUE   "\033[34m"
#define YELLOW "\033[33m"

static FILE *output_stream = NULL;
static logger_output_type output_type = EXTERNAL_STREAM;
static logger_mode_type mode_type = LOGGER_MODE_INFO;
static int color_enabled = 1;

const char* logger_mode_string(const logger_mode_type type) {
    switch (type) {
        case LOGGER_MODE_DEBUG:                   return "DEBUG";
        case LOGGER_MODE_INFO:                    return "INFO";
        case LOGGER_MODE_WARNING:                 return "WARNING";
        case LOGGER_MODE_ERROR:                   return "ERROR";
        default: soft_assert(false, "WRONG MODE");return "?";
    }
}

void logger_time_string(char *buff, size_t n) {
    hard_assert(buff != nullptr, "buff is nullptr");
    // HH:MM:SS:YYYY-MM-DD 
    time_t t = time(NULL);
    struct tm tmv;
    localtime_r(&t, &tmv);
    if(!strftime(buff, n, "%H:%M:%S:%Y-%m-%d", &tmv)) {
        soft_assert(false, "Invalid time input");
    }
}

const char* logger_color_on(const logger_mode_type mode) {
    if (!color_enabled) return "";
    switch (mode) {
        case LOGGER_MODE_DEBUG:   return CYAN;
        case LOGGER_MODE_INFO:    return BLUE; 
        case LOGGER_MODE_WARNING: return YELLOW;
        case LOGGER_MODE_ERROR:   return RED; 
        default:                  return "";
    }
}

void logger_initialize_stream(FILE *stream) {
    if (output_type == OWNED_FILE && output_stream) {
        fclose(output_stream);
    }
    output_stream = stream ? stream : stderr;
    output_type = EXTERNAL_STREAM;
}

int logger_initialize_file(const char *path) {
    hard_assert(path != nullptr, "File path is empty");
    FILE *f = fopen(path, "a");
    hard_assert(path != nullptr, "Incorrect read of logger file");

    if (output_type == OWNED_FILE && output_stream) {
        fclose(output_stream);
    }
    output_stream = f;
    output_type = OWNED_FILE;
    return 0;
}

void logger_close() {
    if (output_type == OWNED_FILE && output_stream) fclose(output_stream);
}

void logger_log_message(const logger_mode_type mode,
                        const char *file, int line,
                        const char *format, ...)
{
    if (!output_stream) {
        color_enabled = 0;
        output_stream = stderr;
    }
    char Time[32] = "";
    logger_time_string(Time, sizeof Time);
    if(output_type != EXTERNAL_STREAM) {
        fprintf(output_stream, "%s. %s:%d. %s. ",
            Time, file, line, logger_mode_string(mode));
    } else {
        const char *color_on  = logger_color_on(mode);
        fprintf(output_stream, "[%s] %s:%d. %s%s%s. ",
            Time, file, line, color_on, logger_mode_string(mode), RESET);
    }

    va_list ap;
    va_start(ap, format);
    vfprintf(output_stream, format, ap);
    va_end(ap);
    
    fputc('\n', output_stream);
}