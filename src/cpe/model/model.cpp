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

void Model::add_constraint(dof::Dof dof, double v) {
  for (std::size_t i = 0; i < nodes.size(); ++i) add_constraint(dof, v, i);
}

void Model::add_constraint(dof::Dof dof, double v, std::size_t i) {
  assign_global_dof_indices();
  if (constraints.count(i) == 0) constraints[i] = dof::NONE;
  constraints[i] = static_cast<dof::Dof>(constraints[i] | dof);
  if (dof & dof::X) global_dof[nodes[i].global_dof_index[dof::IX]] = v;
  if (dof & dof::Y) global_dof[nodes[i].global_dof_index[dof::IY]] = v;
  if (dof & dof::Z) global_dof[nodes[i].global_dof_index[dof::IZ]] = v;
  if (dof & dof::DX) global_dof[nodes[i].global_dof_index[dof::IDX]] = v;
  if (dof & dof::DY) global_dof[nodes[i].global_dof_index[dof::IDY]] = v;
  if (dof & dof::DZ) global_dof[nodes[i].global_dof_index[dof::IDZ]] = v;
}

void Model::add_constraint(dof::Dof dof, double v,
                           const std::vector<std::size_t>& is) {
  for (std::size_t i : is) add_constraint(dof, v, i);
}

void Model::add_force(dof::Dof dof, double v) {
  for (std::size_t i = 0; i < nodes.size(); ++i) add_force(dof, v, i);
}

void Model::add_force(dof::Dof dof, double v, std::size_t i) {
  assign_global_dof_indices();
  if (dof & dof::X) global_force[nodes[i].global_dof_index[dof::IX]] = v;
  if (dof & dof::Y) global_force[nodes[i].global_dof_index[dof::IY]] = v;
  if (dof & dof::Z) global_force[nodes[i].global_dof_index[dof::IZ]] = v;
  if (dof & dof::DX) global_force[nodes[i].global_dof_index[dof::IDX]] = v;
  if (dof & dof::DY) global_force[nodes[i].global_dof_index[dof::IDY]] = v;
  if (dof & dof::DZ) global_force[nodes[i].global_dof_index[dof::IDZ]] = v;
}

void Model::add_force(dof::Dof dof, double v,
                      const std::vector<std::size_t>& is) {
  for (std::size_t i : is) add_force(dof, v, i);
}

void Model::assign_global_dof_indices() {
  if (global_dof_indices_assigned_) return;

  std::size_t global_dof_count = 0;
  for (std::size_t i = 0; i < nodes.size(); ++i) {
    for (std::size_t j = 0; j < cpe::model::dof::NUM_STRUC_DOF; ++j) {
      nodes[i].global_dof_index[j] = global_dof_count++;
    }
  }
  global_dof.resize(global_dof_count, 0.0);
  global_force.resize(global_dof_count, 0.0);

  global_dof_indices_assigned_ = true;
}

}  // namespace cpe::model
