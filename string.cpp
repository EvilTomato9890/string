#include <stdio.h>
#include <stdint.h>
#include "asserts.h"
#include "logger.h"
#include "test_part.h"
#include "input.h"
#include "string.h"
int main() {
	logger_initialize_stream(NULL); 
	char str1[] = "12345";
	char str2[] = "543210";
	strncpy(str1, str2, 7);
	LOGGER_DEBUG("ALL OK");
	printf("%s", str1); 
	char str3[5] = {};
	fgets(str3, 5, stdin);
	printf("%s", str3);
}

ssize_t Getline(char **lineptr, size_t *n, FILE *stream) {
    LOGGER_DEBUG("getline started");
    hard_assert(lineptr != nullptr, "Pointer to string is nullptr");
    hard_assert(stream != nullptr, "stream is nullptr");

    int ch = 0;
    size_t curr_size = *n;
    size_t numChars = 0;
    char *ptr;

    if (*lineptr == nullptr) {
        curr_size = 120;
        LOGGER_DEBUG("Memory allocation started, trying to malloc %ld bytes", curr_size);
        *lineptr = (char *)malloc(curr_size);
        if (*lineptr == nullptr) return -1;
        *n = curr_size;
    }

    ptr = *lineptr;

    while ((ch = getc(stream)) != EOF) {
        if (numChars + 1 >= curr_size) {
            curr_size *= 2;
            LOGGER_DEBUG("Memory reallocation started, trying to realloc %ld bytes", curr_size);
            char *new_ptr = (char *)realloc(*lineptr, curr_size);
            if (new_ptr == nullptr) return -1;
            *lineptr = new_ptr;
            ptr = *lineptr + numChars; 
            *n = curr_size;
        }

        *ptr++ = (char)ch;
        numChars++;

        if (ch == '\n') break;
    }

    if (numChars == 0 && ch == EOF) return -1;

    *ptr = '\0'; 
    return numChars;
}

char* strdup(const char *str) {
	char *dupplicate = (char*)malloc((strlen(str) + 1)*sizeof(char));
	strcpy(dupplicate, str);
	return dupplicate;
}

char* fgets(char *str, int numChars, FILE *stream) {
	LOGGER_DEBUG("strcat started");
	hard_assert(str != nullptr, "str is nullptr");
	hard_assert(stream != nullptr, "stream is nullptr");
	int ch = 0;
	while ((ch = getc(stream)) != EOF && ch != '\n' && numChars > 1) {
		numChars--;
		*(str++) = ch;
	}
	*(str++) = '\0';
	if (ferror(stream)) {
		return NULL;
	}
	return str;
}

char* strncat(char* destin, const char* source, size_t num) {
	LOGGER_DEBUG("strcat started");
	hard_assert(destin != nullptr, "destination is nullptr");
	hard_assert(source != nullptr, "source is nullptr");
	char* temp = destin;
	while (*destin != '\0') destin++;
	while (*source != '\0' && num > 0) {
		num--;
		*(destin++) = *(source++);
	}
	return temp;
}


char* strcat(char* destin, const char* source) {
	return strncat(destin, source, SIZE_MAX);
}


char* strncpy(char* destin, const char* source, size_t num) {
	LOGGER_DEBUG("strncpy started");
	hard_assert(destin != nullptr, "destination is nullptr");
	hard_assert(source != nullptr, "source is nullptr");
	char *temp = destin;
	while (*source != '\0' && num > 0) {
		*(destin++) = *(source++);
		num--;
	}
	return temp;
}


char* strcpy(char* destin, const char* source) {
	return strncpy(destin, source, SIZE_MAX);
}

size_t strlen(const char* str) {
	hard_assert(str != nullptr, "str is nullptr");
	LOGGER_DEBUG("strlen started");
	size_t cnt = 0;
	while (*str != '\0') {
		cnt++;
		str++;
	}
	return cnt;
}

const char* strchr(const char* str, char ch) { //Почему в оригинале no const
	hard_assert(str != nullptr, "str is nullptr");
	LOGGER_DEBUG("strchr started");
	while(*str != '\0') {
		if(*(str++) == ch) return str;
	}

	LOGGER_WARNING("%c not found", ch);
	return NULL;
}

int puts(const char* str) {
	hard_assert(str != nullptr, "str is nullptr");
	LOGGER_DEBUG("strchr started");
	while (*str != '\0') {
		if(putchar(*str++) == EOF) return EOF;
	}
	return (putchar('\n') == EOF) ? EOF : 1;
}


