#pragma once // Garante que o arquivo seja incluído apenas uma vez

#include <SDL.h>              // SDL para renderização
#include <core/constants.hpp> // Constantes globais, como MAP_SIZE

// =====================
// Classe Map
// =====================
// Representa o mapa do jogo como uma grade 2D de células.
// Cada célula pode ser parede ou espaço livre.
class Map
{
public:
  // Construtor
  // Inicializa o mapa, normalmente preenchendo m_grid com paredes e espaços vazios
  Map();

  // Renderiza o mapa no formato 2D (mini-mapa)
  void render2D(SDL_Renderer *renderer) const;

  // Verifica se a célula na posição (x, y) é uma parede
  bool isWall(int x, int y) const;

  // Retorna o valor do tile na posição (x, y)
  // Útil para lógicas mais complexas além de parede/livre
  int getTile(int x, int y) const;

private:
  static const int m_size = Constants::MAP_SIZE;        // Tamanho fixo do mapa
  int m_grid[Constants::MAP_SIZE][Constants::MAP_SIZE]; // Matriz representando as células do mapa
};
