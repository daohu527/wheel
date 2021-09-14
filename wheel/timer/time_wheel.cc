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

//  Created Date: 2021-9-9
//  Author: daohu527

#include <future>

#include "timer/time_wheel.h"

namespace wheel {
namespace timer {

constexpr int TimeWheel::TICK_INTERVAL;

TimeWheel::TimeWheel()
    : millisecond_index_(0),
      second_index_(0),
      tick_time_(TickTime{TICK_INTERVAL}),
      stop_(false) {
  // create thread and detach
  std::thread wheel_thread_(&TimeWheel::schedule, &TimeWheel::instance());
  wheel_thread_.detach();
}

TimeWheel::~TimeWheel() {
  // stop wheel_thread_
  stop_ = true;
}

void TimeWheel::moveTickets() {
  std::list<TicketPtr> ticket_ptrs;
  sbuckets_[second_index_].pickTickets(ticket_ptrs);
  for (const TicketPtr ticket_ptr : ticket_ptrs) {
    uint32_t interval = ticket_ptr->duration() / TICK_INTERVAL;
    int index = interval % MILLISECOND_BUCKET_SIZE;
    mbuckets_[index].addTicket(ticket_ptr);
  }
}

bool TimeWheel::addTicket(TicketPtr ticket_ptr) {
  assert(ticket_ptr != nullptr);
  ticket_ptr->setState(Ticket::INIT);

  uint32_t interval = ticket_ptr->duration() / TICK_INTERVAL;
  int index_s = (millisecond_index_ + interval) / MILLISECOND_BUCKET_SIZE;
  int index = second_index_ + index_s;
  // todo(daohu527): we do not support 2 round
  if (index >= SECOND_BUCKET_SIZE)
    index -= SECOND_BUCKET_SIZE;
  sbuckets_[index].addTicket(ticket_ptr);

  moveTickets();
  return true;
}

bool TimeWheel::delTicket(TicketPtr ticket_ptr) {
  assert(ticket_ptr != nullptr);
  ticket_ptr->setState(Ticket::REMOVED);
  return true;
}

void TimeWheel::tick() {
  // sleep
  std::this_thread::sleep_for(tick_time_);

  // update index
  ++millisecond_index_;
  if (millisecond_index_ >= MILLISECOND_BUCKET_SIZE) {
    millisecond_index_ = 0;
    ++second_index_;
    if (second_index_ >= SECOND_BUCKET_SIZE) {
      second_index_ = 0;
    }
    moveTickets();
  }
}

void TimeWheel::runTimeoutTask(const TicketPtr ticket_ptr) {
  std::async(ticket_ptr->task());
}

void TimeWheel::schedule() {
  while (true) {
    // 1. tick
    tick();

    // 2. if stop then break
    if (stop_)
      break;

    // 3. pick tickets and run timeout task
    std::list<TicketPtr> ticket_ptrs;
    mbuckets_[millisecond_index_].pickTickets(ticket_ptrs);
    for (const TicketPtr ticket_ptr : ticket_ptrs) {
      if (ticket_ptr->valid()) {
        runTimeoutTask(ticket_ptr);
        // book new tickets
        if(!ticket_ptr->isOneShot())
          addTicket(ticket_ptr);
      }
    }
  }
}

}  // namespace timer
}  // namespace wheel
