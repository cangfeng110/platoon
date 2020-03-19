// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: modules/ipc/message/protobuf/monitor/online_report.proto

#ifndef PROTOBUF_modules_2fipc_2fmessage_2fprotobuf_2fmonitor_2fonline_5freport_2eproto__INCLUDED
#define PROTOBUF_modules_2fipc_2fmessage_2fprotobuf_2fmonitor_2fonline_5freport_2eproto__INCLUDED

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
#include "modules/ipc/message/protobuf/common/vehicle_state.pb.h"
#include "modules/ipc/message/protobuf/data/static_info.pb.h"
// @@protoc_insertion_point(includes)
namespace atd {
namespace common {
class VehicleState;
class VehicleStateDefaultTypeInternal;
extern VehicleStateDefaultTypeInternal _VehicleState_default_instance_;
}  // namespace common
namespace data {
class EnvironmentInfo;
class EnvironmentInfoDefaultTypeInternal;
extern EnvironmentInfoDefaultTypeInternal _EnvironmentInfo_default_instance_;
class HardwareInfo;
class HardwareInfoDefaultTypeInternal;
extern HardwareInfoDefaultTypeInternal _HardwareInfo_default_instance_;
class HardwareInfo_ConfigsEntry;
class HardwareInfo_ConfigsEntryDefaultTypeInternal;
extern HardwareInfo_ConfigsEntryDefaultTypeInternal _HardwareInfo_ConfigsEntry_default_instance_;
class SoftwareInfo;
class SoftwareInfoDefaultTypeInternal;
extern SoftwareInfoDefaultTypeInternal _SoftwareInfo_default_instance_;
class SoftwareInfo_ConfigsEntry;
class SoftwareInfo_ConfigsEntryDefaultTypeInternal;
extern SoftwareInfo_ConfigsEntryDefaultTypeInternal _SoftwareInfo_ConfigsEntry_default_instance_;
class StaticInfo;
class StaticInfoDefaultTypeInternal;
extern StaticInfoDefaultTypeInternal _StaticInfo_default_instance_;
class StaticInfoConf;
class StaticInfoConfDefaultTypeInternal;
extern StaticInfoConfDefaultTypeInternal _StaticInfoConf_default_instance_;
class UserInfo;
class UserInfoDefaultTypeInternal;
extern UserInfoDefaultTypeInternal _UserInfo_default_instance_;
class VehicleInfo;
class VehicleInfoDefaultTypeInternal;
extern VehicleInfoDefaultTypeInternal _VehicleInfo_default_instance_;
}  // namespace data
namespace monitor {
class VehicleStateReport;
class VehicleStateReportDefaultTypeInternal;
extern VehicleStateReportDefaultTypeInternal _VehicleStateReport_default_instance_;
}  // namespace monitor
}  // namespace atd

namespace atd {
namespace monitor {

namespace protobuf_modules_2fipc_2fmessage_2fprotobuf_2fmonitor_2fonline_5freport_2eproto {
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
}  // namespace protobuf_modules_2fipc_2fmessage_2fprotobuf_2fmonitor_2fonline_5freport_2eproto

// ===================================================================

class VehicleStateReport : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:atd.monitor.VehicleStateReport) */ {
 public:
  VehicleStateReport();
  virtual ~VehicleStateReport();

  VehicleStateReport(const VehicleStateReport& from);

  inline VehicleStateReport& operator=(const VehicleStateReport& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::std::string& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::std::string* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const VehicleStateReport& default_instance();

  static inline const VehicleStateReport* internal_default_instance() {
    return reinterpret_cast<const VehicleStateReport*>(
               &_VehicleStateReport_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(VehicleStateReport* other);

  // implements Message ----------------------------------------------

  inline VehicleStateReport* New() const PROTOBUF_FINAL { return New(NULL); }

  VehicleStateReport* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)
    PROTOBUF_FINAL;
  void CopyFrom(const VehicleStateReport& from);
  void MergeFrom(const VehicleStateReport& from);
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
  void InternalSwap(VehicleStateReport* other);
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

  // optional .atd.data.VehicleInfo vehicle_info = 1;
  bool has_vehicle_info() const;
  void clear_vehicle_info();
  static const int kVehicleInfoFieldNumber = 1;
  const ::atd::data::VehicleInfo& vehicle_info() const;
  ::atd::data::VehicleInfo* mutable_vehicle_info();
  ::atd::data::VehicleInfo* release_vehicle_info();
  void set_allocated_vehicle_info(::atd::data::VehicleInfo* vehicle_info);

  // optional .atd.common.VehicleState vehicle_state = 2;
  bool has_vehicle_state() const;
  void clear_vehicle_state();
  static const int kVehicleStateFieldNumber = 2;
  const ::atd::common::VehicleState& vehicle_state() const;
  ::atd::common::VehicleState* mutable_vehicle_state();
  ::atd::common::VehicleState* release_vehicle_state();
  void set_allocated_vehicle_state(::atd::common::VehicleState* vehicle_state);

  // @@protoc_insertion_point(class_scope:atd.monitor.VehicleStateReport)
 private:
  void set_has_vehicle_info();
  void clear_has_vehicle_info();
  void set_has_vehicle_state();
  void clear_has_vehicle_state();

  ::google::protobuf::internal::InternalMetadataWithArenaLite _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::atd::data::VehicleInfo* vehicle_info_;
  ::atd::common::VehicleState* vehicle_state_;
  friend struct protobuf_modules_2fipc_2fmessage_2fprotobuf_2fmonitor_2fonline_5freport_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// VehicleStateReport

// optional .atd.data.VehicleInfo vehicle_info = 1;
inline bool VehicleStateReport::has_vehicle_info() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void VehicleStateReport::set_has_vehicle_info() {
  _has_bits_[0] |= 0x00000001u;
}
inline void VehicleStateReport::clear_has_vehicle_info() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void VehicleStateReport::clear_vehicle_info() {
  if (vehicle_info_ != NULL) vehicle_info_->::atd::data::VehicleInfo::Clear();
  clear_has_vehicle_info();
}
inline const ::atd::data::VehicleInfo& VehicleStateReport::vehicle_info() const {
  // @@protoc_insertion_point(field_get:atd.monitor.VehicleStateReport.vehicle_info)
  return vehicle_info_ != NULL ? *vehicle_info_
                         : *::atd::data::VehicleInfo::internal_default_instance();
}
inline ::atd::data::VehicleInfo* VehicleStateReport::mutable_vehicle_info() {
  set_has_vehicle_info();
  if (vehicle_info_ == NULL) {
    vehicle_info_ = new ::atd::data::VehicleInfo;
  }
  // @@protoc_insertion_point(field_mutable:atd.monitor.VehicleStateReport.vehicle_info)
  return vehicle_info_;
}
inline ::atd::data::VehicleInfo* VehicleStateReport::release_vehicle_info() {
  // @@protoc_insertion_point(field_release:atd.monitor.VehicleStateReport.vehicle_info)
  clear_has_vehicle_info();
  ::atd::data::VehicleInfo* temp = vehicle_info_;
  vehicle_info_ = NULL;
  return temp;
}
inline void VehicleStateReport::set_allocated_vehicle_info(::atd::data::VehicleInfo* vehicle_info) {
  delete vehicle_info_;
  vehicle_info_ = vehicle_info;
  if (vehicle_info) {
    set_has_vehicle_info();
  } else {
    clear_has_vehicle_info();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.monitor.VehicleStateReport.vehicle_info)
}

// optional .atd.common.VehicleState vehicle_state = 2;
inline bool VehicleStateReport::has_vehicle_state() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void VehicleStateReport::set_has_vehicle_state() {
  _has_bits_[0] |= 0x00000002u;
}
inline void VehicleStateReport::clear_has_vehicle_state() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void VehicleStateReport::clear_vehicle_state() {
  if (vehicle_state_ != NULL) vehicle_state_->::atd::common::VehicleState::Clear();
  clear_has_vehicle_state();
}
inline const ::atd::common::VehicleState& VehicleStateReport::vehicle_state() const {
  // @@protoc_insertion_point(field_get:atd.monitor.VehicleStateReport.vehicle_state)
  return vehicle_state_ != NULL ? *vehicle_state_
                         : *::atd::common::VehicleState::internal_default_instance();
}
inline ::atd::common::VehicleState* VehicleStateReport::mutable_vehicle_state() {
  set_has_vehicle_state();
  if (vehicle_state_ == NULL) {
    vehicle_state_ = new ::atd::common::VehicleState;
  }
  // @@protoc_insertion_point(field_mutable:atd.monitor.VehicleStateReport.vehicle_state)
  return vehicle_state_;
}
inline ::atd::common::VehicleState* VehicleStateReport::release_vehicle_state() {
  // @@protoc_insertion_point(field_release:atd.monitor.VehicleStateReport.vehicle_state)
  clear_has_vehicle_state();
  ::atd::common::VehicleState* temp = vehicle_state_;
  vehicle_state_ = NULL;
  return temp;
}
inline void VehicleStateReport::set_allocated_vehicle_state(::atd::common::VehicleState* vehicle_state) {
  delete vehicle_state_;
  vehicle_state_ = vehicle_state;
  if (vehicle_state) {
    set_has_vehicle_state();
  } else {
    clear_has_vehicle_state();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.monitor.VehicleStateReport.vehicle_state)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)


}  // namespace monitor
}  // namespace atd

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_modules_2fipc_2fmessage_2fprotobuf_2fmonitor_2fonline_5freport_2eproto__INCLUDED
