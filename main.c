#include <stdio.h>

#include "api.h"

int main(void) {
  float  f;
  double d;

  from_str_to_float("01000000110010000000000000000000", &f); // 6.25f
  from_str_to_double("0100000000011001000000000000000000000000000000000000000000000000", &d); // 6.25

  printf("%f %lf\n", f, d);
  return 0;
}
