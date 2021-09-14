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

#pragma once

#include <cassert>
#include <atomic>
#include <memory>
#include <functional>


namespace wheel {
namespace timer {


class Ticket {
 public:
  using TicketId = uint64_t;
  using Task = std::function<void(void)>;

  enum State {
    INIT,
    RUNNING,
    REMOVED,
  };

  // todo(daohu527) : how to generate ticket id?
  Ticket(Task task, uint32_t duration, uint32_t delay_time, bool is_one_shot)
      : ticket_id_(0),
        task_(task),
        duration_(duration),
        delay_time_(delay_time),
        is_one_shot_(is_one_shot),
        state_(Ticket::INIT) {
    assert(duration_ > 0);
  }

  ~Ticket() = default;

  Ticket(const Ticket&) = delete;
  Ticket& operator=(const Ticket&) = delete;

  uint32_t duration() const {
    return duration_;
  }

  bool isOneShot() const {
    return is_one_shot_;
  }

  const Task& task() const {
    return task_;
  }

  void setState(State state) {
    state_ = state;
  }

  bool valid() {
    return state_ != Ticket::REMOVED;
  }

 private:
  TicketId ticket_id_;
  Task task_;
  uint32_t duration_;
  uint32_t delay_time_;
  bool is_one_shot_;

  // 0: not run yet
  // 1: running
  // 2: removed
  std::atomic<uint8_t> state_;
};

typedef std::shared_ptr<Ticket> TicketPtr;

}  // namespace timer
}  // namespace wheel
