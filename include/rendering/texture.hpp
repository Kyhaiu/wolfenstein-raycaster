#pragma once

#include <SDL.h>
#include <string>
#include <unordered_map>
#include <iostream>

class TextureManager
{
public:
  ~TextureManager();

  bool loadTexture(SDL_Renderer *renderer, const std::string &id, const std::string &filePath);

  SDL_Texture *getTexture(const std::string &id) const;

  // Retorna retorna o nome da textura com base no mapa
  std::string getTextureId(int texture_number) const;

  void clear();

private:
  std::unordered_map<std::string, SDL_Texture *> m_textures;
};
