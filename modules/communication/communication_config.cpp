#include <iostream>

#include <libconfig/libconfig.h++>

#include "include/base/Logging.h"
#include "modules/communication/communication_config.h"

namespace platoon {

namespace communication {

#define TJP_COMMUNICATION_CONFIG_FILE "config/tjP-communication.conf"

CommuConfig *CommuConfig::instance_ = new CommuConfig();

CommuConfig *CommuConfig::GetInstance() {
    return instance_;
}

CommuConfig::CommuConfig() {
    libconfig::Config cfg;
    try {
        cfg.readFile(TJP_COMMUNICATION_CONFIG_FILE);
    }catch (const libconfig::FileIOException& e) {
        DIE("Can not open file %s \n", TJP_COMMUNICATION_CONFIG_FILE);
    }catch (const libconfig::ParseException& e) {
        DIE("Can not parse file %s : %d %s \n",
            e.getFile(), e.getLine(), e.getError());
    }

    try {
        const libconfig::Setting& commu = cfg.lookup("communication");
        commu.lookupValue("debug", debug_);
        commu.lookupValue("logPath", log_path_);
    }catch (const libconfig::SettingNotFoundException& e) {
        DIE("Can not config value (%s) \n", e.what());
    }

    PrintConfig();
}


void CommuConfig::PrintConfig() {
    std::cout << "Communication start with \n"
              << "\t Debug="   << (debug_ ? "true":"false") << "\n"
              << "\t LogPath=" << log_path_ << "\n"
              << std::endl;
}

} // namespace commuication

} // namespace platoon
