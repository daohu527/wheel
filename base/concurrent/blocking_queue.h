#pragma once

#include <atomic>
#include <memory>

namespace wheel {
namespace base {

template <typename T>
class BlockingQueue {
 public:
  explict BlockingQueue(uint64_t capacity);
  virtual ~BlockingQueue();

  BlockingQueue(const BlockingQueue& other);
  operator=(const BlockingQueue& other);

  void Push(T& t);

  T& Pop();

  bool Empty() const { return head_ == tail_; }

  size_t Size() const { return capacity_; }

 private:
  std::atomic<uint64_t> head_;
  std::atomic<uint64_t> tail_;
  std::unique_ptr<T> data_;
  uint64_t capacity_;
};


template <typename T>
BlockingQueue<T>::BlockingQueue(uint64_t capacity) 
    : head_(0), tail_(0), capacity_(capacity) {
  data_.reset(new T[capacity]);
}

template<typename T>
void BlockingQueue::Push(T& t) {
  // full

  // 
  if (head_ + 1 > capacity_)
    head_ = 0;
  else
    head_++;
  data_[head_] = t;

  // notifyAll
  condition.notifyAll();
}

template<typename T>
T& BlockingQueue::Pop() {
  // wait for condition
  while(condition) {

  }

  // if valid
  T& d = data[tail_];
  if (tail_ + 1 > capacity_)
    tail_ = 0;
  else
    tail_++;
  
  return d;
}


}    // namespace base
}    // namespace wheel
