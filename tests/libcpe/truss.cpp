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

#include <cmath>
#include <cpe/io/vtk.hpp>
#include <cpe/model/element.hpp>
#include <cpe/model/elementblock.hpp>
#include <cpe/model/material.hpp>
#include <cpe/model/model.hpp>
#include <cpe/model/property.hpp>
#include <filesystem>
#include <memory>

namespace {

double cos(double angle_in_degrees) {
  const double kPi = std::acos(-1.0);
  double angle_in_radians = angle_in_degrees * kPi / 180.0;
  return std::cos(angle_in_radians);
}

double sin(double angle_in_degrees) {
  const double kPi = std::acos(-1.0);
  double angle_in_radians = angle_in_degrees * kPi / 180.0;
  return std::sin(angle_in_radians);
}

TEST(TrussCpeSystemTest, Truss4Bar) {

  // Yang, T. Y., Finite Element Structural Analysis, Prentice-Hall, Inc.,
  //     Englewood Cliffs, N.J., 1986, pp. 91-92
  // Example 4.1

  // Problem parameters
  const double A = 1.0;      // m**2
  const double L = 1.0;      // m
  const double P = 1.0;      // N
  const double theta = 60.0; // deg
  const double E = 70.0e9;   // Pa
  const double nu = 0.32;

  // Define material
  std::shared_ptr<cpe::model::Material> material =
      std::make_shared<cpe::model::Material>("Aluminum", E, nu);

  // Define properties
  std::shared_ptr<cpe::model::Property> property =
      std::make_shared<cpe::model::Property>("square", material);
  (*property)["area"] = A;

  // Create model
  cpe::model::Model model;

  // Define nodelist
  model.nodes_.AddNode(1, 0.0, 0.0);
  model.nodes_.AddNode(2, -L, 0.0);
  model.nodes_.AddNode(3, -L*cos(theta), L*sin(theta));
  model.nodes_.AddNode(4, L, 0.0);
  model.nodes_.AddNode(5, L*cos(theta), -L*sin(theta));

  // Define elements
  using ElementBlock = cpe::model::ElementBlock<cpe::model::Element>;
  std::shared_ptr<ElementBlock> block =
      std::make_shared<ElementBlock>("truss", property, 4);
  model.blocks_.push_back(block);
  block->AddElement(1, 2);
  block->AddElement(1, 3);
  block->AddElement(1, 4);
  block->AddElement(1, 5);

  // Define boundary conditions
  model.AddConstraint(cpe::model::dof::kAllNon2d, 0.0);
  model.AddConstraint(cpe::model::dof::kAll, 0.0, {2, 3, 4, 5});

  // Define loads
  model.AddForce(cpe::model::dof::kX, -P*cos(theta), 1);
  model.AddForce(cpe::model::dof::kY, -P*sin(theta), 1);

  // Assemble the model
  model.Assemble();

  // Compute the solution
  int result = model.Solve();
  EXPECT_GT(result, 0);

  // Output the results
  const std::string filename("truss_4bar.vtu");
  cpe::io::vtk::WriteVtu(filename, model);
  if (std::filesystem::exists(filename)) std::filesystem::remove(filename);
}

TEST(TrussCpeSystemTest, Truss9Bar) {
  // Cook, R. D., Malkus, D. S., Plesha, M. E., Witt, R. J. Concepts and
  //     Applications of Finite Element Analysis, Forth Edition, John Wiley
  //     & Sons, Inc., New York, NY, 2002, pp. 75
  // Problem C2.4

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
  model.nodes_.AddNode(1, 0.0, 0.0);
  model.nodes_.AddNode(2, 0.0, H);
  model.nodes_.AddNode(3, L, 0.0);
  model.nodes_.AddNode(4, L, H);
  model.nodes_.AddNode(5, 2.0 * L, 0.0);
  model.nodes_.AddNode(6, 2.0 * L, H);

  // Define elements
  using ElementBlock = cpe::model::ElementBlock<cpe::model::Element>;
  std::shared_ptr<ElementBlock> block =
      std::make_shared<ElementBlock>("truss", property, 8);
  model.blocks_.push_back(block);
  block->AddElement(1, 3);
  block->AddElement(1, 4);
  block->AddElement(2, 4);
  block->AddElement(3, 4);
  block->AddElement(3, 5);
  block->AddElement(4, 5);
  block->AddElement(4, 6);
  block->AddElement(5, 6);

  // Define boundary conditions
  model.AddConstraint(cpe::model::dof::kAllNon2d, 0.0);
  model.AddConstraint(cpe::model::dof::kAll, 0.0, {1, 2});

  // Define loads
  model.AddForce(cpe::model::dof::kY, -P, 6);

  // Assemble the model
  model.Assemble();

  // Compute the solution
  int result = model.Solve();
  EXPECT_GT(result, 0);

  // Output the results
  const std::string filename("truss_9bar.vtu");
  cpe::io::vtk::WriteVtu(filename, model);
  if (std::filesystem::exists(filename)) std::filesystem::remove(filename);
}

}  // namespace
