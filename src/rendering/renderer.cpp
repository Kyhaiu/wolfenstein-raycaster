#include <rendering/renderer.hpp>
#include <core/Constants.hpp>

/**
 * @brief Construtor do Renderer
 *
 * Inicializa os ponteiros da janela e do renderer como nullptr.
 */
Renderer::Renderer()
    : m_window(nullptr), m_renderer(nullptr) {}

/**
 * @brief Destrutor do Renderer
 *
 * Libera os recursos da SDL, destruindo o renderer e a janela se existirem.
 */
Renderer::~Renderer()
{
  if (m_renderer)
  {
    SDL_DestroyRenderer(m_renderer);
  }
  if (m_window)
  {
    SDL_DestroyWindow(m_window);
  }
}

/**
 * @brief Inicializa a janela e o renderer
 *
 * Cria uma janela SDL centralizada e um renderer acelerado.
 *
 * @return true se a inicialização foi bem-sucedida
 * @return false se ocorreu algum erro na criação da janela ou do renderer
 */
bool Renderer::initialize()
{
  m_window = SDL_CreateWindow(
      "Raycast Engine",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      Constants::WINDOW_WIDTH,
      Constants::WINDOW_HEIGHT,
      SDL_WINDOW_SHOWN);

  if (!m_window)
  {
    std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
    return false;
  }

  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
  if (!m_renderer)
  {
    std::cerr << "Erro ao criar renderer: " << SDL_GetError() << std::endl;
    return false;
  }

  return true;
}

/**
 * @brief Limpa a tela
 *
 * Define a cor de fundo e limpa o buffer de renderização.
 */
void Renderer::clear() const
{
  SDL_SetRenderDrawColor(m_renderer, 78, 78, 78, 255);
  SDL_RenderClear(m_renderer);
}

/**
 * @brief Atualiza a tela
 *
 * Mostra na janela o conteúdo que foi desenhado no renderer.
 */
void Renderer::present() const
{
  SDL_RenderPresent(m_renderer);
}
