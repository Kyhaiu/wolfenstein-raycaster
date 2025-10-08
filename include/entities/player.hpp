#pragma once // Garante que o arquivo seja incluído apenas uma vez no projeto

#include <SDL.h>          // Biblioteca SDL para entrada e renderização
#include <core/types.hpp> // Inclui estruturas como Vector2f
#include <world/map.hpp>  // Inclui a classe Map, para referência do mapa

// =====================
// Classe Player
// =====================
// Representa o jogador no jogo, armazenando posição, direção e ângulo de visão.
// Possui métodos para atualizar posição com base em input, renderizar no mapa
// e verificar colisões com o mapa.
class Player
{
public:
  // Construtor que recebe referência constante ao mapa.
  // Isso permite que o jogador verifique colisões sem copiar o mapa.
  Player(const Map &map);

  // Atualiza o estado do jogador com base nas teclas pressionadas e deltaTime
  void update(const Uint8 *keystates, float deltaTime);

  // Renderiza o jogador no mapa (2D)
  void render(SDL_Renderer *renderer) const;

  // =====================
  // Getters
  // =====================
  Vector2f getPosition() const { return m_position; }   // Retorna posição atual
  Vector2f getDirection() const { return m_direction; } // Retorna direção atual
  float getAngle() const { return m_angle; }            // Retorna ângulo atual
  const Map &getMap() const { return m_map; }           // Retorna referência ao mapa

  // =====================
  // Setters
  // =====================
  void setPosition(float x, float y); // Define a posição do jogador

private:
  const Map &m_map;     // Referência constante ao mapa (não pode ser modificada)
  Vector2f m_position;  // Posição atual do jogador no mundo
  Vector2f m_direction; // Direção do jogador (para onde ele está olhando)
  float m_angle;        // Ângulo de visão do jogador

  // =====================
  // Métodos privados
  // =====================
  void handleInput(const Uint8 *keystates, float deltaTime);
  // Lê o teclado e calcula movimento/rotação

  void updateDirection();
  // Atualiza o vetor de direção baseado no ângulo

  bool checkCollision(float newX, float newY) const;
  // Verifica se a nova posição colidiria com alguma parede no mapa
};
