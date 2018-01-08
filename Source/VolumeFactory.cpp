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

#include "VolumeFactory.hpp"

#include <cassert>
#include <random>

namespace Raymarch {
    // Create a solid cuboid volume all set to the same voxel type.
    Volume VolumeFactory::CreateSolid(std::size_t SizeX, std::size_t SizeY, std::size_t SizeZ, Voxel Value) {
        // Allocate the volume.
        Raymarch::Volume Solid = Raymarch::Volume(SizeX, SizeY, SizeZ);

        // Fill the volume with the voxel type.
        Solid.Fill(Value);

        // Return.
        return Solid;
    }

    // Create a solid ellipsoid volume all set to the same voxel type.
    Volume VolumeFactory::CreateEllipsoid(std::size_t SizeX, std::size_t SizeY, std::size_t SizeZ, Voxel Value) {
        // Allocate the volume.
        Raymarch::Volume Ellipsoid = Raymarch::Volume(SizeX, SizeY, SizeZ);

        // Calculate the radius along each axis.
        const double RadiusX = static_cast<double>(SizeX) / 2.0;
        const double RadiusY = static_cast<double>(SizeY) / 2.0;
        const double RadiusZ = static_cast<double>(SizeZ) / 2.0;

        // Helper function to square values.
        auto Square = [](double Value) -> double { return Value * Value; };

        // Loop over all voxels of the volume and fill them if they are within the ellipsoid.
        for (std::size_t IndexX = 0; IndexX < SizeX; ++IndexX) {
            const double PartialX = Square((static_cast<double>(IndexX) - RadiusX) / RadiusX);
            for (std::size_t IndexY = 0; IndexY < SizeY; ++IndexY) {
                const double PartialY = Square((static_cast<double>(IndexY) - RadiusY) / RadiusY);
                for (std::size_t IndexZ = 0; IndexZ < SizeZ; ++IndexZ) {
                    const double PartialZ = Square((static_cast<double>(IndexZ) - RadiusZ) / RadiusZ);

                    // Evaluate the ellipsoid equation.
                    if (PartialX + PartialY + PartialZ < 1.0) {
                        Ellipsoid(IndexX, IndexY, IndexZ) = Value;
                    }
                }
            }
        }

        // Return.
        return Ellipsoid;
    }

    // Create a sponge cubeoid by randomly setting positions in the cuboid.
    Volume VolumeFactory::CreateRandomSponge(std::size_t SizeX, std::size_t SizeY, std::size_t SizeZ, double Density, Voxel Value) {
        assert(Density >= 0 && Density <= 1);

        // Allocate the volume.
        Raymarch::Volume Sponge = Raymarch::Volume(SizeX, SizeY, SizeZ);

        // Create a random generator.
        std::default_random_engine RandomGenerator;
        RandomGenerator.seed(std::random_device()());
        std::uniform_real_distribution<double> RandomDistribution(0, 1);

        // Loop over all voxels of the volume and fill them randomly.
        for (std::size_t IndexX = 0; IndexX < SizeX; ++IndexX) {
            for (std::size_t IndexY = 0; IndexY < SizeY; ++IndexY) {
                for (std::size_t IndexZ = 0; IndexZ < SizeZ; ++IndexZ) {
                    // Evalueate the random function.
                    if (RandomDistribution(RandomGenerator) < Density) {
                        Sponge(IndexX, IndexY, IndexZ) = Value;
                    }
                }
            }
        }

        // Return.
        return Sponge;
    }

    // Create a column volumn.
    Volume VolumeFactory::CreateColumn(std::size_t SizeX, std::size_t SizeY, std::size_t SizeZ, double Radius, Voxel Value) {
        assert(Radius > 0 && Radius < 1.0);

        // Allocate the volume.
        Raymarch::Volume Column = Raymarch::Volume(SizeX, SizeY, SizeZ);

        // Calculate the radius for the X and Z axis.
        const double RadiusX = static_cast<double>(SizeX) / 2.0;
        const double RadiusZ = static_cast<double>(SizeZ) / 2.0;

        // Helper function to square values.
        auto Square = [](double Value)->double { return Value * Value; };

        // Loop over all voxels of the volume and fill ones that are within the column.
        for (std::size_t IndexX = 0; IndexX < SizeX; ++IndexX) {
            const double PartialX = Square((static_cast<double>(IndexX) - RadiusX) / RadiusX);
            for (std::size_t IndexY = 0; IndexY < SizeY; ++IndexY) {
                for (std::size_t IndexZ = 0; IndexZ < SizeZ; ++IndexZ) {
                    // Fill the top and bottom layers completely.
                    if (IndexY == 0 || IndexY == SizeY-1) {
                        Column(IndexX, IndexY, IndexZ) = Value;
                        continue;
                    }
                    const double PartialZ = Square((static_cast<double>(IndexZ) - RadiusZ) / RadiusZ);
                    // Evaluate the X and Z circle for inclusion in the column.
                    if (PartialX + PartialZ < Radius) {
                        Column(IndexX, IndexY, IndexZ) = Value;
                    }
                }
            }
        }

        // Return.
        return Column;
    }
}
