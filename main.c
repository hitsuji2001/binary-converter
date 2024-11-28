#include <stdio.h>
#include "api.h"

int main(void) {
  Result got = from_hex_to_bin("ABCDEF");

  printf("%s\n", got.data);
  return 0;
}
