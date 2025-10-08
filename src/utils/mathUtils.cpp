#include <utils/mathUtils.hpp>

/**
 * @brief Converte graus para radianos
 *
 * @param degrees Ângulo em graus
 * @return float Ângulo convertido em radianos
 */
float MathUtils::degToRad(float degrees)
{
  return degrees * (Constants::PI / 180.0f);
}

/**
 * @brief Normaliza um ângulo para o intervalo [0, 2π)
 *
 * @param angle Ângulo em radianos
 * @return float Ângulo normalizado
 *
 * @note Usa fmodf para limitar o valor e ajusta valores negativos.
 */
float MathUtils::normalizeAngle(float angle)
{
  angle = fmodf(angle, 2.0f * Constants::PI);
  if (angle < 0.0f)
    angle += 2.0f * Constants::PI;
  return angle;
}

/**
 * @brief Calcula a distância euclidiana entre dois pontos 2D
 *
 * @param x1 Coordenada X do ponto 1
 * @param y1 Coordenada Y do ponto 1
 * @param x2 Coordenada X do ponto 2
 * @param y2 Coordenada Y do ponto 2
 * @return float Distância entre os dois pontos
 */
float MathUtils::distance(float x1, float y1, float x2, float y2)
{
  return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
