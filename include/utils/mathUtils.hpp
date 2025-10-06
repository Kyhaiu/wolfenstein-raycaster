#pragma once

#include <core/constants.hpp>
#include <core/types.hpp>
#include <cmath>

namespace MathUtils
{
  float degToRad(float degrees);
  float normalizeAngle(float angle);
  float distance(float x1, float y1, float x2, float y2);
  Vector2f rotatePoint(float x, float y, float angle);
}
