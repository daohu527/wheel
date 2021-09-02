## tracking

## Features
1. 

## Design
Suppose we need the end-to-end delay of the final message, this message may have passed through multiple nodes. Therefore, a mechanism is needed to ensure that we can track the message link. If we are based on the message id, the frequency of multiple id is inconsistent, how will we choose?

Therefore, we use a dyeing mechanism to complete end-to-end tracking.  

#### dyer
The dyer will dye the message. 

#### color
The color will be added to the message header and used for tracking. 


## Testcase