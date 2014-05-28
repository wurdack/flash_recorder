#include "sys.h"
#include "flash.h"

int main(void) {
    sflash_init();

    // Stupid tests

    uint8_t const pattern[] = {0xaa, 0x55, 0xf0, 0x0f, 0xcc};
    uint8_t buffer[ sizeof(pattern) ];
    uint32_t i;

    sflash_write(0, pattern, sizeof(pattern));
    sflash_read(0, buffer, sizeof(buffer));
    ASSERT(0 == memcmp(pattern, buffer, sizeof(pattern)));

    memset(buffer, 0xff, sizeof(buffer));
    sflash_write(0, buffer, sizeof(buffer));
    sflash_read(0, buffer, sizeof(buffer));
    ASSERT(0 == memcmp(pattern, buffer, sizeof(pattern)));

    sflash_erase_block(0);
    sflash_read(0, buffer, sizeof(buffer));
    for (i = 0; i < sizeof(buffer); ++i) {
        ASSERT(buffer[i] = 0xff);
    }

    printf("You are awesome!\n");
}
