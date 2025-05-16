# 🧾 Guide d'utilisation des fichiers .cfg – Raytracer

Les fichiers `.cfg` définissent les scènes à rendre. Ils utilisent une syntaxe inspirée de libconfig++, lisible par l'utilisateur, et structurée en trois grandes sections :

- `camera` : configuration de la caméra
- `primitives` : objets à afficher dans la scène
- `lights` : sources lumineuses

---

## 🎥 Caméra

La section `camera` définit le point de vue à partir duquel la scène est observée.

```cfg
camera :
{
  resolution = { width = 800; height = 600; };
  position = { x = 0; y = 0; z = 0; };
  rotation = { x = 0; y = 0; z = 0; };
  fieldOfView = 72.0; # En degrés
};
```

### Paramètres de la caméra

- `resolution` : Résolution de l'image rendue, spécifiée par la largeur (`width`) et la hauteur (`height`).
- `position` : Position de la caméra dans l'espace 3D, spécifiée par les coordonnées `x`, `y` et `z`.
- `rotation` : Orientation de la caméra, spécifiée par les angles de rotation autour des axes `x`, `y` et `z`.
- `fieldOfView` : Champ de vision de la caméra, spécifié en degrés. Il détermine l'angle de vue de la caméra.

## 🏗️ Objets

La section `primitives` définit les objets à afficher dans la scène. Chaque objet est défini par son type et ses propriétés.

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

Les transformations sont appliquées aux objets pour les positionner et les orienter dans la scène. Elles peuvent inclure :
- `translate` : Position de l'objet dans l'espace 3D. `x`, `y`, et `Z` sont les coordonnées de translation.
- `rotation` : Orientation de l'objet, spécifiée par les angles de rotation autour des axes `x`, `y` et `z`.
- `scale` : Échelle de l'objet, spécifiée par les facteurs d'échelle `x`, `y` et `z`.

## 📌 Remarques

Les commentaires (# ...) sont autorisés.

L’ordre des blocs (camera, primitives, lights) n’est pas strict.

Chaque primitive est définie sous forme de tableau ( ... ).

Le fichier doit être lisible par la bibliothèque libconfig++.
