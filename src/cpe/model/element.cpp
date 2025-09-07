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
#include <cpe/model/element.hpp>

namespace cpe::model {

void Element::Assemble(const NodeList& nodes,
                       std::shared_ptr<cpe::matrix::Matrix> stiffness_matrix) {
  // TODO come up with a way of allocating these small matrices on the stack

  // Compute stiffness assuming element is along the x-axis
  const Node& n1 = nodes.GetNodeById(nodes_[0]);
  const Node& n2 = nodes.GetNodeById(nodes_[1]);
  const double length = n2.GetDistance(n1);
  const double area = (*property_)["area"];
  const double elastic_modulus = property_->material_->YoungsModulus();
  const double k = area * elastic_modulus / length;
  cpe::matrix::Matrix local_stiff(kNumNodes, kNumNodes);
  local_stiff[0, 0] = local_stiff[1, 1] = k;
  local_stiff[0, 1] = local_stiff[1, 0] = -k;

  // Rotate the stiffness into the correct orientation
  const std::size_t ix1 = dof::kIx;
  const std::size_t iy1 = dof::kIy;
  const std::size_t iz1 = dof::kIz;
  const std::size_t ix2 = ix1 + 3;
  const std::size_t iy2 = iy1 + 3;
  const std::size_t iz2 = iz1 + 3;
  const double l = (n2.x_ - n1.x_) / length;
  const double m = (n2.y_ - n1.y_) / length;
  const double n = (n2.z_ - n1.z_) / length;
  cpe::matrix::Matrix trans(kNumNodes, 3 * kNumNodes);
  trans[0, ix1] = trans[1, ix2] = l;
  trans[0, iy1] = trans[1, iy2] = m;
  trans[0, iz1] = trans[1, iz2] = n;
  cpe::matrix::Matrix stiff = trans.Transpose() * local_stiff * trans;

  // Add contribution to the assembled stiffness matrix
  std::array<std::size_t, 3 * kNumNodes> dof_index;
  dof_index[0] = n1.active_dof_index_[dof::kIx];
  dof_index[1] = n1.active_dof_index_[dof::kIy];
  dof_index[2] = n1.active_dof_index_[dof::kIz];
  dof_index[3] = n2.active_dof_index_[dof::kIx];
  dof_index[4] = n2.active_dof_index_[dof::kIy];
  dof_index[5] = n2.active_dof_index_[dof::kIz];
  std::array<bool, 3 * kNumNodes> is_dof_active;
  is_dof_active[0] = (n1.constrained_dofs_ & dof::kX) == 0;
  is_dof_active[1] = (n1.constrained_dofs_ & dof::kY) == 0;
  is_dof_active[2] = (n1.constrained_dofs_ & dof::kZ) == 0;
  is_dof_active[3] = (n2.constrained_dofs_ & dof::kX) == 0;
  is_dof_active[4] = (n2.constrained_dofs_ & dof::kY) == 0;
  is_dof_active[5] = (n2.constrained_dofs_ & dof::kZ) == 0;
  auto& global_stiff = *stiffness_matrix;
  for (std::size_t i = 0; i < 3 * kNumNodes; ++i) {
    if (!is_dof_active[i]) continue;
    for (std::size_t j = 0; j < 3 * kNumNodes; ++j) {
      if (!is_dof_active[j]) continue;
      std::size_t di = dof_index[i];
      std::size_t dj = dof_index[j];
      global_stiff[di, dj] += stiff[i, j];
    }
  }
}

}  // namespace cpe::model
