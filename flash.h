#include <stdint.h>

int sflash_init(void);
int sflash_erase_block(uint32_t address);
int sflash_write(uint32_t address, void const *buffer, uint32_t size);
int sflash_read(uint32_t address, void *buffer, uint32_t size);
