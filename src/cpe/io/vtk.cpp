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
#include <cpe/io/vtk.hpp>
#include <iomanip>
#include <limits>

namespace cpe::io::vtk {

static unsigned int double_precision =
    std::numeric_limits<double>::max_digits10;
static unsigned int double_width = 24;

void WriteVtuCells(std::ostream& os, const cpe::model::Model& model,
                   int level = 0, int indent = 2) {
  std::string pre(indent * level, ' ');
  std::string ind(indent, ' ');

  std::vector<std::size_t> offsets(model.GetNumElements(), 0);
  std::vector<std::uint8_t> types(model.GetNumElements(), 0);

  os << pre << "<Cells>\n";

  os << pre << ind
     << "<DataArray type=\"Int64\" Name=\"connectivity\" format=\"ascii\">\n";
  for (std::size_t i = 0; i < model.blocks_.size(); ++i) {
    auto& block = *(model.blocks_[i]);
    for (std::size_t j = 0; j < block.GetNumElements(); ++j) {
      os << pre << ind;
      for (unsigned int k = 0; k < block[j].GetNumNodes(); ++k) {
        std::size_t elem_node_index = block[j].kVtkOrder[k];
        std::size_t node_id = block[j][elem_node_index];
        os << ind << model.nodes_.GetNodeIndex(node_id);
      }
      os << "\n";
      if (j >= 1) offsets[j] = offsets[j - 1];
      offsets[j] += block[j].GetNumNodes();
      types[j] = block[j].kVtkType;
    }
  }
  os << pre << ind << "</DataArray>\n";

  os << pre << ind
     << "<DataArray type=\"Int64\" Name=\"offsets\" format=\"ascii\">\n";
  for (std::size_t i : offsets) os << pre << ind << ind << i << "\n";
  os << pre << ind << "</DataArray>\n";

  os << pre << ind
     << "<DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n";
  for (std::uint8_t i : types)
    os << pre << ind << ind << std::to_string(i) << "\n";
  os << pre << ind << "</DataArray>\n";

  os << pre << "</Cells>\n";
}

void WriteVtuPointData(std::ostream& os, const cpe::model::Model& model,
                       int level = 0, int indent = 2) {
  std::string pre(indent * level, ' ');
  std::string ind(indent, ' ');

  os << std::scientific << std::setprecision(double_precision);
  os << pre << "<PointData>\n";

  auto& gdof = *(model.global_dof_);
  os << pre << ind
     << "<DataArray Name=\"Displacement\" type=\"Float64\" NumberOfComponents=\"3\" "
        "format=\"ascii\">\n";
  for (std::size_t i = 0; i < model.GetNumNodes(); ++i) {
    auto& dofs = model.nodes_[i].global_dof_index_;
    std::size_t ix = dofs[cpe::model::dof::kIx];
    std::size_t iy = dofs[cpe::model::dof::kIy];
    std::size_t iz = dofs[cpe::model::dof::kIz];
    os << pre << ind << ind << std::setw(double_width) << gdof[ix]
       << ind << std::setw(double_width) << gdof[iy] << ind
       << std::setw(double_width) << gdof[iz] << "\n";
  }
  os << pre << ind << "</DataArray>\n";

  os << pre << ind
     << "<DataArray Name=\"Rotation\" type=\"Float64\" NumberOfComponents=\"3\" "
        "format=\"ascii\">\n";
  for (std::size_t i = 0; i < model.GetNumNodes(); ++i) {
    auto& dofs = model.nodes_[i].global_dof_index_;
    std::size_t idx = dofs[cpe::model::dof::kIdx];
    std::size_t idy = dofs[cpe::model::dof::kIdy];
    std::size_t idz = dofs[cpe::model::dof::kIdz];
    os << pre << ind << ind << std::setw(double_width) << gdof[idx]
       << ind << std::setw(double_width) << gdof[idy] << ind
       << std::setw(double_width) << gdof[idz] << "\n";
  }
  os << pre << ind << "</DataArray>\n";

  auto& aforce = *(model.applied_force_);
  os << pre << ind
     << "<DataArray Name=\"Applied Force\" type=\"Float64\" NumberOfComponents=\"3\" "
        "format=\"ascii\">\n";
  for (std::size_t i = 0; i < model.GetNumNodes(); ++i) {
    auto& dofs = model.nodes_[i].global_dof_index_;
    std::size_t ix = dofs[cpe::model::dof::kIx];
    std::size_t iy = dofs[cpe::model::dof::kIy];
    std::size_t iz = dofs[cpe::model::dof::kIz];
    os << pre << ind << ind << std::setw(double_width) << aforce[ix]
       << ind << std::setw(double_width) << aforce[iy] << ind
       << std::setw(double_width) << aforce[iz] << "\n";
  }
  os << pre << ind << "</DataArray>\n";

  os << pre << ind
     << "<DataArray Name=\"Applied Moment\" type=\"Float64\" NumberOfComponents=\"3\" "
        "format=\"ascii\">\n";
  for (std::size_t i = 0; i < model.GetNumNodes(); ++i) {
    auto& dofs = model.nodes_[i].global_dof_index_;
    std::size_t idx = dofs[cpe::model::dof::kIdx];
    std::size_t idy = dofs[cpe::model::dof::kIdy];
    std::size_t idz = dofs[cpe::model::dof::kIdz];
    os << pre << ind << ind << std::setw(double_width) << aforce[idx]
       << ind << std::setw(double_width) << aforce[idy] << ind
       << std::setw(double_width) << aforce[idz] << "\n";
  }
  os << pre << ind << "</DataArray>\n";

  os << pre << "</PointData>\n";
}

void WriteVtuPoints(std::ostream& os, const cpe::model::Model& model,
                    int level = 0, int indent = 2) {
  std::string pre(indent * level, ' ');
  std::string ind(indent, ' ');

  os << std::scientific << std::setprecision(double_precision);
  os << pre << "<Points>\n";
  os << pre << ind
     << "<DataArray type=\"Float64\" NumberOfComponents=\"3\" "
        "format=\"ascii\">\n";

  for (std::size_t i = 0; i < model.GetNumNodes(); ++i) {
    os << pre << ind << ind << std::setw(double_width) << model.nodes_[i].x_
       << ind << std::setw(double_width) << model.nodes_[i].y_ << ind
       << std::setw(double_width) << model.nodes_[i].z_ << "\n";
  }

  os << pre << ind << "</DataArray>\n";
  os << pre << "</Points>\n";
}

void WriteVtu(const std::string& outputfile, const cpe::model::Model& model) {
  std::ofstream outputstream(outputfile);
  WriteVtu(outputstream, model);
  outputstream.close();
}

void WriteVtu(std::ostream& os, const cpe::model::Model& model) {
  os << std::scientific
     << std::setprecision(std::numeric_limits<double>::max_digits10);
  os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  os << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\">\n";
  os << "  <UnstructuredGrid>\n";
  os << "    <Piece NumberOfPoints=\"" << model.GetNumNodes()
     << "\" NumberOfCells=\"" << model.GetNumElements() << "\">\n";

  const int indent_level = 3;
  WriteVtuPoints(os, model, indent_level);
  WriteVtuCells(os, model, indent_level);
  if (model.stiffness_matrix_)
    WriteVtuPointData(os, model, indent_level);

  os << "    </Piece>\n";
  os << "  </UnstructuredGrid>\n";
  os << "</VTKFile>\n";
}

}  // namespace cpe::io::vtk
