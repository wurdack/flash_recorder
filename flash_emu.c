/*
    RAM-backed flash emulator. Used for testing the sflash_recorder library.
*/
#include "sys.h"
#include "flash.h"

/** 4k block size for emulated flash */
#define C_FLASH_BLOCK_SIZE      (4*1024)

/** 8MB flash array */
#define C_FLASH_ARRAY_SIZE      (C_FLASH_BLOCK_SIZE * 2048)

/** Erase value for bytes in flash */
#define C_FLASH_ERASE_VALUE     (0xff)

/** Memory that simulates flash storage */
static uint8_t s_sflash_array[C_FLASH_ARRAY_SIZE];

/** Erase wear counter */
static uint32_t s_sflash_wear[C_FLASH_ARRAY_SIZE / C_FLASH_BLOCK_SIZE];

/** Returns true if the address is on a block boundary */
static bool sflash_is_aligned(uint32_t address)
{
    return (address & (C_FLASH_BLOCK_SIZE - 1)) == 0;
}

/** Returns the base address of the block containing 'address' */
static uint32_t sflash_align(uint32_t address)
{
    return (address & ~(C_FLASH_BLOCK_SIZE - 1));
}

/** Returns the block index of the block containing 'address' */
static uint32_t sflash_block_from_address(uint32_t address)
{
    ASSERT(address < C_FLASH_ARRAY_SIZE);
    return address / C_FLASH_BLOCK_SIZE;
}

/**
    Public entry points. See flash.h for interface contract.
*/

int sflash_init(void)
{
    memset(s_sflash_array, C_FLASH_ERASE_VALUE, sizeof(s_sflash_array));
    memset(s_sflash_wear, 0, sizeof(s_sflash_wear));
    return 0;
}

int sflash_geometry_get(uint32_t *block_size, uint32_t *array_size)
{
    ASSERT(block_size);
    ASSERT(array_size);

    *block_size = C_FLASH_BLOCK_SIZE;
    *array_size = C_FLASH_ARRAY_SIZE;
    return 0;
}

int sflash_block_erase(uint32_t address)
{
    ASSERT(sflash_is_aligned(address));
    address = sflash_align(address);
    memset(s_sflash_array + address, C_FLASH_ERASE_VALUE, C_FLASH_BLOCK_SIZE);
    ++s_sflash_wear[sflash_block_from_address(address)];
    return 0;
}

int sflash_write(uint32_t address, void const *buffer, uint32_t size)
{
    ASSERT(address + size <= C_FLASH_ARRAY_SIZE);

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
    ASSERT(address + size <= C_FLASH_ARRAY_SIZE);
    memcpy(buffer, s_sflash_array + address, size);
    return 0;
}
