// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SRC_UI_LIGHT_DRIVERS_AML_LIGHT_AML_LIGHT_H_
#define SRC_UI_LIGHT_DRIVERS_AML_LIGHT_AML_LIGHT_H_

#include <fuchsia/hardware/light/llcpp/fidl.h>
#include <threads.h>

#include <optional>
#include <string>
#include <vector>

#include <ddk/debug.h>
#include <ddktl/device.h>
#include <ddktl/protocol/empty-protocol.h>
#include <ddktl/protocol/gpio.h>
#include <ddktl/protocol/pwm.h>
#include <fbl/array.h>
#include <soc/aml-common/aml-pwm-regs.h>

namespace aml_light {

class AmlLight;
using AmlLightType = ddk::Device<AmlLight, ddk::Messageable>;
using ::llcpp::fuchsia::hardware::light::Capability;
using ::llcpp::fuchsia::hardware::light::Light;
using ::llcpp::fuchsia::hardware::light::Rgb;

class LightDevice {
 public:
  LightDevice(std::string name, ddk::GpioProtocolClient gpio,
              std::optional<ddk::PwmProtocolClient> pwm)
      : name_(std::move(name)), gpio_(gpio), pwm_(pwm) {}

  zx_status_t Init(bool init_on);

  const std::string GetName() const { return name_; }
  bool HasCapability(Capability capability) const;
  uint8_t GetSimpleValue() const { return value_; }
  zx_status_t SetSimpleValue(uint8_t value);

 private:
  std::string name_;
  ddk::GpioProtocolClient gpio_;
  std::optional<ddk::PwmProtocolClient> pwm_;

  uint8_t value_ = 0;
};

class AmlLight : public AmlLightType,
                 public Light::Interface,
                 public ddk::EmptyProtocol<ZX_PROTOCOL_LIGHT> {
 public:
  explicit AmlLight(zx_device_t* parent) : AmlLightType(parent) {}

  static zx_status_t Create(void* ctx, zx_device_t* parent);

  // Device protocol implementation.
  zx_status_t DdkMessage(fidl_msg_t* msg, fidl_txn_t* txn);
  void DdkRelease();

  // FIDL messages.
  void GetName(uint32_t index, GetNameCompleter::Sync completer);
  void GetCount(GetCountCompleter::Sync completer);
  void HasCapability(uint32_t index, Capability capability, HasCapabilityCompleter::Sync completer);
  void GetSimpleValue(uint32_t index, GetSimpleValueCompleter::Sync completer);
  void SetSimpleValue(uint32_t index, uint8_t value, SetSimpleValueCompleter::Sync completer);
  void GetRgbValue(uint32_t index, GetRgbValueCompleter::Sync completer);
  void SetRgbValue(uint32_t index, Rgb value, SetRgbValueCompleter::Sync completer);

  void GetGroupInfo(uint32_t group_id, GetGroupInfoCompleter::Sync completer) {
    completer.Reply(ZX_ERR_NOT_SUPPORTED, nullptr);
  }
  void GetGroupCurrentSimpleValue(uint32_t group_id,
                                  GetGroupCurrentSimpleValueCompleter::Sync completer) {
    completer.Reply(ZX_ERR_NOT_SUPPORTED, ::fidl::VectorView<bool>(nullptr, 0));
  }
  void SetGroupSimpleValue(uint32_t group_id, ::fidl::VectorView<bool> values,
                           SetGroupSimpleValueCompleter::Sync completer) {
    completer.Reply(ZX_ERR_NOT_SUPPORTED);
  }
  void GetGroupCurrentBrightnessValue(uint32_t group_id,
                                      GetGroupCurrentBrightnessValueCompleter::Sync completer) {
    completer.Reply(ZX_ERR_NOT_SUPPORTED, ::fidl::VectorView<uint8_t>(nullptr, 0));
  }
  void SetGroupBrightnessValue(uint32_t group_id, ::fidl::VectorView<uint8_t> values,
                               SetGroupBrightnessValueCompleter::Sync completer) {
    completer.Reply(ZX_ERR_NOT_SUPPORTED);
  }
  void GetGroupCurrentRgbValue(uint32_t group_id,
                               GetGroupCurrentRgbValueCompleter::Sync completer) {
    completer.Reply(ZX_ERR_NOT_SUPPORTED,
                    ::fidl::VectorView<::llcpp::fuchsia::hardware::light::Rgb>(nullptr, 0));
  }
  void SetGroupRgbValue(uint32_t group_id,
                        ::fidl::VectorView<::llcpp::fuchsia::hardware::light::Rgb> values,
                        SetGroupRgbValueCompleter::Sync completer) {
    completer.Reply(ZX_ERR_NOT_SUPPORTED);
  }

 private:
  DISALLOW_COPY_ASSIGN_AND_MOVE(AmlLight);
  friend class FakeAmlLight;

  zx_status_t Init();

  static constexpr size_t kNameLength = ZX_MAX_NAME_LEN;

  std::vector<LightDevice> lights_;
};

}  // namespace aml_light

#endif  // SRC_UI_LIGHT_DRIVERS_AML_LIGHT_AML_LIGHT_H_
