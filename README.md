# Museum
This project is an educational project aimed to create a 3D museum that the user can navigate through common First Person shooter controls. 

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/ooUze3oEYdY/0.jpg)](https://www.youtube.com/watch?v=ooUze3oEYdY)

The following fuctionalities are implemented: 

## 3D models
The 3D models used are the commons models used for educational purposes. The Bunny, Armadillo, Max Planck, Happy, Dragon, etc (The dragon model is not provided)
For each model used, various LOD levels are computed using Vertex Clustering, using both a grid-like data structure, as well as an octree for simultaneous calculation

## Quadric Error Metrics
For the calculation of the represenative vertex in Vertex Clustering, both mean and Quadric error metrics are implemented

## Time Critical Rendering
At runtime, all the visible models are gathered, based on the user's position, and their LOD level to be drawn is calculated using a greedy algorithm that takes into consideration distance, size, number of triangles, etc. 
Different LODS levels are marked with different colors. Red=Best LOD level, Green=Medium LOD level, Blue=Worst lod level

## Visibility
As a precomputation, the map is divided into cells, and the visibility set for each cell is computed. At runtime, this set is used to draw only what's visible, as well for LOD calculation for only the visible 3D models

