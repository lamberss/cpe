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

#include <cmath>
#include <cpe/model/element.hpp>

namespace {

TEST(ElementTest, CreateAndAccess) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("Aluminum", 70.0e9, 0.3);
  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("square", material);
  (*property)["area"] = 1.0;
  const std::size_t n1 = 1000532;
  const std::size_t n2 = 47;
  cpe::model::Element element(property, n1, n2);
  EXPECT_EQ(element[0], n1);
  EXPECT_EQ(element[1], n2);
  EXPECT_EQ(element.GetSupportedDof(), cpe::model::dof::kAllTrans);
}

TEST(ElementTest, AssembleXAllignedConstrained) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("Fake material", 1000.0, 0.3);
  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("square", material);
  (*property)["area"] = 10.0;
  const double length = 100.0;
  const double k = (*property)["area"] * material->YoungsModulus() / length;
  const std::size_t n1 = 0;
  const std::size_t n2 = 1;
  cpe::model::Element element(property, n1, n2);
  cpe::model::NodeList nodes;
  nodes.AddNode(n1, 0.0, 0.0, 0.0);
  nodes.AddNode(n2, length, 0.0, 0.0);
  nodes[0].constrained_dofs_ = cpe::model::dof::kAll;
  nodes[1].constrained_dofs_ = static_cast<cpe::model::dof::Dof>(
      cpe::model::dof::kAllRot | cpe::model::dof::kY | cpe::model::dof::kZ);
  nodes[1].active_dof_index_[0] = 0;

  std::shared_ptr<cpe::matrix::Matrix> stiff =
      std::make_unique<cpe::matrix::Matrix>(1, 1);
  element.Assemble(nodes, stiff);

  double a = (*stiff)[0, 0];
  EXPECT_EQ(a, k);
}

TEST(ElementTest, AssembleXAllignedUnconstrained) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("Fake material", 1000.0, 0.3);
  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("square", material);
  (*property)["area"] = 10.0;
  const double length = 100.0;
  const double k = (*property)["area"] * material->YoungsModulus() / length;
  const std::size_t n1 = 0;
  const std::size_t n2 = 1;
  cpe::model::Element element(property, n1, n2);
  cpe::model::NodeList nodes;
  nodes.AddNode(n1, 0.0, 0.0, 0.0);
  nodes.AddNode(n2, length, 0.0, 0.0);
  nodes[0].active_dof_index_[0] = 0;
  nodes[0].active_dof_index_[1] = 1;
  nodes[0].active_dof_index_[2] = 2;
  nodes[1].active_dof_index_[0] = 3;
  nodes[1].active_dof_index_[1] = 4;
  nodes[1].active_dof_index_[2] = 5;

  std::shared_ptr<cpe::matrix::Matrix> stiff =
      std::make_unique<cpe::matrix::Matrix>(6, 6);
  element.Assemble(nodes, stiff);

  for (std::size_t i = 0; i < 6; ++i) {
    for (std::size_t j = 0; j < 6; ++j) {
      double a = (*stiff)[i, j];
      if (((i == 0) && (j == 0)) || ((i == 3) && (j == 3))) {
        EXPECT_EQ(a, k);
      } else if (((i == 0) && (j == 3)) || ((i == 3) && (j == 0))) {
        EXPECT_EQ(a, -k);
      } else {
        EXPECT_EQ(a, 0.0);
      }
    }
  }
}

TEST(ElementTest, AssembleYAllignedConstrained) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("Fake material", 1000.0, 0.3);
  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("square", material);
  (*property)["area"] = 10.0;
  const double length = 100.0;
  const double k = (*property)["area"] * material->YoungsModulus() / length;
  const std::size_t n1 = 0;
  const std::size_t n2 = 1;
  cpe::model::Element element(property, n1, n2);
  cpe::model::NodeList nodes;
  nodes.AddNode(n1, 0.0, 0.0, 0.0);
  nodes.AddNode(n2, 0.0, length, 0.0);
  nodes[0].constrained_dofs_ = cpe::model::dof::kAll;
  nodes[1].constrained_dofs_ = static_cast<cpe::model::dof::Dof>(
      cpe::model::dof::kAllRot | cpe::model::dof::kX | cpe::model::dof::kZ);
  nodes[1].active_dof_index_[1] = 0;

  std::shared_ptr<cpe::matrix::Matrix> stiff =
      std::make_unique<cpe::matrix::Matrix>(1, 1);
  element.Assemble(nodes, stiff);

  double a = (*stiff)[0, 0];
  EXPECT_EQ(a, k);
}

TEST(ElementTest, AssembleYAllignedUnconstrained) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("Fake material", 1000.0, 0.3);
  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("square", material);
  (*property)["area"] = 10.0;
  const double length = 100.0;
  const double k = (*property)["area"] * material->YoungsModulus() / length;
  const std::size_t n1 = 0;
  const std::size_t n2 = 1;
  cpe::model::Element element(property, n1, n2);
  cpe::model::NodeList nodes;
  nodes.AddNode(n1, 0.0, 0.0, 0.0);
  nodes.AddNode(n2, 0.0, length, 0.0);
  nodes[0].active_dof_index_[0] = 0;
  nodes[0].active_dof_index_[1] = 1;
  nodes[0].active_dof_index_[2] = 2;
  nodes[1].active_dof_index_[0] = 3;
  nodes[1].active_dof_index_[1] = 4;
  nodes[1].active_dof_index_[2] = 5;

  std::shared_ptr<cpe::matrix::Matrix> stiff =
      std::make_unique<cpe::matrix::Matrix>(6, 6);
  element.Assemble(nodes, stiff);

  for (std::size_t i = 0; i < 6; ++i) {
    for (std::size_t j = 0; j < 6; ++j) {
      double a = (*stiff)[i, j];
      if (((i == 1) && (j == 1)) || ((i == 4) && (j == 4))) {
        EXPECT_EQ(a, k);
      } else if (((i == 1) && (j == 4)) || ((i == 4) && (j == 1))) {
        EXPECT_EQ(a, -k);
      } else {
        EXPECT_EQ(a, 0.0);
      }
    }
  }
}

TEST(ElementTest, AssembleZAllignedConstrained) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("Fake material", 1000.0, 0.3);
  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("square", material);
  (*property)["area"] = 10.0;
  const double length = 100.0;
  const double k = (*property)["area"] * material->YoungsModulus() / length;
  const std::size_t n1 = 0;
  const std::size_t n2 = 1;
  cpe::model::Element element(property, n1, n2);
  cpe::model::NodeList nodes;
  nodes.AddNode(n1, 0.0, 0.0, 0.0);
  nodes.AddNode(n2, 0.0, 0.0, length);
  nodes[0].constrained_dofs_ = cpe::model::dof::kAll;
  nodes[1].constrained_dofs_ = static_cast<cpe::model::dof::Dof>(
      cpe::model::dof::kAllRot | cpe::model::dof::kX | cpe::model::dof::kY);
  nodes[1].active_dof_index_[2] = 0;

  std::shared_ptr<cpe::matrix::Matrix> stiff =
      std::make_unique<cpe::matrix::Matrix>(1, 1);
  element.Assemble(nodes, stiff);

  double a = (*stiff)[0, 0];
  EXPECT_EQ(a, k);
}

TEST(ElementTest, AssembleZAllignedUnconstrained) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("Fake material", 1000.0, 0.3);
  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("square", material);
  (*property)["area"] = 10.0;
  const double length = 100.0;
  const double k = (*property)["area"] * material->YoungsModulus() / length;
  const std::size_t n1 = 0;
  const std::size_t n2 = 1;
  cpe::model::Element element(property, n1, n2);
  cpe::model::NodeList nodes;
  nodes.AddNode(n1, 0.0, 0.0, 0.0);
  nodes.AddNode(n2, 0.0, 0.0, length);
  nodes[0].active_dof_index_[0] = 0;
  nodes[0].active_dof_index_[1] = 1;
  nodes[0].active_dof_index_[2] = 2;
  nodes[1].active_dof_index_[0] = 3;
  nodes[1].active_dof_index_[1] = 4;
  nodes[1].active_dof_index_[2] = 5;

  std::shared_ptr<cpe::matrix::Matrix> stiff =
      std::make_unique<cpe::matrix::Matrix>(6, 6);
  element.Assemble(nodes, stiff);

  for (std::size_t i = 0; i < 6; ++i) {
    for (std::size_t j = 0; j < 6; ++j) {
      double a = (*stiff)[i, j];
      if (((i == 2) && (j == 2)) || ((i == 5) && (j == 5))) {
        EXPECT_EQ(a, k);
      } else if (((i == 2) && (j == 5)) || ((i == 5) && (j == 2))) {
        EXPECT_EQ(a, -k);
      } else {
        EXPECT_EQ(a, 0.0);
      }
    }
  }
}

TEST(ElementTest, AssembleArbitraryUnconstrained) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("Fake material", 1000.0, 0.3);
  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("square", material);
  (*property)["area"] = 10.0;
  const double length = 100.0;
  const double p = std::sqrt(length * length / 3.0);
  const double k = (*property)["area"] * material->YoungsModulus() / length;
  const std::size_t n1 = 0;
  const std::size_t n2 = 1;
  cpe::model::Element element(property, n1, n2);
  cpe::model::NodeList nodes;
  nodes.AddNode(n1, 0.0, 0.0, 0.0);
  nodes.AddNode(n2, p, p, p);
  nodes[0].active_dof_index_[0] = 0;
  nodes[0].active_dof_index_[1] = 1;
  nodes[0].active_dof_index_[2] = 2;
  nodes[1].active_dof_index_[0] = 3;
  nodes[1].active_dof_index_[1] = 4;
  nodes[1].active_dof_index_[2] = 5;

  std::shared_ptr<cpe::matrix::Matrix> stiff =
      std::make_unique<cpe::matrix::Matrix>(6, 6);
  element.Assemble(nodes, stiff);

  for (std::size_t i = 0; i < 6; ++i) {
    for (std::size_t j = 0; j < 6; ++j) {
      double a = (*stiff)[i, j];
      if (((i < 3) && (j < 3)) || ((i >= 3) && (j >= 3))) {
        EXPECT_NEAR(a, k / 3.0, 0.0001);
      } else {
        EXPECT_NEAR(a, -k / 3.0, 0.0001);
      }
    }
  }
}

}  // namespace
