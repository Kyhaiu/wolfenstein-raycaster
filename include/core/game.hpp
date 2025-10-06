#pragma once

#include <rendering/renderer.hpp>
#include <rendering/raycaster.hpp>
#include <entities/player.hpp>
#include <world/map.hpp>

#include <iostream>

class Game
{
public:
  Game();
  ~Game();

  bool initialize();
  void run();
  void shutdown();

private:
  void handleEvents();
  void update(float deltaTime);
  void render();

  Renderer m_renderer;
  Player m_player;
  Map m_map;
  Raycaster m_raycaster;

  bool m_isRunning;
  Uint32 m_previousTime;
};
