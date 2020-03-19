#ifndef MODULES_COMMON_MACRO_H_
#define MODULES_COMMON_MACRO_H_

#include <iomanip>

#define DISALLOW_COPY_AND_ASSIGN(classname) \
 private:                                   \
  classname(const classname &);             \
  classname &operator=(const classname &);

#define DISALLOW_IMPLICIT_CONSTRUCTORS(classname) \
 private:                                         \
  classname();                                    \
  DISALLOW_COPY_AND_ASSIGN(classname);

#define DECLARE_SINGLETON(classname)        \
 public:                                    \
  static classname *instance() {            \
    static classname instance;              \
    return &instance;                       \
  }                                         \
  DISALLOW_IMPLICIT_CONSTRUCTORS(classname) \
 private:


#define HAS_MEM_FUNC(func, name)
template<typename T, typename Sign>                                 \
  struct name {                                                       \
      typedef char yes[1];                                            \
      typedef char no [2];                                            \
      template <typename U, U> struct type_check;                     \
      template <typename _1> static yes &chk(type_check<Sign, &_1::func > *); \
      template <typename   > static no  &chk(...);                    \
      static bool const value = sizeof(chk<T>(0)) == sizeof(yes);     \
  };
#endif  // MODULES_COMMON_MACRO_H_
