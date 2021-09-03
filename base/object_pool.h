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

#include <functional>

namespace wheel {
namespace base {

template <class T>
class ObjectFactory {
 public:
  template <class ...Args>
  T createObject(Args&&... args);

  void destroyObject();
};


template <class T, class F = ObjectFactory<T>>
class ObjectPool {
 public:
  using Predicate = std::function<bool(T&)>;

  ObjectPool(std::size_t capacity, std::size_t max_capacity);

  ObjectPool(std::size_t capacity, std::size_t max_capacity, F factory);

  ObjectPool(std::size_t capacity, std::size_t max_capacity, F factory, Predicate pred);

  bool borrowObject(T& t);

  void returnObject(T& t);

 private:

 DECLARE_SINGLETON(ObjectPool)
};

}  // namespace base
}  // namespace wheel
