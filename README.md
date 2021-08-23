# wheel
Publish and Subscribe System for Autonomous Driving.  

## Motivation
Inspired by ROS and Apollo, this project aims to create a new publish and subscribe system that can be used in autonomous driving and robotics industries.  

**The biggest improvement of this project is the subscription publishing mechanism**. At present, most of the system subscription mechanisms are based on topic on the surface, but actually based on object type. we will improve this, and provide two other publish and subscribe mechanisms `Topic-based` and `Content-based`.  

In addition, we hope to provide the concept of `data flow` to better manage and process autonomous driving data but not only a bag.

Finally, we focus on building the underlying system and do not involve any algorithms. We will provide package management tools to support the entire system.

## Features
Finally, we summarize the advantages of this project.
1. New publish and subscribe mechanism
2. Streaming data processing method
3. Package management method

## Quick start

#### Environment
* Operating system: Ubuntu 18.04
* Bazel version: 4.2.0

#### Build
1. Install bazel through this [doc](https://docs.bazel.build/versions/4.0.0/install-ubuntu.html).  
2. Enter the `wheel` directory, and run the `./build.sh`.

## Contribute
This project is still in the process of improvement. We will update the design document synchronously and gradually improve it！
