---
name: 🛠️ Setup TDD Environment
about: Configure the project for Test-Driven Development
title: 'Task: Set up Test-Driven Development (TDD) environment'
labels: 'TDD, setup, task, testing'
assignees: '' # Assign who will be responsible for this setup

---

**Objective**

This task covers setting up the necessary tools and structure to enable Test-Driven Development (TDD) within our Raytracer project. The goal is to have a working testing framework integrated into our build process, allowing us to write tests *before* or alongside implementation code.

**Tasks / Definition of Done**

*   [ ] **Choose a Testing Framework:**
    *   Decide on a C++ unit testing framework (e.g., [Google Test](https://github.com/google/googletest), [Catch2](https://github.com/catchorg/Catch2)). Document the choice here or in `DESIGN.md`.
    *   *Decision:* [Specify chosen framework here]

*   [ ] **Integrate Framework into Build System:**
    *   Modify the `Makefile` or `CMakeLists.txt` to correctly find, compile, and link the chosen testing framework.
    *   Ensure test code can be compiled separately or alongside the main application code.

*   [ ] **Establish Test Directory Structure:**
    *   Create a dedicated directory for test source files (e.g., `tests/`).
    *   Organize subdirectories within `tests/` if needed (e.g., `tests/math/`, `tests/parser/`).

*   [ ] **Create Build Target for Tests:**
    *   Add a specific target or command to the build system to compile and run all tests (e.g., `make test`, `make check`, or using `ctest`).

*   [ ] **Write Initial "Smoke" Test:**
    *   Create a very simple test file within the `tests/` directory (e.g., `tests/core/basic_assertions_test.cpp`).
    *   Write a minimal test case that uses an assertion from the chosen framework to verify the setup (e.g., `ASSERT_EQ(1 + 1, 2);` or `REQUIRE( 1 + 1 == 2 );`).

*   [ ] **Verify Test Execution:**
    *   Successfully compile the entire project, including the test code.
    *   Successfully run the tests using the designated build command (e.g., `make test`).
    *   Confirm that the initial smoke test passes.

*   [ ] **Update CI Workflow (Optional but Recommended):**
    *   If a CI workflow (GitHub Actions) exists, add a step to automatically execute the tests (`make test` or similar) on every push or pull request.

*   [ ] **Document Test Procedure:**
    *   Briefly document how to run the tests in the main `README.md` or a `CONTRIBUTING.md` file.

**Why is this important?**

*   Provides a safety net against regressions.
*   Encourages modular and testable code design.
*   Serves as living documentation for code components.
*   Enables the Red-Green-Refactor cycle of TDD.

**The TDD Cycle Reminder (Red-Green-Refactor)**

1.  **Red:** Write a *failing* test for the functionality you want to add.
2.  **Green:** Write the *simplest possible* production code to make the test pass.
3.  **Refactor:** Improve the production code (and potentially test code) while ensuring all tests still pass.

**Resources**

*   [Link to chosen framework documentation]
*   [Link to relevant TDD articles or tutorials, if desired]

---