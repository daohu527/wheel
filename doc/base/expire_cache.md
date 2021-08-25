## expire_cache

## Features
1. Put\get function similar to hashmap in O(1).
2. Automatically delete elements that have timed out after the last visit.
3. After specifying the creation function, if the element is not created, the pointer is returned.
4. When the element is eliminated, specify the callback.
5. Can be set to a fixed size.

## Design
We use hash linked list to achieve it. 

(todo)The more controversial point is whether a separate timer is needed to clear expired elements.

## Testcase
1. put
2. get
3. over size
4. clear expired elements
5. build function test
6. callback function test
7. multi-thread safe