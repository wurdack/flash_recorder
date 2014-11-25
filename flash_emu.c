#include "sys.h"
#include "flash.h"

/** Block size for emulated flash */
static uint32_t const c_flash_block_size = 4 * 1024;

/** 8MB flash array */
static uint32_t const c_flash_array_size = c_flash_block_size * 2048;

/** Erase value for bytes in flash */
static uint8_t const c_flash_erase_value = 0xff;

/** Memory that simulates flash storage */
static uint8_t s_sflash_array[c_flash_array_size];

/** Erase wear counter */
static uint32_t s_sflash_wear[c_flash_array_size / c_flash_block_size];

/** Returns true if the address is on a block boundary */
static bool sflash_is_aligned(uint32_t address)
{
    return (address & (c_flash_block_size - 1)) == 0;
}

/** Returns the base address of the block containing 'address' */
static uint32_t sflash_align(uint32_t address)
{
    return (address & ~(c_flash_block_size - 1));
}

/** Returns the block index of the block containing 'address' */
static uint32_t sflash_block_from_address(uint32_t address)
{
    ASSERT(address < c_flash_array_size);
    return address / c_flash_block_size;
}

/**
    Public entry points. See flash.h for interface contract.
*/

int sflash_init(void)
{
    memset(s_sflash_array, c_flash_erase_value, sizeof(s_sflash_array));
    memset(s_sflash_wear, 0, sizeof(s_sflash_wear));
    return 0;
}

int sflash_geometry_get(uint32_t *block_size, uint32_t *array_size)
{
    ASSERT(block_size);
    ASSERT(array_size);

    *block_size = c_flash_block_size;
    *array_size = c_flash_array_size;
    return 0;
}

int sflash_block_erase(uint32_t address)
{
    ASSERT(sflash_is_aligned(address));
    address = sflash_align(address);
    memset(s_sflash_array + address, c_flash_erase_value, c_flash_block_size);
    ++s_sflash_wear[sflash_block_from_address(address)];
    return 0;
}

int sflash_write(uint32_t address, void const *buffer, uint32_t size)
{
    ASSERT(address + size <= c_flash_array_size);

    // To simulate how flash behaves, AND each byte into memory. This will
    // prevent a write from turning a 0 bit into a 1 bit.
    // N.B. This is only valid for flash that erases to 0xff. Could make this
    //      configurable, but it currently is not.

    uint8_t const *b = (uint8_t const *)buffer;
    uint32_t i;

    for (i = 0; i < size; ++i) {
        s_sflash_array[address + i] &= b[i];
    }

    return 0;
}

int sflash_read(uint32_t address, void *buffer, uint32_t size)
{
    ASSERT(address + size <= c_flash_array_size);
    memcpy(buffer, s_sflash_array + address, size);
    return 0;
}
