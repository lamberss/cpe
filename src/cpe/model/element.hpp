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
 public:
  Element() = delete;

  Element(std::size_t n1, std::size_t n2) {
    nodes[0] = n1;
    nodes[1] = n2;
  }

  std::size_t operator[](std::size_t i) { return nodes[i]; }
  std::size_t size() const { return nodes.size(); }

  static constexpr std::uint8_t nNodes = 2;
  static constexpr std::uint8_t vtk_type = 3;  // VTK_LINE
  static constexpr std::array<std::uint8_t, nNodes> vtk_order{0, 1};
  std::array<std::size_t, nNodes> nodes;
};

}  // namespace cpe::model
