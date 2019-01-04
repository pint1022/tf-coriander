/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "cuda.h"

#include "tensorflow/stream_executor/cl/cl_timer.h"

#include "tensorflow/stream_executor/cl/cl_driver.h"
#include "tensorflow/stream_executor/cl/cl_gpu_executor.h"
#include "tensorflow/stream_executor/cl/cl_stream.h"
#include "tensorflow/stream_executor/lib/status.h"

namespace perftools {
namespace gputools {
namespace cl {

bool CLTimer::Init() {
  CHECK(start_event_ == nullptr && stop_event_ == nullptr);
  ClContext* context = parent_->cl_context();
  if (!CLDriver::CreateEvent(context, &start_event_,
                               CLDriver::EventFlags::kDefault)
           .ok()) {
    return false;
  }

  if (!CLDriver::CreateEvent(context, &stop_event_,
                               CLDriver::EventFlags::kDefault)
           .ok()) {
    port::Status status = CLDriver::DestroyEvent(context, &start_event_);
    if (!status.ok()) {
      LOG(ERROR) << status;
    }
    return false;
  }

  CHECK(start_event_ != nullptr && stop_event_ != nullptr);
  return true;
}

void CLTimer::Destroy() {
  ClContext* context = parent_->cl_context();
  port::Status status = CLDriver::DestroyEvent(context, &start_event_);
  if (!status.ok()) {
    LOG(ERROR) << status;
  }

  status = CLDriver::DestroyEvent(context, &stop_event_);
  if (!status.ok()) {
    LOG(ERROR) << status;
  }
}

float CLTimer::GetElapsedMilliseconds() const {
  CHECK(start_event_ != nullptr && stop_event_ != nullptr);
  // TODO(leary) provide a way to query timer resolution?
  // CL docs say a resolution of about 0.5us
  float elapsed_milliseconds = NAN;
  (void)CLDriver::GetEventElapsedTime(parent_->cl_context(),
                                        &elapsed_milliseconds, start_event_,
                                        stop_event_);
  return elapsed_milliseconds;
}

bool CLTimer::Start(CLStream *stream) {
  return CLDriver::RecordEvent(parent_->cl_context(), start_event_,
                                 stream->cl_stream())
      .ok();
}

bool CLTimer::Stop(CLStream *stream) {
  return CLDriver::RecordEvent(parent_->cl_context(), stop_event_,
                                 stream->cl_stream())
      .ok();
}

}  // namespace cl
}  // namespace gputools
}  // namespace perftools
