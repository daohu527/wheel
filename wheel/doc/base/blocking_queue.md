## blocking queue
There are usually several interesting problems with blocking queues.
* single producer single consumer
* multiple producer single consumer
* single producer multiple consumer
* multiple producer multiple consumer

There are usually related to multithreading, so you need to know many concepts including `condition_variable`, `semaphore`,`mutex` and `atomic`.  

The first problem we solve is `multiple producer multiple consumer`.

## multiple producer multiple consumer

#### mutex
`mutex` provides mutual exclusion. 

#### condition_variable
`condition_variable` wil always work with a `unique_lock`, which used to help to keep the `condition_variable` be mutually exclusive. In other words, this lock only used for `condition_variable` but not for the data or programs. If you want to work with any kind of lockable type, you can use `condition_variable_any`. [ref](https://en.cppreference.com/w/cpp/thread/condition_variable)  

`condition_variable` handle this task through the producer and consumer patterns of the design pattern. It provides ordering.

#### semaphore
Semaphores are like a mixture of "mutexes" and "condition variables".

#### atomic
In order to introduce lock-free, we added the atomic type. the queue's head and tail use a atomic type. and then we need to move the head and tail to make the things work. Next we need to introduce atomic types.  

atomic is interesting but multi atomic oprate is not atomic. Of course, lock-free will bring many problems, such as you always need to pay attention to whether the operation is atomic. I am often driven crazy by this problem in the process of writing code.  


## Testcase
1. push
2. push wait
3. pop
4. pop wait
5. multi-thread safe
