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

void writeVTU_cells(std::ostream& os, const cpe::model::Model& model,
                    int level = 0, int indent = 2) {
  std::string pre(indent * level, ' ');
  std::string ind(indent, ' ');

  std::vector<std::size_t> offsets(model.get_number_of_elements(), 0);
  std::vector<std::uint8_t> types(model.get_number_of_elements(), 0);

  os << pre << "<Cells>\n";

  os << pre << ind
     << "<DataArray type=\"Int64\" Name=\"connectivity\" format=\"ascii\">\n";
  for (std::size_t i = 0; i < model.blocks.size(); ++i) {
    auto& block = *(model.blocks[i]);
    for (std::size_t j = 0; j < block.size(); ++j) {
      os << pre << ind;
      for (unsigned int k = 0; k < block[j].size(); ++k) {
        os << ind << block[j][block[j].vtk_order[k]];
      }
      os << "\n";
      if (j >= 1) offsets[j] = offsets[j - 1];
      offsets[j] += block[j].size();
      types[j] = block[j].vtk_type;
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

void writeVTU_points(std::ostream& os, const cpe::model::Model& model,
                     int level = 0, int indent = 2) {
  std::string pre(indent * level, ' ');
  std::string ind(indent, ' ');

  os << std::scientific << std::setprecision(double_precision);
  os << pre << "<Points>\n";
  os << pre << ind
     << "<DataArray type=\"Float64\" NumberOfComponents=\"3\" "
        "format=\"ascii\">\n";

  for (std::size_t i = 0; i < model.get_number_of_nodes(); ++i) {
    os << pre << ind << ind << std::setw(double_width) << model.nodes[i].x
       << ind << std::setw(double_width) << model.nodes[i].y << ind
       << std::setw(double_width) << model.nodes[i].z << "\n";
  }

  os << pre << ind << "</DataArray>\n";
  os << pre << "</Points>\n";
}

void writeVTU(const std::string& outputfile, const cpe::model::Model& model) {
  std::ofstream outputstream(outputfile);
  writeVTU(outputstream, model);
  outputstream.close();
}

void writeVTU(std::ostream& os, const cpe::model::Model& model) {
  os << std::scientific
     << std::setprecision(std::numeric_limits<double>::max_digits10);
  os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  os << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\">\n";
  os << "  <UnstructuredGrid>\n";
  os << "    <Piece NumberOfPoints=\"" << model.get_number_of_nodes()
     << "\" NumberOfCells=\"" << model.get_number_of_elements() << "\">\n";

  const int indent_level = 3;
  writeVTU_points(os, model, indent_level);
  writeVTU_cells(os, model, indent_level);

  os << "    </Piece>\n";
  os << "  </UnstructuredGrid>\n";
  os << "</VTKFile>\n";
}

}  // namespace cpe::io::vtk
