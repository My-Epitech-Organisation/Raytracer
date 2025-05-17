# 🧾 .cfg File Usage Guide – Raytracer

.cfg files define the scenes to be rendered. They use a syntax inspired by libconfig++, which is user-readable and structured into three main sections:

- camera: camera configuration
- primitives: objects to display in the scene
- lights: light sources

---

## 🎥 Camera

The camera section defines the point of view from which the scene is observed.

```cfg
camera :
{
  resolution = { width = 800; height = 600; };
  position = { x = 0; y = 0; z = 0; };
  rotation = { x = 0; y = 0; z = 0; };
  fieldOfView = 72.0; # In degrees
};
```

### Camera Parameters

- resolution: Rendered image resolution, specified by width and height.
- position: Camera position in 3D space, defined by the coordinates x, y, and z.
- rotation: Camera orientation, defined by rotation angles around the x, y, and z axes.
- fieldOfView: Camera field of view, specified in degrees. It determines the camera’s viewing angle.

## 🏗️ Objects

The primitives section defines the objects to display in the scene. Each object is defined by its type and properties.

```cfg
primitives :
{
  sphere :
  {
    position = { x = 0; y = 0; z = 0; };
    radius = 1.0;
    color = { r = 1.0; g = 0.0; b = 0.0; };
    material = "diffuse";
  };

  plane :
  {
    position = { x = 0; y = -1; z = 0; };
    normal = { x = 0; y = 1; z = 0; };
    color = { r = 0.5; g = 0.5; b = 0.5; };
    material = "reflective";
  };
};
```

### Transformations

Transformations are applied to objects to position and orient them in the scene. These can include:

- translate: Object position in 3D space. x, y, and z are the translation coordinates.
- rotation: Object orientation, specified by rotation angles around the x, y, and z axes.
- scale: Object scale, specified by the scaling factors x, y, and z.

## 📌 Notes

Comments (# ...) are allowed.

The order of blocks (camera, primitives, lights) is not strict.

Each primitive is defined as an array-like block (...).

The file must be readable by the libconfig++ library.
