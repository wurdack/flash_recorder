/** UTF style encoding/decoding for integers */

/*
     7 bit integers: 0xxx.xxxx
    11 bit integers: 110x.xxxx 10xx.xxxx
    16 bit integers: 1110.xxxx 10xx.xxxx 10xx.xxxx
    21 bit integers: 1111.0xxx 10xx.xxxx 10xx.xxxx 10xx.xxxx
    26 bit integers: 1111.10xx 10xx.xxxx 10xx.xxxx 10xx.xxxx 10xx.xxxx
    31 bit integers: 1111.110x 10xx.xxxx 10xx.xxxx 10xx.xxxx 10xx.xxxx 10xx.xxxx
*/

#include <stdint.h>

uint32_t utf_encode_length_get(uint32_t input)
{
    if (input < 0x80) {
        return 1;

    } else if (input < 0x800) {
        return 2;

    }else if (input < 0x10000) {
        return 3;

    } else if (input < 0x200000) {
        return 4;

    } else if (input < 0x4000000) {
        return 5;

    } else if (input < 0x80000000) {
        return 6;

    } else {
        return 0;
    }
}

/** Return the number of bytes to decode give the lead byte */
uint32_t utf_decode_length_get(uint8_t lead_byte)
{
    if ((lead_byte & 0x80) == 0) {
        return 1;

    } else if ((lead_byte & 0xe0) == 0xc0) {
        return 2;

    } else if ((lead_byte & 0xf0) == 0xe0) {
        return 3;

    } else if ((lead_byte & 0xf8) == 0xf0) {
        return 4;

    } else if ((lead_byte & 0xfc) == 0xf8) {
        return 5;

    } else if ((lead_byte & 0xfe) == 0xfc) {
        return 6;

    } else {
        return 0;
    }
}

/** Encode 'input' as UTF-8 to 'ouput'. Return the size of the encoding */
uint32_t utf_encode(uint32_t input, uint8_t * output, uint32_t output_size)
{
    uint32_t i;
    uint32_t length = utf_encode_length_get(input);

    if (length > output_size) {
        return 0;
    }

    if (!length || length > 6) {
        return 0;
    }

    if (length == 1) {
        output[0] = (uint8_t)input;
        return 1;
    }

    for (i = 0; i < length - 1; ++i) {
        output[length - i - 1] = 0x80 | (input & 0x3f);
        input >>= 6;
    }

    switch (length) {
        case 2:
            output[0] = 0xc0 | input;
            break;
        case 3:
            output[0] = 0xe0 | input;
            break;
        case 4:
            output[0] = 0xf0 | input;
            break;
        case 5:
            output[0] = 0xf8 | input;
            break;
        case 6:
            output[0] = 0xfc | input;
            break;
    }

    return length;
}

/** Decode 'input' as UTF-8, copying value to 'output'. Return bytes consumed.
*/
uint32_t utf_decode(uint8_t const *input, uint32_t input_size, uint32_t *output)
{
    uint32_t i;
    uint32_t length = utf_decode_length_get(*input);

    if (length > input_size) {
        return 0;
    }

    if (!length || length > 6) {
        return 0;
    }

    if (length == 1) {
        *output = *input;
        return length;
    }

    switch (length) {
        case 2:
            *output = *input & 0x1f;
            break;
        case 3:
            *output = *input & 0x0f;
            break;
        case 4:
            *output = *input & 0x07;
            break;
        case 5:
            *output = *input & 0x03;
            break;
        case 6:
            *output = *input & 0x01;
            break;
    }

    ++input;
    for (i = 0; i < length - 1; ++i) {
        *output <<= 6;
        if ((*input & 0xc0) != 0x80) {
            return 0;
        }

        *output |= (*input & 0x3f);
        ++input;
    }

    return length;
}
