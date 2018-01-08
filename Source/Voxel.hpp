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
#ifndef RAYMARCH_VOXEL_HPP
#define RAYMARCH_VOXEL_HPP

#include <cstdint>

namespace Raymarch {
    class Voxel {
    public:
        /// @brief  Unused (2 bits)
        std::uint8_t Saturation : 2;
        /// @brief  Transparancy (3 bits)
        std::uint8_t Alpha : 3;
        /// @brief  Tint from nearby light sources as RGB (3 bits)
        std::uint8_t Tint : 3;

        /// @brief  Colour: 11 hues + white + light grey + dark grey + black (4 bits)
        std::uint8_t Hue : 4;
        /// @brief  Light level, sum total from nearby sources (4 bits)
        std::uint8_t Light : 4;

        /// @brief  Material state: Gas / Liquid / Solid / Plasma (2 bits)
        std::uint8_t State : 2;
        /// @brief  Temperature (3 bits)
        std::uint8_t Temperature : 3;
        /// @brief  Direction: Outwards, N, W, E, S, Up, Down, Inwards (3 bits)
        std::uint8_t Direction : 3;

        /// @brief  Density (2 bits)
        std::uint8_t Density : 2;
        /// @brief  Strength (3 bits)
        std::uint8_t Strength : 3;
        /// @brief  Fill level, amount of material in the voxel (3 bits)
        std::uint8_t FillLevel : 3;

    public:
        /// @brief  Constructor that creates empty voxels.
        Voxel(void);

        /// @brief  Constructor that creates a coloured voxel from an RBGA colour.
        /// @param  R - Value for the red channel.
        /// @param  G - Value for the green channel.
        /// @param  B - Value for the blue channel.
        /// @param  A - Value for the alpha channel.
        Voxel(std::uint8_t R, std::uint8_t G, std::uint8_t B, std::uint8_t A = 255u);

    private:
        /// @brief  Function to convert RGB colour to a 4 bit Hue.
        /// @param  R - Value for the red channel.
        /// @param  G - Value for the green channel.
        /// @param  B - Value for the blue channel.
        /// @return Hue as a 4 bit value.
        std::uint8_t RGB2Hue(std::uint8_t R, std::uint8_t G, std::uint8_t B);
    };
}

#endif // RAYMARCH_VOXEL_HPP
