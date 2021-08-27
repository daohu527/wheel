#pragma once

namespace wheel {
namespace node {

class Subscriber {
 public:
  // receive messages
  void notify();

  void subscribe();

  void unsubscribe();

};

}  // namespace node
}  // namespace wheel
