## Profiler
Profiler provides runtime performance statistics, which includes a library for printing performance logs and an interface for parsing logs in 2 parts.

## Features
#### Function
1. Count the time spent on the entire function
```c++
void function() {
  PERF_FUNCION();
  // code
}
```

#### Block
2. Count the time spent on a piece of code
```c++
void function() {
  PERF_BLOCK(TAG);
  // code
  PERF_BLOCK_END;
}
```

2. Nested block
```c++
void function() {
  PERF_BLOCK(TAG);
  // code
    PERF_BLOCK(TAG);
    // code
    PERF_BLOCK_END;
  // code
  PERF_BLOCK_END;
}
```

3. Omit in scope
```c++
void function() {
  PERF_BLOCK(TAG);
  // code
  for (auto x : str) {
    PERF_BLOCK(TAG);
    // code
    // PERF_BLOCK_END; can be omitted
  }
  // code
  // PERF_BLOCK_END; can be omitted at end function
}
```

## Design
We have not chosen to support the following features
1. Across scope
```c++
void functionA() {
  PERF_BLOCK();
  // code
}

void functionB() {
  // code
  PERF_BLOCK_END();
  // code
}
```
Across scope time statistics will cause matching problems, because the function execution order may not be consistent with the imagination, but will bring mismatched PERF_BLOCK and PERF_BLOCK_END, which may cause other problems, so this may not be a good idea.
You can understand it as `}{`, but it should be `{}`.

2. Overlapping blocks
```c++
void function() {
  PERF_BLOCK("a");
  // code
    PERF_BLOCK("b");
  // code
  PERF_BLOCK_END("a");
  // code
    PERF_BLOCK_END("b");
}
```
We do have this statistical need, but it also introduces matching complexity. Once it is written wrong or forgotten, it will bring errors, and we have given up this feature in the same way.

Finally, we summarize the use of profiler. It's actually a bit of a {} game. You can nest multiple {}, and count their time-consuming.
```
{{}}{}{}{{{}}}
```
Therefore, we restrict PERF_BLOCK_END without input parameters, just as the end symbol. We need PERF_BLOCK to pass in the tag to distinguish which block.

3. Online analysis vs. offline analysis
Offline analysis is relatively easy to implement. It only needs to output logs, but it cannot output performance statistics on the terminal in real time. Therefore, we finally decided that the online calculation time-consuming.

In this way we need a stack to save block_start, and pop the last one at block_end. We can use the thread_local class and extend it to the coroutine local class.

4. PERF_BLOCK vs PERF_BLOCK_START
Since we can omit PERF_BLOCK_END at the end of the scope, so only a `PERF_BLOCK_START` left is stange, change to `PERF_BLOCK` would be more reasonable.
```c++
  for (auto x : str) {
    PERF_BLOCK(TAG);
    // code
    // PERF_BLOCK_END; can be omitted
  }
```

5. Nested block
Nested block need a stack to store the start time of the blocks, so we use a thread_local stack. It's ok for thread only applicalton, but for coroutine, we need a `map<coroutine_name, stack>` data structure. Map need thread-safe but stack not, because only one coroutine runs on a thread at a time.

6. Extra PERF_BLOCK_END
When redundant PERF_BLOCK_END occurs, it will early end the other PERF_BLOCK. Do have some method to avoid this?

7. Profiler save cost
Because of program is always nested running. So how to save the cost in child block is important. Most of the time we print to terminal using stream, we should minimize the time to save block information.

Another solution would be to store all the chunks in memory and print them when the end of the proc is reached.


## Testcase