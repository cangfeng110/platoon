#include "modules/worldmodel/include/math/arrow2d.h"
#include <algorithm>
#include <cmath>
#include <utility>

#include "modules/common/include/util/string_util.h"
#include "modules/worldmodel/include/math/math_utils.h"

namespace atd {
namespace worldmodel {
namespace math {
namespace {

double PtSegDistance(double query_x, double query_y, double start_x,
                     double start_y, double end_x, double end_y,
                     double length) {
  const double x0 = query_x - start_x;
  const double y0 = query_y - start_y;
  const double dx = end_x - start_x;
  const double dy = end_y - start_y;
  const double proj = x0 * dx + y0 * dy;
  if (proj <= 0.0) {
    return hypot(x0, y0);
  }
  if (proj >= length * length) {
    return hypot(x0 - dx, y0 - dy);
  }
  return std::abs(x0 * dy - y0 * dx) / length;
}

}  // namespace

Arrow2d::Arrow2d(const Vec2d &center, const double heading)
    : center_(center),
      heading_(heading),
      cos_heading_(cos(heading)),
      sin_heading_(sin(heading)) {
}

Arrow2d::Arrow2d(const double x, const double y, const double heading)
    :center_(x,y),
      heading_(heading),
      cos_heading_(cos(heading)),
      sin_heading_(sin(heading)) {

}

double Arrow2d::DistanceTo(const Vec2d &point) const {
  return -1;
}

double Arrow2d::DistanceTo(const Arrow2d &arrow) const {
  return -1;
}

void Arrow2d::RotateFromCenter(const double rotate_angle) {
  heading_ = NormalizeAngle(heading_ + rotate_angle);
  cos_heading_ = std::cos(heading_);
  sin_heading_ = std::sin(heading_);
}

void Arrow2d::Shift(const Vec2d &shift_vec) {
  center_ += shift_vec;
}

std::string Arrow2d::DebugString() const {
  return atd::common::util::StrCat("Arrow2d ( center = ", center_.DebugString(),
                      "  heading = ", heading_, " )");
}

}  // namespace math
}  // namespace worldmodel
}  // namespace atd
