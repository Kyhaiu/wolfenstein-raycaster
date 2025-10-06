#pragma once

#include <SDL.h>
#include <iostream>

class Renderer
{
public:
  Renderer();
  ~Renderer();

  bool initialize();
  void clear() const;
  void present() const;
  SDL_Renderer *getSDLRenderer() const { return m_renderer; }

private:
  SDL_Window *m_window;
  SDL_Renderer *m_renderer;
};
