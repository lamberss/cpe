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

#include <cpe/model/element.hpp>
#include <cpe/model/model.hpp>

namespace {

TEST(ModelTest, Create) {
  cpe::model::Model model;
  EXPECT_EQ(model.blocks_.size(), 0);
  EXPECT_EQ(model.nodes_.GetNumNodes(), 0);
}

TEST(ModelTest, AddConstraintSingle) {
  cpe::model::Model model;
  model.nodes_.AddNode(0, 1.0);
  model.nodes_.AddNode(1, 2.0);
  model.AddConstraint(cpe::model::dof::kX, 1.0, 0);
  EXPECT_EQ(model.constraints_[0], cpe::model::dof::kX);
  EXPECT_EQ(model.constraints_[1], cpe::model::dof::kNone);
  model.AddConstraint(cpe::model::dof::kY, 2.0, 0);
  EXPECT_EQ(model.constraints_[0], cpe::model::dof::kX | cpe::model::dof::kY);
  EXPECT_EQ(model.constraints_[1], cpe::model::dof::kNone);
  EXPECT_EQ(model.nodes_[0].global_dof_index_[0], 0);
  EXPECT_EQ(model.nodes_[0].global_dof_index_[5], 5);
  EXPECT_EQ(model.nodes_[1].global_dof_index_[0], 6);
  EXPECT_EQ(model.nodes_[1].global_dof_index_[5], 11);
  EXPECT_EQ(model.global_dof_.size(), 12);
  for (std::size_t i = 0; i < model.global_dof_.size(); ++i) {
    if (i == 0) {
      EXPECT_EQ(model.global_dof_[i], 1.0);
    } else if (i == 1) {
      EXPECT_EQ(model.global_dof_[i], 2.0);
    } else {
      EXPECT_EQ(model.global_dof_[i], 0.0);
    }
  }
}

TEST(ModelTest, AddConstraintMulti) {
  cpe::model::Model model;
  model.nodes_.AddNode(0, 1.0);
  model.nodes_.AddNode(1, 2.0);
  model.AddConstraint(cpe::model::dof::kX, 1.0, {0, 1});
  EXPECT_EQ(model.constraints_[0], cpe::model::dof::kX);
  EXPECT_EQ(model.constraints_[1], cpe::model::dof::kX);
  EXPECT_EQ(model.nodes_[0].global_dof_index_[0], 0);
  EXPECT_EQ(model.nodes_[0].global_dof_index_[5], 5);
  EXPECT_EQ(model.nodes_[1].global_dof_index_[0], 6);
  EXPECT_EQ(model.nodes_[1].global_dof_index_[5], 11);
  EXPECT_EQ(model.global_dof_.size(), 12);
  for (std::size_t i = 0; i < model.global_dof_.size(); ++i) {
    if (i == 0 || i == 6) {
      EXPECT_EQ(model.global_dof_[i], 1.0);
    } else {
      EXPECT_EQ(model.global_dof_[i], 0.0);
    }
  }
}

TEST(ModelTest, AddConstraintAll) {
  cpe::model::Model model;
  model.nodes_.AddNode(0, 1.0);
  model.nodes_.AddNode(1, 2.0);
  model.AddConstraint(cpe::model::dof::kX, 1.0);
  EXPECT_EQ(model.constraints_[0], cpe::model::dof::kX);
  EXPECT_EQ(model.constraints_[1], cpe::model::dof::kX);
  EXPECT_EQ(model.nodes_[0].global_dof_index_[0], 0);
  EXPECT_EQ(model.nodes_[0].global_dof_index_[5], 5);
  EXPECT_EQ(model.nodes_[1].global_dof_index_[0], 6);
  EXPECT_EQ(model.nodes_[1].global_dof_index_[5], 11);
  EXPECT_EQ(model.global_dof_.size(), 12);
  for (std::size_t i = 0; i < model.global_dof_.size(); ++i) {
    if (i == 0 || i == 6) {
      EXPECT_EQ(model.global_dof_[i], 1.0);
    } else {
      EXPECT_EQ(model.global_dof_[i], 0.0);
    }
  }
}

TEST(ModelTest, AddForceSingle) {
  cpe::model::Model model;
  model.nodes_.AddNode(0, 1.0);
  model.nodes_.AddNode(1, 2.0);
  model.AddForce(cpe::model::dof::kX, 1.0, 0);
  model.AddForce(cpe::model::dof::kY, 2.0, 0);
  EXPECT_EQ(model.global_force_.size(), 12);
  for (std::size_t i = 0; i < model.global_force_.size(); ++i) {
    if (i == 0) {
      EXPECT_EQ(model.global_force_[i], 1.0);
    } else if (i == 1) {
      EXPECT_EQ(model.global_force_[i], 2.0);
    } else {
      EXPECT_EQ(model.global_force_[i], 0.0);
    }
  }
}

TEST(ModelTest, AddForceMulti) {
  cpe::model::Model model;
  model.nodes_.AddNode(0, 1.0);
  model.nodes_.AddNode(1, 2.0);
  model.AddForce(cpe::model::dof::kX, 1.0, {0, 1});
  EXPECT_EQ(model.global_force_.size(), 12);
  for (std::size_t i = 0; i < model.global_force_.size(); ++i) {
    if (i == 0 || i == 6) {
      EXPECT_EQ(model.global_force_[i], 1.0);
    } else {
      EXPECT_EQ(model.global_force_[i], 0.0);
    }
  }
}

TEST(ModelTest, AddForceAll) {
  cpe::model::Model model;
  model.nodes_.AddNode(0, 1.0);
  model.nodes_.AddNode(1, 2.0);
  model.AddForce(cpe::model::dof::kX, 1.0);
  EXPECT_EQ(model.global_force_.size(), 12);
  for (std::size_t i = 0; i < model.global_force_.size(); ++i) {
    if (i == 0 || i == 6) {
      EXPECT_EQ(model.global_force_[i], 1.0);
    } else {
      EXPECT_EQ(model.global_force_[i], 0.0);
    }
  }
}

TEST(ModelTest, GetNumberOfElements) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("Aluminum", 70.0e9, 0.3);
  std::shared_ptr<cpe::model::Property> property_square =
      std::make_shared<cpe::model::Property>("square", material);
  (*property_square)["area"] = 1.0;
  std::shared_ptr<cpe::model::Property> property_circle =
      std::make_shared<cpe::model::Property>("circle", material);
  (*property_circle)["area"] = 2.0;
  cpe::model::Model model;
  model.nodes_.AddNode(0, 1.0);
  model.nodes_.AddNode(1, 2.0);
  using element_block_t = cpe::model::ElementBlock<cpe::model::Element>;
  std::shared_ptr<element_block_t> block1 =
      std::make_shared<element_block_t>("truss1", property_square, 4);
  model.blocks_.push_back(block1);
  std::shared_ptr<element_block_t> block2 =
      std::make_shared<element_block_t>("truss2", property_circle, 4);
  model.blocks_.push_back(block2);
  block1->AddElement(0, 2);
  block1->AddElement(0, 3);
  block1->AddElement(1, 3);
  block1->AddElement(2, 3);
  block2->AddElement(2, 4);
  block2->AddElement(3, 4);
  block2->AddElement(3, 5);
  block2->AddElement(4, 5);
  EXPECT_EQ(model.GetNumElements(), 8);
}

TEST(ModelTest, GetNumberOfNodes) {
  cpe::model::Model model;
  model.nodes_.AddNode(0, 1.0);
  model.nodes_.AddNode(1, 2.0);
  EXPECT_EQ(model.GetNumNodes(), 2);
}

}  // namespace
