#ifndef MODULES_COMMON_MATH_SIN_TABLE_H_
#define MODULES_COMMON_MATH_SIN_TABLE_H_

/**
 * @namespace atd::common::math
 * @brief atd::common::math
 */
namespace atd {
namespace common {
namespace math {

//! Used by Angle class to speed-up computation of trigonometric functions.
#define SIN_TABLE_SIZE 16385
extern const float SIN_TABLE[SIN_TABLE_SIZE];

}  // namespace math
}  // namespace common
}  // namespace atd

#endif /* MODULES_COMMON_MATH_SIN_TABLE_H_ */
