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

#include <cpe/model/dof.hpp>
#include <limits>

namespace {

TEST(DoFTest, DofIndex) {
  EXPECT_EQ(cpe::model::dof::IX, 0);
  EXPECT_EQ(cpe::model::dof::IY, 1);
  EXPECT_EQ(cpe::model::dof::IZ, 2);
  EXPECT_EQ(cpe::model::dof::IDX, 3);
  EXPECT_EQ(cpe::model::dof::IDY, 4);
  EXPECT_EQ(cpe::model::dof::IDZ, 5);
  EXPECT_EQ(cpe::model::dof::NUM_STRUC_DOF, 6);
  EXPECT_EQ(cpe::model::dof::INACTIVE_DOF,
            std::numeric_limits<std::size_t>::max());
}

TEST(DoFTest, DoF_None) { EXPECT_EQ(cpe::model::dof::NONE, 0); }

TEST(DoFTest, DoF_Translations) {
  cpe::model::dof::Dof constraint = cpe::model::dof::NONE;
  EXPECT_EQ(constraint | cpe::model::dof::X, 1);
  EXPECT_EQ(constraint | cpe::model::dof::Y, 2);
  EXPECT_EQ(constraint | cpe::model::dof::Z, 4);
}

TEST(DoFTest, DoF_Rotations) {
  cpe::model::dof::Dof constraint = cpe::model::dof::NONE;
  EXPECT_EQ(constraint | cpe::model::dof::DX, 8);
  EXPECT_EQ(constraint | cpe::model::dof::DY, 16);
  EXPECT_EQ(constraint | cpe::model::dof::DZ, 32);
}

TEST(DoFTest, DoF_Combinations) {
  cpe::model::dof::Dof x = cpe::model::dof::X;
  cpe::model::dof::Dof y = cpe::model::dof::Y;
  cpe::model::dof::Dof z = cpe::model::dof::Z;
  cpe::model::dof::Dof dx = cpe::model::dof::DX;
  cpe::model::dof::Dof dy = cpe::model::dof::DY;
  cpe::model::dof::Dof dz = cpe::model::dof::DZ;
  cpe::model::dof::Dof trans = cpe::model::dof::ALL_TRANS;
  cpe::model::dof::Dof rot = cpe::model::dof::ALL_ROT;
  cpe::model::dof::Dof all = cpe::model::dof::ALL;
  cpe::model::dof::Dof non2d = cpe::model::dof::ALL_NON2D;
  EXPECT_EQ(x | y | z, trans);
  EXPECT_EQ(dx | dy | dz, rot);
  EXPECT_EQ(x | y | z | dx | dy | dz, all);
  EXPECT_EQ(z | dx | dy, non2d);
}

}  // namespace
