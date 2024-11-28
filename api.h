#ifndef API_H__
#define API_H__

#ifdef _DEBUG
#include <stdio.h>
#define log_err(fmt, ...) (fprintf(stderr, fmt, ##__VA_ARGS__), fprintf(stderr, "\n"));
#else
#define log_err(fmt, ...)
#endif

#define MAX_BUFFER_SZ (1024)

typedef struct Result {
  char data[MAX_BUFFER_SZ];
  int ok;
} Result;

/*
 * @bin: binary null-terminated string, 
 *       must check if it is actually a binary string
 * @res: return result on success, on failure any
 * @return: 1 on success, 0 on failure
 */
int from_str_to_float(const char *bin, float *res);

/*
 * @bin: binary null-terminated string, 
 *       must check if it is actually a binary string
 * @res: return result on success, on failure any
 * @return: 1 on success, 0 on failure
 */
int from_str_to_double(const char *bin, double *res);

Result from_bin_to_dec(const char *input);
Result from_bin_to_hex(const char *input);

Result from_hex_to_bin(const char *input);
Result from_hex_to_dec(const char *input);

Result from_dec_to_bin(const char *input);
Result from_dec_to_hex(const char *input);

#endif // API_H__
