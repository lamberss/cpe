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
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHkAll THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include <gtest/gtest.h>

#include <cpe/model/dof.hpp>
#include <limits>

namespace {

TEST(DoFTest, DofIndex) {
  EXPECT_EQ(cpe::model::dof::kIx, 0);
  EXPECT_EQ(cpe::model::dof::kIy, 1);
  EXPECT_EQ(cpe::model::dof::kIz, 2);
  EXPECT_EQ(cpe::model::dof::kIdx, 3);
  EXPECT_EQ(cpe::model::dof::kIdy, 4);
  EXPECT_EQ(cpe::model::dof::kIdz, 5);
  EXPECT_EQ(cpe::model::dof::kNumStrucDof, 6);
  EXPECT_EQ(cpe::model::dof::kInactiveDof,
            std::numeric_limits<std::size_t>::max());
}

TEST(DoFTest, DoF_None) { EXPECT_EQ(cpe::model::dof::kNone, 0); }

TEST(DoFTest, DoF_Translations) {
  cpe::model::dof::Dof constraint = cpe::model::dof::kNone;
  EXPECT_EQ(constraint | cpe::model::dof::kX, 1);
  EXPECT_EQ(constraint | cpe::model::dof::kY, 2);
  EXPECT_EQ(constraint | cpe::model::dof::kZ, 4);
}

TEST(DoFTest, DoF_Rotations) {
  cpe::model::dof::Dof constraint = cpe::model::dof::kNone;
  EXPECT_EQ(constraint | cpe::model::dof::kDx, 8);
  EXPECT_EQ(constraint | cpe::model::dof::kDy, 16);
  EXPECT_EQ(constraint | cpe::model::dof::kDz, 32);
}

TEST(DoFTest, DoF_Combinations) {
  cpe::model::dof::Dof x = cpe::model::dof::kX;
  cpe::model::dof::Dof y = cpe::model::dof::kY;
  cpe::model::dof::Dof z = cpe::model::dof::kZ;
  cpe::model::dof::Dof dx = cpe::model::dof::kDx;
  cpe::model::dof::Dof dy = cpe::model::dof::kDy;
  cpe::model::dof::Dof dz = cpe::model::dof::kDz;
  cpe::model::dof::Dof trans = cpe::model::dof::kAllTrans;
  cpe::model::dof::Dof rot = cpe::model::dof::kAllRot;
  cpe::model::dof::Dof all = cpe::model::dof::kAll;
  cpe::model::dof::Dof non2d = cpe::model::dof::kAllNon2d;
  EXPECT_EQ(x | y | z, trans);
  EXPECT_EQ(dx | dy | dz, rot);
  EXPECT_EQ(x | y | z | dx | dy | dz, all);
  EXPECT_EQ(z | dx | dy, non2d);
}

}  // namespace
