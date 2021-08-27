#pragma once

#include <atomic>
#include <functional>


namespace wheel {
namespace timer {

class Ticket {
 public:
  using TicketId = uint64_t;
  using Task = std::function<void(void)>;
  using Ptr = std::shared_ptr<Ticket>;
  
  // todo(daohu527) : how to generate ticket id?
  Ticket(Task task, uint64_t interval, bool is_one_shot)
      : ticket_id_(0),
        task_(task),
        interval_(interval),
        is_one_shot_(is_one_shot),
        remain_round_(0),
        state_(0) {}

  ~Ticket();

  Ticket(const Ticket&) = delete;
  Ticket& operator=(const Ticket&) = delete;

 private:
  TicketId ticket_id_;
  Task task_;
  uint64_t interval_;
  bool is_one_shot_;
  uint64_t remain_round_;

  // 0: not run yet
  // 1: running
  // 2: removed
  std::atomic<uint8_t> state_;
};

}  // namespace timer
}  // namespace wheel
