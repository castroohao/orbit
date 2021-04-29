// Copyright (c) 2020 The Orbit Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <absl/container/flat_hash_map.h>
#include <libfuzzer/libfuzzer_macro.h>
#include <stdint.h>

#include <string>

#include "CaptureClient/CaptureEventProcessor.h"
#include "CaptureClient/CaptureListener.h"
#include "OrbitBase/Result.h"
#include "OrbitClientData/Callstack.h"
#include "OrbitClientData/ProcessData.h"
#include "OrbitClientData/TracepointCustom.h"
#include "OrbitClientData/UserDefinedCaptureData.h"
#include "absl/flags/flag.h"
#include "capture.pb.h"
#include "capture_data.pb.h"
#include "services.pb.h"
#include "tracepoint.pb.h"

ABSL_FLAG(uint16_t, sampling_rate, 1000, "Frequency of callstack sampling in samples per second");
ABSL_FLAG(bool, frame_pointer_unwinding, false, "Use frame pointers for unwinding");

namespace orbit_capture_client {

using orbit_grpc_protos::CaptureResponse;

namespace {
using orbit_client_protos::CallstackEvent;
using orbit_client_protos::LinuxAddressInfo;
using orbit_client_protos::TimerInfo;

class MyCaptureListener : public CaptureListener {
 private:
  void OnCaptureStarted(const orbit_grpc_protos::CaptureStarted& /*capture_started*/,
                        absl::flat_hash_set<uint64_t> /*frame_track_function_ids*/) override {}
  void OnCaptureFinished(const orbit_grpc_protos::CaptureFinished& /*capture_finished*/) override {}
  void OnTimer(const TimerInfo&) override {}
  void OnSystemMemoryUsage(const orbit_grpc_protos::SystemMemoryUsage&) override {}
  void OnKeyAndString(uint64_t, std::string) override {}
  void OnUniqueCallStack(CallStack) override {}
  void OnCallstackEvent(CallstackEvent) override {}
  void OnThreadName(int32_t, std::string) override {}
  void OnThreadStateSlice(orbit_client_protos::ThreadStateSliceInfo) override {}
  void OnAddressInfo(LinuxAddressInfo) override {}
  void OnUniqueTracepointInfo(uint64_t, orbit_grpc_protos::TracepointInfo) override {}
  void OnTracepointEvent(orbit_client_protos::TracepointEventInfo) override {}
  void OnModuleUpdate(uint64_t, orbit_grpc_protos::ModuleInfo) override {}
  void OnModulesSnapshot(uint64_t, std::vector<orbit_grpc_protos::ModuleInfo>) override {}
};
}  // namespace

DEFINE_PROTO_FUZZER(const CaptureResponse& response) {
  MyCaptureListener listener;
  auto processor = CaptureEventProcessor::CreateForCaptureListener(&listener, {});
  processor->ProcessEvents(response.capture_events());
}

}  // namespace orbit_capture_client