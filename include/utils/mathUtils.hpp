#pragma once // Garante inclusão única do arquivo

#include <core/constants.hpp> // Constantes globais (como PI, FOV, etc.)
#include <core/types.hpp>     // Tipos definidos no projeto (ex: Vector2f)
#include <cmath>              // Funções matemáticas padrão (cos, sin, sqrt, etc.)

// =====================
// Namespace MathUtils
// =====================
// Contém funções utilitárias para cálculos matemáticos comuns
// no contexto do jogo, como conversão de ângulos, normalização e rotação de pontos.
namespace MathUtils
{
  // Converte graus para radianos
  float degToRad(float degrees);

  // Normaliza um ângulo para o intervalo [0, 2*PI)
  // Útil para evitar ângulos negativos ou maiores que 360 graus
  float normalizeAngle(float angle);

  // Calcula a distância euclidiana entre dois pontos (x1, y1) e (x2, y2)
  float distance(float x1, float y1, float x2, float y2);

  // Rotaciona um ponto (x, y) em torno da origem pelo ângulo fornecido
  Vector2f rotatePoint(float x, float y, float angle);
}
