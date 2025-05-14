#ifndef RENDERTILE_HPP_
#define RENDERTILE_HPP_

#include <atomic>

namespace RayTracer {

/**
 * @brief Represents a rectangular tile of the final image
 * 
 * Used for dividing the rendering workload among threads
 */
class RenderTile {
 public:
  /**
   * @brief Constructor
   * @param startX X-coordinate of the top-left corner
   * @param startY Y-coordinate of the top-left corner
   * @param width Width of the tile in pixels
   * @param height Height of the tile in pixels
   */
  RenderTile(int startX, int startY, int width, int height);

  /**
   * @brief Get the X-coordinate of the top-left corner
   * @return X-coordinate
   */
  int getStartX() const;

  /**
   * @brief Get the Y-coordinate of the top-left corner
   * @return Y-coordinate
   */
  int getStartY() const;

  /**
   * @brief Get the width of the tile
   * @return Width in pixels
   */
  int getWidth() const;

  /**
   * @brief Get the height of the tile
   * @return Height in pixels
   */
  int getHeight() const;

  /**
   * @brief Get the end X-coordinate (exclusive)
   * @return End X-coordinate
   */
  int getEndX() const;

  /**
   * @brief Get the end Y-coordinate (exclusive)
   * @return End Y-coordinate
   */
  int getEndY() const;

 private:
  int _startX;  ///< X-coordinate of the top-left corner
  int _startY;  ///< Y-coordinate of the top-left corner
  int _width;   ///< Width of the tile in pixels
  int _height;  ///< Height of the tile in pixels
};

/**
 * @brief Creates and manages rendering tiles
 */
class TileManager {
 public:
  /**
   * @brief Constructor
   * @param imageWidth Total image width
   * @param imageHeight Total image height
   * @param tileSize Size of each tile (both width and height)
   */
  TileManager(int imageWidth, int imageHeight, int tileSize = 64);

  /**
   * @brief Get the next tile to render
   * @return Next tile or nullptr if all tiles have been processed
   */
  RenderTile* getNextTile();

  /**
   * @brief Get the total number of tiles
   * @return Total number of tiles
   */
  int getTotalTiles() const;

  /**
   * @brief Get the number of completed tiles
   * @return Number of completed tiles
   */
  int getCompletedTiles() const;

  /**
   * @brief Mark a tile as completed
   */
  void tileCompleted();

  /**
   * @brief Reset the tile counter (for restarting rendering)
   */
  void reset();

  /**
   * @brief Get the progress as a percentage
   * @return Progress (0-100)
   */
  double getProgress() const;

 private:
  int _imageWidth;    ///< Total image width
  int _imageHeight;   ///< Total image height
  int _tileSize;      ///< Size of each tile (both width and height)
  int _numTilesX;     ///< Number of tiles along X-axis
  int _numTilesY;     ///< Number of tiles along Y-axis
  int _totalTiles;    ///< Total number of tiles
  
  std::atomic<int> _currentTileIndex;  ///< Index of the next tile to process
  std::atomic<int> _completedTiles;    ///< Number of completed tiles
};

}  // namespace RayTracer

#endif  // RENDERTILE_HPP_
