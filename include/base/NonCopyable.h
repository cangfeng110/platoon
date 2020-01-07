#ifndef _TJP_BASE_NONCOPYABLE_H_
#define _TJP_BASE_NONCOPYABLE_H_

namespace platoon
{
namespace base
{

class NonCopyable
{
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable& ) = delete;
    NonCopyable& operator=(const NonCopyable &) = delete;
};

}

}

#endif // _TJP_BASE_NONCOPYABLE_H_
