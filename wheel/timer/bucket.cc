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

#include "timer/bucket.h"

namespace wheel {
namespace timer {

void Bucket::pickTickets(std::list<TicketPtr>& ticket_ptrs) {
  std::lock_guard<std::mutex> lock(mutex_);
  std::swap(ticket_ptrs_, ticket_ptrs);
}

void Bucket::addTicket(const TicketPtr& ptr) {
  std::lock_guard<std::mutex> lock(mutex_);
  ticket_ptrs_.push_back(ptr);
}


}  // namespace timer
}  // namespace wheel
