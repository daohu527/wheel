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

namespace shudao {
namespace base {


TEST(AddTest, Add) {
  ExpireLRUCache<int, std::string> cache(5);
  cache.Add(1, "aaa");
  EXPECT_EQ(cache.Get(1), "aaa");
  cache.Add(2, "bbb");
  EXPECT_EQ(cache.Get(1), "aaa");
  EXPECT_EQ(cache.Get(2), "bbb");

  cache.Add(1, "ccc");
  EXPECT_EQ(cache.Get(1), "ccc");

  EXPECT_EQ(cache.Size(), 2);
}

TEST(AddFullTest, AddFull) {
  ExpireLRUCache<int, std::string> cache(5);
  cache.Add(1, "aaa");
  cache.Add(2, "bbb");
  cache.Add(3, "ccc");
  cache.Add(4, "ddd");
  cache.Add(5, "eee");
  cache.Add(6, "fff");     
  EXPECT_EQ(cache.Size(), 5);

  EXPECT_EQ(cache.Get(1), "");
  EXPECT_EQ(cache.Get(2), "bbb");
  EXPECT_EQ(cache.Get(3), "ccc");
  EXPECT_EQ(cache.Get(4), "ddd");
  EXPECT_EQ(cache.Get(5), "eee");
  EXPECT_EQ(cache.Get(6), "fff");
}

TEST(GetTest, Get) {
  ExpireLRUCache<int, std::string> cache(5);
  cache.Add(1, "aaa");
  cache.Add(2, "bbb");

  EXPECT_EQ(cache.Get(1), "aaa");
  EXPECT_EQ(cache.Get(2), "bbb");
  EXPECT_EQ(cache.Get(5), "");
}

TEST(ExpireTest, Expire) {
  ExpireLRUCache<int, std::string> cache(5, 10, [] (int, std::string){});
  cache.Add(1, "aaa");
  EXPECT_EQ(cache.Get(1), "aaa");
  std::this_thread::sleep_for(std::chrono::milliseconds(15));
  EXPECT_EQ(cache.Get(1), "");
  EXPECT_EQ(cache.Size(), 0);
}

TEST(ExpireOldestTest, ExpireOldest) {
  ExpireLRUCache<int, std::string> cache(5, 10, [] (int, std::string){});
  cache.Add(1, "aaa");
  cache.Add(2, "bbb");
  
  EXPECT_EQ(cache.Get(1), "aaa");
  EXPECT_EQ(cache.Get(2), "bbb");
  std::this_thread::sleep_for(std::chrono::milliseconds(9));
  EXPECT_EQ(cache.Get(1), "aaa");
  EXPECT_EQ(cache.Get(2), "bbb");
  EXPECT_EQ(cache.Get(3), "");

  cache.Add(2, "nnn");
  cache.Add(3, "ccc");
  std::this_thread::sleep_for(std::chrono::milliseconds(2));
  EXPECT_EQ(cache.Get(1), "");
  EXPECT_EQ(cache.Get(2), "nnn");
  EXPECT_EQ(cache.Get(3), "ccc");
}

TEST(ExpireCallbackTest, ExpireCallback) {
  int key = 0;
  std::string value;
  ExpireLRUCache<int, std::string> cache(5, 10, 
    [&] (int k, std::string v){
      key = k;
      value = v;
    });
  cache.Add(1, "aaa");
  EXPECT_EQ(cache.Get(1), "aaa");
  EXPECT_EQ(cache.Size(), 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(15));
  EXPECT_EQ(cache.Get(1), "");
  EXPECT_EQ(key, 1);
  EXPECT_EQ(value, "aaa");
}


}  // namespace base
}  // namespace wheel
