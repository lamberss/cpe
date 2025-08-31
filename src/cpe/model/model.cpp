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
#include <cpe/model/model.hpp>

namespace cpe::model {

void Model::add_constraint(dof::Dof dof) {
  for (std::size_t i = 0; i < nodes.size(); ++i) add_constraint(dof, i);
}

void Model::add_constraint(dof::Dof dof, std::size_t i) {
  if (constraints.count(i) == 0) constraints[i] = dof::NONE;
  constraints[i] = static_cast<dof::Dof>(constraints[i] | dof);
}

void Model::add_constraint(dof::Dof dof, const std::vector<std::size_t>& is) {
  for (std::size_t i : is) add_constraint(dof, i);
}

}  // namespace cpe::model
