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
  BlockingQueue(uint64_t capacity);
  virtual ~BlockingQueue();

  BlockingQueue(const BlockingQueue& other) = delete;
  operator=(const BlockingQueue& other) = delete;

  void Push(T& t);

  T& Pop();

  bool Empty() const { return head_ == tail_; }

  size_t Size() const { return capacity_; }

 private:
  std::atomic<uint64_t> head_;
  std::atomic<uint64_t> tail_;
  std::unique_ptr<T> data_;
  uint64_t capacity_;

  std::mutex mutex_;
  std::condition_variable cv_;
};


template <typename T>
BlockingQueue<T>::BlockingQueue(uint64_t capacity) 
    : head_(0), tail_(0), capacity_(capacity) {
  data_.reset(new T[capacity]);
}

template<typename T>
void BlockingQueue<T>::Push(T& t) {
  // full
  if (tail_ - head_ > capacity_) {
    head_++;
  }

  // push 
  tail_++;
  data_[tail_] = t;

  // notify all consumer
  std::unique_lock<std::mutex> lck(mutex_);
  cv_.notify_all();
}

template<typename T>
T& BlockingQueue<T>::Pop() {
  // wait for condition
  std::unique_lock<std::mutex> lck(mutex_);
  while(tail_ == head_) {
    cv_.wait(lck);
  }

  // 
  head_++;
  T& d = data[head_ - 1];

  return d;
}


}    // namespace base
}    // namespace wheel
