#pragma once // Garante que o arquivo seja incluído apenas uma vez no projeto

// Inclusão de headers necessários
#include <rendering/renderer.hpp>  // Classe responsável pela renderização
#include <rendering/raycaster.hpp> // Classe responsável pelo raycasting (visão 3D)
#include <entities/player.hpp>     // Classe do jogador
#include <world/map.hpp>           // Classe do mapa

#include <iostream> // Usado para mensagens de erro no console

// Classe principal do jogo
class Game
{
public:
  Game();  // Construtor da classe
  ~Game(); // Destrutor da classe

  // Inicializa os sistemas do jogo (SDL, renderizador, etc.)
  bool initialize();

  // Loop principal do jogo
  void run();

  // Encerra o jogo e libera recursos
  void shutdown();

private:
  // Funções internas do jogo

  // Processa eventos do SDL (como teclado e janela)
  void handleEvents();

  // Atualiza o estado do jogo (jogador, lógica, etc.)
  void update(float deltaTime);

  // Renderiza o mapa, jogador e visão 3D na tela
  void render();

  // ==========================
  // Atributos do jogo
  // ==========================
  Map m_map;             // Mapa do jogo (grade 2D com paredes)
  Player m_player;       // Jogador
  Renderer m_renderer;   // Renderizador para desenhar na tela
  Raycaster m_raycaster; // Sistema de raycasting para visão 3D

  bool m_isRunning;      // Flag para controlar se o jogo está rodando
  Uint32 m_previousTime; // Armazena o tempo do frame anterior (para calcular deltaTime)
};
