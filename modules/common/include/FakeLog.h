//
// Created by nicola on 18-3-15.
//

#ifndef TEST_GLOG_FAKELOG_H
#define TEST_GLOG_FAKELOG_H

#include <fstream>
#include <memory>

class CFakeLog {
public:
    CFakeLog();
    std::ostream &operator() (void);

private:
    CFakeLog(const CFakeLog &);
    CFakeLog &operator=(const CFakeLog &);

private:
    std::shared_ptr<std::ostream> mp_output;

    static pid_t m_pid;
    static pthread_t m_tid;
    static bool bLogInFile; // if true, streaming into file, or into std::cout

    pid_t m_pid_cur;
    pthread_t m_tid_cur;
};

//std::ofstream g_ofs_out("tmp.log", std::ios::out | std::ios::app);
//extern std::fstream g_ofs_out;

///**
// * Global Assistance Log File Class
// */
//class GAsLogFile {
//public:
//    GAsLogFile();
//    ~GAsLogFile();
//
//private:
//    void init();
//    void clean();
//
//    static unsigned int _counter;
//};
//
//static GAsLogFile g_logfile_assist;


//#define AINFO g_ofs_out << std::endl
//#define AWARN g_ofs_out << std::endl
//#define AERROR g_ofs_out << std::endl
//#define AFATAL g_ofs_out << std::endl
//
//#define ADEBUG g_ofs_out << std::endl
//
//#define AINFO_IF(cond) g_ofs_out << std::endl
//#define AERROR_IF(cond) g_ofs_out << std::endl
//#define ACHECK(cond) g_ofs_out << std::endl
//
//#define CHECK(condition) \
//    if (!condition) \
//        g_ofs_out << std::endl << "Check failed: " #condition " "
//
//#define DCHECK(condition) CHECK(condition)
//
//// CHECK_NOTNULL 宏 已经丧失检查意义
//// 纯粹仅辅助代码编译通过
//#define CHECK_NOTNULL(val) val
//
//#define AINFO_EVERY(freq) g_ofs_out << std::endl
//#define AWARN_EVERY(freq) g_ofs_out << std::endl
//#define AERROR_EVERY(freq) g_ofs_out << std::endl

#define AINFO CFakeLog()() << "[INFO]"
#define AWARN CFakeLog()() << "[WARN]"
#define AERROR CFakeLog()() << "[ERROR]"
#define AFATAL CFakeLog()() << "[FATAL]"

#define ADEBUG CFakeLog()() << "[DEBUG]"

//
#define AINFO_IF(cond) if(cond) AINFO
#define AERROR_IF(cond) if(cond) AERROR
#define ACHECK(cond) if(!cond) CFakeLog()() << "[Check Failed]"

#define CHECK(condition) \
    if (!condition) \
        CFakeLog()() << "Check failed: " #condition " "

#define DCHECK(condition) CHECK(condition)

// CHECK_NOTNULL 宏 已经丧失检查意义
// 纯粹仅辅助代码编译通过
#define CHECK_NOTNULL(val) val

#define AINFO_EVERY(freq) CFakeLog()()
#define AWARN_EVERY(freq) CFakeLog()()
#define AERROR_EVERY(freq) CFakeLog()()

#define RETURN_IF_NULL(ptr)               \
    if (ptr == nullptr) {                 \
        AWARN << #ptr << " is nullptr.";  \
        return;                           \
    }

#define RETURN_VAL_IF_NULL(ptr, val)      \
    if (ptr == nullptr) {                 \
        AWARN << #ptr << " is nullptr.";  \
        return val;                       \
    }

#define RETURN_IF(condition)                   \
    if (condition) {                           \
        AWARN << #condition << " is not met."; \
        return;                                \
    }

#define RETURN_VAL_IF(condition, val)          \
    if (condition) {                           \
        AWARN << #condition << " is not met."; \
        return val;                            \
    }

//#define CHECK_GE(val1, val2) \
//    if (val1 < val2) { \
//        AWARN << "CHECK_GE failed."; \
//    }
#define CHECK_GE(val1, val2) \
    if ((val1) < (val2)) AWARN << "CHECK_GE failed."

//#define CHECK_GT(val1, val2) \
//    if (val1 <= val2) { \
//        AWARN << "CHECK_GT failed."; \
//    }
#define CHECK_GT(val1, val2) \
    if ((val1) <= (val2)) AWARN << "CHECK_GT failed."

//#define CHECK_EQ(val1, val2) \
//    if (val1 != val2) { \
//        AWARN << "CHECK_EQ failed."; \
//    }
#define CHECK_EQ(val1, val2) \
    if ((val1) != (val2)) AWARN << "CHECK_EQ failed."

//#define CHECK_NE(val1, val2) \
//    if (val1 == val2) { \
//        AWARN << "CHECK_NE failed."; \
//    }
#define CHECK_NE(val1, val2) \
    if ((val1) == (val2)) AWARN << "CHECK_NE failed."


//#define CHECK_LE(val1, val2) \
//    if (val1 > val2) { \
//        AWARN << "CHECK_LE failed."; \
//    }
#define CHECK_LE(val1, val2) \
    if ((val1) > (val2)) AWARN << "CHECK_LE failed."

//#define CHECK_LT(val1, val2) \
//    if (val1 >= val2) { \
//        AWARN << "CHECK_LT failed."; \
//    }
#define CHECK_LT(val1, val2) \
    if ((val1) >= (val2)) AWARN << "CHECK_LT failed."

#endif //TEST_GLOG_FAKELOG_H