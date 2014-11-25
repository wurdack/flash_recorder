#include <stdint.h>

/** Initialize the flash array */
extern int sflash_init(void);

/** Get the geometry of the flash array */
extern int sflash_geometry_get(uint32_t *block_size, uint32_t *array_size);

/** Erase a block of flash. 'address' must be block aligned */
extern int sflash_block_erase(uint32_t address);

/** Write 'buffer' to the flash array at 'address'. */
extern int sflash_write(uint32_t address, void const *buffer, uint32_t size);

/** Read from 'address' into 'buffer'. */
extern int sflash_read(uint32_t address, void *buffer, uint32_t size);
