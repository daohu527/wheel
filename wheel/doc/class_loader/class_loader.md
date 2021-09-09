## class_loader

## Features

## Design
There are 2 ways to achieve dynamic loading of so.
1. c++ name mangling will change to c
2. use static method

Use dynamic loading and it will count by itself. There are many additions to this mechanism in ros, but I doubt whether it has lost maintenance.  

Here I am going to refer to the implementation of ros class_loader, but the code seems a bit difficult to understand, so I plan to refactor it.  

## thread safe
Thread safety of dlopen refer to https://stackoverflow.com/questions/9534666/shared-libraries-dlopen-and-thread-safety-of-library-static-pointers

## Reference
https://github.com/ros/class_loader

## Testcase
