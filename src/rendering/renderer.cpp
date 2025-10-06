#include <rendering/renderer.hpp>
#include <core/Constants.hpp>

Renderer::Renderer()
    : m_window(nullptr), m_renderer(nullptr) {}

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

void Renderer::clear() const
{
  SDL_SetRenderDrawColor(m_renderer, 78, 78, 78, 255);
  SDL_RenderClear(m_renderer);
}

void Renderer::present() const
{
  SDL_RenderPresent(m_renderer);
}