#ifndef MODULES_COMMON_MATH_MATH_UTILS_H_
#define MODULES_COMMON_MATH_MATH_UTILS_H_

#include <limits>
#include <utility>

/**
 * @namespace atd::common::math
 * @brief atd::common::math
 */
namespace atd {
namespace common {
namespace math {

/**
 * @brief Compute squared value.
 * @param value The target value to get its squared value.
 * @return Squared value of the input value.
 */
template <typename T>
inline T Square(const T value) {
  return value * value;
}

/**
 * @brief Clamp a value between two bounds.
 *        If the value goes beyond the bounds, return one of the bounds,
 *        otherwise, return the original value.
 * @param value The original value to be clamped.
 * @param bound1 One bound to clamp the value.
 * @param bound2 The other bound to clamp the value.
 * @return The clamped value.
 */
template <typename T>
T Clamp(const T value, T bound1, T bound2) {
  if (bound1 > bound2) {
    std::swap(bound1, bound2);
  }

  if (value < bound1) {
    return bound1;
  } else if (value > bound2) {
    return bound2;
  }
  return value;
}

}  // namespace math
}  // namespace common
}  // namespace atd

#endif  // MODULES_common_MATH_MATH_UTILS_H_
