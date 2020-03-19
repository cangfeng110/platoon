// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: modules/ipc/message/protobuf/localization/pose.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "modules/ipc/message/protobuf/localization/pose.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// @@protoc_insertion_point(includes)

namespace atd {
namespace localization {
class PoseDefaultTypeInternal : public ::google::protobuf::internal::ExplicitlyConstructed<Pose> {
} _Pose_default_instance_;

namespace protobuf_modules_2fipc_2fmessage_2fprotobuf_2flocalization_2fpose_2eproto {

PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::ParseTableField
    const TableStruct::entries[] = {
  {0, 0, 0, ::google::protobuf::internal::kInvalidMask, 0, 0},
};

PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::AuxillaryParseTableField
    const TableStruct::aux[] = {
  ::google::protobuf::internal::AuxillaryParseTableField(),
};
PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::ParseTable const
    TableStruct::schema[] = {
  { NULL, NULL, 0, -1, -1, false },
};


void TableStruct::Shutdown() {
  _Pose_default_instance_.Shutdown();
}

void TableStruct::InitDefaultsImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::internal::InitProtobufDefaults();
  ::atd::common::protobuf_modules_2fipc_2fmessage_2fprotobuf_2fcommon_2fgeometry_2eproto::InitDefaults();
  _Pose_default_instance_.DefaultConstruct();
  _Pose_default_instance_.get_mutable()->position_ = const_cast< ::atd::common::PointENU*>(
      ::atd::common::PointENU::internal_default_instance());
  _Pose_default_instance_.get_mutable()->orientation_ = const_cast< ::atd::common::Quaternion*>(
      ::atd::common::Quaternion::internal_default_instance());
  _Pose_default_instance_.get_mutable()->linear_velocity_ = const_cast< ::atd::common::Point3D*>(
      ::atd::common::Point3D::internal_default_instance());
  _Pose_default_instance_.get_mutable()->linear_acceleration_ = const_cast< ::atd::common::Point3D*>(
      ::atd::common::Point3D::internal_default_instance());
  _Pose_default_instance_.get_mutable()->angular_velocity_ = const_cast< ::atd::common::Point3D*>(
      ::atd::common::Point3D::internal_default_instance());
  _Pose_default_instance_.get_mutable()->linear_acceleration_vrf_ = const_cast< ::atd::common::Point3D*>(
      ::atd::common::Point3D::internal_default_instance());
  _Pose_default_instance_.get_mutable()->angular_velocity_vrf_ = const_cast< ::atd::common::Point3D*>(
      ::atd::common::Point3D::internal_default_instance());
  _Pose_default_instance_.get_mutable()->euler_angles_ = const_cast< ::atd::common::Point3D*>(
      ::atd::common::Point3D::internal_default_instance());
}

void InitDefaults() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &TableStruct::InitDefaultsImpl);
}
void AddDescriptorsImpl() {
  InitDefaults();
  ::atd::common::protobuf_modules_2fipc_2fmessage_2fprotobuf_2fcommon_2fgeometry_2eproto::AddDescriptors();
  ::google::protobuf::internal::OnShutdown(&TableStruct::Shutdown);
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
#endif  // GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER

}  // namespace protobuf_modules_2fipc_2fmessage_2fprotobuf_2flocalization_2fpose_2eproto


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Pose::kPositionFieldNumber;
const int Pose::kOrientationFieldNumber;
const int Pose::kLinearVelocityFieldNumber;
const int Pose::kLinearAccelerationFieldNumber;
const int Pose::kAngularVelocityFieldNumber;
const int Pose::kHeadingFieldNumber;
const int Pose::kLinearAccelerationVrfFieldNumber;
const int Pose::kAngularVelocityVrfFieldNumber;
const int Pose::kEulerAnglesFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Pose::Pose()
  : ::google::protobuf::MessageLite(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    protobuf_modules_2fipc_2fmessage_2fprotobuf_2flocalization_2fpose_2eproto::InitDefaults();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:atd.localization.Pose)
}
Pose::Pose(const Pose& from)
  : ::google::protobuf::MessageLite(),
      _internal_metadata_(NULL),
      _has_bits_(from._has_bits_),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  if (from.has_position()) {
    position_ = new ::atd::common::PointENU(*from.position_);
  } else {
    position_ = NULL;
  }
  if (from.has_orientation()) {
    orientation_ = new ::atd::common::Quaternion(*from.orientation_);
  } else {
    orientation_ = NULL;
  }
  if (from.has_linear_velocity()) {
    linear_velocity_ = new ::atd::common::Point3D(*from.linear_velocity_);
  } else {
    linear_velocity_ = NULL;
  }
  if (from.has_linear_acceleration()) {
    linear_acceleration_ = new ::atd::common::Point3D(*from.linear_acceleration_);
  } else {
    linear_acceleration_ = NULL;
  }
  if (from.has_angular_velocity()) {
    angular_velocity_ = new ::atd::common::Point3D(*from.angular_velocity_);
  } else {
    angular_velocity_ = NULL;
  }
  if (from.has_linear_acceleration_vrf()) {
    linear_acceleration_vrf_ = new ::atd::common::Point3D(*from.linear_acceleration_vrf_);
  } else {
    linear_acceleration_vrf_ = NULL;
  }
  if (from.has_angular_velocity_vrf()) {
    angular_velocity_vrf_ = new ::atd::common::Point3D(*from.angular_velocity_vrf_);
  } else {
    angular_velocity_vrf_ = NULL;
  }
  if (from.has_euler_angles()) {
    euler_angles_ = new ::atd::common::Point3D(*from.euler_angles_);
  } else {
    euler_angles_ = NULL;
  }
  heading_ = from.heading_;
  // @@protoc_insertion_point(copy_constructor:atd.localization.Pose)
}

void Pose::SharedCtor() {
  _cached_size_ = 0;
  ::memset(&position_, 0, reinterpret_cast<char*>(&heading_) -
    reinterpret_cast<char*>(&position_) + sizeof(heading_));
}

Pose::~Pose() {
  // @@protoc_insertion_point(destructor:atd.localization.Pose)
  SharedDtor();
}

void Pose::SharedDtor() {
  if (this != internal_default_instance()) {
    delete position_;
  }
  if (this != internal_default_instance()) {
    delete orientation_;
  }
  if (this != internal_default_instance()) {
    delete linear_velocity_;
  }
  if (this != internal_default_instance()) {
    delete linear_acceleration_;
  }
  if (this != internal_default_instance()) {
    delete angular_velocity_;
  }
  if (this != internal_default_instance()) {
    delete linear_acceleration_vrf_;
  }
  if (this != internal_default_instance()) {
    delete angular_velocity_vrf_;
  }
  if (this != internal_default_instance()) {
    delete euler_angles_;
  }
}

void Pose::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const Pose& Pose::default_instance() {
  protobuf_modules_2fipc_2fmessage_2fprotobuf_2flocalization_2fpose_2eproto::InitDefaults();
  return *internal_default_instance();
}

Pose* Pose::New(::google::protobuf::Arena* arena) const {
  Pose* n = new Pose;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void Pose::Clear() {
// @@protoc_insertion_point(message_clear_start:atd.localization.Pose)
  if (_has_bits_[0 / 32] & 255u) {
    if (has_position()) {
      GOOGLE_DCHECK(position_ != NULL);
      position_->::atd::common::PointENU::Clear();
    }
    if (has_orientation()) {
      GOOGLE_DCHECK(orientation_ != NULL);
      orientation_->::atd::common::Quaternion::Clear();
    }
    if (has_linear_velocity()) {
      GOOGLE_DCHECK(linear_velocity_ != NULL);
      linear_velocity_->::atd::common::Point3D::Clear();
    }
    if (has_linear_acceleration()) {
      GOOGLE_DCHECK(linear_acceleration_ != NULL);
      linear_acceleration_->::atd::common::Point3D::Clear();
    }
    if (has_angular_velocity()) {
      GOOGLE_DCHECK(angular_velocity_ != NULL);
      angular_velocity_->::atd::common::Point3D::Clear();
    }
    if (has_linear_acceleration_vrf()) {
      GOOGLE_DCHECK(linear_acceleration_vrf_ != NULL);
      linear_acceleration_vrf_->::atd::common::Point3D::Clear();
    }
    if (has_angular_velocity_vrf()) {
      GOOGLE_DCHECK(angular_velocity_vrf_ != NULL);
      angular_velocity_vrf_->::atd::common::Point3D::Clear();
    }
    if (has_euler_angles()) {
      GOOGLE_DCHECK(euler_angles_ != NULL);
      euler_angles_->::atd::common::Point3D::Clear();
    }
  }
  heading_ = 0;
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool Pose::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::io::LazyStringOutputStream unknown_fields_string(
      ::google::protobuf::NewPermanentCallback(&_internal_metadata_,
          &::google::protobuf::internal::InternalMetadataWithArenaLite::
              mutable_unknown_fields));
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_string, false);
  // @@protoc_insertion_point(parse_start:atd.localization.Pose)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .atd.common.PointENU position = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_position()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .atd.common.Quaternion orientation = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(18u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_orientation()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .atd.common.Point3D linear_velocity = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(26u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_linear_velocity()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .atd.common.Point3D linear_acceleration = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(34u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_linear_acceleration()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .atd.common.Point3D angular_velocity = 5;
      case 5: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(42u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_angular_velocity()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional double heading = 6;
      case 6: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(49u)) {
          set_has_heading();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &heading_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .atd.common.Point3D linear_acceleration_vrf = 7;
      case 7: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(58u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_linear_acceleration_vrf()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .atd.common.Point3D angular_velocity_vrf = 8;
      case 8: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(66u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_angular_velocity_vrf()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .atd.common.Point3D euler_angles = 9;
      case 9: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(74u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_euler_angles()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:atd.localization.Pose)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:atd.localization.Pose)
  return false;
#undef DO_
}

void Pose::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:atd.localization.Pose)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // optional .atd.common.PointENU position = 1;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      1, *this->position_, output);
  }

  // optional .atd.common.Quaternion orientation = 2;
  if (cached_has_bits & 0x00000002u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      2, *this->orientation_, output);
  }

  // optional .atd.common.Point3D linear_velocity = 3;
  if (cached_has_bits & 0x00000004u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      3, *this->linear_velocity_, output);
  }

  // optional .atd.common.Point3D linear_acceleration = 4;
  if (cached_has_bits & 0x00000008u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      4, *this->linear_acceleration_, output);
  }

  // optional .atd.common.Point3D angular_velocity = 5;
  if (cached_has_bits & 0x00000010u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      5, *this->angular_velocity_, output);
  }

  // optional double heading = 6;
  if (cached_has_bits & 0x00000100u) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(6, this->heading(), output);
  }

  // optional .atd.common.Point3D linear_acceleration_vrf = 7;
  if (cached_has_bits & 0x00000020u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      7, *this->linear_acceleration_vrf_, output);
  }

  // optional .atd.common.Point3D angular_velocity_vrf = 8;
  if (cached_has_bits & 0x00000040u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      8, *this->angular_velocity_vrf_, output);
  }

  // optional .atd.common.Point3D euler_angles = 9;
  if (cached_has_bits & 0x00000080u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      9, *this->euler_angles_, output);
  }

  output->WriteRaw(unknown_fields().data(),
                   static_cast<int>(unknown_fields().size()));
  // @@protoc_insertion_point(serialize_end:atd.localization.Pose)
}

size_t Pose::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:atd.localization.Pose)
  size_t total_size = 0;

  total_size += unknown_fields().size();

  if (_has_bits_[0 / 32] & 255u) {
    // optional .atd.common.PointENU position = 1;
    if (has_position()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          *this->position_);
    }

    // optional .atd.common.Quaternion orientation = 2;
    if (has_orientation()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          *this->orientation_);
    }

    // optional .atd.common.Point3D linear_velocity = 3;
    if (has_linear_velocity()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          *this->linear_velocity_);
    }

    // optional .atd.common.Point3D linear_acceleration = 4;
    if (has_linear_acceleration()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          *this->linear_acceleration_);
    }

    // optional .atd.common.Point3D angular_velocity = 5;
    if (has_angular_velocity()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          *this->angular_velocity_);
    }

    // optional .atd.common.Point3D linear_acceleration_vrf = 7;
    if (has_linear_acceleration_vrf()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          *this->linear_acceleration_vrf_);
    }

    // optional .atd.common.Point3D angular_velocity_vrf = 8;
    if (has_angular_velocity_vrf()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          *this->angular_velocity_vrf_);
    }

    // optional .atd.common.Point3D euler_angles = 9;
    if (has_euler_angles()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          *this->euler_angles_);
    }

  }
  // optional double heading = 6;
  if (has_heading()) {
    total_size += 1 + 8;
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Pose::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const Pose*>(&from));
}

void Pose::MergeFrom(const Pose& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:atd.localization.Pose)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 255u) {
    if (cached_has_bits & 0x00000001u) {
      mutable_position()->::atd::common::PointENU::MergeFrom(from.position());
    }
    if (cached_has_bits & 0x00000002u) {
      mutable_orientation()->::atd::common::Quaternion::MergeFrom(from.orientation());
    }
    if (cached_has_bits & 0x00000004u) {
      mutable_linear_velocity()->::atd::common::Point3D::MergeFrom(from.linear_velocity());
    }
    if (cached_has_bits & 0x00000008u) {
      mutable_linear_acceleration()->::atd::common::Point3D::MergeFrom(from.linear_acceleration());
    }
    if (cached_has_bits & 0x00000010u) {
      mutable_angular_velocity()->::atd::common::Point3D::MergeFrom(from.angular_velocity());
    }
    if (cached_has_bits & 0x00000020u) {
      mutable_linear_acceleration_vrf()->::atd::common::Point3D::MergeFrom(from.linear_acceleration_vrf());
    }
    if (cached_has_bits & 0x00000040u) {
      mutable_angular_velocity_vrf()->::atd::common::Point3D::MergeFrom(from.angular_velocity_vrf());
    }
    if (cached_has_bits & 0x00000080u) {
      mutable_euler_angles()->::atd::common::Point3D::MergeFrom(from.euler_angles());
    }
  }
  if (cached_has_bits & 0x00000100u) {
    set_heading(from.heading());
  }
}

void Pose::CopyFrom(const Pose& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:atd.localization.Pose)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Pose::IsInitialized() const {
  return true;
}

void Pose::Swap(Pose* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Pose::InternalSwap(Pose* other) {
  std::swap(position_, other->position_);
  std::swap(orientation_, other->orientation_);
  std::swap(linear_velocity_, other->linear_velocity_);
  std::swap(linear_acceleration_, other->linear_acceleration_);
  std::swap(angular_velocity_, other->angular_velocity_);
  std::swap(linear_acceleration_vrf_, other->linear_acceleration_vrf_);
  std::swap(angular_velocity_vrf_, other->angular_velocity_vrf_);
  std::swap(euler_angles_, other->euler_angles_);
  std::swap(heading_, other->heading_);
  std::swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::std::string Pose::GetTypeName() const {
  return "atd.localization.Pose";
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// Pose

// optional .atd.common.PointENU position = 1;
bool Pose::has_position() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
void Pose::set_has_position() {
  _has_bits_[0] |= 0x00000001u;
}
void Pose::clear_has_position() {
  _has_bits_[0] &= ~0x00000001u;
}
void Pose::clear_position() {
  if (position_ != NULL) position_->::atd::common::PointENU::Clear();
  clear_has_position();
}
const ::atd::common::PointENU& Pose::position() const {
  // @@protoc_insertion_point(field_get:atd.localization.Pose.position)
  return position_ != NULL ? *position_
                         : *::atd::common::PointENU::internal_default_instance();
}
::atd::common::PointENU* Pose::mutable_position() {
  set_has_position();
  if (position_ == NULL) {
    position_ = new ::atd::common::PointENU;
  }
  // @@protoc_insertion_point(field_mutable:atd.localization.Pose.position)
  return position_;
}
::atd::common::PointENU* Pose::release_position() {
  // @@protoc_insertion_point(field_release:atd.localization.Pose.position)
  clear_has_position();
  ::atd::common::PointENU* temp = position_;
  position_ = NULL;
  return temp;
}
void Pose::set_allocated_position(::atd::common::PointENU* position) {
  delete position_;
  position_ = position;
  if (position) {
    set_has_position();
  } else {
    clear_has_position();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.localization.Pose.position)
}

// optional .atd.common.Quaternion orientation = 2;
bool Pose::has_orientation() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
void Pose::set_has_orientation() {
  _has_bits_[0] |= 0x00000002u;
}
void Pose::clear_has_orientation() {
  _has_bits_[0] &= ~0x00000002u;
}
void Pose::clear_orientation() {
  if (orientation_ != NULL) orientation_->::atd::common::Quaternion::Clear();
  clear_has_orientation();
}
const ::atd::common::Quaternion& Pose::orientation() const {
  // @@protoc_insertion_point(field_get:atd.localization.Pose.orientation)
  return orientation_ != NULL ? *orientation_
                         : *::atd::common::Quaternion::internal_default_instance();
}
::atd::common::Quaternion* Pose::mutable_orientation() {
  set_has_orientation();
  if (orientation_ == NULL) {
    orientation_ = new ::atd::common::Quaternion;
  }
  // @@protoc_insertion_point(field_mutable:atd.localization.Pose.orientation)
  return orientation_;
}
::atd::common::Quaternion* Pose::release_orientation() {
  // @@protoc_insertion_point(field_release:atd.localization.Pose.orientation)
  clear_has_orientation();
  ::atd::common::Quaternion* temp = orientation_;
  orientation_ = NULL;
  return temp;
}
void Pose::set_allocated_orientation(::atd::common::Quaternion* orientation) {
  delete orientation_;
  orientation_ = orientation;
  if (orientation) {
    set_has_orientation();
  } else {
    clear_has_orientation();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.localization.Pose.orientation)
}

// optional .atd.common.Point3D linear_velocity = 3;
bool Pose::has_linear_velocity() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
void Pose::set_has_linear_velocity() {
  _has_bits_[0] |= 0x00000004u;
}
void Pose::clear_has_linear_velocity() {
  _has_bits_[0] &= ~0x00000004u;
}
void Pose::clear_linear_velocity() {
  if (linear_velocity_ != NULL) linear_velocity_->::atd::common::Point3D::Clear();
  clear_has_linear_velocity();
}
const ::atd::common::Point3D& Pose::linear_velocity() const {
  // @@protoc_insertion_point(field_get:atd.localization.Pose.linear_velocity)
  return linear_velocity_ != NULL ? *linear_velocity_
                         : *::atd::common::Point3D::internal_default_instance();
}
::atd::common::Point3D* Pose::mutable_linear_velocity() {
  set_has_linear_velocity();
  if (linear_velocity_ == NULL) {
    linear_velocity_ = new ::atd::common::Point3D;
  }
  // @@protoc_insertion_point(field_mutable:atd.localization.Pose.linear_velocity)
  return linear_velocity_;
}
::atd::common::Point3D* Pose::release_linear_velocity() {
  // @@protoc_insertion_point(field_release:atd.localization.Pose.linear_velocity)
  clear_has_linear_velocity();
  ::atd::common::Point3D* temp = linear_velocity_;
  linear_velocity_ = NULL;
  return temp;
}
void Pose::set_allocated_linear_velocity(::atd::common::Point3D* linear_velocity) {
  delete linear_velocity_;
  linear_velocity_ = linear_velocity;
  if (linear_velocity) {
    set_has_linear_velocity();
  } else {
    clear_has_linear_velocity();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.localization.Pose.linear_velocity)
}

// optional .atd.common.Point3D linear_acceleration = 4;
bool Pose::has_linear_acceleration() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
void Pose::set_has_linear_acceleration() {
  _has_bits_[0] |= 0x00000008u;
}
void Pose::clear_has_linear_acceleration() {
  _has_bits_[0] &= ~0x00000008u;
}
void Pose::clear_linear_acceleration() {
  if (linear_acceleration_ != NULL) linear_acceleration_->::atd::common::Point3D::Clear();
  clear_has_linear_acceleration();
}
const ::atd::common::Point3D& Pose::linear_acceleration() const {
  // @@protoc_insertion_point(field_get:atd.localization.Pose.linear_acceleration)
  return linear_acceleration_ != NULL ? *linear_acceleration_
                         : *::atd::common::Point3D::internal_default_instance();
}
::atd::common::Point3D* Pose::mutable_linear_acceleration() {
  set_has_linear_acceleration();
  if (linear_acceleration_ == NULL) {
    linear_acceleration_ = new ::atd::common::Point3D;
  }
  // @@protoc_insertion_point(field_mutable:atd.localization.Pose.linear_acceleration)
  return linear_acceleration_;
}
::atd::common::Point3D* Pose::release_linear_acceleration() {
  // @@protoc_insertion_point(field_release:atd.localization.Pose.linear_acceleration)
  clear_has_linear_acceleration();
  ::atd::common::Point3D* temp = linear_acceleration_;
  linear_acceleration_ = NULL;
  return temp;
}
void Pose::set_allocated_linear_acceleration(::atd::common::Point3D* linear_acceleration) {
  delete linear_acceleration_;
  linear_acceleration_ = linear_acceleration;
  if (linear_acceleration) {
    set_has_linear_acceleration();
  } else {
    clear_has_linear_acceleration();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.localization.Pose.linear_acceleration)
}

// optional .atd.common.Point3D angular_velocity = 5;
bool Pose::has_angular_velocity() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
void Pose::set_has_angular_velocity() {
  _has_bits_[0] |= 0x00000010u;
}
void Pose::clear_has_angular_velocity() {
  _has_bits_[0] &= ~0x00000010u;
}
void Pose::clear_angular_velocity() {
  if (angular_velocity_ != NULL) angular_velocity_->::atd::common::Point3D::Clear();
  clear_has_angular_velocity();
}
const ::atd::common::Point3D& Pose::angular_velocity() const {
  // @@protoc_insertion_point(field_get:atd.localization.Pose.angular_velocity)
  return angular_velocity_ != NULL ? *angular_velocity_
                         : *::atd::common::Point3D::internal_default_instance();
}
::atd::common::Point3D* Pose::mutable_angular_velocity() {
  set_has_angular_velocity();
  if (angular_velocity_ == NULL) {
    angular_velocity_ = new ::atd::common::Point3D;
  }
  // @@protoc_insertion_point(field_mutable:atd.localization.Pose.angular_velocity)
  return angular_velocity_;
}
::atd::common::Point3D* Pose::release_angular_velocity() {
  // @@protoc_insertion_point(field_release:atd.localization.Pose.angular_velocity)
  clear_has_angular_velocity();
  ::atd::common::Point3D* temp = angular_velocity_;
  angular_velocity_ = NULL;
  return temp;
}
void Pose::set_allocated_angular_velocity(::atd::common::Point3D* angular_velocity) {
  delete angular_velocity_;
  angular_velocity_ = angular_velocity;
  if (angular_velocity) {
    set_has_angular_velocity();
  } else {
    clear_has_angular_velocity();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.localization.Pose.angular_velocity)
}

// optional double heading = 6;
bool Pose::has_heading() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
void Pose::set_has_heading() {
  _has_bits_[0] |= 0x00000100u;
}
void Pose::clear_has_heading() {
  _has_bits_[0] &= ~0x00000100u;
}
void Pose::clear_heading() {
  heading_ = 0;
  clear_has_heading();
}
double Pose::heading() const {
  // @@protoc_insertion_point(field_get:atd.localization.Pose.heading)
  return heading_;
}
void Pose::set_heading(double value) {
  set_has_heading();
  heading_ = value;
  // @@protoc_insertion_point(field_set:atd.localization.Pose.heading)
}

// optional .atd.common.Point3D linear_acceleration_vrf = 7;
bool Pose::has_linear_acceleration_vrf() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
void Pose::set_has_linear_acceleration_vrf() {
  _has_bits_[0] |= 0x00000020u;
}
void Pose::clear_has_linear_acceleration_vrf() {
  _has_bits_[0] &= ~0x00000020u;
}
void Pose::clear_linear_acceleration_vrf() {
  if (linear_acceleration_vrf_ != NULL) linear_acceleration_vrf_->::atd::common::Point3D::Clear();
  clear_has_linear_acceleration_vrf();
}
const ::atd::common::Point3D& Pose::linear_acceleration_vrf() const {
  // @@protoc_insertion_point(field_get:atd.localization.Pose.linear_acceleration_vrf)
  return linear_acceleration_vrf_ != NULL ? *linear_acceleration_vrf_
                         : *::atd::common::Point3D::internal_default_instance();
}
::atd::common::Point3D* Pose::mutable_linear_acceleration_vrf() {
  set_has_linear_acceleration_vrf();
  if (linear_acceleration_vrf_ == NULL) {
    linear_acceleration_vrf_ = new ::atd::common::Point3D;
  }
  // @@protoc_insertion_point(field_mutable:atd.localization.Pose.linear_acceleration_vrf)
  return linear_acceleration_vrf_;
}
::atd::common::Point3D* Pose::release_linear_acceleration_vrf() {
  // @@protoc_insertion_point(field_release:atd.localization.Pose.linear_acceleration_vrf)
  clear_has_linear_acceleration_vrf();
  ::atd::common::Point3D* temp = linear_acceleration_vrf_;
  linear_acceleration_vrf_ = NULL;
  return temp;
}
void Pose::set_allocated_linear_acceleration_vrf(::atd::common::Point3D* linear_acceleration_vrf) {
  delete linear_acceleration_vrf_;
  linear_acceleration_vrf_ = linear_acceleration_vrf;
  if (linear_acceleration_vrf) {
    set_has_linear_acceleration_vrf();
  } else {
    clear_has_linear_acceleration_vrf();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.localization.Pose.linear_acceleration_vrf)
}

// optional .atd.common.Point3D angular_velocity_vrf = 8;
bool Pose::has_angular_velocity_vrf() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
void Pose::set_has_angular_velocity_vrf() {
  _has_bits_[0] |= 0x00000040u;
}
void Pose::clear_has_angular_velocity_vrf() {
  _has_bits_[0] &= ~0x00000040u;
}
void Pose::clear_angular_velocity_vrf() {
  if (angular_velocity_vrf_ != NULL) angular_velocity_vrf_->::atd::common::Point3D::Clear();
  clear_has_angular_velocity_vrf();
}
const ::atd::common::Point3D& Pose::angular_velocity_vrf() const {
  // @@protoc_insertion_point(field_get:atd.localization.Pose.angular_velocity_vrf)
  return angular_velocity_vrf_ != NULL ? *angular_velocity_vrf_
                         : *::atd::common::Point3D::internal_default_instance();
}
::atd::common::Point3D* Pose::mutable_angular_velocity_vrf() {
  set_has_angular_velocity_vrf();
  if (angular_velocity_vrf_ == NULL) {
    angular_velocity_vrf_ = new ::atd::common::Point3D;
  }
  // @@protoc_insertion_point(field_mutable:atd.localization.Pose.angular_velocity_vrf)
  return angular_velocity_vrf_;
}
::atd::common::Point3D* Pose::release_angular_velocity_vrf() {
  // @@protoc_insertion_point(field_release:atd.localization.Pose.angular_velocity_vrf)
  clear_has_angular_velocity_vrf();
  ::atd::common::Point3D* temp = angular_velocity_vrf_;
  angular_velocity_vrf_ = NULL;
  return temp;
}
void Pose::set_allocated_angular_velocity_vrf(::atd::common::Point3D* angular_velocity_vrf) {
  delete angular_velocity_vrf_;
  angular_velocity_vrf_ = angular_velocity_vrf;
  if (angular_velocity_vrf) {
    set_has_angular_velocity_vrf();
  } else {
    clear_has_angular_velocity_vrf();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.localization.Pose.angular_velocity_vrf)
}

// optional .atd.common.Point3D euler_angles = 9;
bool Pose::has_euler_angles() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
void Pose::set_has_euler_angles() {
  _has_bits_[0] |= 0x00000080u;
}
void Pose::clear_has_euler_angles() {
  _has_bits_[0] &= ~0x00000080u;
}
void Pose::clear_euler_angles() {
  if (euler_angles_ != NULL) euler_angles_->::atd::common::Point3D::Clear();
  clear_has_euler_angles();
}
const ::atd::common::Point3D& Pose::euler_angles() const {
  // @@protoc_insertion_point(field_get:atd.localization.Pose.euler_angles)
  return euler_angles_ != NULL ? *euler_angles_
                         : *::atd::common::Point3D::internal_default_instance();
}
::atd::common::Point3D* Pose::mutable_euler_angles() {
  set_has_euler_angles();
  if (euler_angles_ == NULL) {
    euler_angles_ = new ::atd::common::Point3D;
  }
  // @@protoc_insertion_point(field_mutable:atd.localization.Pose.euler_angles)
  return euler_angles_;
}
::atd::common::Point3D* Pose::release_euler_angles() {
  // @@protoc_insertion_point(field_release:atd.localization.Pose.euler_angles)
  clear_has_euler_angles();
  ::atd::common::Point3D* temp = euler_angles_;
  euler_angles_ = NULL;
  return temp;
}
void Pose::set_allocated_euler_angles(::atd::common::Point3D* euler_angles) {
  delete euler_angles_;
  euler_angles_ = euler_angles;
  if (euler_angles) {
    set_has_euler_angles();
  } else {
    clear_has_euler_angles();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.localization.Pose.euler_angles)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace localization
}  // namespace atd

// @@protoc_insertion_point(global_scope)
