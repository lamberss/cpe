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

#include <cpe/linearsolver/ssor.hpp>

namespace {

TEST(SSORTest, SolveGS) {
  cpe::matrix::Matrix A(5, 5);
  A[0, 0] = A[1, 1] = A[2, 2] = A[3, 3] = A[4, 4] = 4.0;
  A[0, 1] = A[1, 2] = A[2, 3] = A[3, 4] = -1.0;
  A[1, 0] = A[2, 1] = A[3, 2] = A[4, 3] = -1.0;
  A[0, 3] = A[3, 0] = A[1, 4] = A[4, 1] = 1.0;
  cpe::matrix::Matrix b(5, 1);
  b[0] = b[1] = b[2] = b[3] = b[4] = 100.0;
  cpe::matrix::Matrix x(5, 1);
  int num_iter = cpe::linearsolver::ssor::Solve(A, x, b, 1.0);
  EXPECT_EQ(num_iter, 15);
  EXPECT_NEAR(x[0], 25.000000, 0.0001);
  EXPECT_NEAR(x[1], 35.714285, 0.0001);
  EXPECT_NEAR(x[2], 42.857143, 0.0001);
  EXPECT_NEAR(x[3], 35.714285, 0.0001);
  EXPECT_NEAR(x[4], 25.000000, 0.0001);
}

TEST(SSORTest, SolveOverRelax) {
  cpe::matrix::Matrix A(5, 5);
  A[0, 0] = A[1, 1] = A[2, 2] = A[3, 3] = A[4, 4] = 4.0;
  A[0, 1] = A[1, 2] = A[2, 3] = A[3, 4] = -1.0;
  A[1, 0] = A[2, 1] = A[3, 2] = A[4, 3] = -1.0;
  A[0, 3] = A[3, 0] = A[1, 4] = A[4, 1] = 1.0;
  cpe::matrix::Matrix b(5, 1);
  b[0] = b[1] = b[2] = b[3] = b[4] = 100.0;
  cpe::matrix::Matrix x(5, 1);
  int num_iter = cpe::linearsolver::ssor::Solve(A, x, b, 1.1);
  EXPECT_EQ(num_iter, 13);
  EXPECT_NEAR(x[0], 25.000000, 0.0001);
  EXPECT_NEAR(x[1], 35.714285, 0.0001);
  EXPECT_NEAR(x[2], 42.857143, 0.0001);
  EXPECT_NEAR(x[3], 35.714285, 0.0001);
  EXPECT_NEAR(x[4], 25.000000, 0.0001);
}

}  // namespace
