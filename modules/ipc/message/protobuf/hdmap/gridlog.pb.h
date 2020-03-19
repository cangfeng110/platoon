// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: modules/ipc/message/protobuf/hdmap/gridlog.proto

#ifndef PROTOBUF_modules_2fipc_2fmessage_2fprotobuf_2fhdmap_2fgridlog_2eproto__INCLUDED
#define PROTOBUF_modules_2fipc_2fmessage_2fprotobuf_2fhdmap_2fgridlog_2eproto__INCLUDED

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
#include "modules/ipc/message/protobuf/common/header.pb.h"
#include "modules/ipc/message/protobuf/common/reserved.pb.h"
#include "modules/ipc/message/protobuf/localization/pose.pb.h"
// @@protoc_insertion_point(includes)
namespace atd {
namespace common {
class Header;
class HeaderDefaultTypeInternal;
extern HeaderDefaultTypeInternal _Header_default_instance_;
class Reserved;
class ReservedDefaultTypeInternal;
extern ReservedDefaultTypeInternal _Reserved_default_instance_;
}  // namespace common
namespace localization {
class Pose;
class PoseDefaultTypeInternal;
extern PoseDefaultTypeInternal _Pose_default_instance_;
}  // namespace localization
namespace map {
class GridLogInfo;
class GridLogInfoDefaultTypeInternal;
extern GridLogInfoDefaultTypeInternal _GridLogInfo_default_instance_;
class LANE_LINK_KEY_GridLog;
class LANE_LINK_KEY_GridLogDefaultTypeInternal;
extern LANE_LINK_KEY_GridLogDefaultTypeInternal _LANE_LINK_KEY_GridLog_default_instance_;
}  // namespace map
}  // namespace atd

namespace atd {
namespace map {

namespace protobuf_modules_2fipc_2fmessage_2fprotobuf_2fhdmap_2fgridlog_2eproto {
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
}  // namespace protobuf_modules_2fipc_2fmessage_2fprotobuf_2fhdmap_2fgridlog_2eproto

// ===================================================================

class LANE_LINK_KEY_GridLog : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:atd.map.LANE_LINK_KEY_GridLog) */ {
 public:
  LANE_LINK_KEY_GridLog();
  virtual ~LANE_LINK_KEY_GridLog();

  LANE_LINK_KEY_GridLog(const LANE_LINK_KEY_GridLog& from);

  inline LANE_LINK_KEY_GridLog& operator=(const LANE_LINK_KEY_GridLog& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::std::string& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::std::string* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const LANE_LINK_KEY_GridLog& default_instance();

  static inline const LANE_LINK_KEY_GridLog* internal_default_instance() {
    return reinterpret_cast<const LANE_LINK_KEY_GridLog*>(
               &_LANE_LINK_KEY_GridLog_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(LANE_LINK_KEY_GridLog* other);

  // implements Message ----------------------------------------------

  inline LANE_LINK_KEY_GridLog* New() const PROTOBUF_FINAL { return New(NULL); }

  LANE_LINK_KEY_GridLog* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)
    PROTOBUF_FINAL;
  void CopyFrom(const LANE_LINK_KEY_GridLog& from);
  void MergeFrom(const LANE_LINK_KEY_GridLog& from);
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
  void InternalSwap(LANE_LINK_KEY_GridLog* other);
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

  // optional int64 m_iRoadId = 1;
  bool has_m_iroadid() const;
  void clear_m_iroadid();
  static const int kMIRoadIdFieldNumber = 1;
  ::google::protobuf::int64 m_iroadid() const;
  void set_m_iroadid(::google::protobuf::int64 value);

  // optional int32 m_iSectionSeq = 2;
  bool has_m_isectionseq() const;
  void clear_m_isectionseq();
  static const int kMISectionSeqFieldNumber = 2;
  ::google::protobuf::int32 m_isectionseq() const;
  void set_m_isectionseq(::google::protobuf::int32 value);

  // optional int32 m_iLaneNum = 3;
  bool has_m_ilanenum() const;
  void clear_m_ilanenum();
  static const int kMILaneNumFieldNumber = 3;
  ::google::protobuf::int32 m_ilanenum() const;
  void set_m_ilanenum(::google::protobuf::int32 value);

  // optional int32 m_iBranch = 4;
  bool has_m_ibranch() const;
  void clear_m_ibranch();
  static const int kMIBranchFieldNumber = 4;
  ::google::protobuf::int32 m_ibranch() const;
  void set_m_ibranch(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:atd.map.LANE_LINK_KEY_GridLog)
 private:
  void set_has_m_iroadid();
  void clear_has_m_iroadid();
  void set_has_m_isectionseq();
  void clear_has_m_isectionseq();
  void set_has_m_ilanenum();
  void clear_has_m_ilanenum();
  void set_has_m_ibranch();
  void clear_has_m_ibranch();

  ::google::protobuf::internal::InternalMetadataWithArenaLite _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::int64 m_iroadid_;
  ::google::protobuf::int32 m_isectionseq_;
  ::google::protobuf::int32 m_ilanenum_;
  ::google::protobuf::int32 m_ibranch_;
  friend struct protobuf_modules_2fipc_2fmessage_2fprotobuf_2fhdmap_2fgridlog_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class GridLogInfo : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:atd.map.GridLogInfo) */ {
 public:
  GridLogInfo();
  virtual ~GridLogInfo();

  GridLogInfo(const GridLogInfo& from);

  inline GridLogInfo& operator=(const GridLogInfo& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::std::string& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }

  inline ::std::string* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const GridLogInfo& default_instance();

  static inline const GridLogInfo* internal_default_instance() {
    return reinterpret_cast<const GridLogInfo*>(
               &_GridLogInfo_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    1;

  void Swap(GridLogInfo* other);

  // implements Message ----------------------------------------------

  inline GridLogInfo* New() const PROTOBUF_FINAL { return New(NULL); }

  GridLogInfo* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)
    PROTOBUF_FINAL;
  void CopyFrom(const GridLogInfo& from);
  void MergeFrom(const GridLogInfo& from);
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
  void InternalSwap(GridLogInfo* other);
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

  // optional .atd.common.Header header = 1;
  bool has_header() const;
  void clear_header();
  static const int kHeaderFieldNumber = 1;
  const ::atd::common::Header& header() const;
  ::atd::common::Header* mutable_header();
  ::atd::common::Header* release_header();
  void set_allocated_header(::atd::common::Header* header);

  // optional .atd.common.Reserved reservedmsg = 2;
  bool has_reservedmsg() const;
  void clear_reservedmsg();
  static const int kReservedmsgFieldNumber = 2;
  const ::atd::common::Reserved& reservedmsg() const;
  ::atd::common::Reserved* mutable_reservedmsg();
  ::atd::common::Reserved* release_reservedmsg();
  void set_allocated_reservedmsg(::atd::common::Reserved* reservedmsg);

  // optional .atd.localization.Pose pose = 3;
  bool has_pose() const;
  void clear_pose();
  static const int kPoseFieldNumber = 3;
  const ::atd::localization::Pose& pose() const;
  ::atd::localization::Pose* mutable_pose();
  ::atd::localization::Pose* release_pose();
  void set_allocated_pose(::atd::localization::Pose* pose);

  // optional .atd.map.LANE_LINK_KEY_GridLog currentlaneinfo = 4;
  bool has_currentlaneinfo() const;
  void clear_currentlaneinfo();
  static const int kCurrentlaneinfoFieldNumber = 4;
  const ::atd::map::LANE_LINK_KEY_GridLog& currentlaneinfo() const;
  ::atd::map::LANE_LINK_KEY_GridLog* mutable_currentlaneinfo();
  ::atd::map::LANE_LINK_KEY_GridLog* release_currentlaneinfo();
  void set_allocated_currentlaneinfo(::atd::map::LANE_LINK_KEY_GridLog* currentlaneinfo);

  // optional int64 m_iSpeed = 5;
  bool has_m_ispeed() const;
  void clear_m_ispeed();
  static const int kMISpeedFieldNumber = 5;
  ::google::protobuf::int64 m_ispeed() const;
  void set_m_ispeed(::google::protobuf::int64 value);

  // @@protoc_insertion_point(class_scope:atd.map.GridLogInfo)
 private:
  void set_has_header();
  void clear_has_header();
  void set_has_reservedmsg();
  void clear_has_reservedmsg();
  void set_has_pose();
  void clear_has_pose();
  void set_has_currentlaneinfo();
  void clear_has_currentlaneinfo();
  void set_has_m_ispeed();
  void clear_has_m_ispeed();

  ::google::protobuf::internal::InternalMetadataWithArenaLite _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::atd::common::Header* header_;
  ::atd::common::Reserved* reservedmsg_;
  ::atd::localization::Pose* pose_;
  ::atd::map::LANE_LINK_KEY_GridLog* currentlaneinfo_;
  ::google::protobuf::int64 m_ispeed_;
  friend struct protobuf_modules_2fipc_2fmessage_2fprotobuf_2fhdmap_2fgridlog_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// LANE_LINK_KEY_GridLog

// optional int64 m_iRoadId = 1;
inline bool LANE_LINK_KEY_GridLog::has_m_iroadid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void LANE_LINK_KEY_GridLog::set_has_m_iroadid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void LANE_LINK_KEY_GridLog::clear_has_m_iroadid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void LANE_LINK_KEY_GridLog::clear_m_iroadid() {
  m_iroadid_ = GOOGLE_LONGLONG(0);
  clear_has_m_iroadid();
}
inline ::google::protobuf::int64 LANE_LINK_KEY_GridLog::m_iroadid() const {
  // @@protoc_insertion_point(field_get:atd.map.LANE_LINK_KEY_GridLog.m_iRoadId)
  return m_iroadid_;
}
inline void LANE_LINK_KEY_GridLog::set_m_iroadid(::google::protobuf::int64 value) {
  set_has_m_iroadid();
  m_iroadid_ = value;
  // @@protoc_insertion_point(field_set:atd.map.LANE_LINK_KEY_GridLog.m_iRoadId)
}

// optional int32 m_iSectionSeq = 2;
inline bool LANE_LINK_KEY_GridLog::has_m_isectionseq() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void LANE_LINK_KEY_GridLog::set_has_m_isectionseq() {
  _has_bits_[0] |= 0x00000002u;
}
inline void LANE_LINK_KEY_GridLog::clear_has_m_isectionseq() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void LANE_LINK_KEY_GridLog::clear_m_isectionseq() {
  m_isectionseq_ = 0;
  clear_has_m_isectionseq();
}
inline ::google::protobuf::int32 LANE_LINK_KEY_GridLog::m_isectionseq() const {
  // @@protoc_insertion_point(field_get:atd.map.LANE_LINK_KEY_GridLog.m_iSectionSeq)
  return m_isectionseq_;
}
inline void LANE_LINK_KEY_GridLog::set_m_isectionseq(::google::protobuf::int32 value) {
  set_has_m_isectionseq();
  m_isectionseq_ = value;
  // @@protoc_insertion_point(field_set:atd.map.LANE_LINK_KEY_GridLog.m_iSectionSeq)
}

// optional int32 m_iLaneNum = 3;
inline bool LANE_LINK_KEY_GridLog::has_m_ilanenum() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void LANE_LINK_KEY_GridLog::set_has_m_ilanenum() {
  _has_bits_[0] |= 0x00000004u;
}
inline void LANE_LINK_KEY_GridLog::clear_has_m_ilanenum() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void LANE_LINK_KEY_GridLog::clear_m_ilanenum() {
  m_ilanenum_ = 0;
  clear_has_m_ilanenum();
}
inline ::google::protobuf::int32 LANE_LINK_KEY_GridLog::m_ilanenum() const {
  // @@protoc_insertion_point(field_get:atd.map.LANE_LINK_KEY_GridLog.m_iLaneNum)
  return m_ilanenum_;
}
inline void LANE_LINK_KEY_GridLog::set_m_ilanenum(::google::protobuf::int32 value) {
  set_has_m_ilanenum();
  m_ilanenum_ = value;
  // @@protoc_insertion_point(field_set:atd.map.LANE_LINK_KEY_GridLog.m_iLaneNum)
}

// optional int32 m_iBranch = 4;
inline bool LANE_LINK_KEY_GridLog::has_m_ibranch() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void LANE_LINK_KEY_GridLog::set_has_m_ibranch() {
  _has_bits_[0] |= 0x00000008u;
}
inline void LANE_LINK_KEY_GridLog::clear_has_m_ibranch() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void LANE_LINK_KEY_GridLog::clear_m_ibranch() {
  m_ibranch_ = 0;
  clear_has_m_ibranch();
}
inline ::google::protobuf::int32 LANE_LINK_KEY_GridLog::m_ibranch() const {
  // @@protoc_insertion_point(field_get:atd.map.LANE_LINK_KEY_GridLog.m_iBranch)
  return m_ibranch_;
}
inline void LANE_LINK_KEY_GridLog::set_m_ibranch(::google::protobuf::int32 value) {
  set_has_m_ibranch();
  m_ibranch_ = value;
  // @@protoc_insertion_point(field_set:atd.map.LANE_LINK_KEY_GridLog.m_iBranch)
}

// -------------------------------------------------------------------

// GridLogInfo

// optional .atd.common.Header header = 1;
inline bool GridLogInfo::has_header() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void GridLogInfo::set_has_header() {
  _has_bits_[0] |= 0x00000001u;
}
inline void GridLogInfo::clear_has_header() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void GridLogInfo::clear_header() {
  if (header_ != NULL) header_->::atd::common::Header::Clear();
  clear_has_header();
}
inline const ::atd::common::Header& GridLogInfo::header() const {
  // @@protoc_insertion_point(field_get:atd.map.GridLogInfo.header)
  return header_ != NULL ? *header_
                         : *::atd::common::Header::internal_default_instance();
}
inline ::atd::common::Header* GridLogInfo::mutable_header() {
  set_has_header();
  if (header_ == NULL) {
    header_ = new ::atd::common::Header;
  }
  // @@protoc_insertion_point(field_mutable:atd.map.GridLogInfo.header)
  return header_;
}
inline ::atd::common::Header* GridLogInfo::release_header() {
  // @@protoc_insertion_point(field_release:atd.map.GridLogInfo.header)
  clear_has_header();
  ::atd::common::Header* temp = header_;
  header_ = NULL;
  return temp;
}
inline void GridLogInfo::set_allocated_header(::atd::common::Header* header) {
  delete header_;
  header_ = header;
  if (header) {
    set_has_header();
  } else {
    clear_has_header();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.map.GridLogInfo.header)
}

// optional .atd.common.Reserved reservedmsg = 2;
inline bool GridLogInfo::has_reservedmsg() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void GridLogInfo::set_has_reservedmsg() {
  _has_bits_[0] |= 0x00000002u;
}
inline void GridLogInfo::clear_has_reservedmsg() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void GridLogInfo::clear_reservedmsg() {
  if (reservedmsg_ != NULL) reservedmsg_->::atd::common::Reserved::Clear();
  clear_has_reservedmsg();
}
inline const ::atd::common::Reserved& GridLogInfo::reservedmsg() const {
  // @@protoc_insertion_point(field_get:atd.map.GridLogInfo.reservedmsg)
  return reservedmsg_ != NULL ? *reservedmsg_
                         : *::atd::common::Reserved::internal_default_instance();
}
inline ::atd::common::Reserved* GridLogInfo::mutable_reservedmsg() {
  set_has_reservedmsg();
  if (reservedmsg_ == NULL) {
    reservedmsg_ = new ::atd::common::Reserved;
  }
  // @@protoc_insertion_point(field_mutable:atd.map.GridLogInfo.reservedmsg)
  return reservedmsg_;
}
inline ::atd::common::Reserved* GridLogInfo::release_reservedmsg() {
  // @@protoc_insertion_point(field_release:atd.map.GridLogInfo.reservedmsg)
  clear_has_reservedmsg();
  ::atd::common::Reserved* temp = reservedmsg_;
  reservedmsg_ = NULL;
  return temp;
}
inline void GridLogInfo::set_allocated_reservedmsg(::atd::common::Reserved* reservedmsg) {
  delete reservedmsg_;
  reservedmsg_ = reservedmsg;
  if (reservedmsg) {
    set_has_reservedmsg();
  } else {
    clear_has_reservedmsg();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.map.GridLogInfo.reservedmsg)
}

// optional .atd.localization.Pose pose = 3;
inline bool GridLogInfo::has_pose() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void GridLogInfo::set_has_pose() {
  _has_bits_[0] |= 0x00000004u;
}
inline void GridLogInfo::clear_has_pose() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void GridLogInfo::clear_pose() {
  if (pose_ != NULL) pose_->::atd::localization::Pose::Clear();
  clear_has_pose();
}
inline const ::atd::localization::Pose& GridLogInfo::pose() const {
  // @@protoc_insertion_point(field_get:atd.map.GridLogInfo.pose)
  return pose_ != NULL ? *pose_
                         : *::atd::localization::Pose::internal_default_instance();
}
inline ::atd::localization::Pose* GridLogInfo::mutable_pose() {
  set_has_pose();
  if (pose_ == NULL) {
    pose_ = new ::atd::localization::Pose;
  }
  // @@protoc_insertion_point(field_mutable:atd.map.GridLogInfo.pose)
  return pose_;
}
inline ::atd::localization::Pose* GridLogInfo::release_pose() {
  // @@protoc_insertion_point(field_release:atd.map.GridLogInfo.pose)
  clear_has_pose();
  ::atd::localization::Pose* temp = pose_;
  pose_ = NULL;
  return temp;
}
inline void GridLogInfo::set_allocated_pose(::atd::localization::Pose* pose) {
  delete pose_;
  pose_ = pose;
  if (pose) {
    set_has_pose();
  } else {
    clear_has_pose();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.map.GridLogInfo.pose)
}

// optional .atd.map.LANE_LINK_KEY_GridLog currentlaneinfo = 4;
inline bool GridLogInfo::has_currentlaneinfo() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void GridLogInfo::set_has_currentlaneinfo() {
  _has_bits_[0] |= 0x00000008u;
}
inline void GridLogInfo::clear_has_currentlaneinfo() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void GridLogInfo::clear_currentlaneinfo() {
  if (currentlaneinfo_ != NULL) currentlaneinfo_->::atd::map::LANE_LINK_KEY_GridLog::Clear();
  clear_has_currentlaneinfo();
}
inline const ::atd::map::LANE_LINK_KEY_GridLog& GridLogInfo::currentlaneinfo() const {
  // @@protoc_insertion_point(field_get:atd.map.GridLogInfo.currentlaneinfo)
  return currentlaneinfo_ != NULL ? *currentlaneinfo_
                         : *::atd::map::LANE_LINK_KEY_GridLog::internal_default_instance();
}
inline ::atd::map::LANE_LINK_KEY_GridLog* GridLogInfo::mutable_currentlaneinfo() {
  set_has_currentlaneinfo();
  if (currentlaneinfo_ == NULL) {
    currentlaneinfo_ = new ::atd::map::LANE_LINK_KEY_GridLog;
  }
  // @@protoc_insertion_point(field_mutable:atd.map.GridLogInfo.currentlaneinfo)
  return currentlaneinfo_;
}
inline ::atd::map::LANE_LINK_KEY_GridLog* GridLogInfo::release_currentlaneinfo() {
  // @@protoc_insertion_point(field_release:atd.map.GridLogInfo.currentlaneinfo)
  clear_has_currentlaneinfo();
  ::atd::map::LANE_LINK_KEY_GridLog* temp = currentlaneinfo_;
  currentlaneinfo_ = NULL;
  return temp;
}
inline void GridLogInfo::set_allocated_currentlaneinfo(::atd::map::LANE_LINK_KEY_GridLog* currentlaneinfo) {
  delete currentlaneinfo_;
  currentlaneinfo_ = currentlaneinfo;
  if (currentlaneinfo) {
    set_has_currentlaneinfo();
  } else {
    clear_has_currentlaneinfo();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.map.GridLogInfo.currentlaneinfo)
}

// optional int64 m_iSpeed = 5;
inline bool GridLogInfo::has_m_ispeed() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void GridLogInfo::set_has_m_ispeed() {
  _has_bits_[0] |= 0x00000010u;
}
inline void GridLogInfo::clear_has_m_ispeed() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void GridLogInfo::clear_m_ispeed() {
  m_ispeed_ = GOOGLE_LONGLONG(0);
  clear_has_m_ispeed();
}
inline ::google::protobuf::int64 GridLogInfo::m_ispeed() const {
  // @@protoc_insertion_point(field_get:atd.map.GridLogInfo.m_iSpeed)
  return m_ispeed_;
}
inline void GridLogInfo::set_m_ispeed(::google::protobuf::int64 value) {
  set_has_m_ispeed();
  m_ispeed_ = value;
  // @@protoc_insertion_point(field_set:atd.map.GridLogInfo.m_iSpeed)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


}  // namespace map
}  // namespace atd

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_modules_2fipc_2fmessage_2fprotobuf_2fhdmap_2fgridlog_2eproto__INCLUDED
