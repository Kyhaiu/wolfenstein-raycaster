#pragma once // Garante que o arquivo seja incluído apenas uma vez

#include <SDL.h>               // Biblioteca SDL para renderização
#include <world/map.hpp>       // Inclui a classe Map
#include <entities/player.hpp> // Inclui a classe Player

#include <utils/mathUtils.hpp> // Inclui os métodos matematicos
#include <core/constants.hpp>  // Inclui as constantes

#include <cmath>    // Para funções matemáticas (seno, cosseno, etc.)
#include <iostream> // Para mensagens de depuração, se necessário

// =====================
// Classe Raycaster
// =====================
// Responsável por lançar "raios" a partir do jogador para detectar paredes e
// calcular distâncias, tanto para exibição 2D do mapa quanto para renderização
// da visão 3D.
class Raycaster
{
public:
  // Construtor que recebe referência ao mapa e ao jogador.
  // Usadas referências constantes para não copiar os dados.
  Raycaster(const Map &map, const Player &player);

  // Renderiza os raios no mapa 2D, útil para depuração
  void renderRays2D(SDL_Renderer *renderer) const;

  // Renderiza a visão 3D com base nas distâncias dos raios
  void render3DView(SDL_Renderer *renderer) const;

private:
  const Map &m_map;       // Referência ao mapa do mundo
  const Player &m_player; // Referência ao jogador, origem dos raios

  // =====================
  // Estrutura interna RayHit
  // =====================
  // Armazena informações sobre onde o raio atingiu uma parede
  struct RayHit
  {
    float distance;  // Distância do jogador até o ponto de colisão
    Vector2f point;  // Coordenadas do ponto de colisão
    bool isVertical; // True se o raio bateu em uma parede vertical, false se horizontal
    Uint8 shade;     // Valor de "sombreamento" para visualização (mais próximo = mais claro)
  };

  // Lança um raio em um determinado ângulo e retorna os dados do ponto de impacto
  RayHit castRay(float rayAngle) const;
};
