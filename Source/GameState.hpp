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
#ifndef RAYMARCH_GAMESTATE_HPP
#define RAYMARCH_GAMESTATE_HPP

#include "Volume.hpp"

#include <array>
#include <vector>

namespace Raymarch {
    /// @brief  GameState holds the state of the game parameters.
    class GameState {
    public:
        /// @brief  Input keys.
        enum class KeyType {
            Up, Down, Left, Right
        };

        /// @brief  Input key states.
        enum KeyStateType {
            Press, Release
        };

    private:
        /// @brief  The offset of the visible scene in the map.
        std::array<int, 3> SceneOffset;

    private:
        /// @brief  The player position.
        std::array<float, 3> ScenePosition;

        /// @brief  The player velocity.
        std::array<float, 3> SceneVelocity;

    private:
        /// @brief  The global light position.
        std::array<float, 3> LightPosition;

    private:
        /// @brief  The camera position, the scene is rendered from this.
        std::array<float, 3> CameraPosition;

        /// @brief  The camera target, the camera points towards this.
        std::array<float, 3> CameraTarget;

    private:
        /// @brief  The near clip distance of the renderer.
        float NearClip;

        /// @brief  The field of view of the renderer.
        float FieldOfView;

    private:
        /// @brief  The maximum render distance before fog.
        float FogDistance;

        /// @brief  The fog colour of the renderer.
        std::array<float, 3> FogColour;

    private:
        /// @brief  An array of volumes to render at locations.
        std::vector<std::pair<std::array<int, 3>, Volume> > Map;

        /// @brief  The scene rendered by the renderer, constructed from the map.
        Volume Scene;

    public:
        /// @brief  Constructor to initialise member valiables based on the scene size.
        /// @param  SceneSize - The size of the scene that will be rendered.
        GameState(const std::array<std::size_t, 3>& SceneSize = {{64, 32, 64}});

    public:
        /// @brief  Clear the map.
        void ClearMap(void);

        /// @brief  Set the map.
        /// @param  Map - The new map which will overwrite the current map.
        void SetMap(const std::vector<std::pair<std::array<int, 3>, Volume> >& Map);

        /// @brief  Get the map.
        /// @return The current map.
        const std::vector<std::pair<std::array<int, 3>, Volume> >& GetMap(void) const;

        /// @brief  Add a volume model to the map at a position.
        /// @param  Position - The position at which to place the model.
        /// @param  Model - The volume storing the voxels of the model.
        void AddToMap(const std::array<int, 3>& Position, const Volume& Model);

    public:
        /// @brief  Get the scene offset.
        /// @return The current scene offset.
        const std::array<int, 3>& GetSceneOffset(void) const;

        /// @brief  Get the global light position.
        /// @return The current global light position.
        const std::array<float, 3>& GetLightPosition(void) const;

        /// @brief  Get the camera position.
        /// @return The current camera position.
        const std::array<float, 3>& GetCameraPosition(void) const;

        /// @brief  Get the camera target.
        /// @return The current camera target.
        const std::array<float, 3>& GetCameraTarget(void) const;

        /// @brief  Get the near clip distance of the renderer.
        /// @return The current near clip.
        float GetNearClip(void) const;

        /// @brief  The field of view of the renderer.
        /// @return The current field of view.
        float GetFieldOfView(void) const;

        /// @brief  The maximum render distance before fog.
        /// @return The current fog distance.
        float GetFogDistance(void) const;

        /// @brief  The fog colour of the renderer.
        /// @return The current fog colour.
        const std::array<float, 3>& GetFogColour(void) const;

        /// @brief  Get the scene volume to render.
        /// @return The current scene volume.
        const Volume& GetScene(void) const;

    public:
        /// @brief  Input key presses to the state.
        /// @param  Key - The input key.
        /// @param  State - The state of the key.
        void Input(KeyType Key, KeyStateType State);

        /// @brief  Update the state given a time step.
        /// @param  DeltaTime - The time since update was last called.
        void Update(float DeltaTime);
	};
}

#endif // RAYMARCH_GAMESTATE_HPP
