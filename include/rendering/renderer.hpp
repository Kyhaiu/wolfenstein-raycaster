#pragma once // Garante que este arquivo seja incluído apenas uma vez

#include <SDL.h>    // Biblioteca SDL para criação de janelas e renderização
#include <iostream> // Para mensagens de erro/depuração

// =====================
// Classe Renderer
// =====================
// Responsável por criar a janela do jogo e gerenciar a renderização
// usando SDL. Encapsula SDL_Window e SDL_Renderer para facilitar o uso.
class Renderer
{
public:
  // Construtor
  Renderer();

  // Destrutor
  ~Renderer();

  // Inicializa a janela e o renderer SDL
  // Retorna true se inicializado com sucesso, false caso contrário
  bool initialize();

  // Limpa o conteúdo da tela para preparar uma nova renderização
  void clear() const;

  // Apresenta o que foi desenhado no renderer na tela
  void present() const;

  // Retorna o ponteiro para o SDL_Renderer, necessário para desenhar shapes
  SDL_Renderer *getSDLRenderer() const { return m_renderer; }

private:
  SDL_Window *m_window;     // Ponteiro para a janela SDL
  SDL_Renderer *m_renderer; // Ponteiro para o renderer SDL
};
