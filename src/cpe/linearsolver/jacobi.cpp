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
#include <cmath>
#include <cpe/linearsolver/jacobi.hpp>
#include <iomanip>
#include <iostream>

namespace cpe::linearsolver::jacobi {

int Solve(const cpe::matrix::Matrix& A, cpe::matrix::Matrix& x,
          const cpe::matrix::Matrix& b) {
  constexpr double min_value = 1.0e-12;
  constexpr double update_absolute_tolerance = 1.0e-6;
  cpe::matrix::Matrix residual(A.GetNumRows(), 1);
  cpe::matrix::Matrix update(A.GetNumRows(), 1);
  cpe::matrix::Matrix x_old(A.GetNumRows(), 1);

  std::cout << std::setw(10) << "Iteration";
  std::cout << std::setw(15) << "|R|";
  std::cout << std::setw(15) << "|R| / |x|";
  std::cout << std::setw(15) << "|dx|";
  std::cout << std::setw(15) << "|dx| / |x|";
  std::cout << std::endl;
  std::cout << std::setw(10) << "---------";
  std::cout << std::setw(15) << "-------------";
  std::cout << std::setw(15) << "-------------";
  std::cout << std::setw(15) << "-------------";
  std::cout << std::setw(15) << "-------------";
  std::cout << std::endl;

  constexpr std::size_t maximum_iterations = 1000;
  std::size_t iteration_count = 0;
  for (std::size_t it = 0; it < maximum_iterations; ++it) {
    iteration_count++;

    double update_absolute_error = 0.0;
    double update_relative_error = 0.0;
    double residual_absolute_error = 0.0;
    double residual_relative_error = 0.0;
    for (std::size_t i = 0; i < A.GetNumRows(); ++i) {
      residual[i] = b[i];
      for (std::size_t j = 0; j < A.GetNumRows(); ++j) {
        residual[i] -= A[i, j] * x_old[j];
      }
      update[i] = residual[i] / A[i, i];
      x[i] = x_old[i] + update[i];
      residual_absolute_error += (residual[i] * residual[i]);
      residual_relative_error +=
          (residual[i] * residual[i]) / std::max(x[i] * x[i], min_value);
      update_absolute_error += (update[i] * update[i]);
      update_relative_error +=
          (update[i] * update[i]) / std::max(x[i] * x[i], min_value);
    }
    residual_absolute_error = std::sqrt(residual_absolute_error);
    residual_relative_error = std::sqrt(residual_relative_error);
    update_absolute_error = std::sqrt(update_absolute_error);
    update_relative_error = std::sqrt(update_relative_error);

    std::cout << std::setw(10) << iteration_count;
    std::cout << std::setprecision(5) << std::scientific;
    std::cout << std::setw(15) << residual_absolute_error;
    std::cout << std::setw(15) << residual_relative_error;
    std::cout << std::setw(15) << update_absolute_error;
    std::cout << std::setw(15) << update_relative_error;
    std::cout << std::endl;

    if (update_absolute_error <= update_absolute_tolerance) break;
    for (std::size_t i = 0; i < A.GetNumRows(); ++i) x_old[i] = x[i];
  }

  return iteration_count;
}

}  // namespace cpe::linearsolver::jacobi
