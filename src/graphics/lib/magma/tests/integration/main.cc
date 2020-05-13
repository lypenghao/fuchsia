// Copyright 2020 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>
#include <src/lib/fxl/command_line.h>

#include "test_magma_abi.h"

uint32_t gVendorId;

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);

  fxl::CommandLine command_line = fxl::CommandLineFromArgcArgv(argc, argv);

  std::string vendor_id;
  if (command_line.GetOptionValue("vendor-id", &vendor_id)) {
    gVendorId = strtoul(vendor_id.c_str(), nullptr, 0);
  }

  return RUN_ALL_TESTS();
}
