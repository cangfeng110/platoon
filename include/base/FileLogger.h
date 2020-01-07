#ifndef _INCLUDE_BASE_FILELOGGER_H_
#define _INCLUDE_BASE_FILELOGGER_H_

#include <vector>
#include <memory>
#include <mutex>

#include "include/base/Timestamp.h"
#include "include/base/NonCopyable.h"

namespace platoon
{
namespace base
{

class FileLogger : public NonCopyable
{
public:

    FileLogger(int64_t flushIntervalSecond = 2,
               bool threadSafe = false,
               const std::string& path = "",
               int64_t bufferSize = 4096,
               int64_t rollSizeByte = 1024 * 1024 * 1024,
               int64_t rollIntervalSecond = 24 * 60 * 60);
    ~FileLogger();

    void log(const char* data, int64_t size);

    void flush(Timestamp now);

    int64_t logSize();

private:

    void roll(Timestamp now);
    void flushWithoutLock(Timestamp now);

    static std::string logFileName(const std::string& path);

    int64_t _rollSizeByte;
    int64_t _rollIntervalSecond;
    int64_t _flushIntervalSecond;
    int64_t _bufferSize;
    const std::string _path;

    Timestamp _lastRoll;
    Timestamp _lastFlush;
    int64_t _currentLogBytesFromLastRoll;

    std::vector<char> _buffer;

    bool _threadSafe;

    class Writter;
    std::unique_ptr<Writter> _writter;
    std::unique_ptr<std::mutex> _mutex;
};

}
}


#endif // _INCLUDE_BASE_FILELOGGER_H_
