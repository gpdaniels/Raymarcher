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
#ifndef RAYMARCH_RENDERER_HPP
#define RAYMARCH_RENDERER_HPP

#include "Volume.hpp"

#include "GameState.hpp"

#include <GL/glew.h>

#include <array>

namespace Raymarch {
	class Renderer {
	private:
        /// @brief  The width of the OpenGL viewport.
        std::size_t ScreenWidth;

        /// @brief  The height of the OpenGL viewport.
        std::size_t ScreenHeight;

	private:
        /// @brief  The renderer vertex shader.
        GLuint VertexShader;

        /// @brief  The renderer fragment shader to perform Fast Approximate Anti-Aliasing (FXAA).
        GLuint FragmentShaderFXAA;

        /// @brief  The renderer fragment shader to raymarch the voxel volume.
        GLuint FragmentShaderVoxel;

        /// @brief  The combined vertex and fragment shaders for FXAA.
        GLuint ShaderProgramFXAA;

        /// @brief  The combined vertex and fragment shaders for volume renderers.
        GLuint ShaderProgramVoxel;

        /// @brief  The framebuffer used to store the intermediate render of the voxels.
        GLuint FrameBufferFXAA;

        /// @brief  The texture used to access the intermediate FXAA framebuffer.
        GLuint TextureFXAA;

        /// @brief  The volume texture storing the voxel data.
        GLuint TextureVoxel;

    private:
        GLint ShaderUniformScreenResolution;

        GLint ShaderUniformOffset;
        GLint ShaderUniformLightPosition;
        GLint ShaderUniformCameraPosition;
        GLint ShaderUniformCameraTarget;

        GLint ShaderUniformNearClip;
        GLint ShaderUniformFieldOfView;
        GLint ShaderUniformFogColour;
        GLint ShaderUniformFogDistance;

        GLint ShaderUniformFramebufferResolution;
        GLint ShaderUniformVolumeSize;

	public:
        /// @brief  Constructor that specifies the size of the renderer viewport.
        Renderer(std::size_t ScreenWidth, std::size_t ScreenHeight);

    private:
        /// @brief  Round input to a power of two greater than or equal to the input value.
        /// @param  Value - The input to round.
        /// @return A power of two greater than or equal to the input value.
        std::size_t CeilPowerOfTwo(std::size_t Value);

    public:
        /// @brief  Render the gamestate to the current OpenGL window.
        /// @param  State - the state of the game.
        void Render(const GameState& State);
	};
}

#endif // RAYMARCH_RENDERER_HPP
