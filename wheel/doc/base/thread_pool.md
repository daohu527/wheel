## thread_pool

## Features
1. Create a fixed number of threads.
2. Put tasks into the thread pool and return the result asynchronously.
3. If the running thread reaches the maximum number, the task is put into the queue, and an exception is returned if the queue is full.

## Design

#### thread
we first create fixed size thread, each thread can perform tasks. Then we can make an abstraction of threads. We should call it a processor?

(todo) We can also create threads at startup until the upper limit is reached, this will be the next development content.

#### task
The task put into the thread pool should be abstracted as an asynchronous task. The execution result of the task will be returned in a form similar to the `std::function`.  

#### queue
we also need a thread safe queue to hold the tasks, considering that there are multiple producers and consumers, the queue can adopt the producer and consumer model.

If the queue is full, we need to raise a exception?  

## Testcase
1. create thread
2. run task
3. task queue full exception