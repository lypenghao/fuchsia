// Copyright 2018 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SRC_DEVELOPER_DEBUG_ZXDB_SYMBOLS_BASE_TYPE_H_
#define SRC_DEVELOPER_DEBUG_ZXDB_SYMBOLS_BASE_TYPE_H_

#include <stdint.h>

#include "src/developer/debug/zxdb/symbols/type.h"

namespace zxdb {

// A DWARF "base type" is a compiler built-in primitive type like an integer. The file will include
// a definition for something named "int" that is defined as being a signed integer of 4 bytes, etc.
class BaseType final : public Type {
 public:
  // These are the DWARF base type encodings. The integer value of each is the constant from the
  // DWARF spec for that value. This isn't an enum because the value is an integer that can include
  // ranges of user-defined values.
  //
  // The DWARF list encompasses some types not generated by our toolchain, and that other values can
  // appear in symbols. Code is not expected to handle every one of these values. To make it clear
  // which values we handle, the irrelevant values are removed. But code should not assume that only
  // these values might ever appear.
  //
  // Note on void types: DWARF encodes void* as a pointer to nothing and normally when there's
  // nothing returned, it just lists no return type. But special-casing this in the expression
  // evaluation code is awkward so we also treat "kBaseTypeNone" as referring to a void type.
  static constexpr int kBaseTypeNone = 0x00;  // Means uninitialized or void.
  static constexpr int kBaseTypeAddress = 0x01;
  static constexpr int kBaseTypeBoolean = 0x02;
  static constexpr int kBaseTypeFloat = 0x04;
  static constexpr int kBaseTypeSigned = 0x05;
  static constexpr int kBaseTypeSignedChar = 0x06;
  static constexpr int kBaseTypeUnsigned = 0x07;
  static constexpr int kBaseTypeUnsignedChar = 0x08;
  static constexpr int kBaseTypeUTF = 0x10;  // Byte size defines encoding.

  // Returns wiether the given base type is a signed integer.
  static bool IsSigned(int base_type);

  // Symbol overrides.
  const BaseType* AsBaseType() const final;
  const std::string& GetAssignedName() const final;

  // Returns one of kBaseType* or possibly something else if the language is new or unusual. Don't
  // handle, but also don't crash on unexpected values.
  int base_type() const { return base_type_; }
  void set_base_type(int type) { base_type_ = type; }

 private:
  FRIEND_REF_COUNTED_THREAD_SAFE(BaseType);
  FRIEND_MAKE_REF_COUNTED(BaseType);

  BaseType();
  BaseType(int base_type, uint32_t byte_size, const std::string& name);
  ~BaseType();

  int base_type_ = kBaseTypeNone;
};

}  // namespace zxdb

#endif  // SRC_DEVELOPER_DEBUG_ZXDB_SYMBOLS_BASE_TYPE_H_
