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
#pragma once

#include <cpe/model/node.hpp>
#include <map>
#include <vector>

namespace cpe::model {

class NodeList {
 public:
  NodeList(std::size_t n = 0);

  auto begin() const { return node_ids_.begin(); }
  auto end() const { return node_ids_.end(); }

  void AddNode(std::size_t id, double x, double y = 0.0, double z = 0.0);

  Node& GetNodeById(std::size_t id) { return nodes_[GetNodeIndex(id)]; }
  const Node& GetNodeById(std::size_t id) const {
    return nodes_[node_ids_.at(id)];
  }
  std::size_t GetNodeIndex(const std::size_t& id) const { return node_ids_.at(id); }

  std::size_t GetNumNodes() { return nodes_.size(); }
  std::size_t GetNumNodes() const { return nodes_.size(); }

  Node& operator[](std::size_t i) { return nodes_[i]; }
  const Node& operator[](std::size_t i) const { return nodes_[i]; }

  void Reserve(std::size_t n) { nodes_.reserve(n); }

 private:
  std::map<std::size_t, std::size_t> node_ids_;
  std::vector<Node> nodes_;
};

}  // namespace cpe::model
