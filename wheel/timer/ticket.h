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

#include <atomic>
#include <memory>
#include <functional>


namespace wheel {
namespace timer {

typedef TicketPtr std::shared_ptr<Ticket>;

class Ticket {
 public:
  using TicketId = uint64_t;
  using Task = std::function<void(void)>;

  // todo(daohu527) : how to generate ticket id?
  Ticket(Task task, uint64_t interval, bool is_one_shot)
      : ticket_id_(0),
        task_(task),
        interval_(interval),
        is_one_shot_(is_one_shot),
        remain_rounds_(0),
        state_(0) {}

  ~Ticket();

  Ticket(const Ticket&) = delete;
  Ticket& operator=(const Ticket&) = delete;

  uint64_t duration() const;

 private:
  TicketId ticket_id_;
  Task task_;
  uint64_t interval_;
  bool is_one_shot_;
  uint64_t remain_rounds_;
  uint32_t delay_time_;

  // 0: not run yet
  // 1: running
  // 2: removed
  std::atomic<uint8_t> state_;
};

}  // namespace timer
}  // namespace wheel
