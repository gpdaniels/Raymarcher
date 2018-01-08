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

#include "Voxel.hpp"

#include <algorithm>

namespace Raymarch {
    // Constructor for empty voxels.
    Voxel::Voxel(void) {
        this->Saturation = 0;
        this->Alpha = 0;
        this->Tint = 0;
        this->Hue = 0;
        this->Light = 0;
        this->State = 0;
        this->Temperature = 0;
        this->Direction = 0;
        this->Density = 0;
        this->Strength = 0;
        this->FillLevel = 0;
    }

    // Constructor for solid voxels created from an RGBA colour.
    Voxel::Voxel(std::uint8_t R, std::uint8_t G, std::uint8_t B, std::uint8_t A) {
        this->Saturation = 3;
        this->Alpha = A >> 5;
        this->Tint = 0;
        this->Hue = RGB2Hue(R, G, B);
        this->Light = 0b1000;
        this->State = 0;
        this->Temperature = 0;
        this->Direction = 0;
        this->Density = 0;
        this->Strength = 0;
        this->FillLevel = 0;
    }

    // Helper function to convert an RGB colour to a hue.
    std::uint8_t Voxel::RGB2Hue(std::uint8_t R, std::uint8_t G, std::uint8_t B) {
        std::uint8_t Max = std::max(R, std::max(G, B));
        std::uint8_t Min = std::min(R, std::min(G, B));
        float Result;
        if (Max == Min) {
            // Todo: Store in < 4
            Result = Max / 85;
            return static_cast<std::uint8_t>(Result);
        }
        if (Max == R) {
            Result = (((float)G - (float)B) / ((float)Max - (float)Min) + (G < B ? 6.0f : 0)) / 6.0f;
        }
        if (Max == G) {
            Result = (((float)B - (float)R) / ((float)Max - (float)Min) + 2.0f) / 6.0f;
        }
        if (Max == B) {
            Result = (((float)R - (float)G) / ((float)Max - (float)Min) + 4.0f) / 6.0f;
        }
        Result = std::round(Result * 11.0f) + 4;
        return static_cast<std::uint8_t>(Result);
    }
}
