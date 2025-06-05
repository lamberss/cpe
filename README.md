<a name="top"></a>
# cpe - Computational Physics Environment

## Table of Contents
- [About](#about)
- [How to Build](#how-to-build)
- [License](#license)

<a name="about"></a>
## About

This project exists to be my playground for learning what is involved in
programming various computational physics codes.

By "computational physics code," I do not mean a code that would be suitable as
a physics engine for a game.  I mean a code that is capable of computing the
detailed, "accurate" physics of a body.  For example, a "computational
structural dynamics" (CSD) code would be able to take a meshed body and compute
how it would bend and deform over time.  Another example would be a
"computational fluid dynamics" (CFD) code that computes the pressure, temperature,
density, and velocity distributions around a body moving through a fluid.
In general these types of physics codes are too expensive to be used in
real-time applications like games or flight simulators.

My primary objective is to learn how the codes work.  Things like performance
and scalability to large problems are definitely lesser concerns at this point.

<a name="how-to-build"></a>
## How to Build

### Linux

To build on Linux, open a shell and run the following commands

```shell
# Clone the project
git clone https://github.com/lamberss/cpe.git

# Make a build directory and move into it
mkdir -p cpe/BUILD

# Configure the build (change build type and install prefix as desired)
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../INSTALL ..

# Build the application
cmake --build .

# Install the application
cmake --install .
```

<a name="license"></a>
## License

This product is distributed under the terms of the
[MIT License](https://opensource.org/license/mit).

[Back to top](#top)
