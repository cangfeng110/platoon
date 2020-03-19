#ifndef MODULES_COMMON_ATD_APP_H_
#define MODULES_COMMON_ATD_APP_H_

#include <thread>
#include "modules/common/include/status/status.h"
#include "modules/common/include/Config.h"
/**
 * @namespace atd::common
 * @brief atd::common
 */
namespace atd {
namespace common {


/**
 * @class AtdApp
 *
 * @brief The base module class to define the interface of an Atd app.
 */
class AtdApp {
public:

    AtdApp();

    /**
   * @brief module name. It is used to uniquely identify the app.
   */
    virtual std::string Name() const = 0;

    /**
   * @brief this is the entry point of an Atd App. It initializes the app,
   * starts the app, and stop the app when the ros has shutdown.
   */
    virtual int Spin();

    /**
   * The default destructor.
   */
    virtual ~AtdApp() = default;

protected:
    /**
   * @brief The module initialization function. This is the first function being
   * called when the App starts. Usually this function loads the configurations,
   * subscribe the data from sensors or other modules.
   * @return Status initialization status
   */
    virtual void Init() = 0;

    /**
   * @brief The module start function.
   * @return Status start status
   */
    virtual atd::common::Status Start() = 0;

    /**
   * @brief The module step function. one time calculate
   */
    virtual void Step() = 0;

    /**
   * @brief The module stop function.
   */
    virtual void Stop() = 0;


    void onTimer(int64_t intervaltimetime);




private:


};

void atd_app_sigint_handler(int signal_num);

}  // namespace common
}  // namespace atd


#ifdef CC_X86_64
#define ATD_MAIN(APP)                                       \
    int main(int argc, char **argv) {                            \
    APP atd_app_;                                           \
    atd_app_.Spin();                                        \
    return 0;                                                  \
    }
#endif //CC_X86_64

#if defined(CC_TTTECH) || defined(CC_PX2)
#define ATD_MAIN(APP)                   \
    int main(int argc, char *argv[]) {  \
        APP atd_app_;                   \
        atd_app_.Spin();                \
        return 0;                       \
    }
#endif // defined(CC_TTTECH) || defined(CC_PX2)

#endif  // MODULES_COMMON_ATD_APP_H_
