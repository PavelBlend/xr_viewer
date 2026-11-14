#ifndef STRING_C
#define STRING_C

#include <stdio.h>
#include <inttypes.h>

#include "file_io.h"

uint32_t get_string_length(char* string) {
    uint32_t length = 0;

    while (string[length] != 0) {
        length++;
    }

    return length;
}

char* read_string_zero(FileData* file_data, uint32_t* offset) {

    // get string length

    uint32_t offset_zero = *offset;

    while ((uint8_t)*(file_data->bytes + offset_zero) != 0) {
        offset_zero++;
    }

    uint32_t string_length = offset_zero - *offset;

    // read string
    char* string = malloc(string_length + 1);

    for (uint32_t i=0; i<string_length; i++) {
        string[i] = file_data->bytes[*offset + i];
    }
    string[string_length] = 0;

    return string;

}

#endif
