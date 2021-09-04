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

#include <assert>
#include <functional>
#include <list>
#include <memory>
#include <mutex>


namespace wheel {
namespace base {

template <class C, class P = C*>
class ObjectFactory {
 public:
  template <class ...Args>
  P createObject(Args&&... args) {
    return new C(std::forward<Args>(args)...);
  }

  void destroyObject(P ptr) {
    delete ptr;
  }
};

template <class C>
class ObjectFactory <C, std::shared_ptr<C>> {
 public:
  template <class ...Args>
  std::shared_ptr<C> createObject(Args&&... args) {
    return std::make_shared<C>(std::forward<Args>(args)...);
  }

  void destroyObject(std::shared_ptr<C> ptr) {}
};

template <class C, class P = C*, class F = ObjectFactory<C, P>>
class ObjectPool {
 public:
  using size_type = std::size_t;
  using Predicate = std::function<bool(P)>;

  ObjectPool(size_type capacity, size_type peak_capacity)
      : capacity_(capacity),
        peak_capacity_(peak_capacity),
        size_(0) {
    assert(capacity_ <= peak_capacity_);
  }

  ObjectPool(size_type capacity, size_type peak_capacity, F factory)
      : capacity_(capacity),
        peak_capacity_(peak_capacity),
        size_(0),
        factory_(factory) {
    assert(capacity_ <= peak_capacity_);
  }

  ObjectPool(size_type capacity, 
             size_type peak_capacity, 
             F factory, 
             Predicate pred)
      : capacity_(capacity),
        peak_capacity_(peak_capacity),
        size_(0),
        factory_(factory),
        pred_(pred) {
    assert(capacity_ <= peak_capacity_);
  }

  ~ObjectPool() {
    typename std::list<P>::iterator it = pool_.begin();
    while (it != pool_.end()) {
      factory_.destroyObject(*it);
      ++it;
    }
  }

  P borrowObject();

  void returnObject(P ptr);

  size_type capacity() const {
    return capacity_;
  }

  size_type peak_capacity() const {
    return peak_capacity_;
  }

  size_type size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return size_;
  }

  size_type available() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return peak_capacity_ - size_ + _pool.size();
  }

 private:
  const size_type capacity_;
  const size_type peak_capacity_;
  size_type size_;
  std::list<P> pool_;

  F factory_;
  Predicate pred_;

  mutable std::mutex mutex_;

 DECLARE_SINGLETON(ObjectPool)
};

template <class C, class P = C*, class F = ObjectFactory<C, P>>
P ObjectPool<C, P, F>::borrowObject() {
  std::lock_guard<std::mutex> lock(mutex_);

  if (!pool_.empty()) {
    P ptr = pool_.front();
    pool_.pop_front();
    return ptr;
  } else if (size_ < peak_capacity_) {
    P ptr = factory_.createObject();
    size_++;
    return ptr;
  } else {
    return nullptr;
  }
}

template <class C, class P = C*, class F = ObjectFactory<C, P>>
void ObjectPool<C, P, F>::returnObject(P ptr) {
  std::lock_guard<std::mutex> lock(mutex_);

  if (size_ < capacity_) {
    pool_.push_front(ptr);
  } else {
    factory_.destroyObject(ptr);
    size_--;
  }
}

}  // namespace base
}  // namespace wheel
