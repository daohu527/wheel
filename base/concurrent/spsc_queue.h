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
#include <memory>
#include <semaphore>

namespace wheel {
namespace base {

template <typename T>
class SPSCQueue {
 public:
  struct Node {
    Node(T t) : val(t), next(nullptr) {}
    T val;
    Node* next;
  };

  SPSCQueue(const SPSCQueue&) = delete;
  SPSCQueue& operator=(const SPSCQueue&) = delete;

  SPSCQueue();

  virtual ~SPSCQueue();

  bool waitEnqueue(const T& t);

  bool waitDequeue(T& t);

 private:
  Node* head_;
  Node* tail_;

  std::counting_semaphore readSemaphore_;
};


template <typename T>
SPSCQueue::SPSCQueue() : head_(nullptr), tail_(nullptr), readSemaphore_(0) {}

template <typename T>
SPSCQueue::~SPSCQueue() {

}

template <typename T>
bool SPSCQueue::enqueue(const T& t) {
  if (!tail_) {
    tail_ = new Node(t);
    head_ = tail_;
  } else {
    tail_->next = new Node(t);
    tail_ = tail_->next;
  }
  readSemaphore_.release();
  return true;
}

template <typename T>
bool SPSCQueue::waitDequeue(T& t) {
  readSemaphore_.acquire();
  t = head_->val;
  head_ = head_->next;
  return true;
}


}  // namespace base
}  // namespace wheel
