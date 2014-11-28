#include <stdlib.h>

#include "sys.h"
#include "flash.h"
#include "encode_decode.h"

int test_encoder(void)
{
    uint8_t buffer[8];

    uint32_t tests[] = {0, 0x50, 0x7f, 0x80, 0x400, 0x7ff, 0x800, 0x1234};
    uint32_t i;
    uint32_t j;
    uint32_t length_in;
    uint32_t length_out;

    uint32_t encodee;
    uint32_t decodee;

    for (i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i) {
        encodee = tests[i];
        length_in = utf_encode(encodee, buffer, sizeof(buffer));
        ASSERT(length_in);
        length_out = utf_decode(buffer, length_in, &decodee);
        ASSERT(length_in == length_out);
        ASSERT(encodee == decodee);
    }

    for (i = 0; i < 1000; ++i) {
        encodee = rand() & 0x7fffffff;
        length_in = utf_encode(encodee, buffer, sizeof(buffer));
        ASSERT(length_in);
        length_out = utf_decode(buffer, length_in, &decodee);
        ASSERT(length_in == length_out);
        ASSERT(encodee == decodee);
    }

    return 0;
}

int main(void)
{
    sflash_init();

    // Stupid tests

    uint8_t const pattern[] = {0xaa, 0x55, 0xf0, 0x0f, 0xcc};
    uint8_t buffer[ sizeof(pattern) ];
    uint32_t i;

    printf("1. Verify writes to erased page stick.\n");
    sflash_block_erase(0);
    sflash_write(0, pattern, sizeof(pattern));
    sflash_read(0, buffer, sizeof(buffer));
    ASSERT(0 == memcmp(pattern, buffer, sizeof(pattern)));

    printf("2. Verify writing 0xff doesn't corrupt existing data.\n");
    memset(buffer, 0xff, sizeof(buffer));
    sflash_write(0, buffer, sizeof(buffer));
    sflash_read(0, buffer, sizeof(buffer));
    ASSERT(0 == memcmp(pattern, buffer, sizeof(pattern)));

    printf("3. Verify erasing the block really erased the block.\n");
    sflash_block_erase(0);
    sflash_read(0, buffer, sizeof(buffer));
    for (i = 0; i < sizeof(buffer); ++i) {
        ASSERT(buffer[i] = 0xff);
    }

    // encoder test
    printf("4. utf-8 encoder/decoder\n");
    test_encoder();

    printf("Passed.\n");
}
