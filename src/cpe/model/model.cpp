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
#include <cpe/linearsolver/ssor.hpp>
#include <cpe/model/model.hpp>

namespace cpe::model {

Model::Model() : global_dof_indices_assigned_(false) {};

void Model::AddConstraint(dof::Dof dof, double v) {
  for (std::size_t i = 0; i < nodes_.GetNumNodes(); ++i) {
    AddConstraint(dof, v, i);
  }
}

void Model::AddConstraint(dof::Dof dof, double v, std::size_t i) {
  AssignGlobalDofIndices();
  if (constraints_.count(i) == 0) constraints_[i] = dof::kNone;
  constraints_[i] = static_cast<dof::Dof>(constraints_[i] | dof);
  Node& node = nodes_.GetNodeById(i);
  node.constrained_dofs_ = static_cast<dof::Dof>(node.constrained_dofs_ | dof);
  if (dof & dof::kX) global_dof_[node.global_dof_index_[dof::kIx]] = v;
  if (dof & dof::kY) global_dof_[node.global_dof_index_[dof::kIy]] = v;
  if (dof & dof::kZ) global_dof_[node.global_dof_index_[dof::kIz]] = v;
  if (dof & dof::kDx) global_dof_[node.global_dof_index_[dof::kIdx]] = v;
  if (dof & dof::kDy) global_dof_[node.global_dof_index_[dof::kIdy]] = v;
  if (dof & dof::kDz) global_dof_[node.global_dof_index_[dof::kIdz]] = v;
}

void Model::AddConstraint(dof::Dof dof, double v,
                          const std::vector<std::size_t>& is) {
  for (std::size_t i : is) AddConstraint(dof, v, i);
}

void Model::AddForce(dof::Dof dof, double v) {
  for (std::size_t i = 0; i < nodes_.GetNumNodes(); ++i) AddForce(dof, v, i);
}

void Model::AddForce(dof::Dof dof, double v, std::size_t i) {
  AssignGlobalDofIndices();
  Node& node = nodes_.GetNodeById(i);
  if (dof & dof::kX) global_force_[node.global_dof_index_[dof::kIx]] = v;
  if (dof & dof::kY) global_force_[node.global_dof_index_[dof::kIy]] = v;
  if (dof & dof::kZ) global_force_[node.global_dof_index_[dof::kIz]] = v;
  if (dof & dof::kDx) global_force_[node.global_dof_index_[dof::kIdx]] = v;
  if (dof & dof::kDy) global_force_[node.global_dof_index_[dof::kIdy]] = v;
  if (dof & dof::kDz) global_force_[node.global_dof_index_[dof::kIdz]] = v;
}

void Model::AddForce(dof::Dof dof, double v,
                     const std::vector<std::size_t>& is) {
  for (std::size_t i : is) AddForce(dof, v, i);
}

void Model::Assemble() {
  const std::array<dof::Dof, dof::kNumStrucDof> dofs{
      dof::kX, dof::kY, dof::kZ, dof::kDx, dof::kDy, dof::kDz};

  // Add constraints for any dof not supported by any elements in the model
  dof::Dof supported_dof = dof::kNone;
  for (std::size_t i = 0; i < blocks_.size(); ++i) {
    supported_dof =
        static_cast<dof::Dof>(supported_dof | blocks_[i]->GetSupportedDof());
  }
  for (std::size_t i = 0; i < dof::kNumStrucDof; ++i) {
    if ((supported_dof & dofs[i]) == 0) {
      AddConstraint(dofs[i], 0.0);
    }
  }

  // Determine how many active dofs their are
  std::size_t num_active_dof = 0;
  for (std::size_t i = 0; i < nodes_.GetNumNodes(); ++i) {
    for (std::size_t j = 0; j < dof::kNumStrucDof; ++j) {
      if ((constraints_[i] & dofs[j]) == 0) {
        nodes_.GetNodeById(i).active_dof_index_[j] = num_active_dof++;
      }
    }
  }

  // Allocate storage for matrices
  active_dof_ = std::make_shared<cpe::matrix::Matrix>(num_active_dof, 1);
  active_force_ = std::make_shared<cpe::matrix::Matrix>(num_active_dof, 1);
  stiffness_matrix_ =
      std::make_shared<cpe::matrix::Matrix>(num_active_dof, num_active_dof);

  // Assemble the stiffness matrix
  for (std::size_t i = 0; i < blocks_.size(); ++i)
    blocks_[i]->Assemble(nodes_, stiffness_matrix_);

  // Populate the force vector
  for (std::size_t i = 0; i < nodes_.GetNumNodes(); ++i) {
    for (std::size_t j = 0; j < dof::kNumStrucDof; ++j) {
      if ((constraints_[i] & dofs[j]) == 0) {
        std::size_t ai = nodes_.GetNodeById(i).active_dof_index_[j];
        std::size_t gi = nodes_.GetNodeById(i).global_dof_index_[j];
        (*active_force_)[ai] = global_force_[gi];
      }
    }
  }
}

std::size_t Model::GetNumElements() const {
  std::size_t result = 0;
  for (std::size_t i = 0; i < blocks_.size(); ++i) {
    result += blocks_[i]->GetNumElements();
  }
  return result;
}

void Model::Solve() {
  std::ignore = cpe::linearsolver::ssor::Solve(*stiffness_matrix_, *active_dof_,
                                               *active_force_, 1.0e-10, 1.5);
}

void Model::AssignGlobalDofIndices() {
  if (global_dof_indices_assigned_) return;

  std::size_t global_dof_count = 0;
  for (std::size_t i = 0; i < nodes_.GetNumNodes(); ++i) {
    for (std::size_t j = 0; j < dof::kNumStrucDof; ++j) {
      nodes_.GetNodeById(i).global_dof_index_[j] = global_dof_count++;
    }
  }
  global_dof_.resize(global_dof_count, 0.0);
  global_force_.resize(global_dof_count, 0.0);

  global_dof_indices_assigned_ = true;
}

}  // namespace cpe::model
