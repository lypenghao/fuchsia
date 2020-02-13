// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Thin layer that writes/reads to audio registers in the EE_AUDIO block and
// the PDM block.

#ifndef SRC_MEDIA_AUDIO_DRIVERS_GAUSS_PDM_INPUT_A113_AUDIO_DEVICE_H_
#define SRC_MEDIA_AUDIO_DRIVERS_GAUSS_PDM_INPUT_A113_AUDIO_DEVICE_H_

#include <ddk/device.h>
#include <ddk/mmio-buffer.h>
#include <ddk/platform-defs.h>
#include <ddk/protocol/platform/device.h>

#include "a113-audio-regs.h"

__BEGIN_CDECLS

typedef struct a113_audio_device {
  pdev_protocol_t pdev;
  mmio_buffer_t ee_audio_mmio;
  mmio_buffer_t pdm_mmio;

  zx_handle_t pdm_irq;
  zx_handle_t bti;
} a113_audio_device_t;

zx_status_t a113_audio_device_init(a113_audio_device_t* audio_device, zx_device_t* parent);

uint32_t a113_ee_audio_read(a113_audio_device_t* audio_device, uint32_t reg);
void a113_ee_audio_write(a113_audio_device_t* audio_device, uint32_t reg, uint32_t value);
void a113_ee_audio_update_bits(a113_audio_device_t* audio_device, uint32_t reg, uint32_t mask,
                               uint32_t value);

uint32_t a113_pdm_read(a113_audio_device_t* audio_device, uint32_t reg);
void a113_pdm_write(a113_audio_device_t* audio_device, uint32_t reg, uint32_t val);
void a113_pdm_update_bits(a113_audio_device_t* audio_device, uint32_t reg, uint32_t mask,
                          uint32_t val);
__END_CDECLS

#endif  // SRC_MEDIA_AUDIO_DRIVERS_GAUSS_PDM_INPUT_A113_AUDIO_DEVICE_H_
