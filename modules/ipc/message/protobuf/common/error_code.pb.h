// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: modules/ipc/message/protobuf/common/error_code.proto

#ifndef PROTOBUF_modules_2fipc_2fmessage_2fprotobuf_2fcommon_2ferror_5fcode_2eproto__INCLUDED
#define PROTOBUF_modules_2fipc_2fmessage_2fprotobuf_2fcommon_2ferror_5fcode_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3003000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3003000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_util.h>
// @@protoc_insertion_point(includes)
namespace atd {
namespace common {
class StatusPb;
class StatusPbDefaultTypeInternal;
extern StatusPbDefaultTypeInternal _StatusPb_default_instance_;
}  // namespace common
}  // namespace atd

namespace atd {
namespace common {

namespace protobuf_modules_2fipc_2fmessage_2fprotobuf_2fcommon_2ferror_5fcode_2eproto {
// Internal implementation detail -- do not call these.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[];
  static const ::google::protobuf::uint32 offsets[];
  static void InitDefaultsImpl();
  static void Shutdown();
};
void AddDescriptors();
void InitDefaults();
}  // namespace protobuf_modules_2fipc_2fmessage_2fprotobuf_2fcommon_2ferror_5fcode_2eproto

enum ErrorCode {
  OK = 0,
  DR_ERROR = 100,
  MATCH_ERROR = 200,
  GRIDMAP_ERROR = 300,
  LOCALIZATIONABS_ERROR = 400,
  DRIVER_MELANE_ERROR = 500,
  DRIVER_US_ERROR = 501,
  DRIVER_INS_ERROR = 502,
  DRIVER_UWB_ERROR = 503,
  DRIVER_VCU_ERROR = 504,
  CAN_CLIENT_ERROR_BASE = 505,
  CAN_CLIENT_ERROR_FRAME_NUM = 506,
  CAN_CLIENT_ERROR_SEND_FAILED = 507,
  CAN_CLIENT_ERROR_RECV_FAILED = 508,
  CANBUS_ERROR = 509,
  FSM_Socket_creation_ERROR = 510,
  FSM_Registration_ERROR = 511,
  FSM_HeartBeatTimeout_ERROR = 512,
  FSM_ResultFB_ERROR = 513,
  FSM_PassingReq_ERROR = 514,
  FSM_ERROR = 515,
  LOCALIZATION_ERROR = 3000,
  LOCALIZATION_ERROR_MSG = 3100,
  LOCALIZATION_ERROR_LIDAR = 3200,
  LOCALIZATION_ERROR_INTEG = 3300,
  LOCALIZATION_ERROR_GNSS = 3400,
  PERCEPTION_ERROR = 4000,
  PERCEPTION_ERROR_TF = 4001,
  PERCEPTION_ERROR_PROCESS = 4002,
  PERCEPTION_FATAL = 4003,
  PERCEPTION_ERROR_EyeQ4Lane = 4004,
  PERCEPTION_ERROR_HDMapLane = 4005,
  PERCEPTION_ERROR_MATCHHDMapLane_to_EyeQLane = 4006,
  PERCEPTION_ERROR_RectifyHDMapLane = 4007,
  PERCEPTION_ERROR_NOT_ALL_EyeQ4Lane_VALID = 4008,
  PERCEPTION_ERROR_EyeQ4Lane_Line_SLope_Too_Big = 4009,
  PREDICTION_ERROR = 5000,
  PLANNING_ERROR = 6000,
  HDMAP_DATA_ERROR = 7000,
  HDMAP_ROUTE_FAILROUTE = 7001,
  HDMAP_MATCHGRID_NOMATCH = 7002,
  SYS_END_OF_INPUT = 9000,
  SYS_FILEREAD_ERROR = 9001,
  SYS_INIT_ERROR = 9002,
  HTTP_LOGIC_ERROR = 10000,
  HTTP_RUNTIME_ERROR = 10001
};
bool ErrorCode_IsValid(int value);
const ErrorCode ErrorCode_MIN = OK;
const ErrorCode ErrorCode_MAX = HTTP_RUNTIME_ERROR;
const int ErrorCode_ARRAYSIZE = ErrorCode_MAX + 1;

// ===================================================================

class StatusPb : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:atd.common.StatusPb) */ {
 public:
  StatusPb();
  virtual ~StatusPb();

  StatusPb(const StatusPb& from);

  inline StatusPb& operator=(const StatusPb& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::std::string& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::std::string* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const StatusPb& default_instance();

  static inline const StatusPb* internal_default_instance() {
    return reinterpret_cast<const StatusPb*>(
               &_StatusPb_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(StatusPb* other);

  // implements Message ----------------------------------------------

  inline StatusPb* New() const PROTOBUF_FINAL { return New(NULL); }

  StatusPb* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)
    PROTOBUF_FINAL;
  void CopyFrom(const StatusPb& from);
  void MergeFrom(const StatusPb& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  void DiscardUnknownFields();
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(StatusPb* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::std::string GetTypeName() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string msg = 2;
  bool has_msg() const;
  void clear_msg();
  static const int kMsgFieldNumber = 2;
  const ::std::string& msg() const;
  void set_msg(const ::std::string& value);
  #if LANG_CXX11
  void set_msg(::std::string&& value);
  #endif
  void set_msg(const char* value);
  void set_msg(const char* value, size_t size);
  ::std::string* mutable_msg();
  ::std::string* release_msg();
  void set_allocated_msg(::std::string* msg);

  // optional .atd.common.ErrorCode error_code = 1 [default = OK];
  bool has_error_code() const;
  void clear_error_code();
  static const int kErrorCodeFieldNumber = 1;
  ::atd::common::ErrorCode error_code() const;
  void set_error_code(::atd::common::ErrorCode value);

  // @@protoc_insertion_point(class_scope:atd.common.StatusPb)
 private:
  void set_has_error_code();
  void clear_has_error_code();
  void set_has_msg();
  void clear_has_msg();

  ::google::protobuf::internal::InternalMetadataWithArenaLite _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr msg_;
  int error_code_;
  friend struct protobuf_modules_2fipc_2fmessage_2fprotobuf_2fcommon_2ferror_5fcode_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// StatusPb

// optional .atd.common.ErrorCode error_code = 1 [default = OK];
inline bool StatusPb::has_error_code() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void StatusPb::set_has_error_code() {
  _has_bits_[0] |= 0x00000002u;
}
inline void StatusPb::clear_has_error_code() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void StatusPb::clear_error_code() {
  error_code_ = 0;
  clear_has_error_code();
}
inline ::atd::common::ErrorCode StatusPb::error_code() const {
  // @@protoc_insertion_point(field_get:atd.common.StatusPb.error_code)
  return static_cast< ::atd::common::ErrorCode >(error_code_);
}
inline void StatusPb::set_error_code(::atd::common::ErrorCode value) {
  assert(::atd::common::ErrorCode_IsValid(value));
  set_has_error_code();
  error_code_ = value;
  // @@protoc_insertion_point(field_set:atd.common.StatusPb.error_code)
}

// optional string msg = 2;
inline bool StatusPb::has_msg() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void StatusPb::set_has_msg() {
  _has_bits_[0] |= 0x00000001u;
}
inline void StatusPb::clear_has_msg() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void StatusPb::clear_msg() {
  msg_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_msg();
}
inline const ::std::string& StatusPb::msg() const {
  // @@protoc_insertion_point(field_get:atd.common.StatusPb.msg)
  return msg_.GetNoArena();
}
inline void StatusPb::set_msg(const ::std::string& value) {
  set_has_msg();
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:atd.common.StatusPb.msg)
}
#if LANG_CXX11
inline void StatusPb::set_msg(::std::string&& value) {
  set_has_msg();
  msg_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:atd.common.StatusPb.msg)
}
#endif
inline void StatusPb::set_msg(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_msg();
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:atd.common.StatusPb.msg)
}
inline void StatusPb::set_msg(const char* value, size_t size) {
  set_has_msg();
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:atd.common.StatusPb.msg)
}
inline ::std::string* StatusPb::mutable_msg() {
  set_has_msg();
  // @@protoc_insertion_point(field_mutable:atd.common.StatusPb.msg)
  return msg_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* StatusPb::release_msg() {
  // @@protoc_insertion_point(field_release:atd.common.StatusPb.msg)
  clear_has_msg();
  return msg_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void StatusPb::set_allocated_msg(::std::string* msg) {
  if (msg != NULL) {
    set_has_msg();
  } else {
    clear_has_msg();
  }
  msg_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), msg);
  // @@protoc_insertion_point(field_set_allocated:atd.common.StatusPb.msg)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)


}  // namespace common
}  // namespace atd

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::atd::common::ErrorCode> : ::google::protobuf::internal::true_type {};

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_modules_2fipc_2fmessage_2fprotobuf_2fcommon_2ferror_5fcode_2eproto__INCLUDED
