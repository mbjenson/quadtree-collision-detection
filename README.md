# Quadtree-Accelerated Collision Detection
Visualization of how a quadtree can be used to massively increase the performance of object collision detection in 2D

## Contents
* [**Preview**](#preview)
* [**Build**](#build)
* [**Scene Settup**](#scene-settup)
* [**Logic**](#logic)
* [**Rendering**](#rendering)
* [**User-Interface and Controls**](#user-interface-and-controls)


## Preview

<img width="400" alt="program in action with 4000 objects" src="https://github.com/mbjenson/quadtree-collision-detection/assets/115751313/ebb3d9d5-67d7-4fe9-a46c-0dfc82591ada">

*program in action with 4000 objects*

## Build
*note: uses `g++` and `cmake`*

Clone repository


    git clone https://github.com/mbjenson/quadtree-collision-detection.git
Navigate to the project root directory containing `CMakeLists.txt` and create build directory

    mkdir build
    cd build
    cmake ..
    
compile executable with `make`.
compiled binary will be located in `/bin/QuadtreeDemo`

## Scene Settup

When running the exectuable, you will be prompted for the following information in the command line
* `object count`: number of physics objects in the scene
* `object size range 'min max'`: range for randomly generated physics objects
* `velocity range 'min max'`: initial object velocity range
* `collision detection type`: choose whether to use the quadtree or to use bruteforce to calculate the object collisions

#### recommended scenerios:
I *highly recommend* trying out the following scenerios first
1) object count = 10, object size range = 100 140, object velocity range = 10 200, use quadtree acceleration (1)
2) object count = 100, object size range = 40 60, object velocity range = 20 200, use quadtree acceleration (1)
3) object count = 3000, object size range = 4 8, object velocity range = 20 60, use quadtree acceleration (1)\
   *compare this test's performance with the brute force method*
5) object count = 15000, object size range 2 4, object veloctiy range = 10 100, use quadtree acceleration (1)\
   *compare this test's performance with the brute force method*

## Logic
This quadtree implementation stands out because:
* it can store objects that have massive bounding boxes and tiny bounding boxes. It does this by storing values in both leaf and interior nodes. For values stored in interior nodes, the collisions are evaluated recursively with all values stored in the children of its containing node.
* bounding boxes are computed on the fly. This may seem computationally heavy, however, its performance impact turned out to be quite small and it is extremely useful to have a structure like this be more lightweight.

## Rendering
Rendering is done with the [SFML](https://github.com/SFML/SFML) graphics API.

This program spends very little time drawing which is owed to the single vertex draw call. This was accomplished by using a bunch of functions that take rectangles and translate the information into verticies which are then placed into a vertex array which is sent to the GPU.

## User-Interface and Controls
#### info
* The frames-per-second, scene object count, and realtime object collision count are displayed on the top left of the screen
#### controlling the scene
* place object at cursor position : Space
* toggle quadtree visibility : N
* reset the view : R
* move camera : WASD / Arrow keys
* zoom camera : ctrl +/-
