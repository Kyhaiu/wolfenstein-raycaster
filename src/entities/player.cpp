#include <entities/player.hpp>
#include <utils/mathUtils.hpp>

Player::Player()
    : m_position(300.0f, 300.0f), m_angle(0.0f)
{
  updateDirection();
}

void Player::update(const Uint8 *keystates, float deltaTime)
{
  handleInput(keystates, deltaTime);
}

void Player::render(SDL_Renderer *renderer) const
{
  // Desenha o jogador
  SDL_Rect playerRect = {
      static_cast<int>(m_position.x) - 2,
      static_cast<int>(m_position.y) - 2,
      5, 5};
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderFillRect(renderer, &playerRect);

  // Desenha a direção
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderDrawLine(renderer,
                     static_cast<int>(m_position.x),
                     static_cast<int>(m_position.y),
                     static_cast<int>(m_position.x + m_direction.x * 20.0f),
                     static_cast<int>(m_position.y + m_direction.y * 20.0f));
}

void Player::setPosition(float x, float y)
{
  m_position.x = x;
  m_position.y = y;
}

void Player::handleInput(const Uint8 *keystates, float deltaTime)
{
  const float rotationSpeed = 2.0f * deltaTime;
  const float moveSpeed = 100.0f * deltaTime;

  if (keystates[SDL_SCANCODE_A])
  {
    m_angle -= rotationSpeed;
    m_angle = MathUtils::normalizeAngle(m_angle);
    updateDirection();
  }

  if (keystates[SDL_SCANCODE_D])
  {
    m_angle += rotationSpeed;
    m_angle = MathUtils::normalizeAngle(m_angle);
    updateDirection();
  }

  if (keystates[SDL_SCANCODE_W])
  {
    m_position.x += m_direction.x * moveSpeed;
    m_position.y += m_direction.y * moveSpeed;
  }

  if (keystates[SDL_SCANCODE_S])
  {
    m_position.x -= m_direction.x * moveSpeed;
    m_position.y -= m_direction.y * moveSpeed;
  }
}

void Player::updateDirection()
{
  m_direction.x = cosf(m_angle);
  m_direction.y = sinf(m_angle);
}