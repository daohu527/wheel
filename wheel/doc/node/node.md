## node



## Features

## Design

#### Different ways to consume messages
1. After different nodes consume the message, the message is still stored in the memory. 
2. The message is deleted after the consumption is completed.

At present, most systems use the first method, and the second method can be directly concurrent.  

## Testcase