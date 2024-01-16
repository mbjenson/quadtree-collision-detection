## Quadtree-Accelerated Collision Detection

#### Visualizes of how collision detection in 2D can be optimized using a quadtree data structure.

### Contents
1. [**Build**](#build)
2. [**Scene Settup**](#scene-settup)
3. [**Logic**](#logic)
4. [**Rendering**](#rendering)

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

Run executable

    ./bin/QuadtreeDemo
### Scene Settup

When running the exectuable, you will be prompted for the following information in the command line
* `object count`: number of physics objects in the scene
* `object size range 'min max'`: range for randomly generated physics objects
* `velocity range 'min max'`: initial object velocity range
* `collision detection type`: choose whether to use the quadtree or to use bruteforce to calculate the object collisions

#### recommended scenerios:
I *highly recommend* trying out the following scenerios first
1) object count = 10, object size range = 100 140, object velocity range = 10 200, use quadtree acceleration (1)
2) object count = 100, object size range = 40 60, object velocity range = 20 200, use quadtree acceleration (1)
3) object count = 3000, object size range = 4 8, object velocity range = 20 60, use quadtree acceleration (1)
   *compare this test's performance with the brute force method*
5) object count = 15000, object size range 2 4, object veloctiy range = 10 100, use quadtree acceleration (1)
   *compare this test's performance with the brute force method*

### Logic
This quadtree implementation stands out because:
* it can store objects that have massive bounding boxes and tiny bounding boxes. It does this by storing values in both leaf and interior nodes. The collisions are then evaluated recursively with all children of a node, should it contains value(s) and have children.
* bounding boxes for the values it stores are computed on the fly. This may seem computationally heavy, however, its performance impact turns out to be quite small and it is useful to have a more lightweight structure like this.

### Rendering
Rendering is done with the [SFML](https://github.com/SFML/SFML) graphics API.

This program spends very little time drawing which is owed to the single vertex draw call. This was accomplished by using a bunch of functions that take rectangles and translate the information into verticies which are then placed into a vertex array which is sent to the GPU.
