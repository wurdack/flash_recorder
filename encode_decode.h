uint32_t utf_encode_length_get(uint32_t input);
uint32_t utf_decode_length_get(uint8_t lead_byte);
uint32_t utf_encode(uint32_t input, uint8_t * output, uint32_t output_size);
uint32_t utf_decode(uint8_t const *input, uint32_t input_size, uint32_t *output);
