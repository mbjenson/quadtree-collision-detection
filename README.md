## Quadtree-Accelerated Collision Detection

Visualizes of how collision detection in 2D can be optimized using a quadtree

### Contents
1. [**Build**](#build)
2. [**Run**](#run)
3. [**Scene Settup**](#scene-settup)

### Build
*note: uses `g++` and `cmake`*

Clone repository

    git clone https://github.com/mbjenson/quadtree-collision-detection.git
Navigate to project root with `CMakeLists.txt` and create build directory

    mkdir build
    cd build
generate project build files

    cmake ..
finally, compile

    make

### Run
Run executable

    ./bin/QuadtreeDemo
### Scene Settup
*I recommend finding a scenerio that works and comparing it with, to without quadtree to see the performance difference.*

When running the exectuable, you will be prompted for the following information in the command line
* `object count`: number of physics objects in the scene
* `object size range 'min max'`: range for randomly generated physics objects
* `velocity range 'min max'`: initial object velocity range
* `collision detection` type: choose whether to use the quadtree or to use bruteforce to calculate the object collisions

#### recommended scenerios:
1) 10000 objects, object size 2 4, object velocity 20 90, use quadtree (1)
2) 500 objects, object size 20 40, object velocity 30 90, use quadtree (1)














