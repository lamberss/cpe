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

Model::Model() : global_dof_indices_assigned_(false) {};

void Model::AddConstraint(dof::Dof dof, double v) {
  for (std::size_t i = 0; i < nodes.size(); ++i) AddConstraint(dof, v, i);
}

void Model::AddConstraint(dof::Dof dof, double v, std::size_t i) {
  AssignGlobalDofIndices();
  if (constraints.count(i) == 0) constraints[i] = dof::kNone;
  constraints[i] = static_cast<dof::Dof>(constraints[i] | dof);
  if (dof & dof::kX) global_dof[nodes[i].global_dof_index[dof::kIx]] = v;
  if (dof & dof::kY) global_dof[nodes[i].global_dof_index[dof::kIy]] = v;
  if (dof & dof::kZ) global_dof[nodes[i].global_dof_index[dof::kIz]] = v;
  if (dof & dof::kDx) global_dof[nodes[i].global_dof_index[dof::kIdx]] = v;
  if (dof & dof::kDy) global_dof[nodes[i].global_dof_index[dof::kIdy]] = v;
  if (dof & dof::kDz) global_dof[nodes[i].global_dof_index[dof::kIdz]] = v;
}

void Model::AddConstraint(dof::Dof dof, double v,
                          const std::vector<std::size_t>& is) {
  for (std::size_t i : is) AddConstraint(dof, v, i);
}

void Model::AddForce(dof::Dof dof, double v) {
  for (std::size_t i = 0; i < nodes.size(); ++i) AddForce(dof, v, i);
}

void Model::AddForce(dof::Dof dof, double v, std::size_t i) {
  AssignGlobalDofIndices();
  if (dof & dof::kX) global_force[nodes[i].global_dof_index[dof::kIx]] = v;
  if (dof & dof::kY) global_force[nodes[i].global_dof_index[dof::kIy]] = v;
  if (dof & dof::kZ) global_force[nodes[i].global_dof_index[dof::kIz]] = v;
  if (dof & dof::kDx) global_force[nodes[i].global_dof_index[dof::kIdx]] = v;
  if (dof & dof::kDy) global_force[nodes[i].global_dof_index[dof::kIdy]] = v;
  if (dof & dof::kDz) global_force[nodes[i].global_dof_index[dof::kIdz]] = v;
}

void Model::AddForce(dof::Dof dof, double v,
                     const std::vector<std::size_t>& is) {
  for (std::size_t i : is) AddForce(dof, v, i);
}

std::size_t Model::GetNumElements() const {
  std::size_t result = 0;
  for (std::size_t i = 0; i < blocks.size(); ++i) {
    result += blocks[i]->GetNumElements();
  }
  return result;
}

void Model::AssignGlobalDofIndices() {
  if (global_dof_indices_assigned_) return;

  std::size_t global_dof_count = 0;
  for (std::size_t i = 0; i < nodes.size(); ++i) {
    for (std::size_t j = 0; j < cpe::model::dof::kNumStrucDof; ++j) {
      nodes[i].global_dof_index[j] = global_dof_count++;
    }
  }
  global_dof.resize(global_dof_count, 0.0);
  global_force.resize(global_dof_count, 0.0);

  global_dof_indices_assigned_ = true;
}

}  // namespace cpe::model
