# 🎯 Raytracer – B-OOP-400

**RAYTRACER: YOUR CPU GOES BRRRRR!**  
Projet Epitech - B-OOP-400 (Object-Oriented Programming)

---

## 📝 Overview

Raytracer is a 3D image rendering engine based on ray tracing. It simulates the reverse path of light to generate realistic images from scenes described in a configuration file.

---

## ⚙️ Compilation

### Makefile

```bash
make          # Compiles the project
make clean    # Removes object files
make fclean   # Removes object files + binary
make re       # Recompiles everything
make normalize # Applying clang format to all C++ files
make check_normalize # Check if all C++ files are normalized
make tests_run # Run tests
make cov # Generate code coverage report
```

### 🚀 Usage

```bash
./raytracer <SCENE_FILE>
```

-d is to render the scene in sfml window.

```bash
./raytracer <SCENE_FILE> -d
```

#### Example

```bash
./raytracer scenes/demo_sphere.cfg
```

```bash
./raytracer scenes/demo_cylinder.cfg -d
```

---

## ✅ Implemented Features

- See [FEATURES.md](docs/FEATURES.md) for the full list of features.

---

## 👥 Contributors

This project was developed by a team of three students as part of the B-OOP-400 module at Epitech.

- See [CONTRIBUTING.md](CONTRIBUTING.md) for the full list of members.
