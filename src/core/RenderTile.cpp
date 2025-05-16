/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** RenderTile
*/

/**
 * @file RenderTile.cpp
 * @brief Implementation of the tile-based rendering system for parallel
 * processing
 * @author Santi
 * @date 2025-05-16
 * @version 1.0
 */

#include "RenderTile.hpp"
#include <algorithm>

namespace RayTracer {

// RenderTile implementation
RenderTile::RenderTile(int startX, int startY, int width, int height)
    : _startX(startX), _startY(startY), _width(width), _height(height) {}

int RenderTile::getStartX() const {
  return _startX;
}

int RenderTile::getStartY() const {
  return _startY;
}

int RenderTile::getWidth() const {
  return _width;
}

int RenderTile::getHeight() const {
  return _height;
}

int RenderTile::getEndX() const {
  return _startX + _width;
}

int RenderTile::getEndY() const {
  return _startY + _height;
}

// TileManager implementation
TileManager::TileManager(int imageWidth, int imageHeight, int tileSize)
    : _imageWidth(imageWidth),
      _imageHeight(imageHeight),
      _tileSize(tileSize),
      _currentTileIndex(0),
      _completedTiles(0) {
  // Calculate number of tiles in each dimension
  _numTilesX = (_imageWidth + tileSize - 1) / tileSize;
  _numTilesY = (_imageHeight + tileSize - 1) / tileSize;
  _totalTiles = _numTilesX * _numTilesY;
}

RenderTile* TileManager::getNextTile() {
  // Get current tile index and increment atomically
  int tileIndex = _currentTileIndex.fetch_add(1);

  // Check if all tiles have been processed
  if (tileIndex >= _totalTiles) {
    return nullptr;
  }

  // Calculate tile coordinates
  int tileX = tileIndex % _numTilesX;
  int tileY = tileIndex / _numTilesX;

  // Calculate tile dimensions, handling edge tiles
  int startX = tileX * _tileSize;
  int startY = tileY * _tileSize;
  int width = std::min(_tileSize, _imageWidth - startX);
  int height = std::min(_tileSize, _imageHeight - startY);

  // Create and return the tile
  return new RenderTile(startX, startY, width, height);
}

int TileManager::getTotalTiles() const {
  return _totalTiles;
}

int TileManager::getCompletedTiles() const {
  return _completedTiles.load();
}

void TileManager::tileCompleted() {
  _completedTiles.fetch_add(1);
}

void TileManager::reset() {
  _currentTileIndex.store(0);
  _completedTiles.store(0);
}

double TileManager::getProgress() const {
  if (_totalTiles == 0) {
    return 100.0;
  }
  return 100.0 * _completedTiles.load() / _totalTiles;
}

}  // namespace RayTracer
