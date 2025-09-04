// MIT License
//
// Copyright (c) 2025 Steven E. Lamberson, Jr.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include <gtest/gtest.h>

#include <cpe/matrix/matrix.hpp>

namespace {

TEST(MatrixTest, Create) {
  constexpr unsigned int c = 3;
  constexpr unsigned int r = 2;
  cpe::matrix::Matrix m(r, c);
  EXPECT_EQ(m.GetNumColumns(), c);
  EXPECT_EQ(m.GetNumRows(), r);
  EXPECT_EQ(m.GetAllocatedSize(), c * r * sizeof(double));
  for (auto i = 0U; i < r; ++i) {
    for (auto j = 0U; j < c; ++j) {
      // Working around a google test bug
      double v = m[i, j];
      EXPECT_EQ(v, 0.0);
    }
  }
}

TEST(MatrixTest, AddAssignScalar) {
  constexpr unsigned int c = 3;
  constexpr unsigned int r = 2;
  constexpr double increment = 5000.0;
  cpe::matrix::Matrix m(r, c);
  for (auto i = 0U; i < r; ++i) {
    for (auto j = 0U; j < c; ++j) {
      m[i, j] =
          5.0 + 100.0 * static_cast<double>(i) + 10 * static_cast<double>(j);
    }
  }

  m += increment;

  for (auto i = 0U; i < r; ++i) {
    for (auto j = 0U; j < c; ++j) {
      double e = increment + 5.0 + 100.0 * static_cast<double>(i) +
                 10 * static_cast<double>(j);
      double v = m[i, j];
      EXPECT_EQ(v, e);
    }
  }
}

TEST(MatrixTest, AddAssignMatrix) {
  constexpr unsigned int c = 3;
  constexpr unsigned int r = 2;
  cpe::matrix::Matrix m1(r, c);
  cpe::matrix::Matrix m2(r, c);
  for (auto i = 0U; i < r; ++i) {
    for (auto j = 0U; j < c; ++j) {
      m1[i, j] =
          5.0 + 100.0 * static_cast<double>(i) + 10 * static_cast<double>(j);
      m2[i, j] =
          5.0 + 100.0 * static_cast<double>(i) + 10 * static_cast<double>(j);
    }
  }

  m1 += m2;

  for (auto i = 0U; i < r; ++i) {
    for (auto j = 0U; j < c; ++j) {
      double e = 2.0 * (5.0 + 100.0 * static_cast<double>(i) +
                        10 * static_cast<double>(j));
      double v = m1[i, j];
      EXPECT_EQ(v, e);
    }
  }
}

TEST(MatrixTest, AddMatrix) {
  constexpr unsigned int c = 3;
  constexpr unsigned int r = 2;
  cpe::matrix::Matrix m1(r, c);
  cpe::matrix::Matrix m2(r, c);
  for (auto i = 0U; i < r; ++i) {
    for (auto j = 0U; j < c; ++j) {
      m1[i, j] =
          5.0 + 100.0 * static_cast<double>(i) + 10 * static_cast<double>(j);
      m2[i, j] =
          5.0 + 100.0 * static_cast<double>(i) + 10 * static_cast<double>(j);
    }
  }

  cpe::matrix::Matrix m3 = m1 + m2;

  for (auto i = 0U; i < r; ++i) {
    for (auto j = 0U; j < c; ++j) {
      double e = 2.0 * (5.0 + 100.0 * static_cast<double>(i) +
                        10 * static_cast<double>(j));
      double v = m3[i, j];
      EXPECT_EQ(v, e);
    }
  }
}

TEST(MatrixTest, MultiplyAssignScalar) {
  constexpr unsigned int c = 3;
  constexpr unsigned int r = 2;
  constexpr double scale = 2.0;
  cpe::matrix::Matrix m(r, c);
  for (auto i = 0U; i < r; ++i) {
    for (auto j = 0U; j < c; ++j) {
      m[i, j] =
          5.0 + 100.0 * static_cast<double>(i) + 10 * static_cast<double>(j);
    }
  }

  m *= scale;

  for (auto i = 0U; i < r; ++i) {
    for (auto j = 0U; j < c; ++j) {
      double e = scale * (5.0 + 100.0 * static_cast<double>(i) +
                          10 * static_cast<double>(j));
      double v = m[i, j];
      EXPECT_EQ(v, e);
    }
  }
}

TEST(MatrixTest, MultiplyMatrix) {
  constexpr unsigned int m = 2;
  constexpr unsigned int n = 3;
  cpe::matrix::Matrix a(m, n);
  cpe::matrix::Matrix b(n, m);
  a[0, 0] = 1.0;
  a[0, 1] = 2.0;
  a[0, 2] = 3.0;
  a[1, 0] = 4.0;
  a[1, 1] = 5.0;
  a[1, 2] = 6.0;
  b[0, 0] = 7.0;
  b[0, 1] = 8.0;
  b[1, 0] = 9.0;
  b[1, 1] = 10.0;
  b[2, 0] = 11.0;
  b[2, 1] = 12.0;

  cpe::matrix::Matrix c = a * b;

  EXPECT_EQ(c.GetNumColumns(), m);
  EXPECT_EQ(c.GetNumRows(), m);

  const double c00 = c[0, 0];
  const double c01 = c[0, 1];
  const double c10 = c[1, 0];
  const double c11 = c[1, 1];

  EXPECT_EQ(c00, 58.0);
  EXPECT_EQ(c01, 64.0);
  EXPECT_EQ(c10, 139.0);
  EXPECT_EQ(c11, 154.0);
}

}  // namespace
