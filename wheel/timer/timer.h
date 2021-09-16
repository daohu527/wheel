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

#include <memory>

#include "timer/ticket.h"

namespace wheel {
namespace timer {

class Timer {
 public:
  enum TimeUnit {
    seconds,
    milliseconds,
  };

  Timer(Ticket::Task task, uint32_t interval, TimeUnit unit, bool is_one_shot)
      : task_(task),
        interval_(interval),
        unit_(unit),
        delay_time_(0),
        is_one_shot_(is_one_shot),
        ticket_ptr_(nullptr) {
    assert(interval > 0);
    createTicket();
  }

  // add 'delay_time' for constructor
  Timer(Ticket::Task task,
        uint32_t interval,
        TimeUnit unit,
        uint32_t delay_time,
        bool is_one_shot)
      : task_(task),
        interval_(interval),
        unit_(unit),
        delay_time_(delay_time),
        is_one_shot_(is_one_shot),
        ticket_ptr_(nullptr) {
    assert(interval > 0);
    createTicket();
  }

  Timer(const Timer&) = delete;
  Timer& operator=(const Timer&) = delete;

  ~Timer() = default;

  void start();

  void cancel();

 private:
  uint32_t getMilliseconds();

  void createTicket();

 private:
  Ticket::Task task_;
  uint32_t interval_;
  TimeUnit unit_;
  uint32_t delay_time_;
  bool is_one_shot_;

  TicketPtr ticket_ptr_;
};

}  // namespace timer
}  // namespace wheel
