#include <world/map.hpp>

/**
 * @brief Construtor do mapa
 *
 * Inicializa a grade do mapa com um layout fixo de paredes (1) e espaços vazios (0)
 */
Map::Map()
{
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

/**
 * @brief Renderiza o mapa 2D na tela (minimapa)
 *
 * @param renderer Ponteiro para o SDL_Renderer
 *
 * @note Paredes são desenhadas em branco, espaços vazios em cinza escuro
 * @note Cada célula do mapa é dimensionada proporcionalmente à tela
 */
void Map::render2D(SDL_Renderer *renderer) const
{
  const int mapDisplayWidth = static_cast<int>(Constants::WINDOW_WIDTH * Constants::MAP_VIEW_RATIO);
  const int mapDisplayHeight = Constants::WINDOW_HEIGHT;
  const float cellWidth = static_cast<float>(mapDisplayWidth) / Constants::MAP_SIZE;
  const float cellHeight = static_cast<float>(mapDisplayHeight) / Constants::MAP_SIZE;

  for (int y = 0; y < Constants::MAP_SIZE; y++)
  {
    for (int x = 0; x < Constants::MAP_SIZE; x++)
    {
      if (m_grid[y][x] == 1)
      {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // parede branca
      }
      else
      {
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // fundo escuro
      }

      SDL_Rect cell = {
          static_cast<int>(x * cellWidth),
          static_cast<int>(y * cellHeight),
          static_cast<int>(cellWidth - 1),
          static_cast<int>(cellHeight - 1)};
      SDL_RenderFillRect(renderer, &cell);
    }
  }
}

/**
 * @brief Verifica se uma célula do mapa é parede
 *
 * @param x Coordenada X da célula
 * @param y Coordenada Y da célula
 * @return true se a célula é parede ou está fora do mapa
 * @return false se a célula é espaço vazio
 */
bool Map::isWall(int x, int y) const
{
  if (x >= 0 && x < Constants::MAP_SIZE && y >= 0 && y < Constants::MAP_SIZE)
  {
    return m_grid[y][x] == 1;
  }
  return true; // Fora do mapa é considerado parede
}

/**
 * @brief Retorna o valor de uma célula do mapa
 *
 * @param x Coordenada X da célula
 * @param y Coordenada Y da célula
 * @return int 1 se for parede, 0 se for espaço vazio
 *
 * @note Valores fora do mapa retornam 1 (parede)
 */
int Map::getTile(int x, int y) const
{
  if (x >= 0 && x < Constants::MAP_SIZE && y >= 0 && y < Constants::MAP_SIZE)
  {
    return m_grid[y][x];
  }
  return 1; // Fora do mapa é considerado parede
}