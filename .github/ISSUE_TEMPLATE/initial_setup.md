---
name: 🚀 Initial Project Setup
about: Initial tasks to kickstart the Raytracer project (Docs, Setup, TDD)
title: 'Setup: Initial Phase - Understanding, Documentation & TDD Bootstrap'
labels: 'setup, documentation, TDD, bootstrap, priority:high'
assignees: '' # Assign the whole team if needed

---

**Goal of this issue**

This issue tracks the essential first steps to get the project started on a solid foundation. It includes understanding the requirements, initial documentation of our approach, and setting up the development environment, including the groundwork for Test-Driven Development (TDD).

**Tasks to Complete ("Definition of Done")**

*   [ ] **Reading and Understanding:**
    *   [ ] Carefully read the provided documentation/brief for the Raytracer project.
    *   [ ] Ensure every team member has a common understanding of raytracing basics and project goals. Organize a discussion session if needed.

*   [ ] **Initial Documentation:**
    *   [ ] Create a document (`README.md` or a dedicated `DESIGN.md`) summarizing:
        *   Our understanding of the problem and expected deliverables.
        *   The planned high-level architecture (main modules, data flow).
        *   Technology choices (C++ language, compiler, potential libraries).
        *   The chosen coding standard (`clang-format`? Specific rules?).
        *   The chosen unit testing framework (Google Test, Catch2, other?).

*   [ ] **Technical Bootstrap (Structure & Build):**
    *   [ ] Create the basic project directory structure (e.g., `src/`, `include/`, `tests/`, `build/`, `scenes/`, `docs/`).
    *   [ ] Set up the build system (e.g., `Makefile` or `CMakeLists.txt`) capable of compiling a "hello world" or an empty structure.
    *   [ ] Configure the `.gitignore` file to exclude generated files, binaries, etc.

*   [ ] **TDD & CI Environment Setup:**
    *   [ ] Integrate the chosen unit testing framework into the build system.
    *   [ ] Write a very first, simple test (e.g., `ASSERT_EQ(1+1, 2);`) to prove that tests can be compiled and run. **This is the first "Red-Green" cycle!**
    *   [ ] (Optional but recommended) Set up a basic GitHub Actions workflow that:
        *   Compiles the project.
        *   Runs the test suite (even if it only contains the initial test).
    *   [ ] (Optional but recommended) Configure `clang-format` and/or `clang-tidy` with the chosen rules.

**Why is this important?**

*   Ensures shared understanding before coding begins.
*   Lays down solid technical foundations (build, tests).
*   Introduces the TDD discipline from the start.
*   Allows automating checks early (CI).

**Useful Resources (add as needed)**

*   Link to project brief/documentation: [Insert link here]
*   Test framework documentation: [Insert link here]
*   TDD tutorial: [Insert link here]

---