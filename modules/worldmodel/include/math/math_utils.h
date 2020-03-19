#ifndef MODULES_WORLDMODEL_MATH_MATH_UTILS_H_
#define MODULES_WORLDMODEL_MATH_MATH_UTILS_H_

#include <limits>
#include <utility>

#include "modules/worldmodel/include/math/vec2d.h"
#include "modules/ipc/message/protobuf/common/geometry.pb.h"
#include "modules/ipc/message/protobuf/common/pnc_point.pb.h"
#include "modules/ipc/message/protobuf/perception/perception_obstacle.pb.h"

/**
 * @namespace atd::worldmodel::math
 * @brief atd::worldmodel::math
 */
namespace atd {
namespace worldmodel {
namespace math {

using atd::common::SLPoint;
using atd::common::Point3D;
using atd::common::PointENU;
using atd::common::SpeedPoint;
using atd::common::PathPoint;
/**
 * @brief create a SL point
 * @param s the s value
 * @param l the l value
 * @return a SLPoint instance
 */
SLPoint MakeSLPoint(const double s, const double l);

template <typename T>
math::Vec2d MakeVec2d(const T& t) {
  return math::Vec2d(t.x(), t.y());
}

Point3D MakePoint3D(const double x, const double y,const double z);

PointENU MakePointENU(const double x, const double y, const double z);

PointENU MakePointENU(const math::Vec2d& xy);

//atd::perception::Point MakePerceptionPoint(const double x, const double y,const double z);

SpeedPoint MakeSpeedPoint(const double s, const double t, const double v,
                          const double a, const double da);

PathPoint MakePathPoint(const double x, const double y, const double z,
                        const double theta, const double kappa,
                        const double dkappa, const double ddkappa);

/**
 * uniformly slice a segment [start, end] to num + 1 pieces
 * the result sliced will contain the n + 1 points that slices the provided
 * segment. `start` and `end` will be the first and last element in `sliced`.
 */
void uniform_slice(double start, double end, uint32_t num,
                   std::vector<double>* sliced);


double Sqr(const double x);

/**
 * @brief Cross product between two 2-D vectors from the worldmodel start point,
 *        and end at two other points.
 * @param start_point The worldmodel start point of two vectors in 2-D.
 * @param end_point_1 The end point of the first vector.
 * @param end_point_2 The end point of the second vector.
 *
 * @return The cross product result.
 */
double CrossProd(const Vec2d &start_point, const Vec2d &end_point_1,
                 const Vec2d &end_point_2);

/**
 * @brief Inner product between two 2-D vectors from the worldmodel start point,
 *        and end at two other points.
 * @param start_point The worldmodel start point of two vectors in 2-D.
 * @param end_point_1 The end point of the first vector.
 * @param end_point_2 The end point of the second vector.
 *
 * @return The inner product result.
 */
double InnerProd(const Vec2d &start_point, const Vec2d &end_point_1,
                 const Vec2d &end_point_2);

/**
 * @brief Cross product between two vectors.
 *        One vector is formed by 1st and 2nd parameters of the function.
 *        The other vector is formed by 3rd and 4th parameters of the function.
 * @param x0 The x coordinate of the first vector.
 * @param y0 The y coordinate of the first vector.
 * @param x1 The x coordinate of the second vector.
 * @param y1 The y coordinate of the second vector.
 *
 * @return The cross product result.
 */
double CrossProd(const double x0, const double y0, const double x1,
                 const double y1);

/**
 * @brief Inner product between two vectors.
 *        One vector is formed by 1st and 2nd parameters of the function.
 *        The other vector is formed by 3rd and 4th parameters of the function.
 * @param x0 The x coordinate of the first vector.
 * @param y0 The y coordinate of the first vector.
 * @param x1 The x coordinate of the second vector.
 * @param y1 The y coordinate of the second vector.
 *
 * @return The inner product result.
 */
double InnerProd(const double x0, const double y0, const double x1,
                 const double y1);

/**
 * @brief Wrap angle to [0, 2 * PI).
 * @param angle the original value of the angle.
 * @return The wrapped value of the angle.
 */
double WrapAngle(const double angle);

/**
 * @brief Normalize angle to [-PI, PI).
 * @param angle the original value of the angle.
 * @return The normalized value of the angle.
 */
double NormalizeAngle(const double angle);

/**
 * @brief Calculate the difference between angle from and to
 * @param from the start angle
 * @param from the end angle
 * @return The difference between from and to. The range is between [0, PI).
 */
double AngleDiff(const double from, const double to);

/**
 * @brief Get a random integer between two integer values by a random seed.
 * @param s The lower bound of the random integer.
 * @param t The upper bound of the random integer.
 * @param random_seed The random seed.
 * @return A random integer between s and t based on the input random_seed.
 */
int RandomInt(const int s, const int t, unsigned int rand_seed = 1);

/**
 * @brief Get a random double between two integer values by a random seed.
 * @param s The lower bound of the random double.
 * @param t The upper bound of the random double.
 * @param random_seed The random seed.
 * @return A random double between s and t based on the input random_seed.
 */
double RandomDouble(const double s, const double t, unsigned int rand_seed = 1);


// Gaussian
double Gaussian(const double u, const double std, const double x);

// Sigmoid
double Sigmoid(const double x);



}  // namespace math
}  // namespace worldmodel
}  // namespace atd

#endif  // MODULES_WORLDMODEL_MATH_MATH_UTILS_H_
