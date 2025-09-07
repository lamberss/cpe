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
#pragma once

#include <vector>

namespace cpe::matrix {

class Matrix {
 public:
  Matrix(std::size_t n_rows, std::size_t n_cols);

  double& operator[](std::size_t i, std::size_t j) {
    return data_[i * n_cols_ + j];
  }
  const double& operator[](std::size_t i, std::size_t j) const {
    return data_[i * n_cols_ + j];
  }

  double& operator[](std::size_t index) { return data_[index]; }
  const double& operator[](std::size_t index) const { return data_[index]; }

  Matrix& operator+=(double rhs);
  Matrix& operator+=(const Matrix& rhs);
  friend Matrix operator+(Matrix lhs, const Matrix& rhs);

  Matrix& operator*=(double rhs);
  friend Matrix operator*(Matrix lhs, const Matrix& rhs);

  std::size_t GetAllocatedSize() const {
    return sizeof(data_[0]) * data_.capacity();
  }
  std::size_t GetNumColumns() const { return n_cols_; }
  std::size_t GetNumRows() const { return n_rows_; }

  Matrix Transpose() const;

 private:
  std::size_t n_cols_;
  std::size_t n_rows_;
  std::vector<double> data_;
};

}  // namespace cpe::matrix
