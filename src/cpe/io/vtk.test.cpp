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
#include <gtest/gtest.h>

#include <cpe/io/vtk.hpp>
#include <cpe/model/model.hpp>
#include <sstream>

namespace {

TEST(VTKTest, WriteVtu) {
  std::stringstream os;

  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("Aluminum", 70.0e9, 0.3);

  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("square", material);
  (*property)["area"] = 1.0;

  cpe::model::Model model;
  model.nodes.AddNode(0, -1.0, 0.0, 0.0);
  model.nodes.AddNode(1, 1.0, 0.0, 0.0);
  model.nodes.AddNode(2, 0.0, 1.0, 0.0);

  using element_block_t = cpe::model::ElementBlock<cpe::model::Element>;
  std::shared_ptr<element_block_t> block =
      std::make_shared<element_block_t>("truss", property, 8);
  model.blocks.push_back(block);
  block->AddElement(0, 1);
  block->AddElement(1, 2);
  block->AddElement(0, 2);

  std::string expected = R"(<?xml version="1.0" encoding="UTF-8"?>
<VTKFile type="UnstructuredGrid" version="0.1">
  <UnstructuredGrid>
    <Piece NumberOfPoints="3" NumberOfCells="3">
      <Points>
        <DataArray type="Float64" NumberOfComponents="3" format="ascii">
          -1.00000000000000000e+00   0.00000000000000000e+00   0.00000000000000000e+00
           1.00000000000000000e+00   0.00000000000000000e+00   0.00000000000000000e+00
           0.00000000000000000e+00   1.00000000000000000e+00   0.00000000000000000e+00
        </DataArray>
      </Points>
      <Cells>
        <DataArray type="Int64" Name="connectivity" format="ascii">
          0  1
          1  2
          0  2
        </DataArray>
        <DataArray type="Int64" Name="offsets" format="ascii">
          2
          4
          6
        </DataArray>
        <DataArray type="UInt8" Name="types" format="ascii">
          3
          3
          3
        </DataArray>
      </Cells>
    </Piece>
  </UnstructuredGrid>
</VTKFile>
)";

  cpe::io::vtk::WriteVtu(os, model);

  EXPECT_EQ(os.str(), expected);
}

}  // namespace
