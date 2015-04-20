#include <stdint.h>
#include "encoder_decoder.h"

typedef struct tag_node_t {
    uint32_t next_key;      /**< Logical offset of next key */
    uint32_t next_value;    /**< Logical offset of next value for current key */

    /** Node pointers are followed by the key name and value */
    uint8_t key[];          /**< Length prefixed name of key */
    uint8_t value[];        /**< Length prefixed value for key */
} node_t;
