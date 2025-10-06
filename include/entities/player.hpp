#pragma once

#include <SDL.h>
#include <core/types.hpp>

class Player
{
public:
  Player();
  void update(const Uint8 *keystates, float deltaTime);
  void render(SDL_Renderer *renderer) const;

  // Getters
  Vector2f getPosition() const { return m_position; }
  Vector2f getDirection() const { return m_direction; }
  float getAngle() const { return m_angle; }

  // Setters
  void setPosition(float x, float y);

private:
  Vector2f m_position;
  Vector2f m_direction;
  float m_angle;

  void handleInput(const Uint8 *keystates, float deltaTime);
  void updateDirection();
};
