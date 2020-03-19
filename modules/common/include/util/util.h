#ifndef MODULES_COMMON_UTIL_UTIL_H_
#define MODULES_COMMON_UTIL_UTIL_H_

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>
#include <cmath>
#include "google/protobuf/util/message_differencer.h"

namespace atd {
namespace common {
namespace util {

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename ProtoA, typename ProtoB>
bool IsProtoEqual(const ProtoA& a, const ProtoB& b) {
  return google::protobuf::util::MessageDifferencer::Equals(a, b);
}

struct PairHash {
  template <typename T, typename U>
  size_t operator()(const std::pair<T, U>& pair) const {
    return std::hash<T>()(pair.first) ^ std::hash<U>()(pair.second);
  }
};


template <typename T>
bool WithinBound(T start, T end, T value) {
  return value >= start && value <= end;
}


template <typename T>
T MaxElement(T element1, T element2){
  if(element1>element2){
      return element1;
  }
  else{
      return element2;
  }
}

template <typename T>
T MinElement(T element1, T element2) {
  if(element1<element2){
      return element1;
  }
  else{
      return element2;
  }
}


template <typename Container>
typename Container::value_type MaxElement(const Container& elements) {
  return *std::max_element(elements.begin(), elements.end());
}

template <typename Container>
typename Container::value_type MinElement(const Container& elements) {
  return *std::min_element(elements.begin(), elements.end());
}

template <typename T>
std::unordered_set<T> Intersection(const std::unordered_set<T>& s1,
                                   const std::unordered_set<T>& s2) {
  if (s1.size() < s2.size()) {
    std::unordered_set<T> result;
    for (const auto& v : s1) {
      if (s2.count(v) > 0) {
        result.insert(v);
      }
    }
    return result;
  } else {
    return intersection(s2, s1);
  }
}

/**
 * calculate the distance beteween Point u and Point v, which are all have
 * member function x() and y() in XY dimension.
 * @param u one point that has member function x() and y().
 * @param b one point that has member function x() and y().
 * @return sqrt((u.x-v.x)^2 + (u.y-v.y)^2), i.e., the Euclid distance on XY
 * dimension.
 */
template <typename U, typename V>
double DistanceXY(const U& u, const V& v) {
  return std::hypot(u.x() - v.x(), u.y() - v.y());
}

/**
 * Check if two points u and v are the same point on XY dimension.
 * @param u one point that has member function x() and y().
 * @param v one point that has member function x() and y().
 * @return sqrt((u.x-v.x)^2 + (u.y-v.y)^2) < epsilon, i.e., the Euclid distance
 * on XY dimension.
 */
template <typename U, typename V>
bool SamePointXY(const U& u, const V& v) {
  constexpr double kMathEpsilonSqr = 1e-8 * 1e-8;
  return (u.x() - v.x()) * (u.x() - v.x()) < kMathEpsilonSqr &&
         (u.y() - v.y()) * (u.y() - v.y()) < kMathEpsilonSqr;
}

}  // namespace util
}  // namespace common
}  // namespace atd

#endif  // MODULES_COMMON_UTIL_UTIL_H_
