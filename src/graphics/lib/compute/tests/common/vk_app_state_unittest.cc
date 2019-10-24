// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "vk_app_state.h"

#include <gtest/gtest.h>

TEST(vkAppStateTest, DefautInit)
{
  vk_app_state_t app = {};

  const vk_app_state_config_t config = {
    // Empty configuration.
  };

  ASSERT_TRUE(vk_app_state_init(&app, &config)) << "Could not initialize vk_app_state_t instance";

  ASSERT_TRUE(app.instance);
  ASSERT_TRUE(app.d);
  ASSERT_TRUE(app.pd);

  ASSERT_FALSE(app.ac);  // no allocator by default.
  ASSERT_FALSE(app.pc);  // no pipeline cache by default.

  ASSERT_NE(app.qfi, UINT32_MAX);  // graphics queue is used by default
  ASSERT_EQ(app.compute_qfi, UINT32_MAX);

  ASSERT_FALSE(app.has_debug_report);
  ASSERT_FALSE(app.has_amd_statistics);
  ASSERT_FALSE(app.has_subgroup_size_control);

  vk_app_state_destroy(&app);
}

TEST(vkAppStateTest, EnableValidation)
{
  vk_app_state_t app = {};

  const vk_app_state_config_t config = {
    .enable_validation = true,
  };

  ASSERT_TRUE(vk_app_state_init(&app, &config)) << "Could not initialize vk_app_state_t instance";

  ASSERT_TRUE(app.instance);
  ASSERT_TRUE(app.d);
  ASSERT_TRUE(app.pd);

  ASSERT_FALSE(app.ac);  // no allocator by default.
  ASSERT_FALSE(app.pc);  // no pipeline cache by default.

  ASSERT_NE(app.qfi, UINT32_MAX);  // graphics queue is used by default
  ASSERT_EQ(app.compute_qfi, UINT32_MAX);

  ASSERT_FALSE(app.has_debug_report);
  ASSERT_FALSE(app.has_amd_statistics);
  ASSERT_FALSE(app.has_subgroup_size_control);

  vk_app_state_destroy(&app);
}

TEST(vkAppStateTest, EnableDebug)
{
  vk_app_state_t app = {};

  const vk_app_state_config_t config = {
    .enable_validation   = true,  // required by debug_report
    .enable_debug_report = true,
  };

  ASSERT_TRUE(vk_app_state_init(&app, &config)) << "Could not initialize vk_app_state_t instance";

  ASSERT_TRUE(app.instance);
  ASSERT_TRUE(app.d);
  ASSERT_TRUE(app.pd);

  ASSERT_FALSE(app.ac);  // no allocator by default.
  ASSERT_FALSE(app.pc);  // no pipeline cache by default.

  ASSERT_NE(app.qfi, UINT32_MAX);  // graphics queue is used by default
  ASSERT_EQ(app.compute_qfi, UINT32_MAX);

  ASSERT_TRUE(app.has_debug_report);  // debug report enabled!
  ASSERT_FALSE(app.has_amd_statistics);
  ASSERT_FALSE(app.has_subgroup_size_control);

  vk_app_state_destroy(&app);
}

TEST(vkAppStateTest, EnablePipelineCache)
{
  vk_app_state_t app = {};

  const vk_app_state_config_t config = {
    .enable_pipeline_cache = true,
  };

  ASSERT_TRUE(vk_app_state_init(&app, &config)) << "Could not initialize vk_app_state_t instance";

  ASSERT_TRUE(app.instance);
  ASSERT_TRUE(app.d);
  ASSERT_TRUE(app.pd);

  ASSERT_FALSE(app.ac);  // no allocator by default.
  ASSERT_TRUE(app.pc);   // pipeline cache should be created!

  ASSERT_NE(app.qfi, UINT32_MAX);  // graphics queue is used by default
  ASSERT_EQ(app.compute_qfi, UINT32_MAX);

  ASSERT_FALSE(app.has_debug_report);
  ASSERT_FALSE(app.has_amd_statistics);
  ASSERT_FALSE(app.has_subgroup_size_control);

  vk_app_state_destroy(&app);
}

TEST(vkAppStateTest, EnableComputeQueueOnly)
{
  vk_app_state_t app = {};

  const vk_app_state_config_t config = {
    .device_config = {
      .required_queues = VK_QUEUE_COMPUTE_BIT,
    },
  };

  ASSERT_TRUE(vk_app_state_init(&app, &config)) << "Could not initialize vk_app_state_t instance";

  ASSERT_TRUE(app.instance);
  ASSERT_TRUE(app.d);
  ASSERT_TRUE(app.pd);

  ASSERT_FALSE(app.ac);  // no allocator by default.
  ASSERT_FALSE(app.pc);  // no pipeline cache by default.

  ASSERT_EQ(app.qfi, UINT32_MAX);          // graphics queue should not be selected.
  ASSERT_NE(app.compute_qfi, UINT32_MAX);  // compute queue should be selected.

  ASSERT_FALSE(app.has_debug_report);
  ASSERT_FALSE(app.has_amd_statistics);
  ASSERT_FALSE(app.has_subgroup_size_control);
}

TEST(vkAppStateTest, EnableComputeQueueAndSwapchain)
{
  vk_app_state_t app = {};

  const vk_app_state_config_t config = {
    .require_swapchain = true,
    .device_config = {
      .required_queues = VK_QUEUE_COMPUTE_BIT,
    },
  };

  ASSERT_TRUE(vk_app_state_init(&app, &config)) << "Could not initialize vk_app_state_t instance";

  ASSERT_TRUE(app.instance);
  ASSERT_TRUE(app.d);
  ASSERT_TRUE(app.pd);

  ASSERT_FALSE(app.ac);  // no allocator by default.
  ASSERT_FALSE(app.pc);  // no pipeline cache by default.

  ASSERT_NE(app.qfi, UINT32_MAX);          // graphics queue should be selected.
  ASSERT_NE(app.compute_qfi, UINT32_MAX);  // compute queue should be selected.

  ASSERT_FALSE(app.has_debug_report);
  ASSERT_FALSE(app.has_amd_statistics);
  ASSERT_FALSE(app.has_subgroup_size_control);
}

TEST(vkAppStateTest, RequireSwapchain)
{
  vk_app_state_t app = {};

  const vk_app_state_config_t config = {
    .require_swapchain = true,
  };

  ASSERT_TRUE(vk_app_state_init(&app, &config)) << "Could not initialize vk_app_state_t instance";

  ASSERT_TRUE(app.instance);
  ASSERT_TRUE(app.d);
  ASSERT_TRUE(app.pd);

  ASSERT_FALSE(app.ac);  // no allocator by default.
  ASSERT_FALSE(app.pc);  // no pipeline cache by default.

  ASSERT_NE(app.qfi, UINT32_MAX);  // graphics queue is used by default
  ASSERT_EQ(app.compute_qfi, UINT32_MAX);

  ASSERT_FALSE(app.has_debug_report);
  ASSERT_FALSE(app.has_amd_statistics);
  ASSERT_FALSE(app.has_subgroup_size_control);

  VkSurfaceKHR surface = vk_app_state_create_surface(&app, 100, 100);
  ASSERT_TRUE(surface);
  vkDestroySurfaceKHR(app.instance, surface, app.ac);

  vk_app_state_destroy(&app);
}
