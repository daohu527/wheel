## Singleton

## Features
1. Globally unique object

## Design
We use static local variable to create singleton, but there're also some problems like 
1. there are dependencies between singletons. 
2. how to keep the singleton atomic.

We can also use `std::call once` to achieve this too. Can ref to folly's `Singleton`.

## Testcase
1. create singleton
