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
#include <cpe/model/element.hpp>
#include <cpe/model/elementblock.hpp>
#include <cpe/model/material.hpp>
#include <cpe/model/model.hpp>
#include <cpe/model/property.hpp>
#include <filesystem>
#include <memory>

namespace {

TEST(TrussCpeSystemTest, Truss) {
  // Problem parameters
  const double b = 0.005;   // m
  const double H = 0.12;    // m
  const double L = 0.16;    // m
  const double P = 1.0;     // N
  const double E = 70.0e9;  // Pa
  const double nu = 0.32;

  // Define material
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("Aluminum", E, nu);

  // Define properties
  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("square", material);
  (*property)["area"] = b * b;

  // Create model
  cpe::model::Model model;

  // Define nodelist
  model.nodes.add(0, 0.0, 0.0);
  model.nodes.add(1, 0.0, H);
  model.nodes.add(2, L, 0.0);
  model.nodes.add(3, L, H);
  model.nodes.add(4, 2.0 * L, 0.0);
  model.nodes.add(5, 2.0 * L, H);

  // Define elements
  using element_block_t = cpe::model::ElementBlock<cpe::model::Element>;
  std::shared_ptr<element_block_t> block =
      std::make_shared<element_block_t>("truss", property, 8);
  model.blocks.push_back(block);
  block->emplace_back(0, 2);
  block->emplace_back(0, 3);
  block->emplace_back(1, 3);
  block->emplace_back(2, 3);
  block->emplace_back(2, 4);
  block->emplace_back(3, 4);
  block->emplace_back(3, 5);
  block->emplace_back(4, 5);

  // Define boundary conditions
  model.add_constraint(cpe::model::dof::ALL, 0.0, {0, 1});
  model.add_constraint(cpe::model::dof::ALL_NON2D, 0.0);

  // Define loads
  model.add_force(cpe::model::dof::Y, -P, 5);

  // Assemble the model

  // Compute the solution

  // Output the results
  const std::string filename("truss.vtu");
  cpe::io::vtk::writeVTU(filename, model);
  if (std::filesystem::exists(filename)) std::filesystem::remove(filename);
}

}  // namespace
