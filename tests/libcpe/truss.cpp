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
  model.nodes_.AddNode(0, 0.0, 0.0);
  model.nodes_.AddNode(1, 0.0, H);
  model.nodes_.AddNode(2, L, 0.0);
  model.nodes_.AddNode(3, L, H);
  model.nodes_.AddNode(4, 2.0 * L, 0.0);
  model.nodes_.AddNode(5, 2.0 * L, H);

  // Define elements
  using ElementBlock = cpe::model::ElementBlock<cpe::model::Element>;
  std::shared_ptr<ElementBlock> block =
      std::make_shared<ElementBlock>("truss", property, 8);
  model.blocks_.push_back(block);
  block->AddElement(0, 2);
  block->AddElement(0, 3);
  block->AddElement(1, 3);
  block->AddElement(2, 3);
  block->AddElement(2, 4);
  block->AddElement(3, 4);
  block->AddElement(3, 5);
  block->AddElement(4, 5);

  // Define boundary conditions
  model.AddConstraint(cpe::model::dof::kAll, 0.0, {0, 1});
  model.AddConstraint(cpe::model::dof::kAllNon2d, 0.0);

  // Define loads
  model.AddForce(cpe::model::dof::kY, -P, 5);

  // Assemble the model
  model.Assemble();

  // Compute the solution
  model.Solve();

  // Output the results
  const std::string filename("truss.vtu");
  cpe::io::vtk::WriteVtu(filename, model);
  if (std::filesystem::exists(filename)) std::filesystem::remove(filename);
}

}  // namespace
