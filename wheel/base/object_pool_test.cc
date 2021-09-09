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

#include "base/object_pool.h"

namespace wheel {
namespace base {

struct Object {
 int x;
};

TEST(ObjectPool, init) {
  ObjectPool<Object> pool(5, 10);
  EXPECT_EQ(pool.capacity(), 5);
  EXPECT_EQ(pool.peak_capacity(), 10);
  EXPECT_EQ(pool.size(), 0);
  
  Object* obj = pool.borrowObject();
  EXPECT_TRUE(obj);
  EXPECT_EQ(pool.size(), 1);
}

}  // namespace base
}  // namespace wheel
