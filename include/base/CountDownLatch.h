#ifndef _INCLUDE_BASE_COUNTDOWNLATCH_H_
#define _INCLUDE_BASE_COUNTDOWNLATCH_H_

#include <mutex>
#include <condition_variable>

#include "include/base/NonCopyable.h"

namespace platoon
{

namespace base
{

class CountDownLatch : public NonCopyable
{
public:
    CountDownLatch(int count);
    ~CountDownLatch();

    void wait();

    void countDown();

    int getCount();

private:
    int _count;
    std::mutex _mutex;
    std::condition_variable _conditionVariable;
};

}

}

#endif // _TJP_BASE_COUNTDOWNLATCH_H_
