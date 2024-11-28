#include <gtest/gtest.h>
#include "api.h"

TEST(IEEE754, F32_EQ_6P25) {
  int res;
  float got;
  res = from_str_to_float("01000000110010000000000000000000", &got);
  EXPECT_EQ(res, true);
  EXPECT_FLOAT_EQ(got, 6.25f);
}

TEST(IEEE754, F64_EQ_6P25) {
  int res;
  double got;
  res = from_str_to_double("0100000000011001000000000000000000000000000000000000000000000000", &got);
  EXPECT_EQ(res, true);
  EXPECT_DOUBLE_EQ(got, 6.25);
}
