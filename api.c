#include "api.h"

#include <stdbool.h>
#include <string.h>

#define F32_SIGNED_BITS   (1)
#define F32_EXPONENT_BITS (8)
#define F32_MANTISSA_BITS (23)

#define F64_SIGNED_BITS   (1)
#define F64_EXPONENT_BITS (11)
#define F64_MANTISSA_BITS (52)

#define MODULO ((u32)(1e9 + 7))
#define NIBBLE_SZ (4)

typedef unsigned int u32;
typedef unsigned char u8;
typedef unsigned long long u64;
typedef u8 Nibble[NIBBLE_SZ + 1];

typedef enum {
  None,
  Binary,
  Hexadecimal,
  Decimal,
} Kind;

typedef struct {
  u32 size;
  bool exact;
} MatchSize;

bool nibcmp(const Nibble n1, const char* n2) {
  for (u32 i = 0; i < NIBBLE_SZ; ++i) {
    if (n1[i] != n2[i]) return false;
  }
  return true;
}

char from_nibble_to_hex(const Nibble data) {
  if (nibcmp(data, "0000")) return '0';
  if (nibcmp(data, "0001")) return '1';
  if (nibcmp(data, "0010")) return '2';
  if (nibcmp(data, "0011")) return '3';
  if (nibcmp(data, "0100")) return '4';
  if (nibcmp(data, "0101")) return '5';
  if (nibcmp(data, "0110")) return '6';
  if (nibcmp(data, "0111")) return '7';
  if (nibcmp(data, "1000")) return '8';
  if (nibcmp(data, "1001")) return '9';
  if (nibcmp(data, "1010")) return 'A';
  if (nibcmp(data, "1011")) return 'B';
  if (nibcmp(data, "1100")) return 'C';
  if (nibcmp(data, "1101")) return 'D';
  if (nibcmp(data, "1110")) return 'E';
  if (nibcmp(data, "1111")) return 'F';
  return '?';
}

bool from_char_to_nibble(const char c, Nibble res) {
  memset(res, 0, sizeof(Nibble));
  
  if (
      (c < '0' || c > '9') && 
      (c < 'a' || c > 'z') &&
      (c < 'A' || c > 'Z')
  ) return false;

  switch (c) {
    case '0': {
        res[0] = '0';
        res[1] = '0';
        res[2] = '0';
        res[3] = '0';
    } break; 
    case '1': {
        res[0] = '0';
        res[1] = '0';
        res[2] = '0';
        res[3] = '1';
    } break;
    case '2': {
        res[0] = '0';
        res[1] = '0';
        res[2] = '1';
        res[3] = '0';
    } break;
    case '3': {
        res[0] = '0';
        res[1] = '0';
        res[2] = '1';
        res[3] = '1';
    } break;
    case '4': {
        res[0] = '0';
        res[1] = '1';
        res[2] = '0';
        res[3] = '0';
    } break;
    case '5': {
        res[0] = '0';
        res[1] = '1';
        res[2] = '0';
        res[3] = '1';
    } break;
    case '6': {
        res[0] = '0';
        res[1] = '1';
        res[2] = '1';
        res[3] = '0';
    } break;
    case '7': {
        res[0] = '0';
        res[1] = '1';
        res[2] = '1';
        res[3] = '1';
    } break;
    case '8': {
        res[0] = '1';
        res[1] = '0';
        res[2] = '0';
        res[3] = '0';
    } break;
    case '9': {
        res[0] = '1';
        res[1] = '0';
        res[2] = '0';
        res[3] = '1';
    } break;
    case 'a':
    case 'A': {
        res[0] = '1';
        res[1] = '0';
        res[2] = '1';
        res[3] = '0';
    } break;
    case 'b':
    case 'B': {
        res[0] = '1';
        res[1] = '0';
        res[2] = '1';
        res[3] = '1';
    } break;
    case 'c':
    case 'C': {
        res[0] = '1';
        res[1] = '1';
        res[2] = '0';
        res[3] = '0';
    } break;
    case 'd':
    case 'D': {
        res[0] = '1';
        res[1] = '1';
        res[2] = '0';
        res[3] = '1';
    } break;
    case 'e':
    case 'E': {
        res[0] = '1';
        res[1] = '1';
        res[2] = '1';
        res[3] = '0';
    } break;
    case 'f':
    case 'F': {
        res[0] = '1';
        res[1] = '1';
        res[2] = '1';
        res[3] = '1';
    } break;
  }

  return true;
}

u32 count_char_of_numbers(u64 num) {
  u32 res = 0;
  while (num > 0) {
    res++;
    num /= 10;
  }
  return res;
}

u32 is_valid_input(const char *input, MatchSize expected, Kind kind) {
  u32 len = 0;
  const char *ptr = input;
  bool error = false;
  
  char c = *ptr;
  while (c != '\0' && !error) {
    switch (kind) {
      case Binary: 
        if (c != '0' && c != '1') error = true;
        break;
      case Hexadecimal:
        if ((c < '0' && c > '9') || (c < 'a' && c > 'f') || (c < 'A' && c > 'F')) {
          error = true;
        }
        break;
      case Decimal:
        if (c < '0' && c > '9') error = true;
        break;
      default:
        error = true;
        break;
    }
    len++;
    ptr++;
    c = *ptr;
  }

  if (error) {
    log_err("[Error]: Unexpected token %c", c);
    return (u32)-1;
  }
  if ((expected.exact == true && expected.size != len)) {
    log_err("[Error]: Unexpected string length: %u. Expected %u", len, expected.size);
    return (u32)-1;
  }
  if (expected.exact == false && (len >= expected.size || len == 0)) {
    log_err("[Error]: Unexpected string length %u. Expected 0 < len < %u", len, expected.size);
    return (u32)-1;
  }

  return len;
}

int from_str_to_float(const char *input, float *res) {
  u32 len;
  u32 value = 0;
  u32 expected_size = F32_SIGNED_BITS + F32_EXPONENT_BITS + F32_MANTISSA_BITS;

  *res = 0.0f;
  len = is_valid_input(input, (MatchSize){ expected_size, true }, Binary);
  if (len == (u32)-1) return false;

  for (u32 i = 0; i < len; ++i) {
    if (input[i] == '0') continue;
    value = value | (1u << (len - i - 1));
  }

  *res = *((float*)(&value));
  return true;
}

int from_str_to_double(const char *input, double *res) {
  u32 len;
  u64 value = 0;
  u32 expected_size = F64_SIGNED_BITS + F64_EXPONENT_BITS + F64_MANTISSA_BITS;

  *res = 0.0;
  len = is_valid_input(input, (MatchSize){ expected_size, true }, Binary);
  if (len == (u32)-1) return false;

  for (u32 i = 0; i < len; ++i) {
    if (input[i] == '0') continue;
    value = value | (1ull << (len - i - 1));
  }

  *res = *((double*)(&value));
  return true;
}

Result from_bin_to_dec(const char *input) {
  Result res;
  u64 value = 0;
  u32 len = is_valid_input(input, (MatchSize) { MAX_BUFFER_SZ, false }, Binary);
  if (len == (u32)-1) return res;

  for (u32 i = 0; i < len; ++i) {
    if (input[i] == '0') continue;
    value += 1ull << (len - i - 1); // pow(2, len - i - 1);
  }

  len = count_char_of_numbers(value);
  for (int i = len - 1; i >= 0; i--) {
    res.data[i] = (value % 10) + '0';
    value /= 10;
  }
  res.data[len] = '\0';
  res.ok = true;

  return res;
}

Result from_bin_to_hex(const char *input) {
  Result res;
  Nibble nibble = { '0', '0', '0', '0' };
  u32 len = is_valid_input(input, (MatchSize) { MAX_BUFFER_SZ, false }, Binary);
  if (len == (u32)-1) return res;

  u32 blob_idx;
  u32 input_idx = 0;

  if (len % NIBBLE_SZ == 0) {
    blob_idx = 0;
    len = len / NIBBLE_SZ;
  } else {
    blob_idx = NIBBLE_SZ - (len % NIBBLE_SZ);
    len = len / NIBBLE_SZ + 1;
  }

  for (u32 i = blob_idx; i < NIBBLE_SZ; ++i) {
    nibble[i] = input[input_idx++];
  }

  for (u32 i = 0; i < len; ++i) {
    res.data[i] = from_nibble_to_hex(nibble);

    for (u32 j = 0; j < NIBBLE_SZ; ++j) {
      nibble[j] = input[input_idx++];
    }
  }
  res.data[len] = '\0';
  res.ok = true;

  return res;
}

Result from_hex_to_bin(const char *input) {
  Result res;
  Nibble nibble;
  u32 len = is_valid_input(input, (MatchSize) { MAX_BUFFER_SZ, false }, Hexadecimal);
  if (len == (u32)-1) return res;

  u32 index = 0;
  bool first_one = false;
  for (u32 i = 0; i < len; ++i) {
    if (!from_char_to_nibble(input[i], nibble)) return res;
    for (u32 j = 0; j < NIBBLE_SZ; ++j) {
      if (i != 0) res.data[index++] = nibble[j];
      else {
        if (nibble[j] == '0' && !first_one) continue;
        if (nibble[j] == '1' && !first_one) first_one = true;
        res.data[index++] = nibble[j];
      }
    }
  }
  res.ok = true;
  res.data[index] = '\0';
  
  return res;
}

Result from_hex_to_dec(const char *input) {
  (void) input;
  return (Result) {};
}

Result from_dec_to_bin(const char *input) {
  (void) input;
  return (Result) {};
}

Result from_dec_to_hex(const char *input) {
  (void) input;
  return (Result) {};
}
