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

#include <cpe/model/property.hpp>
#include <string>
#include <utility>
#include <vector>

namespace cpe::model {

template <typename T>
class ElementBlock {
 public:
  ElementBlock() = delete;
  ElementBlock(const ElementBlock&) = delete;
  ElementBlock(ElementBlock&&) = delete;
  ElementBlock& operator=(const ElementBlock&) = delete;
  ElementBlock& operator=(ElementBlock&&) = delete;

  ElementBlock(const std::string& n, std::shared_ptr<Property> p,
               std::size_t r = 0)
      : name(n), property(p) {
    reserve(r);
  }

  const std::string name;
  const std::shared_ptr<Property> property;

  std::size_t capacity() { return elements_.capacity(); }
  template <class... Args>
  void emplace_back(Args&&... args) {
    elements_.emplace_back(std::forward<Args>(args)...);
  }
  T& operator[](std::size_t i) { return elements_[i]; }
  void reserve(std::size_t c) { elements_.reserve(c); }
  std::size_t size() { return elements_.size(); }

 private:
  std::vector<T> elements_;
};

}  // namespace cpe::model
