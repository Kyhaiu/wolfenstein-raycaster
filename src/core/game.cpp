#include <core/game.hpp>

Game::Game()
    : m_raycaster(m_map, m_player), m_isRunning(false), m_previousTime(0) {}

Game::~Game()
{
  shutdown();
}

bool Game::initialize()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
    return false;
  }

  if (!m_renderer.initialize())
  {
    return false;
  }

  m_player.setPosition(300.0f, 300.0f);
  m_isRunning = true;
  m_previousTime = SDL_GetTicks();

  return true;
}

void Game::run()
{
  while (m_isRunning)
  {
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - m_previousTime) / 1000.0f;
    m_previousTime = currentTime;

    handleEvents();
    update(deltaTime);
    render();
  }
}

void Game::shutdown()
{
  SDL_Quit();
}

void Game::handleEvents()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
    {
      m_isRunning = false;
    }
  }
}

void Game::update(float deltaTime)
{
  const Uint8 *keystates = SDL_GetKeyboardState(NULL);
  m_player.update(keystates, deltaTime);
}

void Game::render()
{
  m_renderer.clear();

  m_map.render2D(m_renderer.getSDLRenderer());
  m_player.render(m_renderer.getSDLRenderer());
  m_raycaster.renderRays2D(m_renderer.getSDLRenderer());
  m_raycaster.render3DView(m_renderer.getSDLRenderer());

  m_renderer.present();
}