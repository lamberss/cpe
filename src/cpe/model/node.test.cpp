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

#include <cpe/model/node.hpp>

namespace {

static constexpr std::array<std::size_t, cpe::model::dof::NUM_STRUC_DOF>
    inactive{cpe::model::dof::INACTIVE_DOF, cpe::model::dof::INACTIVE_DOF,
             cpe::model::dof::INACTIVE_DOF, cpe::model::dof::INACTIVE_DOF,
             cpe::model::dof::INACTIVE_DOF, cpe::model::dof::INACTIVE_DOF};

TEST(NodeTest, Create0) {
  cpe::model::Node n;
  EXPECT_EQ(n.x, 0.0);
  EXPECT_EQ(n.y, 0.0);
  EXPECT_EQ(n.z, 0.0);
  EXPECT_EQ(n.constrained_dofs, cpe::model::dof::NONE);
  EXPECT_EQ(n.active_dof_index, inactive);
  EXPECT_EQ(n.global_dof_index, inactive);
}

TEST(NodeTest, Create1) {
  const double x = 1.0;
  cpe::model::Node n(x);
  EXPECT_EQ(n.x, x);
  EXPECT_EQ(n.y, 0.0);
  EXPECT_EQ(n.z, 0.0);
  EXPECT_EQ(n.constrained_dofs, cpe::model::dof::NONE);
  EXPECT_EQ(n.active_dof_index, inactive);
  EXPECT_EQ(n.global_dof_index, inactive);
}

TEST(NodeTest, Create2) {
  const double x = 1.0, y = 2.0;
  cpe::model::Node n(x, y);
  EXPECT_EQ(n.x, x);
  EXPECT_EQ(n.y, y);
  EXPECT_EQ(n.z, 0.0);
  EXPECT_EQ(n.constrained_dofs, cpe::model::dof::NONE);
  EXPECT_EQ(n.active_dof_index, inactive);
  EXPECT_EQ(n.global_dof_index, inactive);
}

TEST(NodeTest, Create3) {
  const double x = 1.0, y = 2.0, z = 3.0;
  cpe::model::Node n(x, y, z);
  EXPECT_EQ(n.x, x);
  EXPECT_EQ(n.y, y);
  EXPECT_EQ(n.z, z);
  EXPECT_EQ(n.constrained_dofs, cpe::model::dof::NONE);
  EXPECT_EQ(n.active_dof_index, inactive);
  EXPECT_EQ(n.global_dof_index, inactive);
}

}  // namespace
