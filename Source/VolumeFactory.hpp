/*
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
THE SOFTWARE
*/

#pragma once
#ifndef RAYMARCH_VOLUMEFACTORY_HPP
#define RAYMARCH_VOLUMEFACTORY_HPP

#include "Volume.hpp"

namespace Raymarch {
    /// @brief  VolumeFactory provides a number of factory functions to create volumes.
    class VolumeFactory {
    private:
        /// @brief  Deleted destructor.
        ~VolumeFactory(void) = delete;
        /// @brief  Deleted constructor.
        VolumeFactory(void) = delete;

    public:
        /// @brief  Create a solid cuboid volume of a given voxel type.
        /// @param  SizeX - Width of the volume.
        /// @param  SizeY - Height of the volume.
        /// @param  SizeZ - Depth of the volume.
        /// @param  Value - The voxel value.
        static Volume CreateSolid(std::size_t SizeX, std::size_t SizeY, std::size_t SizeZ, Voxel Value);

        /// @brief  Create an ellipsoid of a given voxel type.
        /// @param  SizeX - Width of the volume.
        /// @param  SizeY - Height of the volume.
        /// @param  SizeZ - Depth of the volume.
        /// @param  Value - The voxel value.
        static Volume CreateEllipsoid(std::size_t SizeX, std::size_t SizeY, std::size_t SizeZ, Voxel Value);

        /// @brief  Create a sponge by only setting random positions in a cuboid volume to a given voxel type.
        /// @param  SizeX - Width of the volume.
        /// @param  SizeY - Height of the volume.
        /// @param  SizeZ - Depth of the volume.
        /// @param  Density - The fill ratio of the sponge between 0.0 and 1.0.
        /// @param  Value - The voxel value.
        static Volume CreateRandomSponge(std::size_t SizeX, std::size_t SizeY, std::size_t SizeZ, double Density, Voxel Value);

        /// @brief  Create a column volume of a given voxel type.
        /// @param  SizeX - Width of the volume.
        /// @param  SizeY - Height of the volume.
        /// @param  SizeZ - Depth of the volume.
        /// @param  Radius - The radius of the column radius between 0.0 and 1.0.
        /// @param  Value - The voxel value.
        static Volume CreateColumn(std::size_t SizeX, std::size_t SizeY, std::size_t SizeZ, double Radius, Voxel Value);
    };
}

#endif // RAYMARCH_VOLUMEFACTORY_HPP
