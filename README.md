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
*program in action with 4000 objects*

<img width="400" alt="program in action with 4000 objects" src="https://github.com/mbjenson/quadtree-collision-detection/assets/115751313/ebb3d9d5-67d7-4fe9-a46c-0dfc82591ada">


*Demo Video*

https://github.com/mbjenson/quadtree-collision-detection/assets/115751313/adc2295e-a4e4-4d9d-a1cc-2c6d736d7d40

## Build

Dependencies
* `gcc`
* `SFML 3.0.1` ([SFML website](https://www.sfml-dev.org))

Clone this repository (or download the .zip)

    git clone https://github.com/mbjenson/quadtree-collision-detection.git

Navigate to the project root directory containing `Makefile` and run the following command (note: this may take some time):

    make build
    
then run using

    make run

## Usage

When you start up the program you will be asked for th following information:
* `object count`: number of physics objects in the scene
* `object size range 'min max'`: range for randomly generated physics objects
* `velocity range 'min max'`: initial object velocity range
* `collision detection type`: choose collision algorithm

#### recommended scenerios:
1)  
    object count = '100'

    object size range = '20 40'

    object velocity range = '100 300'

    use quadtree = '1'

2)  
    object count = '10000'

    object size range = '3 6'

    object velocity range = '30 50'

    use quadtree = '1'

*compare performance of bruteforce collision detection by setting `use quadtree` to '0'.*

## Logic
This quadtree implementation stands out because:
* it can store objects that have massive bounding boxes and tiny bounding boxes. It does this by storing objects in either the leaf or interior nodes of the tree, depending on the object size. For values stored in interior nodes, the collisions are evaluated recursively with all values stored in the children of its containing node.
* bounding boxes are computed on the fly. This may seem computationally heavy, however, its performance impact turned out to be negligible and it allows the quadtree to be very lightweight in terms of memory.

## Rendering
Rendering is done with the [SFML](https://github.com/SFML/SFML) graphics API.
