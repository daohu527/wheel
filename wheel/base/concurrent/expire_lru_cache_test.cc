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

#include <string>
#include <thread>

#include "gtest/gtest.h"
#include "base/concurrent/expire_lru_cache.h"

namespace wheel {
namespace base {


TEST(AddTest, add) {
  ExpireLRUCache<int, std::string> cache(5);
  cache.add(1, "aaa");
  EXPECT_EQ(cache.get(1), "aaa");
  cache.add(2, "bbb");
  EXPECT_EQ(cache.get(1), "aaa");
  EXPECT_EQ(cache.get(2), "bbb");

  cache.add(1, "ccc");
  EXPECT_EQ(cache.get(1), "ccc");

  EXPECT_EQ(cache.size(), 2);
}

TEST(AddFullTest, AddFull) {
  ExpireLRUCache<int, std::string> cache(5);
  cache.add(1, "aaa");
  cache.add(2, "bbb");
  cache.add(3, "ccc");
  cache.add(4, "ddd");
  cache.add(5, "eee");
  cache.add(6, "fff");
  EXPECT_EQ(cache.size(), 5);

  EXPECT_EQ(cache.get(1), "");
  EXPECT_EQ(cache.get(2), "bbb");
  EXPECT_EQ(cache.get(3), "ccc");
  EXPECT_EQ(cache.get(4), "ddd");
  EXPECT_EQ(cache.get(5), "eee");
  EXPECT_EQ(cache.get(6), "fff");
}

TEST(GetTest, get) {
  ExpireLRUCache<int, std::string> cache(5);
  cache.add(1, "aaa");
  cache.add(2, "bbb");

  EXPECT_EQ(cache.get(1), "aaa");
  EXPECT_EQ(cache.get(2), "bbb");
  EXPECT_EQ(cache.get(5), "");
}

TEST(ExpireTest, Expire) {
  ExpireLRUCache<int, std::string> cache(5, 10, [] (int, std::string){});
  cache.add(1, "aaa");
  EXPECT_EQ(cache.get(1), "aaa");
  std::this_thread::sleep_for(std::chrono::milliseconds(15));
  EXPECT_EQ(cache.get(1), "");
  EXPECT_EQ(cache.size(), 0);
}

TEST(ExpireOldestTest, ExpireOldest) {
  ExpireLRUCache<int, std::string> cache(5, 10, [] (int, std::string){});
  cache.add(1, "aaa");
  cache.add(2, "bbb");

  EXPECT_EQ(cache.get(1), "aaa");
  EXPECT_EQ(cache.get(2), "bbb");
  std::this_thread::sleep_for(std::chrono::milliseconds(9));
  EXPECT_EQ(cache.get(1), "aaa");
  EXPECT_EQ(cache.get(2), "bbb");
  EXPECT_EQ(cache.get(3), "");

  cache.add(2, "nnn");
  cache.add(3, "ccc");
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  EXPECT_EQ(cache.get(1), "");
  EXPECT_EQ(cache.get(2), "nnn");
  EXPECT_EQ(cache.get(3), "ccc");
}

TEST(ExpireCallbackTest, ExpireCallback) {
  int key = 0;
  std::string value;
  ExpireLRUCache<int, std::string> cache(5, 10,
    [&] (int k, std::string v){
      key = k;
      value = v;
    });
  cache.add(1, "aaa");
  EXPECT_EQ(cache.get(1), "aaa");
  EXPECT_EQ(cache.size(), 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(15));
  EXPECT_EQ(cache.get(1), "");
  EXPECT_EQ(key, 1);
  EXPECT_EQ(value, "aaa");
}


}  // namespace base
}  // namespace wheel
