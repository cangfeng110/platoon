#ifndef _INCLUDE_BASE_LOGGING_H_
#define _INCLUDE_BASE_LOGGING_H_

#include <sstream>
#include <memory>
#include <functional>

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "include/base/FileLogger.h"
#include "include/base/Timestamp.h"
#include "include/base/NonCopyable.h"


namespace platoon
{
namespace base
{

class Logging : public NonCopyable
{
public:
    enum LogLevel
    {
        Debug,
        Info,
        Warn,
        Error,
        Die,
        LogLevelNum
    };

    Logging(LogLevel logLevel, const char* file, const char* func, int line,
            bool verbose = true, bool abort = false);
    ~Logging();

    typedef std::function<void(const char*, int64_t)> LogFunc;
    typedef std::function<void(Timestamp)> FlushFunc;

    static void setLogLevel(LogLevel logLevel)
    {
        assert(logLevel < LogLevelNum);
        s_logLevel = logLevel;
    }

    static LogLevel logLevel() { return s_logLevel; }

    static void setLogFunc(const LogFunc& logFunc) { s_logFunc = logFunc; }

    static void setFlushFunc(const FlushFunc& flushFunc) { s_flushFunc = flushFunc; }

    static void initLogging(LogLevel logLevel, const LogFunc& logFunc,
                            const FlushFunc& flushFunc)
    {
        s_logFunc = logFunc;
        s_flushFunc = flushFunc;
        s_logLevel = logLevel;
    }

    template<typename T>
    Logging& operator<<(const T& data)
    {
        _stream << data;
        return *this;
    }

    template<typename T>
    Logging& operator<<(const T* data)
    {
        _stream << "0x" << std::hex << reinterpret_cast<int64_t>(data);
        return *this;
    }

    Logging& operator<<(const char* data)
    {
        _stream << data;
        return *this;
    }

    Logging& operator<<(char* data)
    {
        _stream << reinterpret_cast<const char*>(data);
        return *this;
    }

private:
    bool _abort;
    std::ostringstream _stream;

    static LogFunc s_logFunc;
    static FlushFunc s_flushFunc;
    static LogLevel s_logLevel;
};

}

}

#define LOG(level, verbose) \
    if (platoon::base::Logging::logLevel() <= level) \
        platoon::base::Logging((level), __FILE__, __func__, __LINE__, (verbose))

#define LDEBUG LOG(platoon::base::Logging::LogLevel::Debug, true)
#define LINFO  LOG(platoon::base::Logging::LogLevel::Info, true)
#define LWARN  platoon::base::Logging(platoon::base::Logging::LogLevel::Warn, \
                __FILE__, __func__, __LINE__, true)
#define LERROR platoon::base::Logging(platoon::base::Logging::LogLevel::Error, \
                __FILE__, __func__, __LINE__, true)
#define LDIE   platoon::base::Logging(platoon::base::Logging::LogLevel::Die, \
                __FILE__, __func__, __LINE__, true, true)

// strerror_r dosen't fill buffer passed in ?
#define DIE(format, ...) \
    { \
        char buffer[64] = {0}; \
        const char* estr = strerror_r(errno, buffer, sizeof(buffer)); \
        const char* fstr = ::strrchr(__FILE__, '/'); \
        if (fstr == nullptr) { \
            fstr = __FILE__; \
        }else { \
            ++fstr; \
        } \
        ::fprintf(stderr, "(DIE, %s, %s, %d) %s : "#format" \n", \
                  fstr, __func__, __LINE__, estr, ##__VA_ARGS__); \
        ::abort(); \
    }


#endif // _INCLUDE_BASE_LOGGING_H_
