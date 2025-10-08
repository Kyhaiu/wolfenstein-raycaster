#include <rendering/texture.hpp>

TextureManager::~TextureManager()
{
  clear();
}

/**
 * @brief Carrega uma textura e associa a um ID
 *
 * @param renderer Renderer SDL utilizado para criar a textura
 * @param id Identificador da textura
 * @param filePath Caminho do arquivo da textura
 * @return true se carregou com sucesso
 */
bool TextureManager::loadTexture(SDL_Renderer *renderer, const std::string &id, const std::string &filePath)
{
  SDL_Surface *surface = SDL_LoadBMP(filePath.c_str());
  if (!surface)
  {
    std::cerr << "Erro ao carregar BMP: " << SDL_GetError() << std::endl;
    return false;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  if (!texture)
  {
    std::cerr << "Erro ao criar textura: " << SDL_GetError() << std::endl;
    return false;
  }

  m_textures[id] = texture;
  return true;
}

/**
 * @brief Retorna a textura associada ao ID
 *
 * @param id Identificador da textura
 * @return Ponteiro para SDL_Texture, nullptr se não existir
 */
SDL_Texture *TextureManager::getTexture(const std::string &id) const
{
  auto it = m_textures.find(id);
  if (it != m_textures.end())
    return it->second;
  return nullptr;
}

std::string TextureManager::getTextureId(int texture_number) const
{
  switch (texture_number)
  {
  case 0:
    return "barrel";
  case 1:
    return "bluestone";
  case 2:
    return "colorstone";
  case 3:
    return "eagle";
  case 4:
    return "greenlight";
  case 5:
    return "greystone";
  case 6:
    return "mossy";
  case 7:
    return "pillar";
  case 8:
    return "purplestone";
  case 9:
    return "redbrick";
  case 10:
    return "wood";
  default:
    return "bluestone";
  }
}

/**
 * @brief Limpa todas as texturas carregadas
 */
void TextureManager::clear()
{
  for (auto &pair : m_textures)
  {
    if (pair.second)
      SDL_DestroyTexture(pair.second);
  }
  m_textures.clear();
}
