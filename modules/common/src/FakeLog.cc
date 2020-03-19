//
// Created by nicola on 18-3-23.
//

#include "modules/common/include/Config.h"
#include "modules/common/include/FakeLog.h"

#include <unistd.h>
#include <iostream>
#include <pthread.h>

pid_t CFakeLog::m_pid = getpid();
pthread_t CFakeLog::m_tid = pthread_self();

#ifdef CC_TTTECH
bool CFakeLog::bLogInFile = false;
#else
bool CFakeLog::bLogInFile = true;
#endif


CFakeLog::CFakeLog() {
    m_pid_cur = getpid();
    m_tid_cur = pthread_self();

    if (mp_output == nullptr || m_pid != m_pid_cur || m_tid != m_tid_cur) {
        if (bLogInFile) {
            // write into file
            std::string cur_file_path = "/tmp/fklg_pid_" + std::to_string(m_pid_cur)
                    + "_tid_" + std::to_string(m_tid_cur) + ".log";
            mp_output.reset(new std::fstream(cur_file_path, std::ios::out | std::ios::app));
        } else {
            // write into std::cout, set `_Deleter`
            mp_output.reset(&std::cout, [](std::ostream *) {});
        }
    }
}

std::ostream &CFakeLog::operator()() {
    //(*mp_output) << "--pid[" << m_pid_cur << "]tid[" << m_tid_cur << "]--" << std::endl;
    (*mp_output) << std::endl;
    return *mp_output;
}

//static char g_dummy[sizeof(std::fstream)];
//std::fstream g_ofs_out("/opt/tttech/motionwise/lcm_publish_example/tmp.log", std::ios::out | std::ios::app);

//std::fstream g_ofs_out(CREATE_FULL_PATH(tmp.log), std::ios::out | std::ios::app);

//unsigned int GAsLogFile::_counter = 0;
//
//void GAsLogFile::init() {
//    new(&g_ofs_out) std::fstream;
//    g_ofs_out.open("/opt/tttech/motionwise/lcm_publish_example/tmp.log", std::ios::out | std::ios::app);
//}
//
//void GAsLogFile::clean() {
//    g_ofs_out.close();
//}
//
//GAsLogFile::GAsLogFile() {
//    if (_counter++ == 0) {
//        init();
//    }
//}
//
//GAsLogFile::~GAsLogFile() {
//    if (--_counter == 0) {
//        clean();
//    }
//}

