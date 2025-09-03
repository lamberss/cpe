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

static constexpr std::size_t kNumStrucDof = 6;
static constexpr std::size_t kInactiveDof =
    std::numeric_limits<std::size_t>::max();

enum DofIndex { kIx = 0, kIy = 1, kIz = 2, kIdx = 3, kIdy = 4, kIdz = 5 };

enum Dof : std::int8_t {
  kNone = 0,       // 00000000
  kX = 1,          // 00000001
  kY = 2,          // 00000010
  kZ = 4,          // 00000100
  kAllTrans = 7,   // 00000111
  kDx = 8,         // 00001000
  kDy = 16,        // 00010000
  kAllNon2d = 28,  // 00011100
  kDz = 32,        // 00100000
  kAllRot = 56,    // 00111000
  kAll = 63        // 00111111
};

}  // namespace cpe::model::dof
