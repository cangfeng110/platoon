#ifndef MODULES_WORLDMODEL_MATH_QUATERNION_H_
#define MODULES_WORLDMODEL_MATH_QUATERNION_H_

#include "modules/worldmodel/include/math/math_utils.h"
#include "modules/worldmodel/include/math/euler_angles_zxy.h"
#include <cmath>

#include "eigen3/Eigen/Dense"
#include "eigen3/Eigen/Geometry"


#include "modules/ipc/message/protobuf/common/geometry.pb.h"

/**
 * @namespace atd::worldmodel::math
 * @brief atd::worldmodel::math
 */
namespace atd {
namespace worldmodel {
namespace math {

using atd::common::Quaternion;
/*
 * @brief Returns heading (in radians) in [-PI, PI), with 0 being East.
 * Note that x/y/z is East/North/Up.
 *
 * @param qw Quaternion w-coordinate
 * @param qx Quaternion x-coordinate
 * @param qy Quaternion y-coordinate
 * @param qz Quaternion z-coordinate
 *
 * @return Heading encoded by given quaternion
 */
inline double QuaternionToHeading(const double qw, const double qx,
                                  const double qy, const double qz) {
  EulerAnglesZXYd euler_angles(qw, qx, qy, qz);
  // euler_angles.yaw() is zero when the car is pointing North, but
  // the heading is zero when the car is pointing East.
  return NormalizeAngle(euler_angles.yaw() + M_PI_2);
}

/*
 * @brief Returns heading (in radians) in [-PI, PI), with 0 being East.
 * Note that x/y/z is East/North/Up.
 *
 * @param q Eigen::Quaternion
 *
 * @return Heading encoded by given quaternion
 */
template <typename T>
inline double QuaternionToHeading(const Eigen::Quaternion<T> &q) {
  return QuaternionToHeading(q.w(), q.x(), q.y(), q.z());
}

/*
 * @brief Returns a quaternion encoding a rotation with zero roll, zero pitch,
 * and the specified heading/yaw.
 * Note that heading is zero at East and yaw is zero at North.
 * Satisfies QuaternionToHeading(HeadingToQuaternion(h)) = h.
 *
 * @param heading The heading to encode in the rotation
 *
 * @return Quaternion encoding rotation by given heading
 */
template <typename T>
inline Eigen::Quaternion<T> HeadingToQuaternion(const double heading) {
  // Note that heading is zero at East and yaw is zero at North.
  EulerAnglesZXY<T> euler_angles(heading - M_PI_2);
  return euler_angles.ToQuaternion();
}

/*
 * @brief Applies the rotation defined by a quaternion to a given vector.
 * Note that x/y/z is East/North/Up.
 *
 * @param orientation Quaternion
 * @param original Vector (in East-North-Up frame)
 *
 * @return Rotated vector
 */
inline Eigen::Vector3d QuaternionRotate(const Quaternion &orientation,
                                        const Eigen::Vector3d &original) {
  Eigen::Quaternion<double> quaternion(orientation.qw(), orientation.qx(),
                                       orientation.qy(), orientation.qz());
  return quaternion.toRotationMatrix() * original;
}

inline Eigen::Vector3d InverseQuaternionRotate(const Quaternion &orientation,
                                               const Eigen::Vector3d &rotated) {
  Eigen::Quaternion<double> quaternion(orientation.qw(), orientation.qx(),
                                       orientation.qy(), orientation.qz());
  return quaternion.toRotationMatrix().inverse() * rotated;
}

}  // namespace math
}  // namespace worldmodel
}  // namespace atd

#endif /* MODULES_WORLDMODEL_MATH_QUATERNION_H_ */
