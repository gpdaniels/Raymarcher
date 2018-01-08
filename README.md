
# Raymarch #
-------------

This repository aims to demonstrate a simple implementation of a GLSL based OpenGL raycaster.

|Coloured blocks:  | Moving in the volume: | Time passing in the volume: |
|--|--|--|
|![Image of coloured blocks in the volume](https://raw.githubusercontent.com/gpdaniels/Raymarcher/master/Data/ColoursSmall.png "Image of coloured blocks in the volume") |![Animation of moving in the volume](https://raw.githubusercontent.com/gpdaniels/Raymarcher/master/Data/MoveSmall.gif "Animation of moving in the volume")|![Animation of light changing in the volume](https://raw.githubusercontent.com/gpdaniels/Raymarcher/master/Data/LightSmall.gif "Animation of light changing in the volume")|

Video of an early version:

[![Video initial version of renderer](https://img.youtube.com/vi/IOxeKkoE0Mg/0.jpg)](https://www.youtube.com/watch?v=IOxeKkoE0Mg)

## Scene ##

On load a voxel volume is generated consisting of:
- A grey bottom floor layer.
- Two green "grass" layers.
- A number of blue columns that are randomly placed.
- A set of six coloured blocks.
- A red sphere.

The whole scene is volumetric and can be changed very easily in the code.

## Controls ##

Use the arrow keys to move around.

There is a day/night cycle that occurs about once a minute.

## Inspriation ##

This project was inspired by the WebGL GLSL raymarcher written by Rye Terrell:
https://github.com/ivl/Voxgrind

[![Video initial version of renderer](https://img.youtube.com/vi/PM3NzaomfGU/0.jpg)](https://www.youtube.com/watch?v=PM3NzaomfGU)

## License ##

The MIT License

Copyright (c) 2017 Geoffrey Daniels. http://gpdaniels.com/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
