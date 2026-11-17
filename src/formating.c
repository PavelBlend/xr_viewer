#ifndef FORMATTING_C
#define FORMATTING_C

#include <inttypes.h>

// перевести число uint32_t в строку, у которой между разрядами будут пробелы
void format_uint32_with_spaces(uint32_t value, char* out) {

    // переводим число в строку символов
    char temp[11];
    uint8_t pos = 0;

    if (value == 0) {
        temp[pos] = '0';
        pos++;
    } else {
        while (value != 0) {
            temp[pos] = (char)('0' + (value % 10));
            value /= 10;
            pos++;
        }
    }
    temp[pos] = '\0';

    // добавить пробелы
    size_t out_i = 0;
    int digits_written = 0;

    if (value > 1000) {
        for (int i=0; i <= pos-1; i++) {

            out[out_i] = temp[i];
            out_i++;

            digits_written++;

            if (i > 0 && digits_written % 3 == 0) {
                out[out_i] = ' ';
                out_i++;
            }
        }
        out[out_i] = '\0';
    } else {

        // если число меньше 1000, то пробелы не нужны
        for (int i=0; i <= pos-1; i++) {
            out[out_i] = temp[i];
            out_i++;
        }

    }

    // отзеркалить строку (дойти до половины, чтобы два раза не отзеркалить)
    char temp_char;
    for (int i=0; i<out_i/2; i++) {
        temp_char = out[i];
        out[i] = out[out_i - 1 - i];
        out[out_i - 1 - i] = temp_char;
    }

}

#endif    // FORMATTING_C
