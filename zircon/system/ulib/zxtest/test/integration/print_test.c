// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdint.h>
#include <sys/types.h>

#include <zxtest/zxtest.h>

// Sanity tests that enforce compile time check for printing primitive types, and preventing
// undefined symbols.
TEST(CPrintTest, Uint32) {
  uint32_t a = 0;

  ASSERT_EQ(a, 0u);
}

TEST(CPrintTest, Int32) {
  int32_t a = 0;

  ASSERT_EQ(a, 0);
}

TEST(CPrintTest, Uint64) {
  int64_t a = 0;

  ASSERT_EQ(a, 0u);
}

TEST(CPrintTest, Int64) {
  int64_t a = 0;

  ASSERT_EQ(a, 0);
}

TEST(CPrintTest, Float) {
  float a = 0.0;

  ASSERT_EQ(a, 0.0);
}

TEST(CPrintTest, Double) {
  double a = 0.0;

  ASSERT_EQ(a, 0.0);
}

TEST(CPrintTest, Str) {
  const char* a = "MyStr";

  ASSERT_STR_EQ(a, "MyStr");
}

TEST(CPrintTest, Char) {
  char c = 'a';

  ASSERT_EQ(c, 'a');
}

TEST(CPrintTest, UnsignedChar) {
  unsigned char c = 'a';

  ASSERT_EQ(c, 'a');
}

TEST(CPrintTest, SignedChar) {
  signed char c = 'a';

  ASSERT_EQ(c, 'a');
}

TEST(CPrintTest, LongLong) {
  long long a = 0;

  ASSERT_EQ(a, 0);
}

TEST(CPrintTest, OffT) {
  off_t a = 0;

  ASSERT_EQ(a, 0);
}
