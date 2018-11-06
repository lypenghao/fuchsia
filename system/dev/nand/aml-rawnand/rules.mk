# Copyright 2018 The Fuchsia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_TYPE := driver

MODULE_SRCS += \
    $(LOCAL_DIR)/aml-rawnand.c \
    $(LOCAL_DIR)/onfi.c \

MODULE_STATIC_LIBS := \
    system/ulib/ddk \
    system/ulib/sync \

MODULE_LIBS := \
    system/ulib/driver \
    system/ulib/c \
    system/ulib/zircon \

MODULE_FIDL_LIBS := \
    system/fidl/zircon-nand \

MODULE_HEADER_DEPS := $(LOCAL_DIR) system/dev/lib/amlogic

MODULE_BANJO_LIBS := \
    system/banjo/ddk-protocol-platform-device \
    system/banjo/ddk-protocol-rawnand \

include make/module.mk
