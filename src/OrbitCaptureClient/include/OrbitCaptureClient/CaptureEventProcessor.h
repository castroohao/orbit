// Copyright (c) 2020 The Orbit Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ORBIT_CAPTURE_CLIENT_CAPTURE_EVENT_PROCESSOR_H_
#define ORBIT_CAPTURE_CLIENT_CAPTURE_EVENT_PROCESSOR_H_

#include <absl/container/flat_hash_set.h>

#include <cstdint>
#include <functional>
#include <string>

#include "OrbitCaptureClient/CaptureListener.h"
#include "capture.pb.h"

class CaptureEventProcessor {
 public:
  CaptureEventProcessor() = default;
  virtual ~CaptureEventProcessor() = default;

  virtual void ProcessEvent(const orbit_grpc_protos::ClientCaptureEvent& event) = 0;

  template <typename Iterable>
  void ProcessEvents(const Iterable& events) {
    for (const auto& event : events) {
      ProcessEvent(event);
    }
  }

  static std::unique_ptr<CaptureEventProcessor> CreateForCaptureListener(
      CaptureListener* capture_listener, absl::flat_hash_set<uint64_t> frame_track_function_ids);
};

#endif  // ORBIT_GL_CAPTURE_EVENT_PROCESSOR_H_
