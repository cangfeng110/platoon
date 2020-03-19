#ifndef MODULES_WORLDMODEL_MATH_ARROW2D_H_
#define MODULES_WORLDMODEL_MATH_ARROW2D_H_

#include <limits>
#include <string>
#include <vector>

#include "modules/worldmodel/include/math/vec2d.h"

/**
 * @namespace atd::worldmodel::math
 * @brief atd::worldmodel::math
 */
namespace atd {
namespace worldmodel {
namespace math {

/**
 * @class Arrow2d
 * @brief Rectangular (undirected) arrow in 2-D x,y,heading.
 *
 * This class is referential-agnostic,
 */
class Arrow2d {
 public:
  Arrow2d() = default;
  /**
   * @brief Constructor which takes the center, heading, length and width.
   * @param center The center of the rectangular bounding arrow.
   * @param heading The angle between the x-axis and the heading-axis,
   *        measured counter-clockwise.
   */
  Arrow2d(const Vec2d &center, const double heading);

  /**
   * @brief Constructor which takes the center, heading, length and width.
   * @param x the value of x-axis.
   * @param y the value of y-axis.
   * @param heading The angle between the x-axis and the heading-axis,
   *        measured counter-clockwise.
   */
  Arrow2d(const double x,const double y, const double heading);

  /**
   * @brief Getter of the center of the arrow
   * @return The center of the arrow
   */
  const Vec2d &center() const { return center_; }

  /**
   * @brief Getter of the x-coordinate of the center of the arrow
   * @return The x-coordinate of the center of the arrow
   */
  double center_x() const { return center_.x(); }

  /**
   * @brief Getter of the y-coordinate of the center of the arrow
   * @return The y-coordinate of the center of the arrow
   */
  double center_y() const { return center_.y(); }

  double heading() const { return heading_; }

  //! Setter for x component
  void set_center_x(const double x) { center_.set_x(x); }

  //! Setter for y component
  void set_center_y(const double y) {center_.set_y(y); }

  //! Setter for y component
  void set_heading(const double heading) {heading_=heading; }


  /**
   * @brief Getter of the cosine of the heading
   * @return The cosine of the heading
   */
  double cos_heading() const { return cos_heading_; }

  /**
   * @brief Getter of the sine of the heading
   * @return The sine of the heading
   */
  double sin_heading() const { return sin_heading_; }

  /**
   * @brief Determines the distance between the arrow and a given point
   * @param point The point whose distance to the arrow we wish to compute
   * @return A distance
   */
  double DistanceTo(const Vec2d &point) const;

  /**
   * @brief Determines the distance between two arrowes
   * @param arrow The arrow whose distance to this arrow we want to compute
   * @return A distance
   */
  double DistanceTo(const Arrow2d &arrow) const;

  /**
   * @brief Rotate from center.
   * @param rotate_angle Angle to rotate.
   */
  void RotateFromCenter(const double rotate_angle);

  /**
   * @brief Shifts this arrow by a given vector
   * @param shift_vec The vector determining the shift
   */
  void Shift(const Vec2d &shift_vec);

  /**
   * @brief Gets a human-readable description of the arrow
   * @return A debug-string
   */
  std::string DebugString() const;

 private:
  Vec2d center_;
  double heading_ = 0.0;
  double cos_heading_ = 1.0;
  double sin_heading_ = 0.0;
};

}  // namespace math
}  // namespace worldmodel
}  // namespace atd

#endif /* MODULES_WORLDMODEL_MATH_ARROW2D_H_ */
