## time_wheel

## Features
1. multi-level time wheel
2. async task scheduling
3. thread-safed

## Design
We adopt a multi-level time wheel design. And start a separate thread for scheduling. 

#### bucket
We use two or three cycle array to represent a dial, each dial contains a number of buckets. Each bucket represents the smallest unit of timewheel. 

The bucket contains a linked list, a linked list of currently existing tasks, each task is called a ticket.

#### ticket
ticket is a node in task list, which is a linked list. Ticket should include `current lap`, because the interval time of tasks may exceed the total time of the lowest level of time wheel.

Every time we remove the task that has timed out from the current bucket and store it in the next bucket, if there is an invalid timer, we will remove the task directly.

todo(daohu527): We can optimize the linked list, such as the priority queue to get overtime tasks? 

#### schedule
The schedule includes 2 parts, one is the time wheel itself, the other one is timeout tasks in the bucket.
1. time wheel itself use a separate thread to schedule
2. timeout tasks use an async interface to scheduling

todo(daohu527): How to get a high-precision clock?

#### questiones
1. If we have a task execution time exceeds the set time interval, then multiple tasks may be executed at the same time.

## Testcase
#### bucket
1. add/del ticket to bucket
2. ticket next bucket

#### ticket
1. ticket is or not timeout
2. ticket reduce the number of laps

#### time wheel
1. single level time wheel trigger
2. multi-level time wheel trigger
3. tasks scheduling
