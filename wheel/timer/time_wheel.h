// Copyright 2021 daohu527@gmail.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//  Created Date: 2021-8-25
//  Author: daohu527

#pragma once

#include <list>
#include <memory>
#include <thread>

#include "base/macros.h"
#include "timer/bucket.h"

namespace wheel {
namespace timer {


class TimeWheel {
 public:
  using TickTime = std::chrono::milliseconds;

  ~TimeWheel();

  bool addTicket(TicketPtr ticket_ptr);

  bool delTicket(TicketPtr ticket_ptr);

 private:
  // time wheel main process
  void schedule();

  // timer tick
  void tick();

  void moveTickets();

  void runTimeoutTask(const TicketPtr ticket_ptr);

 private:
  // default interval 1ms
  static constexpr int TICK_INTERVAL = 1;
  static constexpr int MILLISECOND_BUCKET_SIZE = 512;
  static constexpr int SECOND_BUCKET_SIZE = 256;

  Bucket mbuckets_[MILLISECOND_BUCKET_SIZE];
  Bucket sbuckets_[SECOND_BUCKET_SIZE];

  uint32_t millisecond_index_;
  uint32_t second_index_;

  TickTime tick_time_;

  std::thread wheel_thread_;

  bool stop_;

  DECLARE_SINGLETON(TimeWheel)
};


}  // namespace timer
}  // namespace wheel
