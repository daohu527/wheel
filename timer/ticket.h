#pragma once

#include <atomic>
#include <functional>


namespace wheel {
namespace timer {

class Ticket {
 public:
  using TicketId = uint64_t;
  using Task = std::function<void(void)>;
   
  Ticket(TicketId id) : ticket_id_(id), state_(0) {}

  ~Ticket();

  Ticket(const Ticket&) = delete;
  Ticket& operator=(const Ticket&) = delete;



 private:
  TicketId ticket_id_;
  Task task_;
  uint64_t remain_round_;

  // 0: not run yet
  // 1: running
  // 2: removed
  std::atomic<uint8_t> state_;
};

}  // namespace timer
}  // namespace wheel
