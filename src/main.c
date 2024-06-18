#include <stdio.h>
#include <stddef.h>
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds/stb_ds.h"

typedef struct {
    char key;
    size_t value;
} BadMatchTable;

size_t max(size_t x, size_t y) {
    return (x > y) ? x : y;
}

const char* search_string(const char* needle, const char* haystack)
{
    size_t needle_len = strlen(needle);
    size_t haystack_len = strlen(haystack);

    if (needle_len > haystack_len) {
        return NULL;
    }

    if (needle_len == 0 || haystack_len == 0) {
        return NULL;
    }

    BadMatchTable* table = NULL;

    // populating the bad match table...
    for (size_t i = 0; i < needle_len; i++) {
        int index = hmgeti(table, needle[i]);

        if (index == -1) {
            hmput(table, needle[i], max(needle_len - i - 1, 1));
        } else {
            // just update the value with a new value.
            table[index].value = max(needle_len - i - 1, 1);
        }
    }

    size_t offset = 0;
    while (1) {
        const char* start = haystack + offset;

        if (offset >= haystack_len) {
            hmfree(table);
            return NULL;
        }

        size_t i = needle_len - 1;
        for (;i > 0; i--) {
            if (needle[i] != start[i]) {
                int index = hmgeti(table, start[needle_len - 1]);
                if (index == -1) {
                    offset += needle_len;
                } else {
                    offset += table[index].value;
                }

                break;
            }
        }

        if (i == 0) {
            hmfree(table);
            return start;
        }
    }

    hmfree(table);
    return NULL;
}

const char** search_string_all_occurences(const char* needle, const char* haystack) {
    const char** strings = NULL;

    size_t needle_len = strlen(needle);
    size_t haystack_len = strlen(haystack);

    const char* offset = haystack;
    while (1) {
        const char* result = search_string(needle, offset);
        if (result == NULL) {
            return strings;
        } else {
            offset = result;
            arrput(strings, offset);

            offset += needle_len;
        }
    }

    return strings;
}

int main()
{
    char buffer[5000];

    FILE* file = fopen("text.txt", "r");

    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);

    const char* string = search_string("motor", buffer);
    if (string) {
        printf("%s", string);
    }

    const char** result = search_string_all_occurences("fruit", buffer);
    printf("occurences: %zu\n", arrlen(result));

    arrfree(result);
}
