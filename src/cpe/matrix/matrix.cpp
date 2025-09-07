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
#include <cpe/matrix/matrix.hpp>

namespace cpe::matrix {

Matrix::Matrix(std::size_t n_rows, std::size_t n_cols)
    : n_cols_(n_cols), n_rows_(n_rows), data_(n_cols * n_rows, 0.0) {};

Matrix& Matrix::operator+=(double rhs) {
  for (std::size_t i = 0; i < data_.size(); ++i) data_[i] += rhs;
  return *this;
}

Matrix& Matrix::operator+=(const Matrix& rhs) {
  for (std::size_t i = 0; i < data_.size(); ++i) data_[i] += rhs.data_[i];
  return *this;
}

Matrix operator+(Matrix lhs, const Matrix& rhs) {
  lhs += rhs;
  return lhs;
}

Matrix& Matrix::operator*=(double rhs) {
  for (std::size_t i = 0; i < data_.size(); ++i) data_[i] *= rhs;
  return *this;
}

Matrix operator*(Matrix lhs, const Matrix& rhs) {
  // TODO ensure lhs.n_cols_ == rhs.n_rows_
  Matrix result(lhs.n_rows_, rhs.n_cols_);
  for (std::size_t i = 0; i < lhs.n_rows_; ++i) {
    for (std::size_t j = 0; j < rhs.n_cols_; ++j) {
      result[i, j] = 0.0;
      for (std::size_t k = 0; k < lhs.n_cols_; ++k) {
        result[i, j] += lhs[i, k] * rhs[k, j];
      }
    }
  }
  return result;
}

Matrix Matrix::Transpose() const {
  Matrix result(GetNumColumns(), GetNumRows());
  for (std::size_t i = 0; i < GetNumRows(); ++i) {
    for (std::size_t j = 0; j < GetNumColumns(); ++j) {
      result[j, i] = (*this)[i, j];
    }
  }
  return result;
}

}  // namespace cpe::matrix
