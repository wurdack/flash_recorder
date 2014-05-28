#include "sys.h"
#include "flash.h"

// TODO do these need to be static?
uint32_t const c_flash_size = 8 * 1024 * 1024;
uint32_t const c_flash_block_size = 4 * 1024;
uint8_t const c_flash_erase_value = 0xff;

/** Memory that simulates flash storage */
static uint8_t s_sflash_array[c_flash_size];

/** Erase wear counter */
static uint32_t s_sflash_wear[c_flash_size / c_flash_block_size];

static bool sflash_is_aligned(uint32_t address) {
    return (address & (c_flash_block_size - 1)) == 0;
}

static uint32_t sflash_align(uint32_t address) {
    return (address & ~(c_flash_block_size - 1));
}

static uint32_t sflash_block_from_address(uint32_t address) {
    ASSERT(address < c_flash_size);
    return address / c_flash_block_size;
}

int sflash_init(void) {
    memset(s_sflash_array, c_flash_erase_value, sizeof(s_sflash_array));
    memset(s_sflash_wear, 0, sizeof(s_sflash_wear));
    return 0;
}

int sflash_erase_block(uint32_t address) {
    ASSERT(sflash_is_aligned(address));
    address = sflash_align(address);
    memset(s_sflash_array + address, c_flash_erase_value, c_flash_block_size);
    ++s_sflash_wear[sflash_block_from_address(address)];
    return 0;
}

int sflash_write(uint32_t address, void const* buffer, uint32_t size) {
    ASSERT(address + size <= c_flash_size);

    // To simulate how flash behaves, AND each byte into memory. This will
    // prevent a write from turning a 0 bit into a 1 bit.
    // N.B. This is only valid for flash that erases to 0xff. Could make this
    //      configurable, but it currently is not.

    uint8_t const * b = (uint8_t const*)buffer;
    uint32_t i;

    for (i = 0; i < size; ++i) {

        s_sflash_array[address + i] &= b[i];
    }

    return 0;
}

int sflash_read(uint32_t address, void* buffer, uint32_t size) {
    ASSERT(address + size <= c_flash_size);
    memcpy(buffer, s_sflash_array + address, size);
    return 0;
}
