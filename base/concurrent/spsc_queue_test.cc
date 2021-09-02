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

//  Created Date: 2021-8-24
//  Author: daohu527

#include "gtest/gtest.h"

#include "base/concurrent/spsc_queue.h"


namespace wheel {
namespace base {

TEST(SPSCQueue, enqueue) {
  SPSCQueue<int> q(5);
  q.enqueue(1);
  q.enqueue(2);
  EXPECT_EQ(q.size(), 2);
  EXPECT_EQ(q.capacity(), 5);
}

TEST(SPSCQueue, dequeue) {
  SPSCQueue<int> q(5);
  q.enqueue(1);
  q.enqueue(2);
  int t;
  q.dequeue(t);
  EXPECT_EQ(t, 1);
  q.dequeue(t);
  EXPECT_EQ(t, 2);
  EXPECT_TRUE(q.empty());
}

}  // namespace base
}  // namespace wheel
