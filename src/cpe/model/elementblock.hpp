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

#include <cpe/matrix/matrix.hpp>
#include <cpe/model/element.hpp>
#include <cpe/model/property.hpp>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace cpe::model {

class ElementBlockBase {
 public:
  virtual ~ElementBlockBase() = default;
  virtual void Assemble(const NodeList&,
                        std::shared_ptr<cpe::matrix::Matrix>) = 0;
  virtual std::size_t GetNumElements() const = 0;
  virtual dof::Dof GetSupportedDof() const { return dof::kAll; }
  virtual void Reserve(std::size_t) = 0;
  virtual Element& operator[](std::size_t) = 0;
};

template <typename T>
class ElementBlock : public ElementBlockBase {
 public:
  ElementBlock() = delete;
  ElementBlock(const ElementBlock&) = delete;
  ElementBlock(ElementBlock&&) = delete;
  ElementBlock& operator=(const ElementBlock&) = delete;
  ElementBlock& operator=(ElementBlock&&) = delete;

  ElementBlock(const std::string& n, std::shared_ptr<Property> p,
               std::size_t r = 0)
      : name_(n), property_(p) {
    Reserve(r);
  }

  template <class... Args>
  void AddElement(Args&&... args) {
    elements_.emplace_back(property_, std::forward<Args>(args)...);
  }
  void Assemble(const NodeList& nodes,
                std::shared_ptr<cpe::matrix::Matrix> stiffness_matrix) {
    for (std::size_t i = 0; i < GetNumElements(); ++i) {
      elements_[i].Assemble(nodes, stiffness_matrix);
    }
  }
  std::size_t Capacity() { return elements_.capacity(); }
  std::size_t GetNumElements() const { return elements_.size(); }
  dof::Dof GetSupportedDof() const { return T::GetSupportedDof(); }
  T& operator[](std::size_t i) { return elements_[i]; }
  void Reserve(std::size_t c) { elements_.reserve(c); }

  const std::string name_;
  const std::shared_ptr<Property> property_;

 private:
  std::vector<T> elements_;
};

}  // namespace cpe::model
