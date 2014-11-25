#include "sys.h"
#include "flash.h"

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

    printf("Passed.\n");
}
