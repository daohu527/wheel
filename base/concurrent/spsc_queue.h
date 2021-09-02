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
  typedef size_t size_type;
  constexpr size_t LOCKFREE_CACHELINE_BYTES = 64;

  SPSCQueue(const SPSCQueue&) = delete;
  SPSCQueue& operator=(const SPSCQueue&) = delete;

  explicit SPSCQueue(size_type size)
      : size_(size + 1),
        storage_(new T[size_]),
        head_(0),
        tail_(0) {
    std::assert(size > 0);
  }


  ~SPSCQueue() {
    if (storage_) {
      delete[] storage_;
      storage_ = nullptr;
    }
  }

  // Producer only
  bool enqueue(const T& t);

  // Consumer only
  bool dequeue(T& t);

  bool empty() const;

  size_type size() const;

  size_type capacity() const {
    return size_ - 1;
  }

 private:
  using index_type = uint64_t;
  using AtomicIndex = std::atomic<index_type>;
  
  index_type nextIndex(const index_type& index) {
    index += 1;
    while(index >= size_) {
      index -= size_;
    }
    return index;
  }

 private:
  const size_type size_;
  T* const storage_;

  alignas(LOCKFREE_CACHELINE_BYTES) AtomicIndex head_;
  alignas(LOCKFREE_CACHELINE_BYTES) AtomicIndex tail_;
};


template <typename T>
bool SPSCQueue<T>::enqueue(const T& t) {
  auto cur_tail = tail_.load(std::memory_order_relaxed);
  auto next_tail = nextIndex(cur_tail);
  // full
  if (next_tail == head_.load(std::memory_order_relaxed))
    return false;

  storage_[cur_tail] = t;
  tail_.store(next_tail, std::memory_order_relaxed);
  return true;
}

template <typename T>
bool SPSCQueue<T>::dequeue(T& t) {
  auto cur_head = head_.load(std::memory_order_relaxed);
  if (cur_head == tail_.load(std::memory_order_relaxed))
    return false;

  auto next_head = nextIndex(cur_head);
  t = storage_[cur_head];
  // memory order
  head_.store(next_head, std::memory_order_relaxed);
  return true;
}

template <typename T>
bool SPSCQueue<T>::empty() const {
  return head_.load(std::memory_order_relaxed)
            == tail_.load(std::memory_order_relaxed);
}

template <typename T>
size_type SPSCQueue<T>::size() const {
  auto cur_head = head_.load(std::memory_order_relaxed);
  auto cur_tail = tail_.load(std::memory_order_relaxed);
  if (cur_tail > cur_head)
    return cur_tail - cur_head;
  
  return cur_tail + (size_ - 1) - cur_head;
}


}  // namespace base
}  // namespace wheel
