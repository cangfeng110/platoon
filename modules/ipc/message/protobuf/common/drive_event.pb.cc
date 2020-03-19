// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: modules/ipc/message/protobuf/common/drive_event.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "modules/ipc/message/protobuf/common/drive_event.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// @@protoc_insertion_point(includes)

namespace atd {
namespace common {
class DriveEventDefaultTypeInternal : public ::google::protobuf::internal::ExplicitlyConstructed<DriveEvent> {
} _DriveEvent_default_instance_;

namespace protobuf_modules_2fipc_2fmessage_2fprotobuf_2fcommon_2fdrive_5fevent_2eproto {

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
  _DriveEvent_default_instance_.Shutdown();
}

void TableStruct::InitDefaultsImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::internal::InitProtobufDefaults();
  ::atd::common::protobuf_modules_2fipc_2fmessage_2fprotobuf_2fcommon_2fheader_2eproto::InitDefaults();
  ::atd::localization::protobuf_modules_2fipc_2fmessage_2fprotobuf_2flocalization_2fpose_2eproto::InitDefaults();
  _DriveEvent_default_instance_.DefaultConstruct();
  _DriveEvent_default_instance_.get_mutable()->header_ = const_cast< ::atd::common::Header*>(
      ::atd::common::Header::internal_default_instance());
  _DriveEvent_default_instance_.get_mutable()->location_ = const_cast< ::atd::localization::Pose*>(
      ::atd::localization::Pose::internal_default_instance());
}

void InitDefaults() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &TableStruct::InitDefaultsImpl);
}
void AddDescriptorsImpl() {
  InitDefaults();
  ::atd::common::protobuf_modules_2fipc_2fmessage_2fprotobuf_2fcommon_2fheader_2eproto::AddDescriptors();
  ::atd::localization::protobuf_modules_2fipc_2fmessage_2fprotobuf_2flocalization_2fpose_2eproto::AddDescriptors();
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

}  // namespace protobuf_modules_2fipc_2fmessage_2fprotobuf_2fcommon_2fdrive_5fevent_2eproto


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int DriveEvent::kHeaderFieldNumber;
const int DriveEvent::kEventFieldNumber;
const int DriveEvent::kLocationFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

DriveEvent::DriveEvent()
  : ::google::protobuf::MessageLite(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    protobuf_modules_2fipc_2fmessage_2fprotobuf_2fcommon_2fdrive_5fevent_2eproto::InitDefaults();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:atd.common.DriveEvent)
}
DriveEvent::DriveEvent(const DriveEvent& from)
  : ::google::protobuf::MessageLite(),
      _internal_metadata_(NULL),
      _has_bits_(from._has_bits_),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  event_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.has_event()) {
    event_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.event_);
  }
  if (from.has_header()) {
    header_ = new ::atd::common::Header(*from.header_);
  } else {
    header_ = NULL;
  }
  if (from.has_location()) {
    location_ = new ::atd::localization::Pose(*from.location_);
  } else {
    location_ = NULL;
  }
  // @@protoc_insertion_point(copy_constructor:atd.common.DriveEvent)
}

void DriveEvent::SharedCtor() {
  _cached_size_ = 0;
  event_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&header_, 0, reinterpret_cast<char*>(&location_) -
    reinterpret_cast<char*>(&header_) + sizeof(location_));
}

DriveEvent::~DriveEvent() {
  // @@protoc_insertion_point(destructor:atd.common.DriveEvent)
  SharedDtor();
}

void DriveEvent::SharedDtor() {
  event_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (this != internal_default_instance()) {
    delete header_;
  }
  if (this != internal_default_instance()) {
    delete location_;
  }
}

void DriveEvent::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const DriveEvent& DriveEvent::default_instance() {
  protobuf_modules_2fipc_2fmessage_2fprotobuf_2fcommon_2fdrive_5fevent_2eproto::InitDefaults();
  return *internal_default_instance();
}

DriveEvent* DriveEvent::New(::google::protobuf::Arena* arena) const {
  DriveEvent* n = new DriveEvent;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void DriveEvent::Clear() {
// @@protoc_insertion_point(message_clear_start:atd.common.DriveEvent)
  if (_has_bits_[0 / 32] & 7u) {
    if (has_event()) {
      GOOGLE_DCHECK(!event_.IsDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited()));
      (*event_.UnsafeRawStringPointer())->clear();
    }
    if (has_header()) {
      GOOGLE_DCHECK(header_ != NULL);
      header_->::atd::common::Header::Clear();
    }
    if (has_location()) {
      GOOGLE_DCHECK(location_ != NULL);
      location_->::atd::localization::Pose::Clear();
    }
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool DriveEvent::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::io::LazyStringOutputStream unknown_fields_string(
      ::google::protobuf::NewPermanentCallback(&_internal_metadata_,
          &::google::protobuf::internal::InternalMetadataWithArenaLite::
              mutable_unknown_fields));
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_string, false);
  // @@protoc_insertion_point(parse_start:atd.common.DriveEvent)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .atd.common.Header header = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_header()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional string event = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(18u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_event()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .atd.localization.Pose location = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(26u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_location()));
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
  // @@protoc_insertion_point(parse_success:atd.common.DriveEvent)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:atd.common.DriveEvent)
  return false;
#undef DO_
}

void DriveEvent::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:atd.common.DriveEvent)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // optional .atd.common.Header header = 1;
  if (cached_has_bits & 0x00000002u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      1, *this->header_, output);
  }

  // optional string event = 2;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->event(), output);
  }

  // optional .atd.localization.Pose location = 3;
  if (cached_has_bits & 0x00000004u) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      3, *this->location_, output);
  }

  output->WriteRaw(unknown_fields().data(),
                   static_cast<int>(unknown_fields().size()));
  // @@protoc_insertion_point(serialize_end:atd.common.DriveEvent)
}

size_t DriveEvent::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:atd.common.DriveEvent)
  size_t total_size = 0;

  total_size += unknown_fields().size();

  if (_has_bits_[0 / 32] & 7u) {
    // optional string event = 2;
    if (has_event()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->event());
    }

    // optional .atd.common.Header header = 1;
    if (has_header()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          *this->header_);
    }

    // optional .atd.localization.Pose location = 3;
    if (has_location()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          *this->location_);
    }

  }
  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void DriveEvent::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const DriveEvent*>(&from));
}

void DriveEvent::MergeFrom(const DriveEvent& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:atd.common.DriveEvent)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 7u) {
    if (cached_has_bits & 0x00000001u) {
      set_has_event();
      event_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.event_);
    }
    if (cached_has_bits & 0x00000002u) {
      mutable_header()->::atd::common::Header::MergeFrom(from.header());
    }
    if (cached_has_bits & 0x00000004u) {
      mutable_location()->::atd::localization::Pose::MergeFrom(from.location());
    }
  }
}

void DriveEvent::CopyFrom(const DriveEvent& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:atd.common.DriveEvent)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool DriveEvent::IsInitialized() const {
  return true;
}

void DriveEvent::Swap(DriveEvent* other) {
  if (other == this) return;
  InternalSwap(other);
}
void DriveEvent::InternalSwap(DriveEvent* other) {
  event_.Swap(&other->event_);
  std::swap(header_, other->header_);
  std::swap(location_, other->location_);
  std::swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  std::swap(_cached_size_, other->_cached_size_);
}

::std::string DriveEvent::GetTypeName() const {
  return "atd.common.DriveEvent";
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// DriveEvent

// optional .atd.common.Header header = 1;
bool DriveEvent::has_header() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
void DriveEvent::set_has_header() {
  _has_bits_[0] |= 0x00000002u;
}
void DriveEvent::clear_has_header() {
  _has_bits_[0] &= ~0x00000002u;
}
void DriveEvent::clear_header() {
  if (header_ != NULL) header_->::atd::common::Header::Clear();
  clear_has_header();
}
const ::atd::common::Header& DriveEvent::header() const {
  // @@protoc_insertion_point(field_get:atd.common.DriveEvent.header)
  return header_ != NULL ? *header_
                         : *::atd::common::Header::internal_default_instance();
}
::atd::common::Header* DriveEvent::mutable_header() {
  set_has_header();
  if (header_ == NULL) {
    header_ = new ::atd::common::Header;
  }
  // @@protoc_insertion_point(field_mutable:atd.common.DriveEvent.header)
  return header_;
}
::atd::common::Header* DriveEvent::release_header() {
  // @@protoc_insertion_point(field_release:atd.common.DriveEvent.header)
  clear_has_header();
  ::atd::common::Header* temp = header_;
  header_ = NULL;
  return temp;
}
void DriveEvent::set_allocated_header(::atd::common::Header* header) {
  delete header_;
  header_ = header;
  if (header) {
    set_has_header();
  } else {
    clear_has_header();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.common.DriveEvent.header)
}

// optional string event = 2;
bool DriveEvent::has_event() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
void DriveEvent::set_has_event() {
  _has_bits_[0] |= 0x00000001u;
}
void DriveEvent::clear_has_event() {
  _has_bits_[0] &= ~0x00000001u;
}
void DriveEvent::clear_event() {
  event_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_event();
}
const ::std::string& DriveEvent::event() const {
  // @@protoc_insertion_point(field_get:atd.common.DriveEvent.event)
  return event_.GetNoArena();
}
void DriveEvent::set_event(const ::std::string& value) {
  set_has_event();
  event_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:atd.common.DriveEvent.event)
}
#if LANG_CXX11
void DriveEvent::set_event(::std::string&& value) {
  set_has_event();
  event_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:atd.common.DriveEvent.event)
}
#endif
void DriveEvent::set_event(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_event();
  event_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:atd.common.DriveEvent.event)
}
void DriveEvent::set_event(const char* value, size_t size) {
  set_has_event();
  event_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:atd.common.DriveEvent.event)
}
::std::string* DriveEvent::mutable_event() {
  set_has_event();
  // @@protoc_insertion_point(field_mutable:atd.common.DriveEvent.event)
  return event_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
::std::string* DriveEvent::release_event() {
  // @@protoc_insertion_point(field_release:atd.common.DriveEvent.event)
  clear_has_event();
  return event_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
void DriveEvent::set_allocated_event(::std::string* event) {
  if (event != NULL) {
    set_has_event();
  } else {
    clear_has_event();
  }
  event_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), event);
  // @@protoc_insertion_point(field_set_allocated:atd.common.DriveEvent.event)
}

// optional .atd.localization.Pose location = 3;
bool DriveEvent::has_location() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
void DriveEvent::set_has_location() {
  _has_bits_[0] |= 0x00000004u;
}
void DriveEvent::clear_has_location() {
  _has_bits_[0] &= ~0x00000004u;
}
void DriveEvent::clear_location() {
  if (location_ != NULL) location_->::atd::localization::Pose::Clear();
  clear_has_location();
}
const ::atd::localization::Pose& DriveEvent::location() const {
  // @@protoc_insertion_point(field_get:atd.common.DriveEvent.location)
  return location_ != NULL ? *location_
                         : *::atd::localization::Pose::internal_default_instance();
}
::atd::localization::Pose* DriveEvent::mutable_location() {
  set_has_location();
  if (location_ == NULL) {
    location_ = new ::atd::localization::Pose;
  }
  // @@protoc_insertion_point(field_mutable:atd.common.DriveEvent.location)
  return location_;
}
::atd::localization::Pose* DriveEvent::release_location() {
  // @@protoc_insertion_point(field_release:atd.common.DriveEvent.location)
  clear_has_location();
  ::atd::localization::Pose* temp = location_;
  location_ = NULL;
  return temp;
}
void DriveEvent::set_allocated_location(::atd::localization::Pose* location) {
  delete location_;
  location_ = location;
  if (location) {
    set_has_location();
  } else {
    clear_has_location();
  }
  // @@protoc_insertion_point(field_set_allocated:atd.common.DriveEvent.location)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace common
}  // namespace atd

// @@protoc_insertion_point(global_scope)
