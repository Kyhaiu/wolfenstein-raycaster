#include <rendering/raycaster.hpp>

/**
 * @brief Construtor do Raycaster
 *
 * Inicializa o Raycaster com referência ao mapa e ao jogador.
 *
 * @param map Referência constante ao mapa
 * @param player Referência constante ao jogador
 */
Raycaster::Raycaster(const Map &map, const Player &player)
    : m_map(map), m_player(player)
{
}

/**
 * @brief Renderiza os raios do jogador no minimapa 2D
 *
 * Desenha linhas que representam os raios lançados pelo jogador no
 * minimapa, mostrando colisões verticais em azul e horizontais em verde.
 *
 * @param renderer Ponteiro para SDL_Renderer
 */
void Raycaster::renderRays2D(SDL_Renderer *renderer) const
{
  const int numRays = Constants::NUM_RAYS;
  const float fov = MathUtils::degToRad(Constants::FOV);
  const Vector2f playerPos = m_player.getPosition();
  const float playerAngle = m_player.getAngle();

  // Área do minimapa
  const int mapDisplayWidth = static_cast<int>(Constants::WINDOW_WIDTH * Constants::MAP_VIEW_RATIO);
  const int mapDisplayHeight = Constants::WINDOW_HEIGHT;

  const float cellWidth = static_cast<float>(mapDisplayWidth) / static_cast<float>(Constants::MAP_SIZE);
  const float cellHeight = static_cast<float>(mapDisplayHeight) / static_cast<float>(Constants::MAP_SIZE);

  const float scaleX = cellWidth / static_cast<float>(Constants::WALL_2D_SIZE);
  const float scaleY = cellHeight / static_cast<float>(Constants::WALL_2D_SIZE);

  const float screenPlayerX = playerPos.x * scaleX;
  const float screenPlayerY = playerPos.y * scaleY;

  float rayAngle = playerAngle - (fov / 2.0f);

  for (int rayIndex = 0; rayIndex < numRays; ++rayIndex)
  {
    rayAngle = MathUtils::normalizeAngle(rayAngle);

    float tanValue = tanf(rayAngle);
    float rayEndX, rayEndY;
    float distVertical = 1e6f, distHorizontal = 1e6f;
    float verticalHitX = 0.0f, verticalHitY = 0.0f;
    float horizontalHitX = 0.0f, horizontalHitY = 0.0f;

    // ==========================
    // Checagem vertical
    // ==========================
    {
      int depth = 0;
      float stepX, stepY;

      if (cosf(rayAngle) > 0.001f) // direita
      {
        rayEndX = static_cast<float>((static_cast<int>(playerPos.x / Constants::WALL_2D_SIZE) * Constants::WALL_2D_SIZE) + Constants::WALL_2D_SIZE);
        rayEndY = playerPos.y + (rayEndX - playerPos.x) * tanValue;
        stepX = static_cast<float>(Constants::WALL_2D_SIZE);
        stepY = stepX * tanValue;
      }
      else if (cosf(rayAngle) < -0.001f) // esquerda
      {
        rayEndX = static_cast<float>((static_cast<int>(playerPos.x / Constants::WALL_2D_SIZE) * Constants::WALL_2D_SIZE) - 0.0001f);
        rayEndY = playerPos.y + (rayEndX - playerPos.x) * tanValue;
        stepX = static_cast<float>(-Constants::WALL_2D_SIZE);
        stepY = stepX * tanValue;
      }
      else
      {
        rayEndX = playerPos.x;
        rayEndY = playerPos.y;
        depth = Constants::MAP_SIZE;
      }

      while (depth < Constants::MAP_SIZE)
      {
        int mapX = static_cast<int>(rayEndX) / Constants::WALL_2D_SIZE;
        int mapY = static_cast<int>(rayEndY) / Constants::WALL_2D_SIZE;

        if (mapX >= 0 && mapX < Constants::MAP_SIZE && mapY >= 0 && mapY < Constants::MAP_SIZE)
        {
          if (m_map.isWall(mapX, mapY))
          {
            verticalHitX = rayEndX;
            verticalHitY = rayEndY;
            distVertical = (playerPos.x - verticalHitX) * (playerPos.x - verticalHitX) +
                           (playerPos.y - verticalHitY) * (playerPos.y - verticalHitY);
            break;
          }
          rayEndX += stepX;
          rayEndY += stepY;
          depth++;
        }
        else
          break;
      }
    }

    // ==========================
    // Checagem horizontal
    // ==========================
    {
      int depth = 0;
      float stepX, stepY;
      float inverseTan = 1.0f / tanValue;

      if (sinf(rayAngle) > 0.001f) // baixo
      {
        rayEndY = static_cast<float>((static_cast<int>(playerPos.y / Constants::WALL_2D_SIZE) * Constants::WALL_2D_SIZE) + Constants::WALL_2D_SIZE);
        rayEndX = playerPos.x + (rayEndY - playerPos.y) * inverseTan;
        stepY = static_cast<float>(Constants::WALL_2D_SIZE);
        stepX = stepY * inverseTan;
      }
      else if (sinf(rayAngle) < -0.001f) // cima
      {
        rayEndY = static_cast<float>((static_cast<int>(playerPos.y / Constants::WALL_2D_SIZE) * Constants::WALL_2D_SIZE) - 0.0001f);
        rayEndX = playerPos.x + (rayEndY - playerPos.y) * inverseTan;
        stepY = static_cast<float>(-Constants::WALL_2D_SIZE);
        stepX = stepY * inverseTan;
      }
      else
      {
        rayEndX = playerPos.x;
        rayEndY = playerPos.y;
        depth = Constants::MAP_SIZE;
      }

      while (depth < Constants::MAP_SIZE)
      {
        int mapX = static_cast<int>(rayEndX) / Constants::WALL_2D_SIZE;
        int mapY = static_cast<int>(rayEndY) / Constants::WALL_2D_SIZE;

        if (mapX >= 0 && mapX < Constants::MAP_SIZE && mapY >= 0 && mapY < Constants::MAP_SIZE)
        {
          if (m_map.isWall(mapX, mapY))
          {
            horizontalHitX = rayEndX;
            horizontalHitY = rayEndY;
            distHorizontal = (playerPos.x - horizontalHitX) * (playerPos.x - horizontalHitX) +
                             (playerPos.y - horizontalHitY) * (playerPos.y - horizontalHitY);
            break;
          }
          rayEndX += stepX;
          rayEndY += stepY;
          depth++;
        }
        else
          break;
      }
    }

    // ==========================
    // Escolha do hit mais próximo
    // ==========================
    bool hasHit = (distVertical < 1e6f) || (distHorizontal < 1e6f);
    if (!hasHit)
    {
      rayAngle += fov / static_cast<float>(numRays);
      continue;
    }

    float finalHitX = (distVertical < distHorizontal) ? verticalHitX : horizontalHitX;
    float finalHitY = (distVertical < distHorizontal) ? verticalHitY : horizontalHitY;

    float screenHitX = finalHitX * scaleX;
    float screenHitY = finalHitY * scaleY;

    if (distVertical < distHorizontal)
      SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    else
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_RenderDrawLine(renderer,
                       static_cast<int>(screenPlayerX),
                       static_cast<int>(screenPlayerY),
                       static_cast<int>(screenHitX),
                       static_cast<int>(screenHitY));

    rayAngle += fov / static_cast<float>(numRays);
  }
}

/**
 * @brief Renderiza a visão 3D do jogador
 *
 * Desenha colunas verticais representando paredes, com correção de fish-eye
 * e cores diferentes para parede, teto e chão.
 *
 * @param renderer Ponteiro para SDL_Renderer
 */
void Raycaster::render3DView(SDL_Renderer *renderer) const
{
  const int numRays = Constants::NUM_RAYS;
  const float fov = MathUtils::degToRad(Constants::FOV);
  const Vector2f playerPos = m_player.getPosition();
  const float playerAngle = m_player.getAngle();

  const int mapDisplayWidth = static_cast<int>(Constants::WINDOW_WIDTH * Constants::MAP_VIEW_RATIO);
  const int render3DWidth = Constants::WINDOW_WIDTH - mapDisplayWidth;
  const int wallStripWidth = render3DWidth / numRays;

  float rayAngle = playerAngle - (fov / 2.0f);

  for (int rayIndex = 0; rayIndex < numRays; rayIndex++)
  {
    rayAngle = MathUtils::normalizeAngle(rayAngle);

    float tanValue = tanf(rayAngle);
    float rayEndX, rayEndY;
    float distVertical = 1e6f, distHorizontal = 1e6f;
    float verticalHitX = 0.0f, verticalHitY = 0.0f;
    float horizontalHitX = 0.0f, horizontalHitY = 0.0f;

    // Verificação de colisão vertical
    {
      int depth = 0;
      float stepX, stepY;

      if (cosf(rayAngle) > 0.001f)
      {
        rayEndX = static_cast<float>((static_cast<int>(playerPos.x / Constants::WALL_2D_SIZE) * Constants::WALL_2D_SIZE) + Constants::WALL_2D_SIZE);
        rayEndY = playerPos.y + (rayEndX - playerPos.x) * tanValue;
        stepX = static_cast<float>(Constants::WALL_2D_SIZE);
        stepY = stepX * tanValue;
      }
      else if (cosf(rayAngle) < -0.001f)
      {
        rayEndX = static_cast<float>((static_cast<int>(playerPos.x / Constants::WALL_2D_SIZE) * Constants::WALL_2D_SIZE) - 0.0001f);
        rayEndY = playerPos.y + (rayEndX - playerPos.x) * tanValue;
        stepX = static_cast<float>(-Constants::WALL_2D_SIZE);
        stepY = stepX * tanValue;
      }
      else
      {
        rayEndX = playerPos.x;
        rayEndY = playerPos.y;
        depth = Constants::MAP_SIZE;
      }

      while (depth < Constants::MAP_SIZE)
      {
        int mapX = static_cast<int>(rayEndX) / Constants::WALL_2D_SIZE;
        int mapY = static_cast<int>(rayEndY) / Constants::WALL_2D_SIZE;

        if (mapX >= 0 && mapX < Constants::MAP_SIZE && mapY >= 0 && mapY < Constants::MAP_SIZE)
        {
          if (m_map.isWall(mapX, mapY))
          {
            verticalHitX = rayEndX;
            verticalHitY = rayEndY;
            distVertical = (playerPos.x - verticalHitX) * (playerPos.x - verticalHitX) +
                           (playerPos.y - verticalHitY) * (playerPos.y - verticalHitY);
            break;
          }
          else
          {
            rayEndX += stepX;
            rayEndY += stepY;
            depth++;
          }
        }
        else
          break;
      }
    }

    // Verificação de colisão horizontal
    {
      int depth = 0;
      float stepX, stepY;
      float inverseTan = 1.0f / tanValue;

      if (sinf(rayAngle) > 0.001f)
      {
        rayEndY = static_cast<float>((static_cast<int>(playerPos.y / Constants::WALL_2D_SIZE) * Constants::WALL_2D_SIZE) + Constants::WALL_2D_SIZE);
        rayEndX = playerPos.x + (rayEndY - playerPos.y) * inverseTan;
        stepY = static_cast<float>(Constants::WALL_2D_SIZE);
        stepX = stepY * inverseTan;
      }
      else if (sinf(rayAngle) < -0.001f)
      {
        rayEndY = static_cast<float>((static_cast<int>(playerPos.y / Constants::WALL_2D_SIZE) * Constants::WALL_2D_SIZE) - 0.0001f);
        rayEndX = playerPos.x + (rayEndY - playerPos.y) * inverseTan;
        stepY = static_cast<float>(-Constants::WALL_2D_SIZE);
        stepX = stepY * inverseTan;
      }
      else
      {
        rayEndX = playerPos.x;
        rayEndY = playerPos.y;
        depth = Constants::MAP_SIZE;
      }

      while (depth < Constants::MAP_SIZE)
      {
        int mapX = static_cast<int>(rayEndX) / Constants::WALL_2D_SIZE;
        int mapY = static_cast<int>(rayEndY) / Constants::WALL_2D_SIZE;

        if (mapX >= 0 && mapX < Constants::MAP_SIZE && mapY >= 0 && mapY < Constants::MAP_SIZE)
        {
          if (m_map.isWall(mapX, mapY))
          {
            horizontalHitX = rayEndX;
            horizontalHitY = rayEndY;
            distHorizontal = (playerPos.x - horizontalHitX) * (playerPos.x - horizontalHitX) +
                             (playerPos.y - horizontalHitY) * (playerPos.y - horizontalHitY);
            break;
          }
          rayEndX += stepX;
          rayEndY += stepY;
          depth++;
        }
        else
          break;
      }
    }

    // Cálculo de distância final e desenho
    float distance;
    Uint8 wallShade;
    bool isVerticalHit = (distVertical < distHorizontal);

    if (isVerticalHit)
    {
      distance = sqrtf(distVertical);
      wallShade = 200;
    }
    else
    {
      distance = sqrtf(distHorizontal);
      wallShade = 255;
    }

    // Correção fish-eye
    float angleDiff = rayAngle - playerAngle;
    angleDiff = MathUtils::normalizeAngle(angleDiff);
    distance *= cosf(angleDiff);

    float lineHeight = (Constants::WALL_2D_SIZE * 320.0f) / distance;
    if (lineHeight > static_cast<float>(Constants::WINDOW_HEIGHT))
      lineHeight = static_cast<float>(Constants::WINDOW_HEIGHT);

    float lineOffset = (static_cast<float>(Constants::WINDOW_HEIGHT) / 2.0f) - (lineHeight / 2.0f);

    // Desenho parede, teto e chão
    SDL_SetRenderDrawColor(renderer, wallShade, wallShade, wallShade, 255);
    SDL_Rect wallRect = {mapDisplayWidth + (rayIndex * wallStripWidth), static_cast<int>(lineOffset), wallStripWidth, static_cast<int>(lineHeight)};
    SDL_RenderFillRect(renderer, &wallRect);

    SDL_SetRenderDrawColor(renderer, 50, 50, 100, 255); // teto
    SDL_Rect ceilingRect = {mapDisplayWidth + (rayIndex * wallStripWidth), 0, wallStripWidth, static_cast<int>(lineOffset)};
    SDL_RenderFillRect(renderer, &ceilingRect);

    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255); // chão
    SDL_Rect floorRect = {mapDisplayWidth + (rayIndex * wallStripWidth), static_cast<int>(lineOffset + lineHeight), wallStripWidth, Constants::WINDOW_HEIGHT - static_cast<int>(lineOffset + lineHeight)};
    SDL_RenderFillRect(renderer, &floorRect);

    rayAngle += fov / static_cast<float>(numRays);
  }
}

/**
 * @brief Função de castRay (para uso futuro)
 *
 * Retorna dados de colisão de um raio, mas atualmente não é usada.
 *
 * @param rayAngle Ângulo do raio
 * @return RayHit Estrutura contendo ponto, distância, orientação e sombreado
 */
Raycaster::RayHit Raycaster::castRay(float rayAngle) const
{
  RayHit hit;
  hit.distance = 1e6f;
  hit.point = Vector2f(0, 0);
  hit.isVertical = false;
  hit.shade = 255;

  // A lógica de raycasting completa está em renderRays2D e render3DView
  return hit;
}
