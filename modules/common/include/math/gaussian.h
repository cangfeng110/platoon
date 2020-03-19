#ifndef MODULES_COMMON_MATH_GAUSSIAN_H_
#define MODULES_COMMON_MATH_GAUSSIAN_H_

#include <limits>
#include <utility>
#include <cmath>

/**
 * @namespace atd::common::math
 * @brief atd::common::math
 */
namespace atd {
namespace common {
namespace math {

double Normal(double z);

double Normal(double z,double sigma);

double Normal(double z,double mu,double sigma);


/***************************************************************/
/* 返回标准正态分布的累积函数，该分布的平均值为 0，标准偏差为 1。                           */
/***************************************************************/

double NormSDist(const double z);

/***************************************************************/
/* 返回标准正态分布累积函数的逆函数。该分布的平均值为 0，标准偏差为 1。                    */
/***************************************************************/
double normsinv(const double p);

double getPArea(double z,float gap,double sigma);


}  // namespace math
}  // namespace common
}  // namespace atd

#endif  // MODULES_COMMON_MATH_GAUSSIAN_H_
