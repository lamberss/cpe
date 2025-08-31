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

#include <cpe/model/nodelist.hpp>
#include <stdexcept>

namespace {

TEST(NodeListTest, Create) {
  cpe::model::NodeList node_list;
  EXPECT_EQ(node_list.size(), 0);
}

TEST(NodeListTest, Add) {
  const std::size_t n = 5;
  cpe::model::NodeList node_list;
  EXPECT_EQ(node_list.size(), 0);
  for (std::size_t i = 0; i < n; i++) {
    node_list.add(i, 0.0);
  }
  EXPECT_EQ(node_list.size(), n);
}

TEST(NodeListTest, AddDuplicate) {
  cpe::model::NodeList node_list;
  EXPECT_EQ(node_list.size(), 0);
  node_list.add(0, 0.0);
  EXPECT_THROW(node_list.add(0, 0.0), std::runtime_error);
  EXPECT_EQ(node_list.size(), 1);
}

TEST(NodeListTest, Access) {
  const std::size_t id = 5280;
  const double x = 1.0;
  const double y = 2.0;
  const double z = -10.0;
  cpe::model::NodeList node_list;
  node_list.add(id, x, y, z);

  const cpe::model::Node& node1 = node_list[0];
  EXPECT_EQ(node1.x, x);
  EXPECT_EQ(node1.y, y);
  EXPECT_EQ(node1.z, z);

  const cpe::model::Node& node2 = node_list.getById(id);
  EXPECT_EQ(node2.x, x);
  EXPECT_EQ(node2.y, y);
  EXPECT_EQ(node2.z, z);
}

}  // namespace
