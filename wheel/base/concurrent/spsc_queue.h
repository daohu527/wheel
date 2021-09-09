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
#include <cassert>

namespace wheel {
namespace base {

template <typename T>
class SPSCQueue {
 public:
  static constexpr std::size_t LOCKFREE_CACHELINE_BYTES = 64;

  SPSCQueue(const SPSCQueue&) = delete;
  SPSCQueue& operator=(const SPSCQueue&) = delete;

  explicit SPSCQueue(std::size_t size)
      : size_(size + 1),
        storage_(new T[size + 1]),
        head_(0),
        tail_(0) {
    assert(size > 0);
  }

  ~SPSCQueue() {
    if (storage_) {
      delete[] storage_;
    }
  }

  // Producer only
  bool enqueue(const T& t);

  // Consumer only
  bool dequeue(T& t); // NOLINT: pass reference to get value

  bool empty() const;

  std::size_t size() const;

  std::size_t capacity() const {
    return size_ - 1;
  }

 private:
  using index_type = uint32_t;
  using AtomicIndex = std::atomic<index_type>;

  index_type nextIndex(index_type index) {
    index += 1;
    while (index >= size_) {
      index -= size_;
    }
    return index;
  }

 private:
  const std::size_t size_;
  T* const storage_;

  // todo(daohu527): need to check alignas
  alignas(LOCKFREE_CACHELINE_BYTES) AtomicIndex head_;
  alignas(LOCKFREE_CACHELINE_BYTES) AtomicIndex tail_;
};


template <typename T>
bool SPSCQueue<T>::enqueue(const T& t) {
  auto cur_tail = tail_.load(std::memory_order_relaxed);
  auto next_tail = nextIndex(cur_tail);
  // full
  if (next_tail == head_.load(std::memory_order_acquire))
    return false;

  storage_[cur_tail] = t;
  tail_.store(next_tail, std::memory_order_release);
  return true;
}

template <typename T>
bool SPSCQueue<T>::dequeue(T& t) {  // NOLINT
  auto cur_head = head_.load(std::memory_order_relaxed);
  if (cur_head == tail_.load(std::memory_order_acquire))
    return false;

  auto next_head = nextIndex(cur_head);
  t = storage_[cur_head];
  // memory order
  // todo(daohu527): need to check memory order
  head_.store(next_head, std::memory_order_release);
  return true;
}

template <typename T>
bool SPSCQueue<T>::empty() const {
  return head_.load(std::memory_order_relaxed)
            == tail_.load(std::memory_order_relaxed);
}

template <typename T>
std::size_t SPSCQueue<T>::size() const {
  auto diff = tail_.load(std::memory_order_relaxed)
          - head_.load(std::memory_order_relaxed);
  if (diff > 0)
    return diff;

  return diff + capacity();
}


}  // namespace base
}  // namespace wheel
