#ifndef API_H__
#define API_H__

#ifdef _DEBUG
#include <stdio.h>
#define log_err(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define log_err(fmt, ...)
#endif

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

#endif // API_H__
