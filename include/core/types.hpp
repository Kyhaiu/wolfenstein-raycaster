#pragma once

// Estruturas de dados comuns

/**
 * Vetor 2D Float
 *
 * @param float x
 * @param float y
 *
 */
struct Vector2f
{
  float x, y;
  Vector2f(float x = 0, float y = 0) : x(x), y(y) {}
};

/**
 * Estado do player (posição, direção e angulo de visada)
 *
 * @param Vector2f position - Posição X, Y do jogador
 * @param Vector2f direction - Direção X, Y do jogador (lado que está olhando)
 * @param float angle - Angulo o lado que o jogador está olhando
 *
 */
struct PlayerState
{
  Vector2f position;
  Vector2f direction;
  float angle;
};
