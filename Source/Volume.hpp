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
#ifndef RAYMARCH_VOLUME_HPP
#define RAYMARCH_VOLUME_HPP

#include "Voxel.hpp"

#include <array>
#include <cstdint>
#include <vector>

namespace Raymarch {
    /// @brief  Volume holds a voxel volume.
    class Volume {
    private:
        /// @brief  Size of the volume.
        std::array<std::size_t, 3> Size;

        /// @brief  The volume data.
        std::vector<Voxel> Data;

    public:
        /// @brief  Defaulted constructor.
        Volume(void) = default;

        /// @brief  Constructor that allocates an empty volume.
        /// @param  Size - The size of the volume to allocate.
        Volume(const std::array<std::size_t, 3>& Size);

        /// @brief  Constructor that allocates an empty volume.
        /// @param  SizeX - The width of the volume.
        /// @param  SizeY - The height of the volume.
        /// @param  SizeZ - The depth of the volume.
        Volume(std::size_t SizeX, std::size_t SizeY, std::size_t SizeZ);

    public:
        /// @brief  Get the size of the allocated volume.
        /// @return The size of the volume.
        const std::array<std::size_t, 3> GetSize(void) const;

        /// @brief  Get the width of the volume.
        /// @return The width of the volume.
        std::size_t GetSizeX(void) const;

        /// @brief  Get the height of the volume.
        /// @return The height of the volume.
        std::size_t GetSizeY(void) const;

        /// @brief  Get the depth of the volume.
        /// @return The depth of the volume.
        std::size_t GetSizeZ(void) const;

    public:
        /// @brief  Get a voxel within this volume.
        /// @param  X - The X coordinate within this volume to get.
        /// @param  Y - The Y coordinate within this volume to get.
        /// @param  Z - The Z coordinate within this volume to get.
        /// @return A reference to a voxel within this volume.
        Voxel& operator()(std::size_t X, std::size_t Y, std::size_t Z);

        /// @brief  Get a voxel within this volume.
        /// @param  X - The X coordinate within this volume to get.
        /// @param  Y - The Y coordinate within this volume to get.
        /// @param  Z - The Z coordinate within this volume to get.
        /// @return A const reference to a voxel within this volume.
        const Voxel& operator()(std::size_t X, std::size_t Y, std::size_t Z) const;

    public:
        /// @brief  Get a pointer to the data in this volume.
        /// @return A const pointer to the data in the volume.
        const Voxel* data(void) const;
    public:
        /// @brief  Clear the volume, set all voxels to empty.
        void Clear(void);

        /// @brief  Fill the volume, set all voxels to a given type.
        /// @param  Value - The voxel type used to fill the volume.
        void Fill(Voxel Value);

        /// @brief  Combine this volume with another source.
        /// @param  X - The X location to position the source volume within this volume.
        /// @param  Y - The Y location to position the source volume within this volume.
        /// @param  Z - The Z location to position the source volume within this volume.
        /// @param  Source - The source volume to write into this volume.
        void Insert(int X, int Y, int Z, Volume Source);
	};
}

#endif // RAYMARCH_VOLUME_HPP
