## Timer
A software timer

todo(daohu527): Use other timers, such as minimum heap, red-black tree, etc.

## Features
1. Timer can start and stop
2. Timer can be executed once or cyclically
3. Timer can set the time interval and time unit
4. Timer can delay start

## Design
Realize the timer with a 2-stage time wheel.

## Testcase
1. create a timer and run one shot
2. delay start
3. timer call periodically
4. timer start/stop
