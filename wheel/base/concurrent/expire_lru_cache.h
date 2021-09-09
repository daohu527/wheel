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

#pragma once


#include <chrono>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>


namespace wheel {
namespace base {

template <typename K, typename V>
class ExpireLRUCache {
 private:
  using Clock = std::chrono::steady_clock;
  using Timestamp = std::chrono::time_point<Clock>;
  struct Node {
    K key;
    V value;
    Timestamp timestamp;
  };
  using NodePtr = std::shared_ptr<Node>;
  using NodeIter = typename std::list<NodePtr>::iterator;

 public:
  using ExpiredCallBack = std::function<void(const K&, const V&)>;

  explicit ExpireLRUCache(size_t max_size)
      : capacity_(max_size),
        timeout_(3000),
        expired_callback_(nullptr),
        read_refresh_flag_(false) {}

  ExpireLRUCache(size_t max_size,
                 uint32_t timeout,
                 ExpiredCallBack callback,
                 bool read_refresh_flag = false)
      : capacity_(max_size),
        timeout_(timeout),
        expired_callback_(callback),
        read_refresh_flag_(read_refresh_flag) {}

  ~ExpireLRUCache() = default;

  ExpireLRUCache(const ExpireLRUCache&) = delete;
  ExpireLRUCache& operator=(const ExpireLRUCache&) = delete;

  void add(const K& key, const V& value);

  V get(const K& key);

  size_t size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return map_.size();
  }

 private:
  void expired();

 private:
  // todo(daohu527): We now use global locks,
  // which can be replaced by atomic locks later
  mutable std::mutex mutex_;

  std::unordered_map<K, NodeIter> map_;
  std::list<NodePtr> list_;

  size_t capacity_;
  uint32_t timeout_;
  ExpiredCallBack expired_callback_;
  bool read_refresh_flag_;
};

template <typename K, typename V>
void ExpireLRUCache<K, V>::add(const K& key, const V& value) {
  std::lock_guard<std::mutex> lock(mutex_);
  // if full, delete oldest
  if (map_.size() >= capacity_) {
    NodePtr oldest = list_.back();
    list_.pop_back();
    map_.erase(oldest->key);
  }

  // if exist, delete it in the list, and then add to the front
  // at last overwrite in map.
  if (map_.find(key) != map_.end()) {
    NodeIter iter = map_[key];
    list_.erase(iter);
  }

  auto timestamp = Clock::now();
  NodePtr node_ptr = std::make_shared<Node>(Node{key, value, timestamp});
  list_.push_front(node_ptr);
  map_[key] = list_.begin();
}

template <typename K, typename V>
V ExpireLRUCache<K, V>::get(const K& key) {
  std::lock_guard<std::mutex> lock(mutex_);
  // todo(daohu527): need to move to timer
  expired();

  // not exist
  if (map_.find(key) == map_.end()) {
    return V{};
  }
  // exist, update timestamp
  NodePtr node_ptr = *map_[key];
  if (read_refresh_flag_) {
    node_ptr->timestamp = Clock::now();
    list_.erase(map_[key]);
    list_.push_front(node_ptr);
    map_[key] = list_.begin();
  }
  return node_ptr->value;
}

template <typename K, typename V>
void ExpireLRUCache<K, V>::expired() {
  // todo(daohu527): if we call in get, need to disable this.
  // if we move to a timer, we should use a lock.
  // std::lock_guard<std::mutex> lock(mutex_);

  auto time_now = Clock::now();
  while ( !list_.empty() ) {
    NodePtr oldest = list_.back();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
                  time_now - oldest->timestamp);
    if (diff.count() > timeout_) {
      list_.pop_back();
      map_.erase(oldest->key);
      // expired callback
      if (expired_callback_)
        expired_callback_(oldest->key, oldest->value);
    } else {
      break;
    }
  }
}

}  // namespace base
}  // namespace wheel
