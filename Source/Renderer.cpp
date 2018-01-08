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

#include "Renderer.hpp"
#include "ShaderSource.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

// When debugging check for OpenGL errors after every usage.
#ifdef _DEBUG
    #define CHECK_GL(Expression) Expression; \
    { \
        GLenum EC; \
        while ((EC = glGetError()) != GL_NO_ERROR) { \
            std::cerr << "OpenGL error [" << EC << "] on line [" << __LINE__ << "]: " << gluErrorString(EC) << std::endl; \
            asm("int3"); \
        } \
    }
#else
    #define CHECK_GL(Expression) Expression;
#endif

namespace Raymarch {
    // Constructor that initialises the renderer at the provided size.
    Renderer::Renderer(std::size_t ScreenWidth, std::size_t ScreenHeight)
        : ScreenWidth(ScreenWidth)
        , ScreenHeight(ScreenHeight) {

        // Create the WebGL context.
        CHECK_GL(glViewport(0, 0, this->ScreenWidth, this->ScreenHeight));

        ///////////////////////////////////////////////////////////////////////////
        /// Create the shaders.                                                  //
        ///////////////////////////////////////////////////////////////////////////

        // Create the shader fragments.
        this->VertexShader = CHECK_GL(glCreateShader(GL_VERTEX_SHADER));
        const char* VertexShaderSource = ShaderSource::VertexShaderSource.c_str();
        CHECK_GL(glShaderSource(this->VertexShader, 1, &VertexShaderSource, nullptr));
        CHECK_GL(glCompileShader(this->VertexShader));

        this->FragmentShaderFXAA = CHECK_GL(glCreateShader(GL_FRAGMENT_SHADER));
        const char* FXAAShaderSource = ShaderSource::FragmentShaderSourceFXAA.c_str();
        CHECK_GL(glShaderSource(this->FragmentShaderFXAA, 1, &FXAAShaderSource, nullptr));
        CHECK_GL(glCompileShader(this->FragmentShaderFXAA));

        this->FragmentShaderVoxel = CHECK_GL(glCreateShader(GL_FRAGMENT_SHADER));
        const char* VoxelShaderSource = ShaderSource::FragmentShaderSourceVoxel.c_str();
        CHECK_GL(glShaderSource(this->FragmentShaderVoxel, 1, &VoxelShaderSource, nullptr));
        CHECK_GL(glCompileShader(this->FragmentShaderVoxel));

        // Set up the programs.
        this->ShaderProgramFXAA = CHECK_GL(glCreateProgram());
        CHECK_GL(glAttachShader(this->ShaderProgramFXAA, this->VertexShader));
        CHECK_GL(glAttachShader(this->ShaderProgramFXAA, this->FragmentShaderFXAA));
        CHECK_GL(glLinkProgram(this->ShaderProgramFXAA));

        this->ShaderProgramVoxel = CHECK_GL(glCreateProgram());
        CHECK_GL(glAttachShader(this->ShaderProgramVoxel, this->VertexShader));
        CHECK_GL(glAttachShader(this->ShaderProgramVoxel, this->FragmentShaderVoxel));
        CHECK_GL(glLinkProgram(this->ShaderProgramVoxel));

        // Catch any errors.
        GLint ErrorCode;
        CHECK_GL(glGetShaderiv(this->VertexShader, GL_COMPILE_STATUS, &ErrorCode));
        if (ErrorCode == GL_FALSE) {
            char InfoLogBuffer[1024];
            CHECK_GL(glGetShaderInfoLog(this->VertexShader, 1024, NULL, InfoLogBuffer));
            std::cerr << "The vertex shader failed to compile with the error:" << std::endl << InfoLogBuffer << std::endl;
        }

        CHECK_GL(glGetShaderiv(this->FragmentShaderFXAA, GL_COMPILE_STATUS, &ErrorCode));
        if (ErrorCode == GL_FALSE) {
            char InfoLogBuffer[1024];
            CHECK_GL(glGetShaderInfoLog(this->FragmentShaderFXAA, 1024, NULL, InfoLogBuffer));
            std::cerr << "The fxaa fragment shader failed to compile with the error:" << std::endl << InfoLogBuffer << std::endl;
        }

        CHECK_GL(glGetShaderiv(this->FragmentShaderVoxel, GL_COMPILE_STATUS, &ErrorCode));
        if (ErrorCode == GL_FALSE) {
            char InfoLogBuffer[1024];
            CHECK_GL(glGetShaderInfoLog(this->FragmentShaderVoxel, 1024, NULL, InfoLogBuffer));
            std::cerr << "The voxel fragment shader failed to compile with the error:" << std::endl << InfoLogBuffer << std::endl;
        }

        CHECK_GL(glGetProgramiv(this->ShaderProgramFXAA, GL_LINK_STATUS, &ErrorCode));
        if (ErrorCode == GL_FALSE) {
            char InfoLogBuffer[1024];
            CHECK_GL(glGetProgramInfoLog(this->ShaderProgramFXAA, 1024, NULL, InfoLogBuffer));
            std::cerr << "The fxaa shader program failed to compile with the error:" << std::endl << InfoLogBuffer << std::endl;
        }

        CHECK_GL(glGetProgramiv(this->ShaderProgramVoxel, GL_LINK_STATUS, &ErrorCode));
        if (ErrorCode == GL_FALSE) {
            char InfoLogBuffer[1024];
            CHECK_GL(glGetProgramInfoLog(this->ShaderProgramVoxel, 1024, NULL, InfoLogBuffer));
            std::cerr << "The voxel shader program failed to compile with the error:" << std::endl << InfoLogBuffer << std::endl;
        }

        ///////////////////////////////////////////////////////////////////////////
        /// Configure the FXAA program, uniforms, framebuffers, and texture.     //
        ///////////////////////////////////////////////////////////////////////////

        CHECK_GL(glUseProgram(this->ShaderProgramFXAA));

        // Calculate size of FXAA shader frambuffer.
        std::size_t FramebufferWidth = this->CeilPowerOfTwo(ScreenWidth);
        std::size_t FramebufferHeight = this->CeilPowerOfTwo(ScreenHeight);

        // Set resolution parameter of FXAA shader.
        const GLint ShaderUniformScreenResolution = CHECK_GL(glGetUniformLocation(this->ShaderProgramFXAA, "ScreenResolution"));
        const GLfloat ScreenResolution[2] = { static_cast<float>(FramebufferWidth), static_cast<float>(FramebufferHeight) };
        CHECK_GL(glUniform2fv(ShaderUniformScreenResolution, 1, ScreenResolution));

        // Create a framebuffer to store the intermediate rendered frame
        CHECK_GL(glCreateFramebuffers(1, &this->FrameBufferFXAA));
        CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, this->FrameBufferFXAA));

        // Create a framebuffer texture for FXAA.
        CHECK_GL(glCreateTextures(GL_TEXTURE_2D, 1, &this->TextureFXAA));
        CHECK_GL(glBindTexture(GL_TEXTURE_2D, this->TextureFXAA));
        CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, FramebufferWidth, FramebufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

        CHECK_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->TextureFXAA, 0));

        ///////////////////////////////////////////////////////////////////////////
        /// Create the Voxel program, uniforms, and texture.                     //
        ///////////////////////////////////////////////////////////////////////////

        CHECK_GL(glUseProgram(this->ShaderProgramVoxel));

        this->ShaderUniformScreenResolution      = CHECK_GL(glGetUniformLocation(this->ShaderProgramVoxel, "ScreenResolution"));
        this->ShaderUniformFramebufferResolution = CHECK_GL(glGetUniformLocation(this->ShaderProgramVoxel, "FramebufferResolution"));

        this->ShaderUniformOffset                = CHECK_GL(glGetUniformLocation(this->ShaderProgramVoxel, "SceneOffset"));

        this->ShaderUniformLightPosition         = CHECK_GL(glGetUniformLocation(this->ShaderProgramVoxel, "LightPosition"));

        this->ShaderUniformCameraPosition        = CHECK_GL(glGetUniformLocation(this->ShaderProgramVoxel, "CameraPosition"));
        this->ShaderUniformCameraTarget          = CHECK_GL(glGetUniformLocation(this->ShaderProgramVoxel, "CameraTarget"));

        this->ShaderUniformNearClip              = CHECK_GL(glGetUniformLocation(this->ShaderProgramVoxel, "NearClip"));
        this->ShaderUniformFieldOfView           = CHECK_GL(glGetUniformLocation(this->ShaderProgramVoxel, "FieldOfView"));

        this->ShaderUniformFogDistance           = CHECK_GL(glGetUniformLocation(this->ShaderProgramVoxel, "FogDistance"));
        this->ShaderUniformFogColour             = CHECK_GL(glGetUniformLocation(this->ShaderProgramVoxel, "FogColour"));

        this->ShaderUniformVolumeSize            = CHECK_GL(glGetUniformLocation(this->ShaderProgramVoxel, "VolumeSize"));

        // Clear the colour buffer.
        CHECK_GL(glClearColor(0, 0, 0, 1));
        CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));

        // Configure OpenGL.
        CHECK_GL(glDisable(GL_DEPTH_TEST));
        CHECK_GL(glDisable(GL_CULL_FACE));
        CHECK_GL(glFrontFace(GL_CW));

        // Create the volume texture.
        CHECK_GL(glCreateTextures(GL_TEXTURE_2D, 1, &this->TextureVoxel));
        CHECK_GL(glBindTexture(GL_TEXTURE_2D, this->TextureVoxel));
        CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        CHECK_GL(glActiveTexture(GL_TEXTURE0));

        // Set the sampler.
        const GLint ShaderUniformSampler = CHECK_GL(glGetUniformLocation(this->ShaderProgramVoxel, "BinarySampler"));
        CHECK_GL(glUniform1i(ShaderUniformSampler, 0));
    }

    // Round input to a power of two greater than or equal to the input value.
    std::size_t Renderer::CeilPowerOfTwo(std::size_t Value) {
        --Value;
        Value |= Value >> 1;
        Value |= Value >> 2;
        Value |= Value >> 4;
        Value |= Value >> 8;
        Value |= Value >> 16;
        return Value + 1;
    }

    void Renderer::Render(const GameState& State) {
        // Clear the colour buffer.
        CHECK_GL(glClearColor(State.GetFogColour()[0], State.GetFogColour()[1], State.GetFogColour()[2], 1));
        CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));

        // Render the volume to the framebuffer.
        CHECK_GL(glUseProgram(this->ShaderProgramVoxel));
        CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, this->FrameBufferFXAA));

        // Screen resolution.
        const GLfloat ScreenResolution[2] = { static_cast<float>(this->ScreenWidth), static_cast<float>(this->ScreenHeight) };
        CHECK_GL(glUniform2fv(this->ShaderUniformScreenResolution, 1, ScreenResolution));
        const GLfloat FramebufferResolution[2] = { static_cast<float>(this->CeilPowerOfTwo(State.GetScene().GetSizeX())), static_cast<float>(this->CeilPowerOfTwo(State.GetScene().GetSizeY() * State.GetScene().GetSizeZ())) };
        CHECK_GL(glUniform2fv(this->ShaderUniformFramebufferResolution, 1, FramebufferResolution));

        // Scene offset.
        const GLfloat SceneOffset[3] = {static_cast<float>(State.GetSceneOffset()[0]), static_cast<float>(State.GetSceneOffset()[1]), static_cast<float>(State.GetSceneOffset()[2])};
        CHECK_GL(glUniform3fv(this->ShaderUniformOffset, 1, SceneOffset));

        // Lighting.
        const GLfloat LightPosition[3] = { State.GetLightPosition()[0], State.GetLightPosition()[1], State.GetLightPosition()[2] };
        CHECK_GL(glUniform3fv(this->ShaderUniformLightPosition, 1, LightPosition));

        // Camera.
        const GLfloat CameraPosition[3] = { State.GetCameraPosition()[0], State.GetCameraPosition()[1], State.GetCameraPosition()[2] };
        CHECK_GL(glUniform3fv(this->ShaderUniformCameraPosition, 1, CameraPosition));
        const GLfloat CameraTarget[3] = { State.GetCameraTarget()[0], State.GetCameraTarget()[1], State.GetCameraTarget()[2] };
        CHECK_GL(glUniform3fv(this->ShaderUniformCameraTarget, 1, CameraTarget));

        // Perspective.
        CHECK_GL(glUniform1f(this->ShaderUniformNearClip, State.GetNearClip()));
        CHECK_GL(glUniform1f(this->ShaderUniformFieldOfView, State.GetFieldOfView()));

        // Fog.
        CHECK_GL(glUniform1f(this->ShaderUniformFogDistance, State.GetFogDistance()));
        const GLfloat FogColour[4] = { State.GetFogColour()[0], State.GetFogColour()[1], State.GetFogColour()[2], 1 };
        CHECK_GL(glUniform4fv(this->ShaderUniformFogColour, 1, FogColour));

        // Volume size.
        const GLfloat VolumeSize[3] = { static_cast<float>(State.GetScene().GetSizeX()), static_cast<float>(State.GetScene().GetSizeY()), static_cast<float>(State.GetScene().GetSizeZ()) };
        CHECK_GL(glUniform3fv(this->ShaderUniformVolumeSize, 1, VolumeSize));

        // Volume texture.
        CHECK_GL(glBindTexture(GL_TEXTURE_2D, this->TextureVoxel));

        // Volume sampler.
        const GLint ShaderUniformBinarySampler = CHECK_GL(glGetUniformLocation(this->ShaderProgramVoxel, "BinarySampler"));
        CHECK_GL(glUniform1i(ShaderUniformBinarySampler, 0));

        // Upload
        CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, State.GetScene().GetSizeX(), State.GetScene().GetSizeY() * State.GetScene().GetSizeZ(), 0, GL_RED_INTEGER, GL_UNSIGNED_INT, State.GetScene().data()));
        CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 3));

        // Apply FXAA
        CHECK_GL(glUseProgram(this->ShaderProgramFXAA));
        CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        CHECK_GL(glBindTexture(GL_TEXTURE_2D, this->TextureFXAA));
        const GLint ShaderUniformSampler = CHECK_GL(glGetUniformLocation(this->ShaderProgramFXAA, "Sampler"));
        CHECK_GL(glUniform1i(ShaderUniformSampler, 0));
        CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, 3));
    }
}
