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

#include <array>
#include <cstdint>

namespace cpe::model {

class Element {
 private:
  static constexpr std::uint8_t kNumNodes = 2;

 public:
  Element() = delete;

  Element(std::size_t n1, std::size_t n2) {
    nodes_[0] = n1;
    nodes_[1] = n2;
  }

  std::size_t GetNumNodes() const { return nodes_.size(); }
  std::size_t operator[](std::size_t i) { return nodes_[i]; }

  static constexpr std::uint8_t kVtkType = 3;  // VTK_LINE
  static constexpr std::array<std::uint8_t, kNumNodes> kVtkOrder{0, 1};
  std::array<std::size_t, kNumNodes> nodes_;
};

}  // namespace cpe::model
