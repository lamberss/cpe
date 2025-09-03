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

#include <cpe/model/dof.hpp>
#include <cpe/model/elementblock.hpp>
#include <cpe/model/nodelist.hpp>
#include <memory>
#include <vector>

namespace cpe::model {

class Model {
 public:
  Model();

  void AddConstraint(dof::Dof dof, double v);
  void AddConstraint(dof::Dof dof, double v, std::size_t i);
  void AddConstraint(dof::Dof dof, double v,
                     const std::vector<std::size_t>& is);

  void AddForce(dof::Dof dof, double v);
  void AddForce(dof::Dof dof, double v, std::size_t i);
  void AddForce(dof::Dof dof, double v, const std::vector<std::size_t>& is);

  std::size_t GetNumElements() const;
  std::size_t GetNumNodes() const { return nodes.GetNumNodes(); }

  std::vector<std::shared_ptr<ElementBlockBase> > blocks;
  std::map<std::size_t, dof::Dof> constraints;
  std::vector<double> global_dof;
  std::vector<double> global_force;
  NodeList nodes;

 private:
  void AssignGlobalDofIndices();
  bool global_dof_indices_assigned_;
};

}  // namespace cpe::model
