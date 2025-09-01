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

#include <cpe/model/model.hpp>

namespace {

TEST(ModelTest, Create) {
  cpe::model::Model model;
  EXPECT_EQ(model.blocks.size(), 0);
  EXPECT_EQ(model.nodes.size(), 0);
}

TEST(ModelTest, AddConstraintSingle) {
  cpe::model::Model model;
  model.nodes.add(0, 1.0);
  model.nodes.add(1, 2.0);
  model.add_constraint(cpe::model::dof::X, 1.0, 0);
  EXPECT_EQ(model.constraints[0], cpe::model::dof::X);
  EXPECT_EQ(model.constraints[1], cpe::model::dof::NONE);
  model.add_constraint(cpe::model::dof::Y, 2.0, 0);
  EXPECT_EQ(model.constraints[0], cpe::model::dof::X | cpe::model::dof::Y);
  EXPECT_EQ(model.constraints[1], cpe::model::dof::NONE);
  EXPECT_EQ(model.nodes[0].global_dof_index[0], 0);
  EXPECT_EQ(model.nodes[0].global_dof_index[5], 5);
  EXPECT_EQ(model.nodes[1].global_dof_index[0], 6);
  EXPECT_EQ(model.nodes[1].global_dof_index[5], 11);
  EXPECT_EQ(model.global_dof.size(), 12);
  for (std::size_t i = 0; i < model.global_dof.size(); ++i) {
    if (i == 0) {
      EXPECT_EQ(model.global_dof[i], 1.0);
    } else if (i == 1) {
      EXPECT_EQ(model.global_dof[i], 2.0);
    } else {
      EXPECT_EQ(model.global_dof[i], 0.0);
    }
  }
}

TEST(ModelTest, AddConstraintMulti) {
  cpe::model::Model model;
  model.nodes.add(0, 1.0);
  model.nodes.add(1, 2.0);
  model.add_constraint(cpe::model::dof::X, 1.0, {0, 1});
  EXPECT_EQ(model.constraints[0], cpe::model::dof::X);
  EXPECT_EQ(model.constraints[1], cpe::model::dof::X);
  EXPECT_EQ(model.nodes[0].global_dof_index[0], 0);
  EXPECT_EQ(model.nodes[0].global_dof_index[5], 5);
  EXPECT_EQ(model.nodes[1].global_dof_index[0], 6);
  EXPECT_EQ(model.nodes[1].global_dof_index[5], 11);
  EXPECT_EQ(model.global_dof.size(), 12);
  for (std::size_t i = 0; i < model.global_dof.size(); ++i) {
    if (i == 0 || i == 6) {
      EXPECT_EQ(model.global_dof[i], 1.0);
    } else {
      EXPECT_EQ(model.global_dof[i], 0.0);
    }
  }
}

TEST(ModelTest, AddConstraintAll) {
  cpe::model::Model model;
  model.nodes.add(0, 1.0);
  model.nodes.add(1, 2.0);
  model.add_constraint(cpe::model::dof::X, 1.0);
  EXPECT_EQ(model.constraints[0], cpe::model::dof::X);
  EXPECT_EQ(model.constraints[1], cpe::model::dof::X);
  EXPECT_EQ(model.nodes[0].global_dof_index[0], 0);
  EXPECT_EQ(model.nodes[0].global_dof_index[5], 5);
  EXPECT_EQ(model.nodes[1].global_dof_index[0], 6);
  EXPECT_EQ(model.nodes[1].global_dof_index[5], 11);
  EXPECT_EQ(model.global_dof.size(), 12);
  for (std::size_t i = 0; i < model.global_dof.size(); ++i) {
    if (i == 0 || i == 6) {
      EXPECT_EQ(model.global_dof[i], 1.0);
    } else {
      EXPECT_EQ(model.global_dof[i], 0.0);
    }
  }
}

}  // namespace
