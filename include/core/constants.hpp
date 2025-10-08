#pragma once // Garante que o arquivo seja incluído apenas uma vez no projeto

namespace Constants
{
  // Dimensões da janela do jogo
  constexpr int WINDOW_WIDTH = 1366; // Largura da janela em pixels
  constexpr int WINDOW_HEIGHT = 768; // Altura da janela em pixels

  // Tamanho do mapa (em número de células)
  constexpr int MAP_SIZE = 8; // O mapa é uma grade 8x8

  // Constantes matemáticas
  constexpr float PI = 3.1415926535f; // Valor de pi, usado para cálculos de ângulo
  constexpr float FOV = 60.0f;        // Campo de visão do jogador em graus
  constexpr int NUM_RAYS = 120;       // Número de raios lançados para renderização 3D (raycasting)

  // Proporção da tela usada para cada visualização
  constexpr float MAP_VIEW_RATIO = 0.35f; // 35% da largura da tela é usada para o mapa 2D
  constexpr float VIEW3D_RATIO = 0.65f;   // 65% da largura da tela é usada para a visão 3D

  // O tamanho de cada célula no mapa 2D
  // É relativo à altura da janela, ao tamanho do mapa e à proporção do mapa na tela
  constexpr int WALL_2D_SIZE = static_cast<int>((WINDOW_HEIGHT / MAP_SIZE) * MAP_VIEW_RATIO * 2.5f);
}
