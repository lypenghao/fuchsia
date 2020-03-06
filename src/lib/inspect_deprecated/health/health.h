// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SRC_LIB_INSPECT_DEPRECATED_HEALTH_HEALTH_H_
#define SRC_LIB_INSPECT_DEPRECATED_HEALTH_HEALTH_H_

#include <abs_clock/clock.h>

#include "src/lib/inspect_deprecated/inspect.h"

namespace inspect_deprecated {

// The name of nodes implementing health for a parent node.
constexpr char kHealthNodeName[] = "fuchsia.inspect.Health";

// Health status designating that the node is healthy.
constexpr char kHealthOk[] = "OK";

// Health status designating that the node is not yet healthy, but is still
// starting up and may become healthy.
constexpr char kHealthStartingUp[] = "STARTING_UP";

// Health status designating that the node is not healthy.
constexpr char kHealthUnhealthy[] = "UNHEALTHY";

// The metric representing timestamp in nanoseconds, at which this health node
// has been initialized.
const char kStartTimestamp[] = "start_timestamp_nanos";

// Represents the health associated with a given inspect_deprecated::Node.
//
// This class supports adding a Node with name "fuchsia.inspect.Health" that
// consists of "status" and "message" properties. Nodes implementing
// fuchsia.inspect.Health can be aggregated in health checking scripts
// system-wide.
class NodeHealth {
 public:
  // Constructs a new NodeHealth object that wraps a health designation for the
  // given node.
  //
  // The initial status is STARTING_UP.
  explicit NodeHealth(Node* parent_node);

  // Constructs a new NodeHealth object, which uses the passed-in clock to
  // get the needed timestamps. Useful for testing, for example. Does not
  // take ownership of the clock.
  NodeHealth(Node* parent_node, abs_clock::Clock* clock);

  // Allow moving, disallow copying.
  NodeHealth(NodeHealth&&) = default;
  NodeHealth(const NodeHealth&) = delete;
  NodeHealth& operator=(NodeHealth&&) = default;
  NodeHealth& operator=(const NodeHealth&) = delete;

  // Sets the health of this node to OK, with no message.
  void Ok();

  // Sets the health of this node to STARTING_UP, with no message.
  void StartingUp();

  // Sets the health of this node to STARTING_UP, with the given message.
  void StartingUp(const std::string& message);

  // Sets the health of this node to UNHEALTHY, with the given message.
  void Unhealthy(const std::string& message);

  // Explicitly sets the status to the given value with the given message.
  void SetStatus(const std::string& status, const std::string& message);

 private:
  void SetMessage(const std::string& message);

  Node health_node_;
  StringProperty health_status_;
  fit::optional<StringProperty> health_message_;
  IntMetric timestamp_nanos_;
};

}  // namespace inspect_deprecated

#endif  // SRC_LIB_INSPECT_DEPRECATED_HEALTH_HEALTH_H_
