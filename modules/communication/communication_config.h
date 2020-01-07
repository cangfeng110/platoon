#ifndef _COMMU_CONFIG_H_
#define _COMMU_CONFIG_H_

#include <string>

namespace platoon {

namespace communication{

struct CommuConfig {
public:
    static CommuConfig *GetInstance();

public:
    void PrintConfig();

    // Data members
    bool debug_;
    std::string log_path_;

private:
    static CommuConfig *instance_;

    CommuConfig();
    CommuConfig(const CommuConfig&) = delete;
    CommuConfig& operator=(const CommuConfig&) = delete;
};

} // namespace communication

} // namespace platoon

#endif // _COMMU_CONFIG_H_
