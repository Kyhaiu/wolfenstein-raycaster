#include <world/map.hpp>

Map::Map()
{
  // Inicialização do mapa
  int initialMap[Constants::MAP_SIZE][Constants::MAP_SIZE] = {
      {1, 1, 1, 1, 1, 1, 1, 1},
      {1, 0, 0, 0, 0, 0, 0, 1},
      {1, 0, 1, 0, 0, 0, 0, 1},
      {1, 0, 1, 0, 1, 0, 0, 1},
      {1, 0, 1, 0, 1, 0, 0, 1},
      {1, 0, 0, 0, 1, 1, 1, 1},
      {1, 0, 0, 0, 0, 0, 0, 1},
      {1, 1, 1, 1, 1, 1, 1, 1}};

  for (int y = 0; y < Constants::MAP_SIZE; y++)
  {
    for (int x = 0; x < Constants::MAP_SIZE; x++)
    {
      m_grid[y][x] = initialMap[y][x];
    }
  }
}

void Map::render2D(SDL_Renderer *renderer) const
{
  for (int y = 0; y < Constants::MAP_SIZE; y++)
  {
    for (int x = 0; x < Constants::MAP_SIZE; x++)
    {
      if (m_grid[y][x] == 1)
      {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      }
      else
      {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      }

      SDL_Rect cell = {
          x * Constants::WALL_2D_SIZE + 1,
          y * Constants::WALL_2D_SIZE + 1,
          Constants::WALL_2D_SIZE - 1,
          Constants::WALL_2D_SIZE - 1};
      SDL_RenderFillRect(renderer, &cell);
    }
  }
}

bool Map::isWall(int x, int y) const
{
  if (x >= 0 && x < Constants::MAP_SIZE && y >= 0 && y < Constants::MAP_SIZE)
  {
    return m_grid[y][x] == 1;
  }
  return true; // Fora do mapa é considerado parede
}

int Map::getTile(int x, int y) const
{
  if (x >= 0 && x < Constants::MAP_SIZE && y >= 0 && y < Constants::MAP_SIZE)
  {
    return m_grid[y][x];
  }
  return 1; // Fora do mapa é considerado parede
}