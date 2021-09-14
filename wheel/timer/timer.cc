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

//  Created Date: 2021-8-26
//  Author: daohu527

#include "timer/timer.h"
#include "timer/time_wheel.h"

namespace wheel {
namespace timer {

void Timer::start() {
  // add task to timewheel
  TimeWheel::instance().addTicket(ticket_ptr_);
}

void Timer::cancel() {
  // delete task from timewheel
  TimeWheel::instance().delTicket(ticket_ptr_);
}

uint32_t Timer::getMilliseconds() {
  uint32_t interval_mills = interval_;
  switch (unit_) {
    case Timer::seconds:
      interval_mills *= 1000;
      break;
    case Timer::milliseconds:
      break;
    default:
      // todo(daohu527): error progress
      break;
  }
  return interval_mills;
}

void Timer::createTicket() {
  uint32_t duration = getMilliseconds();
  ticket_ptr_ = std::make_shared<Ticket>(
                    task_, duration, delay_time_, is_one_shot_);
}


}  // namespace timer
}  // namespace wheel
