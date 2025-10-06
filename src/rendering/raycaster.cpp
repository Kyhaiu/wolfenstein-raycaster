#include <rendering/raycaster.hpp>
#include <utils/mathUtils.hpp>
#include <core/constants.hpp>

Raycaster::Raycaster(const Map &map, const Player &player)
    : m_map(map), m_player(player)
{
}

void Raycaster::renderRays2D(SDL_Renderer *renderer) const
{
  const int numRays = Constants::NUM_RAYS;
  const float fov = MathUtils::degToRad(Constants::FOV);
  Vector2f playerPos = m_player.getPosition();
  float playerAngle = m_player.getAngle();

  float rayAngle = playerAngle - (fov / 2.0f);

  for (int rayIndex = 0; rayIndex < numRays; rayIndex++)
  {
    // Normaliza o ângulo do raio
    rayAngle = MathUtils::normalizeAngle(rayAngle);

    float tanValue = tanf(rayAngle);
    float rayEndX, rayEndY;
    float distVertical = 1e6f, distHorizontal = 1e6f;
    float verticalHitX = 0.0f, verticalHitY = 0.0f;
    float horizontalHitX = 0.0f, horizontalHitY = 0.0f;

    // ====================================================================
    // VERIFICAÇÃO DE COLISÃO VERTICAL
    // ====================================================================
    {
      int depth = 0;
      float stepX, stepY;

      // Determina direção inicial baseada no ângulo do raio
      if (cosf(rayAngle) > 0.001f)
      {
        // Raio apontando para a direita
        rayEndX = static_cast<float>((static_cast<int>(playerPos.x / Constants::WALL_2D_SIZE) * Constants::WALL_2D_SIZE) + Constants::WALL_2D_SIZE);
        rayEndY = playerPos.y + (rayEndX - playerPos.x) * tanValue;
        stepX = static_cast<float>(Constants::WALL_2D_SIZE);
        stepY = stepX * tanValue;
      }
      else if (cosf(rayAngle) < -0.001f)
      {
        // Raio apontando para a esquerda
        rayEndX = static_cast<float>((static_cast<int>(playerPos.x / Constants::WALL_2D_SIZE) * Constants::WALL_2D_SIZE) - 0.0001f);
        rayEndY = playerPos.y + (rayEndX - playerPos.x) * tanValue;
        stepX = static_cast<float>(-Constants::WALL_2D_SIZE);
        stepY = stepX * tanValue;
      }
      else
      {
        // Raio apontando para cima/baixo (vertical puro)
        rayEndX = playerPos.x;
        rayEndY = playerPos.y;
        depth = Constants::MAP_SIZE; // Força saída do loop
      }

      // Rastreia o raio até encontrar uma parede ou sair do mapa
      while (depth < Constants::MAP_SIZE)
      {
        int mapX = static_cast<int>(rayEndX) / Constants::WALL_2D_SIZE;
        int mapY = static_cast<int>(rayEndY) / Constants::WALL_2D_SIZE;

        // Verifica se está dentro dos limites do mapa
        if (mapX >= 0 && mapX < Constants::MAP_SIZE && mapY >= 0 && mapY < Constants::MAP_SIZE)
        {
          if (m_map.isWall(mapX, mapY))
          {
            // Colisão com parede vertical encontrada
            verticalHitX = rayEndX;
            verticalHitY = rayEndY;
            distVertical = (playerPos.x - verticalHitX) * (playerPos.x - verticalHitX) +
                           (playerPos.y - verticalHitY) * (playerPos.y - verticalHitY);
            break;
          }
          else
          {
            // Move para o próximo ponto de verificação
            rayEndX += stepX;
            rayEndY += stepY;
            depth++;
          }
        }
        else
        {
          break; // Sai do mapa
        }
      }
    }

    // ====================================================================
    // VERIFICAÇÃO DE COLISÃO HORIZONTAL
    // ====================================================================
    {
      int depth = 0;
      float stepX, stepY;
      float inverseTan = 1.0f / tanValue;

      // Determina direção inicial baseada no ângulo do raio
      if (sinf(rayAngle) > 0.001f)
      {
        // Raio apontando para baixo
        rayEndY = static_cast<float>((static_cast<int>(playerPos.y / Constants::WALL_2D_SIZE) * Constants::WALL_2D_SIZE) + Constants::WALL_2D_SIZE);
        rayEndX = playerPos.x + (rayEndY - playerPos.y) * inverseTan;
        stepY = static_cast<float>(Constants::WALL_2D_SIZE);
        stepX = stepY * inverseTan;
      }
      else if (sinf(rayAngle) < -0.001f)
      {
        // Raio apontando para cima
        rayEndY = static_cast<float>((static_cast<int>(playerPos.y / Constants::WALL_2D_SIZE) * Constants::WALL_2D_SIZE) - 0.0001f);
        rayEndX = playerPos.x + (rayEndY - playerPos.y) * inverseTan;
        stepY = static_cast<float>(-Constants::WALL_2D_SIZE);
        stepX = stepY * inverseTan;
      }
      else
      {
        // Raio apontando para esquerda/direita (horizontal puro)
        rayEndX = playerPos.x;
        rayEndY = playerPos.y;
        depth = Constants::MAP_SIZE; // Força saída do loop
      }

      // Rastreia o raio até encontrar uma parede ou sair do mapa
      while (depth < Constants::MAP_SIZE)
      {
        int mapX = static_cast<int>(rayEndX) / Constants::WALL_2D_SIZE;
        int mapY = static_cast<int>(rayEndY) / Constants::WALL_2D_SIZE;

        // Verifica se está dentro dos limites do mapa
        if (mapX >= 0 && mapX < Constants::MAP_SIZE && mapY >= 0 && mapY < Constants::MAP_SIZE)
        {
          if (m_map.isWall(mapX, mapY))
          {
            // Colisão com parede horizontal encontrada
            horizontalHitX = rayEndX;
            horizontalHitY = rayEndY;
            distHorizontal = (playerPos.x - horizontalHitX) * (playerPos.x - horizontalHitX) +
                             (playerPos.y - horizontalHitY) * (playerPos.y - horizontalHitY);
            break;
          }
          else
          {
            // Move para o próximo ponto de verificação
            rayEndX += stepX;
            rayEndY += stepY;
            depth++;
          }
        }
        else
        {
          break; // Sai do mapa
        }
      }
    }

    // ====================================================================
    // ESCOLHA DO PONTO DE COLISÃO MAIS PRÓXIMO
    // ====================================================================
    float finalHitX, finalHitY;
    bool isVerticalHit = (distVertical < distHorizontal);

    if (isVerticalHit)
    {
      finalHitX = verticalHitX;
      finalHitY = verticalHitY;
      SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Azul para paredes verticais
    }
    else
    {
      finalHitX = horizontalHitX;
      finalHitY = horizontalHitY;
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Verde para paredes horizontais
    }

    // Desenha o raio no mapa 2D
    SDL_RenderDrawLine(renderer,
                       static_cast<int>(playerPos.x),
                       static_cast<int>(playerPos.y),
                       static_cast<int>(finalHitX),
                       static_cast<int>(finalHitY));

    // Avança para o próximo raio
    rayAngle += fov / static_cast<float>(numRays);
  }
}

void Raycaster::render3DView(SDL_Renderer *renderer) const
{
  const int numRays = Constants::NUM_RAYS;
  const float fov = MathUtils::degToRad(Constants::FOV);
  Vector2f playerPos = m_player.getPosition();
  float playerAngle = m_player.getAngle();

  float rayAngle = playerAngle - (fov / 2.0f);

  const int mapDisplayWidth = Constants::MAP_SIZE * Constants::WALL_2D_SIZE;
  const int wallStripWidth = (Constants::WINDOW_WIDTH - mapDisplayWidth) / numRays;

  // Lança raios para cada coluna da visão 3D
  for (int rayIndex = 0; rayIndex < numRays; rayIndex++)
  {
    // Normaliza o ângulo do raio
    rayAngle = MathUtils::normalizeAngle(rayAngle);

    float tanValue = tanf(rayAngle);
    float rayEndX, rayEndY;
    float distVertical = 1e6f, distHorizontal = 1e6f;
    float verticalHitX = 0.0f, verticalHitY = 0.0f;
    float horizontalHitX = 0.0f, horizontalHitY = 0.0f;

    // ====================================================================
    // DETECÇÃO DE COLISÃO VERTICAL
    // ====================================================================
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
        {
          break;
        }
      }
    }

    // ====================================================================
    // DETECÇÃO DE COLISÃO HORIZONTAL
    // ====================================================================
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
          else
          {
            rayEndX += stepX;
            rayEndY += stepY;
            depth++;
          }
        }
        else
        {
          break;
        }
      }
    }

    // ====================================================================
    // CÁLCULO DA ALTURA DA PAREDE 3D
    // ====================================================================
    float distance;
    Uint8 wallShade;
    bool isVerticalHit = (distVertical < distHorizontal);

    if (isVerticalHit)
    {
      distance = sqrtf(distVertical);
      wallShade = 200; // Tom mais escuro para paredes verticais
    }
    else
    {
      distance = sqrtf(distHorizontal);
      wallShade = 255; // Tom mais claro para paredes horizontais
    }

    // Correção do efeito "fish-eye"
    float angleDifference = rayAngle - playerAngle;
    angleDifference = MathUtils::normalizeAngle(angleDifference);
    distance *= cosf(angleDifference);

    // Calcula a altura da linha da parede
    float lineHeight = (Constants::WALL_2D_SIZE * 320.0f) / distance;
    if (lineHeight > static_cast<float>(Constants::WINDOW_HEIGHT))
      lineHeight = static_cast<float>(Constants::WINDOW_HEIGHT);

    float lineOffset = (static_cast<float>(Constants::WINDOW_HEIGHT) / 2.0f) - (lineHeight / 2.0f);

    // ====================================================================
    // RENDERIZAÇÃO DA PAREDE 3D
    // ====================================================================
    SDL_SetRenderDrawColor(renderer, wallShade, wallShade, wallShade, 255);

    SDL_Rect wallRect = {
        mapDisplayWidth + (rayIndex * wallStripWidth), // Posição X (lado direito da tela)
        static_cast<int>(lineOffset),                  // Posição Y (topo da parede)
        wallStripWidth,                                // Largura da faixa
        static_cast<int>(lineHeight)                   // Altura da parede
    };

    SDL_RenderFillRect(renderer, &wallRect);

    // Desenha teto
    SDL_SetRenderDrawColor(renderer, 50, 50, 100, 255);
    SDL_Rect ceilingRect = {
        mapDisplayWidth + (rayIndex * wallStripWidth),
        0,
        wallStripWidth,
        static_cast<int>(lineOffset)};
    SDL_RenderFillRect(renderer, &ceilingRect);

    // Desenha chão
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_Rect floorRect = {
        mapDisplayWidth + (rayIndex * wallStripWidth),
        static_cast<int>(lineOffset + lineHeight),
        wallStripWidth,
        Constants::WINDOW_HEIGHT - static_cast<int>(lineOffset + lineHeight)};
    SDL_RenderFillRect(renderer, &floorRect);

    // Avança para o próximo raio
    rayAngle += fov / static_cast<float>(numRays);
  }
}

Raycaster::RayHit Raycaster::castRay(float rayAngle) const
{
  RayHit hit;
  hit.distance = 1e6f;
  hit.point = Vector2f(0, 0);
  hit.isVertical = false;
  hit.shade = 255;

  // Esta função pode ser usada para raycasting mais preciso se necessário
  // Por enquanto, as funções renderRays2D e render3DView contêm a lógica completa

  return hit;
}