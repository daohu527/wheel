## spsc_queue
We have implemented a single producer, single consumer queue.  

## Features
1. Bounded and unbounded queue implementation
2. Wait and notify pattern
3. single producer, single consumer

## Design
The following are three design patterns. We recorded the analysis process in detail.  

## Semaphore
#### enqueue\waitDequeue and waitEnqueue\dequeue
If we want to achieve the above combination, we need to ensure that the initial value of the `std::binary_semaphore` is 0. Let’s take the following case as an example.  
```c++
template <typename T>
bool SPSCQueue::enqueue(const T& t) {
  data_[head_ % capacity_] = t;
  head_++;
  readSemaphore_.release();
  return true;
}

template <typename T>
bool SPSCQueue::waitDequeue(T& t) {
  readSemaphore_.acquire();
  if (head_ == tail_) return false;

  t = data_[tail_ % capacity_];
  tail_++;
  writeSemaphore_.release();
  return true;
}
```
1. `waitDequeue` will wait and `enqueue` will running.
2. after `enqueue`, both `enqueue` and `waitDequeue` will running. Because it's a `SPSCQueue`, so add the tail will only in consumer and dequeue once then wait for enqueue. There are a few questions here.
* If enqueue 3 times, waitDequeue is executing during this period, then waitDequeue will only be executed 2 times at the end, because `readSemaphore_` is binary. Is it unreasonable?  
* Even if the queue is not empty, dequeue still need wait.  
* head_ will overwrites tail_ when writing.  

So I think it’s not a good design. So we use `counting_semaphore` instead `binary_semaphore`. So we only have one question left `head_ will overwrites tail_ when writing`. Even so, the multi-producer and consumer model still requires head_ and tail_ to be atomic.

If it is a bounded queue, you need to compare head_ and tail_, so the role of the semaphore is not very big? Therefore, it is feasible for us to use semaphores on unbounedqueue. If we implemente an unbounded queue, we don’t need `waitEnqueue` because it's unbounded and we don't need to wait.  

If we do not use semaphores, and directly enqueue and dequeue, there will be a `race condition`.  

#### waitEnqueue\waitDequeue
It implements a process similar to `ping-pong`, writing one at a time, and then reading one. But this loses the meaning of the queue. If `counting_semaphore` is used, then race condition will be introduced. And there is no need to implement other interfaces such as empty, because it is `ping-pong`.  
```c++
template <typename T>
bool SPSCQueue::waitEnqueue(const T& t) {
  writeSemaphore_.acquire();
  data_[head_ % capacity_] = t;
  head_++;
  readSemaphore_.release();
  return true;
}

template <typename T>
bool SPSCQueue::waitDequeue(T& t) {
  readSemaphore_.acquire();
  t = data_[tail_ % capacity_];
  tail_++;
  writeSemaphore_.release();
  return true;
}
```

So I don’t think semaphore is a good implementation?  

## mutex
Adopting a lock is very simple, I think we should skip it.  

## atomic
At last, we need atomic operations to keep lock-free, and semaphores to keep synchronization.

## Testcase