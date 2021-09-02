


#include "timer/time_wheel.h"

namespace wheel {
namespace timer {

TimeWheel::TimeWheel()
    : millisecond_index_(0),
      second_index_(0),
      minute_index_(0),
      interval_(std::chrono::milliseconds{DEFAULT_TICK_INTERVAL}) {
  // 1. create a thread
  std::thread wheel_thread_ = std::thread(TimeWheel::schedule);
  // 2. start to schedule
  wheel_thread_.detach();
}

TimeWheel::~TimeWheel() {
  // release wheel_thread_
}

bool TimeWheel::addTicket(TicketPtr ticket_ptr) {
  std::lock_guard<std::mutex> lock(mutex_);
  // add ticket to bucket
}

bool TimeWheel::delTicket(TicketPtr ticket_ptr) {
  std::lock_guard<std::mutex> lock(mutex_);
  // del ticket to bucket
}

void TimeWheel::tick() {
  // sleep
  std::this_thread::sleep_for(interval_);

  // update index
  millisecond_index_++;
  if (millisecond_index_ >= MILLISECOND_BUCKET_SIZE) {
    millisecond_index_ = 0;
    second_index_++;
    if (second_index_ >= SECOND_BUCKET_SIZE) {
      second_index_ = 0;
      minute_index_++;
      if (minute_index_ >= MINUTE_BUCKET_SIZE) {
        minute_index_ = 0;
      }
    }
  }
}

void TimeWheel::schedule() {
  while (true) {
    // 1. tick
    tick();

    // 2. run timeout task
    buckets_[millisecond_index_].checkAndRun();

    // 3. book new tickets
    std::list<TicketPtr> tickets;
    buckets_[millisecond_index_].rebookTickets(tickets);
    for (TicketPtr ticket_p: tickets) {
      addTicket(ticket_p);
    }
  }
}


}  // namespace timer
}  // namespace wheel
