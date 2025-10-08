#include <entities/player.hpp>

/**
 * @brief Construtor do Player
 *
 * Inicializa o jogador com referência ao mapa, posição inicial e ângulo de visão.
 *
 * @param map Referência constante ao mapa
 */
Player::Player(const Map &map)
    : m_map(map)
{
  m_position = {100.0f, 100.0f};
  m_angle = 0.0f;
  updateDirection();
}

/**
 * @brief Atualiza o estado do jogador
 *
 * Processa entrada do teclado e atualiza a posição do jogador com base em deltaTime.
 *
 * @param keystates Ponteiro para o estado atual do teclado
 * @param deltaTime Tempo em segundos desde o último frame
 */
void Player::update(const Uint8 *keystates, float deltaTime)
{
  handleInput(keystates, deltaTime);
}

/**
 * @brief Processa entrada do teclado
 *
 * Movimenta e rotaciona o jogador de acordo com as teclas W, A, S, D.
 * Garante que o jogador não colida com paredes.
 *
 * @param keystates Ponteiro para o estado atual do teclado
 * @param deltaTime Tempo em segundos desde o último frame
 */
void Player::handleInput(const Uint8 *keystates, float deltaTime)
{
  const float moveSpeed = 100.0f * deltaTime;
  const float rotSpeed = 2.5f * deltaTime;

  float newX = m_position.x;
  float newY = m_position.y;

  if (keystates[SDL_SCANCODE_A])
    m_angle -= rotSpeed;
  if (keystates[SDL_SCANCODE_D])
    m_angle += rotSpeed;

  updateDirection();

  if (keystates[SDL_SCANCODE_W])
  {
    newX += m_direction.x * moveSpeed;
    newY += m_direction.y * moveSpeed;
  }
  if (keystates[SDL_SCANCODE_S])
  {
    newX -= m_direction.x * moveSpeed;
    newY -= m_direction.y * moveSpeed;
  }

  // Só move se não colidir
  if (!checkCollision(newX, newY))
  {
    m_position.x = newX;
    m_position.y = newY;
  }
}

/**
 * @brief Atualiza o vetor direção do jogador
 *
 * Baseado no ângulo atual, recalcula os componentes X e Y da direção.
 */
void Player::updateDirection()
{
  m_direction.x = cosf(m_angle);
  m_direction.y = sinf(m_angle);
}

/**
 * @brief Verifica colisão do jogador com o mapa
 *
 * Converte coordenadas do mundo para células do mapa e verifica se a célula é parede.
 *
 * @param newX Nova posição X do jogador
 * @param newY Nova posição Y do jogador
 * @return true se colidir com parede ou sair dos limites, false caso contrário
 */
bool Player::checkCollision(float newX, float newY) const
{
  int cellX = static_cast<int>(newX / Constants::WALL_2D_SIZE);
  int cellY = static_cast<int>(newY / Constants::WALL_2D_SIZE);

  if (cellX < 0 || cellY < 0 ||
      cellX >= Constants::MAP_SIZE || cellY >= Constants::MAP_SIZE)
    return true;

  return m_map.isWall(cellX, cellY);
}

/**
 * @brief Define a posição do jogador
 *
 * Atualiza as coordenadas X e Y diretamente.
 *
 * @param x Nova posição X
 * @param y Nova posição Y
 */
void Player::setPosition(float x, float y)
{
  m_position.x = x;
  m_position.y = y;
}

/**
 * @brief Renderiza o jogador no mapa 2D
 *
 * Desenha um quadrado azul representando o jogador e uma linha vermelha indicando a direção.
 *
 * @param renderer Ponteiro para o SDL_Renderer
 */
void Player::render(SDL_Renderer *renderer) const
{
  // Área do mapa na tela
  const int mapDisplayWidth = static_cast<int>(Constants::WINDOW_WIDTH * Constants::MAP_VIEW_RATIO);
  const int mapDisplayHeight = Constants::WINDOW_HEIGHT;
  const float cellWidth = static_cast<float>(mapDisplayWidth) / Constants::MAP_SIZE;
  const float cellHeight = static_cast<float>(mapDisplayHeight) / Constants::MAP_SIZE;

  // Converte a posição do jogador (em coordenadas do mundo) para coordenadas de tela no mapa
  float screenX = m_position.x * (cellWidth / Constants::WALL_2D_SIZE);
  float screenY = m_position.y * (cellHeight / Constants::WALL_2D_SIZE);

  // Desenha o jogador (quadrado azul)
  SDL_Rect playerRect = {
      static_cast<int>(screenX) - 3,
      static_cast<int>(screenY) - 3,
      6, 6};
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderFillRect(renderer, &playerRect);

  // Desenha a direção (linha vermelha)
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderDrawLine(renderer,
                     static_cast<int>(screenX),
                     static_cast<int>(screenY),
                     static_cast<int>(screenX + m_direction.x * 20.0f),
                     static_cast<int>(screenY + m_direction.y * 20.0f));
}
