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

#include <cpe/model/material.hpp>
#include <cpe/model/property.hpp>
#include <string>

namespace {

TEST(PropertyTest, Create) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("a", 1.0, 0.1);
  cpe::model::Property property("property", material);
  EXPECT_EQ(property.GetNumAttributes(), 0);
  EXPECT_EQ(property.material->name, material->name);
  EXPECT_EQ(property.name, "property");
}

TEST(PropertyTest, Accessor) {
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("a", 1.0, 0.1);
  cpe::model::Property property("property", material);
  std::string label("label");
  const double value = 5.0;
  property[label] = value;
  EXPECT_EQ(property.GetNumAttributes(), 1);
  EXPECT_EQ(property[label], value);
}

}  // namespace
