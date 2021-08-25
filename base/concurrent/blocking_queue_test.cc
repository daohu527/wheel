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

//  Created Date: 2021-8-20
//  Author: daohu527

#include "gtest/gtest.h"

#include "base/concurrent/blocking_queue.h"

namespace wheel {
namespace base {

TEST(BlockingQueue, size) {
  auto q = new BlockingQueue<int>(100);
  q->enqueue(1);
  q->enqueue(2);

  EXPECT_TRUE(q->size() == 2);
}

TEST(BlockingQueue, single) {
  auto q = new BlockingQueue<int>(100);
  q->enqueue(1);
  q->enqueue(2);

  EXPECT_EQ(q->dequeue(), 1);
  EXPECT_EQ(q->dequeue(), 2);
  EXPECT_TRUE(q->empty());
}

TEST(BlockingQueue, overwrite) {
  auto q = new BlockingQueue<int>(5);
  for(int i = 0; i < 6; i++) {
    q->enqueue(i);
  }

  EXPECT_EQ(q->dequeue(), 1);
  EXPECT_EQ(q->dequeue(), 2);
  EXPECT_EQ(q->dequeue(), 3);
  EXPECT_EQ(q->dequeue(), 4);
  EXPECT_EQ(q->dequeue(), 5);
}

TEST(BlockingQueue, multithread) {

}

}    // namespace base
}    // namespace wheel
