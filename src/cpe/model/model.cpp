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
  const std::array<dof::Dof, dof::kNumStrucDof> kDofs{
      dof::kX, dof::kY, dof::kZ, dof::kDx, dof::kDy, dof::kDz};
  AssignGlobalDofIndices();
  if (constraints_.count(i) == 0) constraints_[i] = dof::kNone;
  constraints_[i] = static_cast<dof::Dof>(constraints_[i] | dof);
  Node& node = nodes_.GetNodeById(i);
  auto& global_dof = (*global_dof_);
  for (std::size_t i = 0; i < kDofs.size(); ++i) {
    if (dof & kDofs[i]) {
      global_dof[node.global_dof_index_[i]] = v;
      global_dof_constrained_[node.global_dof_index_[i]] = true;
    }
  }
}

void Model::AddConstraint(dof::Dof dof, double v,
                          const std::vector<std::size_t>& is) {
  for (std::size_t i : is) AddConstraint(dof, v, i);
}

void Model::AddForce(dof::Dof dof, double v) {
  for (std::size_t i = 0; i < nodes_.GetNumNodes(); ++i) AddForce(dof, v, i);
}

void Model::AddForce(dof::Dof dof, double v, std::size_t i) {
  const std::array<dof::Dof, dof::kNumStrucDof> kDofs{
      dof::kX, dof::kY, dof::kZ, dof::kDx, dof::kDy, dof::kDz};
  AssignGlobalDofIndices();
  Node& node = nodes_.GetNodeById(i);
  auto& global_force = (*global_force_);
  for (std::size_t i = 0; i < kDofs.size(); ++i) {
    if (dof & kDofs[i]) global_force[node.global_dof_index_[i]] = v;
  }
}

void Model::AddForce(dof::Dof dof, double v,
                     const std::vector<std::size_t>& is) {
  for (std::size_t i : is) AddForce(dof, v, i);
}

void Model::Assemble() {
  const std::array<dof::Dof, dof::kNumStrucDof> kDofs{
      dof::kX, dof::kY, dof::kZ, dof::kDx, dof::kDy, dof::kDz};

  // Add constraints for any dof not supported by any elements in the model
  dof::Dof supported_dof = dof::kNone;
  for (std::size_t i = 0; i < blocks_.size(); ++i) {
    supported_dof =
        static_cast<dof::Dof>(supported_dof | blocks_[i]->GetSupportedDof());
  }
  for (std::size_t i = 0; i < dof::kNumStrucDof; ++i) {
    if ((supported_dof & kDofs[i]) == 0) {
      AddConstraint(kDofs[i], 0.0);
    }
  }

  // Assemble the stiffness matrix
  stiffness_matrix_ = std::make_shared<cpe::matrix::Matrix>(
      global_dof_->GetNumRows(), global_dof_->GetNumRows());
  for (std::size_t i = 0; i < blocks_.size(); ++i)
    blocks_[i]->Assemble(nodes_, stiffness_matrix_);

  // Modify system to enforce constraints
  auto& stiff = *stiffness_matrix_;
  auto& dof = *global_dof_;
  auto& force = *global_force_;
  for (std::size_t i = 0; i < dof.GetNumRows(); ++i) {
    if (global_dof_constrained_[i]) {
      for (std::size_t j = 0; j < dof.GetNumRows(); ++j) {
        if (i == j) {
          force[i] = dof[i];
          stiff[i, i] = 1.0;
        } else {
          force[i] -= stiff[j, i] * dof[i];
          stiff[j, i] = 0.0;
          stiff[i, j] = 0.0;
        }
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

int Model::Solve() {
  int result = cpe::linearsolver::ssor::Solve(*stiffness_matrix_, *global_dof_,
                                              *global_force_, 1.0e-10, 1.5);
  return result;
}

void Model::AssignGlobalDofIndices() {
  if (global_dof_indices_assigned_) return;

  std::size_t global_dof_count = 0;
  for (std::size_t i = 0; i < nodes_.GetNumNodes(); ++i) {
    for (std::size_t j = 0; j < dof::kNumStrucDof; ++j) {
      nodes_.GetNodeById(i).global_dof_index_[j] = global_dof_count++;
    }
  }

  global_dof_ = std::make_shared<cpe::matrix::Matrix>(global_dof_count, 1);
  global_force_ = std::make_shared<cpe::matrix::Matrix>(global_dof_count, 1);
  global_dof_constrained_.resize(global_dof_count, false);

  global_dof_indices_assigned_ = true;
}

}  // namespace cpe::model
