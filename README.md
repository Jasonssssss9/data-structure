# Data Structures in Modern C++

This repository provides modular and independent implementations of classic data structures using modern C++. Each data structure resides in its own subdirectory and is built as a standalone CMake project with clean structure, header separation, and unit testing via Catch2.

## Implemented Data Structures

The following data structures are or will be implemented in this repository:

- Sequential List
- Doubly Circular Linked List (with sentinel head node)
- Stack
- Queue
- Heap
- AVL Tree
- Hash Table

Each implementation focuses on clarity, correctness, and maintainability, using modern C++ features where appropriate.

Each subdirectory such as `seq_list` is a fully self-contained CMake project. The root repository does **not** contain any global CMake configuration.

## Development Environment

- **C++ Standard**: C++17 and above
- **Build System**: [CMake](https://cmake.org/) (version ≥ 3.19 required for presets)
- **Test Framework**: [Catch2](https://github.com/catchorg/Catch2), installed via [vcpkg](https://github.com/microsoft/vcpkg)

Typical Workflow per Project： 
~~~bash
# From within the project directory (e.g., seq_list/)
cmake --preset=default         # configure using preset
cmake --build --preset=default # build
ctest --preset=default         # run tests


