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

#include "Volume.hpp"

#include <cassert>

namespace Raymarch {
    // Construct and allocate a volume of a given size.
    Volume::Volume(const std::array<std::size_t, 3>& Size)
        : Size(Size)
        , Data(Size[0] * Size[1] * Size[2]) {
    }

    // Construct and allocate a volume of a given size.
    Volume::Volume(std::size_t SizeX, std::size_t SizeY, std::size_t SizeZ)
        : Size{SizeX, SizeY, SizeZ}
        , Data(Size[0] * Size[1] * Size[2]) {
    }

    // Get the volume size.
    const std::array<std::size_t, 3> Volume::GetSize(void) const {
        return this->Size;
    }

    // Get the volume width.
    std::size_t Volume::GetSizeX(void) const {
        return this->Size[0];
    }

    // Get the volume height.
    std::size_t Volume::GetSizeY(void) const {
        return this->Size[1];
    }

    // Get the volume depth.
    std::size_t Volume::GetSizeZ(void) const {
        return this->Size[2];
    }

    // Get a voxel from within the volume.
    Voxel& Volume::operator()(std::size_t X, std::size_t Y, std::size_t Z) {
        assert(X < this->Size[0]);
        assert(Y < this->Size[1]);
        assert(Z < this->Size[2]);
        return this->Data[X + this->Size[0] * (Y + this->Size[1] * (Z))];
    }

    // Get a voxel from within the volume.
    const Voxel& Volume::operator()(std::size_t X, std::size_t Y, std::size_t Z) const {
        return const_cast<const decltype(this)>(this)->operator()(X, Y, Z);
    }

    // Get the volume data.
    const Voxel* Volume::data(void) const {
        return this->Data.data();
    }

    // Clear the volume to empty voxels.
    void Volume::Clear(void) {
        this->Fill(Voxel());
    }

    // Fill the volume with voxels of the given type.
    void Volume::Fill(Voxel Value) {
        std::fill(this->Data.begin(), this->Data.end(), Value);
    }

    // Copy a source volume into this volume.
    void Volume::Insert(int X, int Y, int Z, Volume Source) {
        for (std::size_t IndexX = 0; IndexX < Source.Size[0]; ++IndexX) {
            if (X + static_cast<int>(IndexX) < 0) continue;
            if (X + IndexX >= this->Size[0]) break;
            for (std::size_t IndexY = 0; IndexY < Source.Size[1]; ++IndexY) {
                if (Y + static_cast<int>(IndexY) < 0) continue;
                if (Y + IndexY >= this->Size[1]) break;
                for (std::size_t IndexZ = 0; IndexZ < Source.Size[2]; ++IndexZ) {
                    if (Z + static_cast<int>(IndexZ) < 0) continue;
                    if (Z + IndexZ >= this->Size[2]) break;
                    this->operator()(X + IndexX, Y + IndexY, Z + IndexZ) = Source(IndexX, IndexY, IndexZ);
                }
            }
        }
    }
}
