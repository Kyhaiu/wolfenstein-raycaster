#pragma once // Garante que o arquivo seja incluído apenas uma vez no projeto

// ==========================
// Estruturas de dados comuns
// ==========================

/**
 * Vetor 2D Float
 *
 * Representa um vetor bidimensional com coordenadas de ponto flutuante (x, y).
 * Pode ser usado para posições, direções ou deslocamentos em 2D.
 *
 * A struct possui um construtor que permite inicializar x e y, padrão é 0.
 */
struct Vector2f
{
  float x, y; // Coordenadas do vetor

  // Construtor com valores padrão
  Vector2f(float x = 0, float y = 0) : x(x), y(y) {}
};

/**
 * Estado do jogador
 *
 * Armazena a posição, direção e ângulo de visada do jogador.
 * Útil para manter o estado atual do player em qualquer momento do jogo.
 *
 * position - posição X,Y do jogador no mundo
 * direction - vetor de direção do jogador (lado que está olhando)
 * angle - ângulo de rotação do jogador (em radianos ou graus, dependendo do projeto)
 */
struct PlayerState
{
  Vector2f position;  // Posição do jogador
  Vector2f direction; // Direção que o jogador está olhando
  float angle;        // Ângulo de visão do jogador
};
