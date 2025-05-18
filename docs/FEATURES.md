# ✨ Features – Raytracer

## ✅ Mandatory Features (Must)

### 🔷 Primitives

- ✅ Sphere
- ✅ Plane

### 🔷 Transformations

- ✅ Translation

### 🔷 Lighting

- ✅ Directional light
- ✅ Ambient light

### 🔷 Materials

- ✅ Solid color (Flat shading)

### 🔷 Scene Configuration

- ✅ Load from external file
- ✅ Camera positioning
- ✅ Placement of primitives and lights

### 🔷 Interface

- ✅ No graphical display required
- ✅ Rendering to `.ppm` image only

---

## 🧩 Recommended Features (Should)

### 🔷 Primitives

- ✅ Cylinder
- ✅ Cone

### 🔷 Transformations

- ✅ Rotation

### 🔷 Lighting Effects

- ✅ Shadows

---

## 💡 Bonus Features (Could)

### 🔷 Additional Primitives

- ✅ Clipped cylinder / cone
- ✅ Torus
- ✅ Triangles
- ⏳ Tanglecube
- ⏳ Support for `.obj` files
- ⏳ Fractals, Möbius strip

### 🔷 Advanced Transformations

- ⏳ Scale, shear
- ✅ Transformation matrix
- ⏳ Scene graph

### 🔷 Advanced Lighting

- ✅ Multiple point lights
- ⏳ Colored light
- ⏳ Phong reflection, ambient occlusion

### 🔷 Materials

- ⏳ Reflection, transparency, refraction
- ⏳ Textures (file, checkerboard, Perlin noise)
- ⏳ Normal mapping

### 🔷 Optimizations

- ✅ Multithreading
- ⏳ Spatial partitioning
- ⏳ Clustering

### 🔷 Interface (bonus)

- ✅ preview (SFML)
- ✅ Early render stop
- ⏳ Real-time preview (SFML)
- ⏳ Auto-reload on scene modification

---

## 🏗️ Architecture Features

### 🔷 Architecture

- ✅ Object-oriented design with interfaces (IPrimitive, ILight)
- ⏳ Plugins system for dynamic loading
- ✅ Design patterns (Factory, Builder)
