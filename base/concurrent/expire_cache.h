#pragma once

#include <mutex>
#include <unordered_map>


namespace wheel {
namespace base {

template <typename K, typename V>
class ExpireCache {
 public:
  void put(K k, V v);

  V get(K k);

 private:
  std::unordered_map<K,V> map_;
  
};


}  // namespace base
}  // namespace wheel
