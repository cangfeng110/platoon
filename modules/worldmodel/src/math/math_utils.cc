#include "modules/worldmodel/include/math/math_utils.h"
#include "modules/worldmodel/include/math/vec2d.h"

#include <utility>

namespace atd {
namespace worldmodel {
namespace math {



void uniform_slice(double start, double end, uint32_t num,
                   std::vector<double>* sliced) {
    if (!sliced || num == 0) {
        return;
    }
    const double delta = (end - start) / num;
    sliced->resize(num + 1);
    double s = start;
    for (uint32_t i = 0; i < num; ++i, s += delta) {
        sliced->at(i) = s;
    }
    sliced->at(num) = end;
}

double Sqr(const double x) { return x * x; }

double CrossProd(const Vec2d& start_point, const Vec2d& end_point_1,
                 const Vec2d& end_point_2) {
  return (end_point_1 - start_point).CrossProd(end_point_2 - start_point);
}

double InnerProd(const Vec2d& start_point, const Vec2d& end_point_1,
                 const Vec2d& end_point_2) {
  return (end_point_1 - start_point).InnerProd(end_point_2 - start_point);
}

double CrossProd(const double x0, const double y0, const double x1,
                 const double y1) {
  return x0 * y1 - x1 * y0;
}

double InnerProd(const double x0, const double y0, const double x1,
                 const double y1) {
  return x0 * x1 + y0 * y1;
}

double WrapAngle(const double angle) {
  const double new_angle = std::fmod(angle, M_PI * 2.0);
  return new_angle < 0 ? new_angle + M_PI * 2.0 : new_angle;
}

double NormalizeAngle(const double angle) {
  double a = std::fmod(angle + M_PI, 2.0 * M_PI);
  if (a < 0.0) {
    a += (2.0 * M_PI);
  }
  return a - M_PI;
}

double AngleDiff(const double from, const double to) {
  return NormalizeAngle(to - from);
}

int RandomInt(const int s, const int t, unsigned int rand_seed) {
  if (s >= t) {
    return s;
  }
  return s + rand_r(&rand_seed) % (t - s + 1);
}

double RandomDouble(const double s, const double t, unsigned int rand_seed) {
  return s + (t - s) / 16383.0 * (rand_r(&rand_seed) & 16383);
}

// Gaussian
double Gaussian(const double u, const double std, const double x) {
  return (1.0 / std::sqrt(2 * M_PI * std * std)) *
         std::exp(-(x - u) * (x - u) / (2 * std * std));
}

// Sigmoid
double Sigmoid(const double x) { return 1.0 / (1.0 + std::exp(-x)); }

}  // namespace math
}  // namespace worldmodel
}  // namespace atd
