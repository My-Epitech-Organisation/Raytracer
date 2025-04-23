# Commit Convention for the Raytracer Project (Based on Conventional Commits)

This document describes the standard for writing commit messages in this project. We follow the [Conventional Commits](https://www.conventionalcommits.org/) specification.

**Why?**

*   **History Readability:** Allows quickly understanding the nature of changes.
*   **Automation:** Facilitates automatic changelog generation, version determination (semver), and integration with other tools.
*   **Communication:** Helps clearly communicate the intent of changes to other team members (and your future self!).

## General Format

Each commit message must follow this format:

```
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```

---

### 1. Type

The `type` must be one of the following. Choose the one that best describes the *primary* nature of your commit:

*   **feat:** A new feature for the user (e.g., adding sphere rendering).
*   **fix:** A bug fix (e.g., correcting an intersection calculation).
*   **docs:** Changes related only to documentation (e.g., adding comments, updating README).
*   **style:** Changes that do not affect the meaning of the code (whitespace, formatting, missing semi-colons, etc.). Often handled by `clang-format`.
*   **refactor:** A code change that neither fixes a bug nor adds a feature (e.g., renaming a variable, reorganizing code without changing external behavior).
*   **perf:** A code change that improves performance.
*   **test:** Adding missing tests or correcting existing tests. Does not include changes to the source code.
*   **build:** Changes that affect the build system or external dependencies (e.g., modifying Makefile, CMakeLists, adding libraries).
*   **ci:** Changes to our CI configuration files and scripts (e.g., configuring GitHub Actions).
*   **chore:** Other changes that don't modify source or test files (e.g., updating build tasks, tool configuration).

---

### 2. Scope (Optional)

The `scope` provides additional contextual information to the commit's type. It's often the name of the module or part of the codebase affected.

*   It must be enclosed in parentheses `()`.
*   Examples for our Raytracer project: `parser`, `math`, `render`, `vector`, `camera`, `scene`, `ci`, `tests`, `build`.

**Examples with scope:**

*   `feat(parser): Add parsing for camera configurations`
*   `fix(math): Correct division by zero in vector normalization`
*   `test(render): Add test for rendering an empty scene`

---

### 3. Description

*   Use the **imperative, present tense**: "add" not "added" or "adds".
*   Start with a lowercase letter.
*   No period (`.`) at the end.

**Examples:**

*   `fix: correct ambient color calculation`
*   `feat(scene): allow adding multiple light sources`

---

### 4. Body (Optional)

*   Use the body to explain the **what** and **why** of the change, if the description is not sufficient.
*   Separated from the description by a blank line.
*   Can contain multiple paragraphs.

**Example:**

```
feat(render): implement shadow casting

Adds the ability to calculate if an intersection point is in shadow
by casting a secondary ray towards each light source.

This greatly improves scene realism by preventing all surfaces
from being uniformly lit.
```

---

### 5. Footer(s) (Optional)

*   Separated from the body (or description if no body) by a blank line.
*   Used to reference Issues or indicate **Breaking Changes**.

*   **Referencing Issues:**
    *   Use keywords like `Refs #issue_number`, `Closes #issue_number`, `Fixes #issue_number`.
    *   `Closes` or `Fixes` will automatically close the issue on GitHub when merged into the main branch.
    *   Example: `Closes #42`, `Refs #15, Refs #20`

*   **Breaking Changes:**
    *   A change that breaks backward compatibility **MUST** be indicated.
    *   Either by appending a `!` after the `type(scope)`: `feat(parser)!: change scene file format`
    *   Or by adding a `BREAKING CHANGE:` block in the footer.
    *   The `BREAKING CHANGE:` block must clearly describe the break and how to migrate.

**Example with footer:**

```
refactor(vector): use double instead of float for precision

Previous calculations lacked precision for complex scenes,
leading to visual artifacts.

BREAKING CHANGE: The Vector3D class now uses `double`.
Any code directly using members or expecting `float`
will need to be updated to use `double`.

Refs #58
```

---

## Full Examples

*   **Simple fix:**
    ```
    fix: correct typo in error message
    ```
*   **Feature with scope:**
    ```
    feat(math): add dot product function
    ```
*   **Complex fix with body and issue reference:**
    ```
    fix(render): resolve artifacts on sphere edges

    The normal calculation was incorrect in certain edge cases,
    causing unexpected black areas. The algorithm has been
    revised to handle these cases.

    Closes #31
    ```
*   **Breaking change (via `!`):**
    ```
    refactor(parser)!: switch from JSON to a custom binary format

    JSON parsing was too slow for large scenes.

    BREAKING CHANGE: The scene file format has changed. Old
    .json files are no longer supported. A conversion tool
    will be provided separately.
    ```
*   **Breaking change (via footer):**
    ```
    feat(camera): introduce new coordinate system

    The previous system was ambiguous. The new system follows
    standard graphics industry conventions.

    BREAKING CHANGE: Camera configuration parameters
    (position, direction, up-vector) have changed meaning.
    Scene files must be updated. See documentation for details.
    ```

---

By following this convention, we will maintain a clean, understandable, and useful Git history! Feel free to ask questions if anything is unclear.