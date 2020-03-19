#ifndef MODULES_COMMON_STATUS_STATUS_H_
#define MODULES_COMMON_STATUS_STATUS_H_

#include <string>
#include "google/protobuf/descriptor.h"
#include "modules/ipc/message/protobuf/common/error_code.pb.h"

/**
 * @namespace atd::common
 * @brief atd::common
 */
namespace atd {
namespace common {

/**
 * @class Status
 *
 * @brief A general class to denote the return status of an API call. It
 * can either be an OK status for success, or a failure status with error
 * message and error code enum.
*/
class Status {
 public:
  /**
   * @brief Create a success status.
   */
  Status() : code_(ErrorCode::OK), msg_() {}
  ~Status() = default;

  /**
   * @brief Create a status with the specified error code and msg as a
   * human-readable string containing more detailed information.
   * @param code the error code.
   * @param msg the message associated with the error.
   */
  Status(ErrorCode code, const std::string &msg) : code_(code), msg_(msg) {}

  /**
   * @brief Create a status with the specified error code and empty msg
   * @param code the error code.
   */
  explicit Status(ErrorCode code) : code_(code), msg_("") {}

//  /**
//   * @brief generate a success status.
//   * @returns a success status
//   */
  //static Status OK() { return Status(); }

  /**
   * @brief check whether the return status is OK.
   * @returns true if the code is ErrorCode::OK
   *          false otherwise
   */
  bool ok() const { return code_ == ErrorCode::OK; }

  /**
   * @brief get the error code
   * @returns the error code
   */
  ErrorCode code() const { return code_; }

  /**
   * @brief defines the logic of testing if two Status are equal
   */
  bool operator==(const Status &rh) const {
    return (this->code_ == rh.code_) && (this->msg_ == rh.msg_);
  }

  /**
   * @brief defines the logic of testing if two Status are unequal
   */
  bool operator!=(const Status &rh) const { return !(*this == rh); }

  /**
   * @brief returns the error message of the status, empty if the status is OK.
   * @returns the error message
   */
  const std::string &error_message() const { return msg_; }

  /**
   * @brief returns a string representation in a readable format.
   * @returns the string "OK" if success.
   *          the internal error message otherwise.
   */
  std::string ToString() const {
    if (ok()) {
      return "OK";
    }
    return std::to_string(code_) + ": " + msg_;
  }

  /**
   * @brief save the error_code and error message to protobuf
   * @param the Status protobuf that will store the message.
   */
  void Save(StatusPb *status_pb) {
    if (!status_pb) {
      return;
    }
    status_pb->set_error_code(code_);
    if (!msg_.empty()) {
      status_pb->set_msg(msg_);
    }
  }

 private:
  ErrorCode code_;
  std::string msg_;
};


}  // namespace common
}  // namespace atd

#endif  // MODULES_COMMON_STATUS_STATUS_H_
