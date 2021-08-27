#pragma once

#include <list>
#include <memory>
#include <vector>

#include "timer/ticket.h"

namespace wheel {
namespace timer {

class Bucket {
 public:
  // Get tasks to be scheduled
  void checkAndRun();
  void rebookTickets(std::list<Ticket::Ptr>& tickets);

 private:
  bool checkTickets();
  void runTimeoutTask();


 private:
  std::unique_ptr<std::list<Ticket>> ticket_list_;
};

}  // namespace timer
}  // namespace wheel
