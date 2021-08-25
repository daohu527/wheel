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

#pragma once

#include <atomic>
#include <semaphore>

namespace wheel {
namespace base {

template <typename T>
class SPSCQueue {
 public:
  SPSCQueue(const SPSCQueue&) = delete;
  SPSCQueue& operator=(const SPSCQueue&) = delete;

  explicit SPSCQueue(uint32_t size)
      : size_(size),
        data_(new T[capacity]),
        readIndex_(0),
        writeIndex_(0) {}

  virtual ~SPSCQueue() {
    if (data_ != nullptr) {
      delete[] data_;
      data_ = nullptr;
    }
  }

  bool enqueue(const T& t) {

  }

  bool waitEnqueue(const T& t) {
    
  }

  T dequeue() {

  }

  T waitDequeue() {

  }

  bool empty() const {}

 private:
  using AtomicIndex = std::atomic<unsigned int>;

  const uint32_t size_;
  T* const data_;
  AtomicIndex readIndex_;
  AtomicIndex writeIndex_;

  std::binary_semaphore readSemaphore_;
  std::binary_semaphore writeSemaphore_;
};


}  // namespace base
}  // namespace wheel
