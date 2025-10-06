#pragma once

#include <SDL.h>
#include <core/constants.hpp>

class Map
{
public:
  Map();
  void render2D(SDL_Renderer *renderer) const;
  bool isWall(int x, int y) const;
  int getTile(int x, int y) const;

private:
  static const int m_size = Constants::MAP_SIZE;
  int m_grid[Constants::MAP_SIZE][Constants::MAP_SIZE];
};
