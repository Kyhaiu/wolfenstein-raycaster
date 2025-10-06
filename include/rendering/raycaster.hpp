#pragma once

#include <SDL.h>
#include <world/map.hpp>
#include <entities/player.hpp>

#include <cmath>
#include <iostream>

class Raycaster
{
public:
  Raycaster(const Map &map, const Player &player);
  void renderRays2D(SDL_Renderer *renderer) const;
  void render3DView(SDL_Renderer *renderer) const;

private:
  const Map &m_map;
  const Player &m_player;

  struct RayHit
  {
    float distance;
    Vector2f point;
    bool isVertical;
    Uint8 shade;
  };

  RayHit castRay(float rayAngle) const;
};
