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
#include <cpe/model/elementblock.hpp>
#include <cpe/model/material.hpp>
#include <cpe/model/property.hpp>

namespace {

class TestElement : public cpe::model::Element {
 public:
  TestElement(std::shared_ptr<cpe::model::Property> property, std::size_t n1,
              std::size_t n2)
      : cpe::model::Element(property, n1, n2) {};
  void Assemble(const cpe::model::NodeList& nodes,
                std::shared_ptr<cpe::matrix::Matrix> stiffness_matrix) {
    const cpe::model::Node& n1 = nodes.GetNodeById(nodes_[0]);
    const cpe::model::Node& n2 = nodes.GetNodeById(nodes_[1]);
    std::array<std::size_t, cpe::model::dof::kNumStrucDof * kNumNodes>
        dof_index;
    dof_index[0] = n1.active_dof_index_[cpe::model::dof::kIx];
    dof_index[1] = n1.active_dof_index_[cpe::model::dof::kIy];
    dof_index[2] = n1.active_dof_index_[cpe::model::dof::kIz];
    dof_index[3] = n1.active_dof_index_[cpe::model::dof::kIdx];
    dof_index[4] = n1.active_dof_index_[cpe::model::dof::kIdy];
    dof_index[5] = n1.active_dof_index_[cpe::model::dof::kIdz];
    dof_index[6] = n2.active_dof_index_[cpe::model::dof::kIx];
    dof_index[7] = n2.active_dof_index_[cpe::model::dof::kIy];
    dof_index[8] = n2.active_dof_index_[cpe::model::dof::kIz];
    dof_index[9] = n2.active_dof_index_[cpe::model::dof::kIdx];
    dof_index[10] = n2.active_dof_index_[cpe::model::dof::kIdy];
    dof_index[11] = n2.active_dof_index_[cpe::model::dof::kIdz];
    auto& global_stiff = *stiffness_matrix;
    for (std::size_t i = 0; i < dof_index.size(); ++i) {
      for (std::size_t j = 0; j < dof_index.size(); ++j) {
        std::size_t di = dof_index[i];
        std::size_t dj = dof_index[j];
        global_stiff[di, dj] += 1.0;
      }
    }
  }
  static cpe::model::dof::Dof GetSupportedDof() {
    return cpe::model::dof::kAll;
  }
};

TEST(PropertyTest, Create) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("a", 1.0, 0.1);
  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("property", material);
  cpe::model::ElementBlock<cpe::model::Element> block("elements", property);
  EXPECT_EQ(block.GetNumElements(), 0);
  EXPECT_EQ(block.GetSupportedDof(), cpe::model::dof::kAllTrans);
  EXPECT_EQ(block.property_->name_, "property");
}

TEST(PropertyTest, AddAndAccess) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("a", 1.0, 0.1);
  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("property", material);
  cpe::model::ElementBlock<TestElement> block("elements", property, 1);
  EXPECT_EQ(block.Capacity(), 1);
  EXPECT_EQ(block.GetNumElements(), 0);
  const unsigned int a = 1;
  const unsigned int b = 2;
  const unsigned int c = 3;
  block.AddElement(a, b);
  block.AddElement(b, c);
  EXPECT_EQ(block.GetNumElements(), 2);
  EXPECT_EQ(block[0][0], a);
  EXPECT_EQ(block[0][1], b);
  EXPECT_EQ(block[1][0], b);
  EXPECT_EQ(block[1][1], c);
}

TEST(PropertyTest, Assemble) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("a", 1.0, 0.1);
  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("property", material);
  cpe::model::ElementBlock<TestElement> block("elements", property, 1);
  EXPECT_EQ(block.Capacity(), 1);
  EXPECT_EQ(block.GetNumElements(), 0);
  const unsigned int a = 1;
  const unsigned int b = 2;
  const unsigned int c = 3;
  block.AddElement(a, b);
  block.AddElement(b, c);
  const std::size_t kNumNodes = 3;
  const std::size_t kNumDof = cpe::model::dof::kNumStrucDof;
  cpe::model::NodeList nodes(kNumNodes);
  nodes.AddNode(a, 0.0, 0.0, 0.0);
  nodes.AddNode(b, 1.0, 1.0, 1.0);
  nodes.AddNode(c, 2.0, 2.0, 2.0);
  std::size_t index = 0;
  for (std::size_t i = 0; i < kNumNodes; ++i) {
    for (std::size_t j = 0; j < kNumDof; ++j) {
      nodes[i].active_dof_index_[j] = index++;
    }
  }
  std::shared_ptr<cpe::matrix::Matrix> stiff =
      std::make_shared<cpe::matrix::Matrix>(kNumNodes * kNumDof,
                                            kNumNodes * kNumDof);
  block.Assemble(nodes, stiff);
}

}  // namespace
