#include "api.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define F32_SIGNED_BITS   (1)
#define F32_EXPONENT_BITS (8)
#define F32_MANTISSA_BITS (23)

#define F64_SIGNED_BITS   (1)
#define F64_EXPONENT_BITS (11)
#define F64_MANTISSA_BITS (52)

int from_str_to_float(const char *input, float *res) {
  uint8_t bit;
  uint32_t value = 0;
  size_t size = strlen(input);
  size_t expected_size = F32_SIGNED_BITS + F32_EXPONENT_BITS + F32_MANTISSA_BITS;

  *res = 0.0f;
  if (size != expected_size) {
    log_err(
        "[Error]: Wrong input format. Expected %zu bits but got %zu bit(s)\n",
        expected_size, size
    );
    return false;
  }

  for (size_t i = 0; i < size; ++i) {
    bit = input[i] - '0';
    if (bit != 0 && bit != 1) {
      log_err("[Error]: Unexpected value: %c\n", input[i]);
      return false;
    }
    if (bit == 0) continue;
    value = value | (1u << (size - i - 1));
  }

  *res = *((float*)(&value));
  return true;
}

int from_str_to_double(const char *input, double *res) {
  uint8_t bit;
  uint64_t value = 0;
  size_t size = strlen(input);
  size_t expected_size = F64_SIGNED_BITS + F64_EXPONENT_BITS + F64_MANTISSA_BITS;

  *res = 0.0;
  if (size != expected_size) {
    log_err(
        "[Error]: Wrong input format. Expected %zu bits but got %zu bit(s)\n",
        expected_size, size
    );
    return false;
  }

  for (size_t i = 0; i < size; ++i) {
    bit = input[i] - '0';
    if (bit != 0 && bit != 1) {
      log_err("[Error]: Unexpected value: %c\n", input[i]);
      return false;
    }
    if (bit == 0) continue;
    value = value | (1ull << (size - i - 1));
  }

  *res = *((double*)(&value));
  return true;
}
