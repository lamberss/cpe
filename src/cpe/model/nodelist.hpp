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
#ifndef CPE_MODEL_NODELIST_HPP
#define CPE_MODEL_NODELIST_HPP

#include <cstddef>
#include <map>
#include <vector>

namespace cpe::model {

struct Node {
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;
};

class NodeList {
 public:
  NodeList(std::size_t number = 0);

  void add(std::size_t id, double x, double y = 0.0, double z = 0.0);

  Node& getById(std::size_t id) { return node_list_[node_ids_[id]]; }
  Node& operator[](std::size_t i) { return node_list_[i]; }

  std::size_t capacity() { return node_list_.capacity(); }
  std::size_t size() { return node_list_.size(); }

 private:
  std::map<std::size_t, std::size_t> node_ids_;
  std::vector<Node> node_list_;
};

}  // namespace cpe::model

#endif  // CPE_MODEL_NODELIST_HPP
