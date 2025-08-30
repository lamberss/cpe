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

#include <cpe/model/elementblock.hpp>
#include <cpe/model/material.hpp>
#include <cpe/model/property.hpp>

namespace {

struct TestElement {
  double a;
  double b;

  TestElement(const double& aa, const double& bb) : a(aa), b(bb) {};
};

TEST(PropertyTest, Create) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("a", 1.0, 0.1);
  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("property", material);
  cpe::model::ElementBlock<TestElement> block("elements", property);
  EXPECT_EQ(block.size(), 0);
  EXPECT_EQ(block.property->name, "property");
}

TEST(PropertyTest, AddAndAccess) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("a", 1.0, 0.1);
  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("property", material);
  cpe::model::ElementBlock<TestElement> block("elements", property, 1);
  EXPECT_EQ(block.capacity(), 1);
  EXPECT_EQ(block.size(), 0);
  const double a = 1.0;
  const double b = 2.0;
  block.emplace_back(a, b);
  EXPECT_EQ(block.size(), 1);
  EXPECT_EQ(block[0].a, a);
  EXPECT_EQ(block[0].b, b);
}

}  // namespace
