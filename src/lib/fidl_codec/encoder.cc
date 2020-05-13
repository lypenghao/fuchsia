// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/lib/fidl_codec/encoder.h"

#include <lib/fidl/txn_header.h>
#include <lib/syslog/cpp/macros.h>

#include <algorithm>

#include "src/lib/fidl_codec/type_visitor.h"
#include "src/lib/fidl_codec/wire_types.h"

namespace fidl_codec {

class NullVisitor : public TypeVisitor {
 public:
  explicit NullVisitor(Encoder* encoder) : encoder_(encoder) {}

 private:
  void VisitType(const Type* type) override {
    FX_LOGS(FATAL) << "Type " << type->Name() << " can't be null.";
  }

  void VisitStringType(const StringType* type) override {
    FX_DCHECK(type->Nullable());
    encoder_->WriteValue<uint64_t>(0);
    encoder_->WriteValue<uint64_t>(0);
  }

  void VisitUnionType(const UnionType* type) override {
    FX_DCHECK(type->Nullable());
    encoder_->WriteValue<uint64_t>(0);
    encoder_->WriteValue<uint32_t>(0);
    encoder_->WriteValue<uint32_t>(0);
    encoder_->WriteValue<uint64_t>(0);
  }

  void VisitStructType(const StructType* type) override {
    FX_DCHECK(type->Nullable());
    encoder_->WriteValue<uint64_t>(0);
  }

  void VisitVectorType(const VectorType* type) override {
    FX_DCHECK(type->Nullable());
    encoder_->WriteValue<uint64_t>(0);
    encoder_->WriteValue<uint64_t>(0);
  }

  Encoder* encoder_;
};

Encoder::Result Encoder::EncodeMessage(uint32_t tx_id, uint64_t ordinal, const uint8_t flags[3],
                                       uint8_t magic, const StructValue& object) {
  Encoder encoder;

  size_t object_size = object.struct_definition().size();
  encoder.AllocateObject(object_size);
  encoder.WriteValue(tx_id);
  encoder.WriteValue(flags[0]);
  encoder.WriteValue(flags[1]);
  encoder.WriteValue(flags[2]);
  encoder.WriteValue(magic);
  encoder.WriteValue(ordinal);
  FX_DCHECK(sizeof(fidl_message_header_t) == encoder.current_offset_);

  // The primary object offsets include the header size, so the offset of the object is zero.
  encoder.VisitStructValueBody(0, &object);

  return Result{std::move(encoder.bytes_), std::move(encoder.handles_)};
}

size_t Encoder::AllocateObject(size_t size) {
  size_t object_offset = bytes_.size();
  bytes_.resize((bytes_.size() + size + 7) & ~7);
  return object_offset;
}

void Encoder::WriteData(const uint8_t* data, size_t size) {
  FX_DCHECK(current_offset_ + size <= bytes_.size())
      << "needs " << size << " bytes at offset " << current_offset_ << " buffer size is "
      << bytes_.size();
  std::copy(data, data + size, bytes_.data() + current_offset_);
  current_offset_ += size;
}

void Encoder::EncodeEnvelope(const Value* value, const Type* for_type) {
  Encoder envelope_encoder;
  envelope_encoder.AllocateObject(for_type->InlineSize());
  value->Visit(&envelope_encoder, for_type);
  WriteValue<uint32_t>(envelope_encoder.bytes_.size());
  WriteValue<uint32_t>(envelope_encoder.handles_.size());
  WriteValue<uint64_t>(UINTPTR_MAX);
  current_offset_ = AllocateObject(envelope_encoder.bytes_.size());
  WriteData(envelope_encoder.bytes_);
  for (const auto handle : envelope_encoder.handles_) {
    handles_.push_back(handle);
  }
}

void Encoder::VisitStructValueBody(size_t offset, const StructValue* node) {
  for (const auto& member : node->struct_definition().members()) {
    auto it = node->fields().find(member.get());
    FX_DCHECK(it != node->fields().end());
    current_offset_ = offset + member->offset();
    it->second->Visit(this, member->type());
  }
}

void Encoder::VisitInvalidValue(const InvalidValue* node, const Type* for_type) {
  FX_LOGS(FATAL) << "Can't encode invalid data.";
}

void Encoder::VisitNullValue(const NullValue* node, const Type* for_type) {
  FX_DCHECK(for_type != nullptr);
  NullVisitor null_visitor(this);
  for_type->Visit(&null_visitor);
}

void Encoder::VisitRawValue(const RawValue* node, const Type* for_type) { WriteData(node->data()); }

void Encoder::VisitBoolValue(const BoolValue* node, const Type* for_type) {
  WriteValue<uint8_t>(node->value());
}

void Encoder::VisitIntegerValue(const IntegerValue* node, const Type* for_type) {
  FX_DCHECK(for_type != nullptr);
  size_t size = for_type->InlineSize();
  uint64_t value = node->absolute_value();
  if (node->negative()) {
    value = -value;
  }
  WriteData(reinterpret_cast<const uint8_t*>(&value), size);
}

void Encoder::VisitDoubleValue(const DoubleValue* node, const Type* for_type) {
  FX_DCHECK(for_type != nullptr);
  size_t size = for_type->InlineSize();
  if (size == sizeof(float)) {
    float value = node->value();
    WriteData(reinterpret_cast<const uint8_t*>(&value), size);
  } else {
    double value = node->value();
    WriteData(reinterpret_cast<const uint8_t*>(&value), size);
  }
}

void Encoder::VisitStringValue(const StringValue* node, const Type* for_type) {
  WriteValue<uint64_t>(node->string().size());
  WriteValue<uint64_t>(UINTPTR_MAX);
  current_offset_ = AllocateObject(node->string().size());
  WriteData(reinterpret_cast<const uint8_t*>(node->string().data()), node->string().size());
}

void Encoder::VisitHandleValue(const HandleValue* node, const Type* for_type) {
  if (node->handle().handle == FIDL_HANDLE_ABSENT) {
    WriteValue<uint32_t>(FIDL_HANDLE_ABSENT);
  } else {
    WriteValue<uint32_t>(FIDL_HANDLE_PRESENT);
    handles_.push_back(node->handle());
  }
}

void Encoder::VisitUnionValue(const UnionValue* node, const Type* for_type) {
  FX_DCHECK(for_type != nullptr);
  WriteValue<uint64_t>(node->member().ordinal());
  EncodeEnvelope(node->value().get(), node->member().type());
}

void Encoder::VisitStructValue(const StructValue* node, const Type* for_type) {
  FX_DCHECK(for_type != nullptr);
  if (for_type->Nullable()) {
    WriteValue<uint64_t>(UINTPTR_MAX);
    size_t object_size = node->struct_definition().size();
    VisitStructValueBody(AllocateObject(object_size), node);
  } else {
    VisitStructValueBody(current_offset_, node);
  }
}

void Encoder::VisitVectorValue(const VectorValue* node, const Type* for_type) {
  FX_DCHECK(for_type != nullptr);
  const Type* component_type = for_type->GetComponentType();
  FX_DCHECK(component_type != nullptr);
  size_t component_size = component_type->InlineSize();
  size_t offset;
  if (for_type->IsArray()) {
    offset = current_offset_;
  } else {
    WriteValue<uint64_t>(node->values().size());
    WriteValue<uint64_t>(UINTPTR_MAX);
    offset = AllocateObject(component_size * node->values().size());
  }
  for (const auto& value : node->values()) {
    current_offset_ = offset;
    value->Visit(this, component_type);
    offset += component_size;
  }
}

void Encoder::VisitTableValue(const TableValue* node, const Type* for_type) {
  WriteValue<uint64_t>(node->highest_member());
  WriteValue<uint64_t>(UINTPTR_MAX);

  constexpr size_t kEnvelopeSize = 2 * sizeof(uint32_t) + sizeof(uint64_t);
  size_t offset = AllocateObject(node->highest_member() * kEnvelopeSize);

  for (Ordinal32 i = 1; i <= node->highest_member(); ++i) {
    current_offset_ = offset;
    auto it = node->members().find(node->table_definition().members()[i].get());
    if ((it == node->members().end()) || it->second->IsNull()) {
      WriteValue<uint32_t>(0);
      WriteValue<uint32_t>(0);
      WriteValue<uint64_t>(0);
    } else {
      EncodeEnvelope(it->second.get(), it->first->type());
    }
    offset += kEnvelopeSize;
  }
}

}  // namespace fidl_codec
