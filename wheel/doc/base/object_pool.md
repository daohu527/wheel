## object pool

## Features
1. object have state like: in-use, free
2. object can set reuse function

## Design
There are three types of recycling objects. Here is a ref [link](https://cloud.tencent.com/developer/article/1800443)  

1. Through destructor. This like a smart point to manage the state of object.
2. Add judgment condition by timer. User can provide a condition and timer will check it periodically, to release the object.  
3. The user actively releases resources.

The above method is the same as the memory management method, 1 and 3 are C++ memory management methods, and 2 is Java memory management methods.  

#### The user actively releases resources
We will provide an active release interface.

## Benchmarks
1. Poco::ObjectPool
2. Boost::pool

## Testcase
