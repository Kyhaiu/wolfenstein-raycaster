#include <core/game.hpp>

/**
 * @brief Construtor da classe Game
 *
 * Inicializa o mapa, jogador, raycaster e variáveis de controle do loop do jogo.
 * O jogador recebe referência ao mapa e o raycaster recebe referência ao mapa e jogador.
 */
Game::Game()
    : m_map(),
      m_player(m_map),
      m_raycaster(m_map, m_player),
      m_isRunning(false),
      m_previousTime(0)
{
}

/**
 * @brief Destrutor da classe Game
 *
 * Garante que a SDL seja finalizada ao destruir a instância do jogo.
 */
Game::~Game()
{
  shutdown();
}

/**
 * @brief Inicializa o jogo
 *
 * Configura SDL, inicializa o renderer e posiciona o jogador.
 *
 * @return true se a inicialização ocorreu corretamente, false caso ocorra algum erro
 */
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

  // --- Carrega texturas ---
  // Podia ser melhor, porém né...
  m_raycaster.getTextureManager().loadTexture(m_renderer.getSDLRenderer(), "barrel", "../assets/textures/barrel.bmp");
  m_raycaster.getTextureManager().loadTexture(m_renderer.getSDLRenderer(), "bluestone", "../assets/textures/bluestone.bmp");
  m_raycaster.getTextureManager().loadTexture(m_renderer.getSDLRenderer(), "colorstone", "../assets/textures/colorstone.bmp");
  m_raycaster.getTextureManager().loadTexture(m_renderer.getSDLRenderer(), "eagle", "../assets/textures/eagle.bmp");
  m_raycaster.getTextureManager().loadTexture(m_renderer.getSDLRenderer(), "greenlight", "../assets/textures/greenlight.bmp");
  m_raycaster.getTextureManager().loadTexture(m_renderer.getSDLRenderer(), "greystone", "../assets/textures/greystone.bmp");
  m_raycaster.getTextureManager().loadTexture(m_renderer.getSDLRenderer(), "mossy", "../assets/textures/mossy.bmp");
  m_raycaster.getTextureManager().loadTexture(m_renderer.getSDLRenderer(), "pillar", "../assets/textures/pillar.bmp");
  m_raycaster.getTextureManager().loadTexture(m_renderer.getSDLRenderer(), "purplestone", "../assets/textures/purplestone.bmp");
  m_raycaster.getTextureManager().loadTexture(m_renderer.getSDLRenderer(), "redbrick", "../assets/textures/redbrick.bmp");
  m_raycaster.getTextureManager().loadTexture(m_renderer.getSDLRenderer(), "wood", "../assets/textures/wood.bmp");
  // ------------------------

  m_player.setPosition(300.0f, 300.0f);
  m_isRunning = true;
  m_previousTime = SDL_GetTicks();

  return true;
}

/**
 * @brief Executa o loop principal do jogo
 *
 * Enquanto m_isRunning for verdadeiro, processa eventos, atualiza lógica e renderiza frames.
 */
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

/**
 * @brief Finaliza o jogo
 *
 * Libera recursos alocados pelo SDL.
 */
void Game::shutdown()
{
  SDL_Quit();
}

/**
 * @brief Processa eventos do SDL
 *
 * Captura eventos como fechamento da janela ou outros inputs do sistema.
 * Caso o evento SDL_QUIT seja detectado, encerra o loop principal.
 */
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

/**
 * @brief Atualiza a lógica do jogo
 *
 * Atualiza o estado do jogador com base no input do teclado.
 *
 * @param deltaTime Tempo em segundos desde o último frame
 */
void Game::update(float deltaTime)
{
  const Uint8 *keystates = SDL_GetKeyboardState(NULL);
  m_player.update(keystates, deltaTime);
}

/**
 * @brief Renderiza todos os elementos do jogo
 *
 * Desenha o mapa 2D, jogador, raios do raycaster e a visão 3D na tela.
 */
void Game::render()
{
  m_renderer.clear();

  m_map.render2D(m_renderer.getSDLRenderer());
  m_player.render(m_renderer.getSDLRenderer());
  m_raycaster.renderRays2D(m_renderer.getSDLRenderer());
  m_raycaster.render3DView(m_renderer.getSDLRenderer());

  m_renderer.present();
}
