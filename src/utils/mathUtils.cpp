#include <utils/mathUtils.hpp>

float MathUtils::degToRad(float degrees)
{
  return degrees * (Constants::PI / 180.0f);
}

float MathUtils::normalizeAngle(float angle)
{
  angle = fmodf(angle, 2.0f * Constants::PI);
  if (angle < 0.0f)
    angle += 2.0f * Constants::PI;
  return angle;
}

float MathUtils::distance(float x1, float y1, float x2, float y2)
{
  return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}