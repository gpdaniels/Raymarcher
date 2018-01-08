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

#include "GameState.hpp"

#include <algorithm>

namespace Raymarch {
    // Constructor that initialises all member variables with workable defaults.
    GameState::GameState(const std::array<std::size_t, 3>& SceneSize) {
        // Offset of the visible scene in the map.
        this->SceneOffset = {static_cast<int>(SceneSize[0]) / 2, 0, static_cast<int>(SceneSize[2]) / 2};

        // Floating point position.
        this->ScenePosition = {static_cast<float>(SceneSize[0]) / 2.0f, 0.0f, static_cast<float>(SceneSize[2]) / 2.0f};

        // Floating point speed.
        this->SceneVelocity = {0, 0, 0};

        // Position of the sun / global light source.
        this->LightPosition  = {0, 1024, 0};

        // Position of the camera.
        this->CameraPosition = {static_cast<float>(SceneSize[0]) / 2.0f, static_cast<float>(SceneSize[1]) / 2.0f, 0.0f};

        // Target of the camera.
        this->CameraTarget   = {static_cast<float>(SceneSize[0]) / 2.0f, 0.0f, static_cast<float>(SceneSize[2]) / 2.0f};

        // Near clip of the renderer.
        this->NearClip = 0.01;

        // Field of view of the renderer.
        this->FieldOfView = 75.0;

        // Fog distance.
        this->FogDistance = 256;

        // Colour of the fog.
        this->FogColour = { 0.5, 0.5, 0.5 };

        // The rendered scene volume, the map is unioned into this before rendering.
        this->Scene = Volume(SceneSize);
    }

    // Get the scene offset, the renderer shader applies noise based on position.
    const std::array<int, 3>& GameState::GetSceneOffset(void) const {
        return this->SceneOffset;
    }

    // Get the light position, the renderer shader applies shadows based on this position.
    const std::array<float, 3>& GameState::GetLightPosition(void) const {
        return this->LightPosition;
    }

    // Get the camera position, the renderer shader renders from this position.
    const std::array<float, 3>& GameState::GetCameraPosition(void) const {
        return this->CameraPosition;
    }

    // Get the camera target, the renderer shader renders looking at this position.
    const std::array<float, 3>& GameState::GetCameraTarget(void) const {
        return this->CameraTarget;
    }

    // Get the near clip distance, the renderer shader only renders objects further away than this.
    float GameState::GetNearClip(void) const {
        return this->NearClip;
    }

    // Get the field of view, the renderer shader projects this field of view onto the window.
    float GameState::GetFieldOfView(void) const {
        return this->FieldOfView;
    }

    // Get the fog distance, the renderer shader renders the fog at this distance.
    float GameState::GetFogDistance(void) const {
        return this->FogDistance;
    }

    // Get the fog colour, the renderer shader colours the distant fog with this colour.
    const std::array<float, 3>& GameState::GetFogColour(void) const {
        return this->FogColour;
    }

    // Get the scene volume, the renderer shader renders this data.
    const Volume& GameState::GetScene(void) const {
        return this->Scene;
    }

    // Clear all models from the map.
    void GameState::ClearMap(void) {
        this->Map.clear();
    }

    // Set a map.
    void GameState::SetMap(const std::vector<std::pair<std::array<int, 3>, Volume> >& Map) {
        this->Map = Map;
    }

    // Get the map.
    const std::vector<std::pair<std::array<int, 3>, Volume> >& GameState::GetMap(void) const {
        return this->Map;
    }

    // Add a model to the map at a position.
    void GameState::AddToMap(const std::array<int, 3>& Position, const Volume& Model) {
        this->Map.push_back(std::make_pair(Position, Model));
    }

    // Apply a key press to the game state.
    void GameState::Input(KeyType Key, KeyStateType State) {
        switch (Key) {
            case KeyType::Up: {
                if      (State == KeyStateType::Press)   this->SceneVelocity[2] = +10.0;
                else if (State == KeyStateType::Release) this->SceneVelocity[2] =  +0.0;
            } break;
            case KeyType::Down: {
                if      (State == KeyStateType::Press)   this->SceneVelocity[2] = -10.0;
                else if (State == KeyStateType::Release) this->SceneVelocity[2] =  +0.0;
            } break;
            case KeyType::Right: {
                if      (State == KeyStateType::Press)   this->SceneVelocity[0] = +10.0;
                else if (State == KeyStateType::Release) this->SceneVelocity[0] =  +0.0;
            } break;
            case KeyType::Left: {
                if      (State == KeyStateType::Press)   this->SceneVelocity[0] = -10.0;
                else if (State == KeyStateType::Release) this->SceneVelocity[0] =  +0.0;
            } break;
        }
    }

    // Update the game state after a time.
    void GameState::Update(float DeltaTime) {
        // Move player.
        for (std::size_t Index = 0; Index < 3; ++Index) {
            this->ScenePosition[Index] += this->SceneVelocity[Index] * DeltaTime;
        }

        // Update scene offset from position.
        for (std::size_t Index = 0; Index < 3; ++Index) {
            this->SceneOffset[Index] = std::round(this->ScenePosition[Index]);
        }

        // Move light around the flat world.
        auto Square = [](float Value) -> float { return Value * Value; };
        float LightDistance = std::sqrt(Square(this->LightPosition[0]) + Square(this->LightPosition[1]));
        float LightAngle = std::atan2(this->LightPosition[1], this->LightPosition[0]);
        LightAngle += 2.0f * M_PI * (DeltaTime / 60.0);
        this->LightPosition[0] = std::cos(LightAngle) * LightDistance;
        this->LightPosition[1] = std::sin(LightAngle) * LightDistance;
        this->LightPosition[2] = this->LightPosition[0];

        // Update fog colour.
        const float NewFogColour = 0.8 - 0.7 * std::abs((std::atan2(this->LightPosition[0], this->LightPosition[1]) / M_PI));
        for (std::size_t Index = 0; Index < 3; ++Index) {
            this->FogColour[Index] = NewFogColour;
        }

        // Clear current scene.
        this->Scene.Clear();

        // Add model data to the scene.
        for (const std::pair<std::array<int, 3>, Volume>& PositionModelPair : this->Map) {
            const std::array<int, 3>& Position = PositionModelPair.first;
            const Volume& Model = PositionModelPair.second;
            this->Scene.Insert(Position[0] - this->SceneOffset[0], Position[1] - this->SceneOffset[1], Position[2] - this->SceneOffset[2], Model);
        }
    }
}
