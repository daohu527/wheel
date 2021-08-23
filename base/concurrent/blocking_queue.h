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

#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>

namespace wheel {
namespace base {

template <typename T>
class BlockingQueue {
 public:
  explicit BlockingQueue(const uint64_t capacity);
  virtual ~BlockingQueue();

  BlockingQueue(const BlockingQueue& other) = delete;
  BlockingQueue& operator=(const BlockingQueue& other) = delete;

  void Push(T t);
  void Push(T& t);

  T Pop();

  bool Empty() const { return !size_.load(); }

  size_t Size() const { return size_.load(); }

  size_t Capacity() const { return capacity_; }
 
 private:
  void Wait();

 private:
  T* data_;
  const uint64_t capacity_;

  std::atomic<uint64_t> head_;
  std::atomic<uint64_t> tail_;
  std::atomic<uint64_t> size_;

  std::mutex mutex_;
  std::condition_variable cv_;
};


template <typename T>
BlockingQueue<T>::BlockingQueue(const uint64_t capacity) 
    : capacity_(capacity), head_(0), tail_(0), size_(0) {
  data_ = new T[capacity];
}

template <typename T>
BlockingQueue<T>::~BlockingQueue() {
  delete[] data_;
  data_ = nullptr;
}

template<typename T>
void BlockingQueue<T>::Push(T& t) {
  // full
  while (size_.load() >= capacity_) {
    tail_++;
    size_--;
  }

  // push 
  uint64_t old_head = head_.load();
  while(!head_.compare_exchange_weak(old_head, old_head + 1)) {

  }
  data_[old_head % capacity_] = t;
  size_++;

  // notify all consumer
  std::unique_lock<std::mutex> lck(mutex_);
  cv_.notify_all();
}

template<typename T>
void BlockingQueue<T>::Push(T t) {
  // full
  while (size_.load() >= capacity_) {
    tail_++;
    size_--;
  }

  // push 
  uint64_t old_head = head_.load();
  while(!head_.compare_exchange_weak(old_head, old_head + 1)) {

  }
  data_[old_head % capacity_] = t;
  size_++;

  // notify all consumer
  std::unique_lock<std::mutex> lck(mutex_);
  cv_.notify_all();
}

template<typename T>
T BlockingQueue<T>::Pop() {
  // wait for condition
  Wait();

  // 
  uint64_t old_tail = tail_.load();
  while(!tail_.compare_exchange_weak(old_tail, old_tail + 1)) {

  }
  size_--;

  return data_[old_tail % capacity_];
}

template<typename T>
void BlockingQueue<T>::Wait() {
  std::unique_lock<std::mutex> lck(mutex_);
  while(!size_.load()) {
    cv_.wait(lck);
  }
}


}    // namespace base
}    // namespace wheel
