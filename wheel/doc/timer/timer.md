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

#### timer
We hope to implement a variable parameter template to achieve flexible timeout callbacks.

Also, we change `stop` to `cancel` to make it more appropriate.

#### TimeUnit
Adding the `minute` unit will introduce some problems, for example you might add `1.2 minute`. But we will encounter problems when we add `1.2 milliseconds`. Because our smallest unit is `1 milliseconds`. So instead of adding minute, it’s better to use seconds instead. For example, you can use 80 seconds instead of 1.2 minutes.
So we only keep `seconds` and `milliseconds`.

#### ticket
We can wrapper the callback in timer and get a new callable function, which will more simple to ticket. For example we can use `std::function<void(void)>` to a callback.

#### callback
(todo) Most callbacks need to carry parameters, although lambada functions can also meet our needs. In the future consider changing to a callback with parameters.

## Testcase
1. create a timer and run one shot
2. delay start
3. timer call periodically
4. timer start/stop
