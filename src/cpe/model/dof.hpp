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

#include <cstdint>
#include <limits>

namespace cpe::model::dof {

static constexpr std::size_t NUM_STRUC_DOF = 6;
static constexpr std::size_t INACTIVE_DOF =
    std::numeric_limits<std::size_t>::max();

enum DofIndex { IX = 0, IY = 1, IZ = 2, IDX = 3, IDY = 4, IDZ = 5 };

enum Dof : std::int8_t {
  NONE = 0,        // 00000000
  X = 1,           // 00000001
  Y = 2,           // 00000010
  Z = 4,           // 00000100
  ALL_TRANS = 7,   // 00000111
  DX = 8,          // 00001000
  DY = 16,         // 00010000
  ALL_NON2D = 28,  // 00011100
  DZ = 32,         // 00100000
  ALL_ROT = 56,    // 00111000
  ALL = 63         // 00111111
};

}  // namespace cpe::model::dof
