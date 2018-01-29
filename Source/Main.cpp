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
#include "Volume.hpp"
#include "VolumeFactory.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <iostream>
#include <random>

// The main entry point.
int main(int ArgumentCount, char* ArgumentArray[]) {
    // Cast the main arguments to void to avoid unused variable warnings.
    static_cast<void>(ArgumentCount);
    static_cast<void>(ArgumentArray);

    // Store the project name for use when printing output.
    constexpr static const char* ProjectName = "Raymarch";

    // Print a header describing the project.
    std::cout << "Project:  " << ProjectName << std::endl;
    std::cout << "Author:   " << "Geoffrey Daniels" << std::endl;
    std::cout << "Contact:  " << "http://gpdaniels.com/" << std::endl;
    std::cout << "Build:    " <<  __DATE__ << " @ " << __TIME__ << std::endl;
    std::cout << "----------" << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    /// Initialise the GLFW.                                                 //
    ///////////////////////////////////////////////////////////////////////////

    std::cout << "Initialising the Graphics Library Framework (GLFW)..." << std::endl;

    if (glfwInit() != GL_TRUE) {
        std::cerr << "Failed to initialize the GLFW." << std::endl;
        return EXIT_FAILURE;
	}

    glfwSetErrorCallback([](int Code, const char* Description)->void{ std::cerr << "GLFW Error [" << Code << "]: " << Description << std::endl; });

    std::cout << "Finished initialising the GLFW." << std::endl;
    std::cout << "----------" << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    /// Create a window and OpenGL context.                                  //
    ///////////////////////////////////////////////////////////////////////////

    std::cout << "Creating a window using the GLFW..." << std::endl;

    constexpr static const int ScreenWidth  = 640;
    constexpr static const int ScreenHeight = 480;

    std::cout << "  Window size: " << ScreenWidth << "x" << ScreenHeight << "." << std::endl;

    // One sample per pixel
    glfwWindowHint(GLFW_SAMPLES, 1);

    // We want OpenGL 3.3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Forward compatible.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // We don't want the old OpenGL.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::cout << "  Creating window..." << std::endl;

    // Create a window and create its OpenGL context.
    GLFWwindow* WindowHandle = glfwCreateWindow(ScreenWidth, ScreenHeight, ProjectName, nullptr, nullptr);
    if (WindowHandle == nullptr) {
        std::cerr << "Failed to create the GLFW window." << std::endl;
		glfwTerminate();
        return EXIT_FAILURE;
	}

    std::cout << "  Making OpenGL context current..." << std::endl;

    // Ensure OpenGL context for this window is current.
    glfwMakeContextCurrent(WindowHandle);

    std::cout << "  Disabling V-Sync..." << std::endl;

    // Disable V-Sync to raymatch frames as fast as possible.
    glfwSwapInterval(0);

    std::cout << "  Configuring input mode..." << std::endl;

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(WindowHandle, GLFW_STICKY_KEYS, GL_TRUE);

    std::cout << "Finished creating the GLFW window." << std::endl;
    std::cout << "----------" << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    /// Initialise GLEW.                                                     //
    ///////////////////////////////////////////////////////////////////////////

    // Initialize GLEW
    std::cout << "Initialising the OpenGL Extension Wrangler (GLEW)..." << std::endl;

    // Magic experimental flag that is apparently needed in a core profile.
    glewExperimental = true;

    // Initialise the GLEW so that OpenGL extension functions can be called.
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Finished initialising the GLEW." << std::endl;
    std::cout << "----------" << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    /// Cleanup any OpenGL Errors thus far.                                  //
    ///////////////////////////////////////////////////////////////////////////

    std::cout << "Configuring OpenGL..." << std::endl;

    std::cout << "  Clearing old errors..." << std::endl;

    // Clear any errors thus far.
    while (glGetError() != GL_NO_ERROR);

    std::cout << "  Creating required (unused) vertex array..." << std::endl;

    // Have to have a VertexArray
    GLuint VA;
    glGenVertexArrays(1, &VA);
    assert(glGetError() == GL_NO_ERROR);

    glBindVertexArray(VA);
    assert(glGetError() == GL_NO_ERROR);

    std::cout << "Finished configuring OpenGL." << std::endl;
    std::cout << "----------" << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    /// Create an environment.                                               //
    ///////////////////////////////////////////////////////////////////////////

    std::cout << "Creating an environment..." << std::endl;

    std::cout << "  Creating a game state..." << std::endl;

    Raymarch::GameState State(std::array<std::size_t, 3>{{128, 32, 128}});

    std::cout << "  Creating a floor volume..." << std::endl;

    // Build the floor
    Raymarch::Voxel FloorVoxel = Raymarch::Voxel(128, 128, 128, 255);
    Raymarch::Volume Floor = Raymarch::VolumeFactory::CreateSolid(512, 1, 512, FloorVoxel);
    State.AddToMap({{0, 0, 0}}, Floor);

    std::cout << "  Creating a grass volume..." << std::endl;

	// Build the grass brownie.
    Raymarch::Voxel GrassVoxel = Raymarch::Voxel(0, 255, 0, 255);
    Raymarch::Volume Grass = Raymarch::VolumeFactory::CreateRandomSponge(512, 3, 512, 0.5, GrassVoxel);
    State.AddToMap({{0, 1, 0}}, Grass);

    std::cout << "  Creating a sphere volume..." << std::endl;

    Raymarch::Voxel SphereVoxel = Raymarch::Voxel(255, 0, 0, 255);
    Raymarch::Volume Sphere = Raymarch::VolumeFactory::CreateEllipsoid(16, 16, 16, SphereVoxel);
    State.AddToMap({{64, 8, 64}}, Sphere);

    std::cout << "  Creating a column volume..." << std::endl;

    Raymarch::Voxel ColumnVoxel = Raymarch::Voxel(0, 0, 128, 32);
    Raymarch::Volume Column = Raymarch::VolumeFactory::CreateColumn(16, 30, 16, 0.3, ColumnVoxel);

    std::cout << "  Creating random locations for 100 columns..." << std::endl;

    std::default_random_engine RandomGenerator;
    RandomGenerator.seed(std::random_device()());
    std::uniform_real_distribution<double> RandomDistribution(0, 1);

	// Generate random positions for 100 columns.
	for (int i = 0; i < 100; i++) {
        int x = std::floor(RandomDistribution(RandomGenerator) * 32) * 16;
        int z = std::floor(RandomDistribution(RandomGenerator) * 32) * 16;
        State.AddToMap({{x, 1, z}}, Column);
	}

    std::cout << "  Creating some coloured block volumes..." << std::endl;

    Raymarch::Voxel BlockVoxelRed   = Raymarch::Voxel(255,   0,   0, 64);
    Raymarch::Voxel BlockVoxelGreen = Raymarch::Voxel(  0, 255,   0, 64);
    Raymarch::Voxel BlockVoxelBlue  = Raymarch::Voxel(  0,   0, 255, 64);
    Raymarch::Voxel BlockVoxelBlack = Raymarch::Voxel(  0,   0,   0, 64);
    Raymarch::Voxel BlockVoxelGrey  = Raymarch::Voxel(128, 128, 128, 64);
    Raymarch::Voxel BlockVoxelWhite = Raymarch::Voxel(255, 255, 255, 64);
    Raymarch::Volume BlockRed   = Raymarch::VolumeFactory::CreateSolid(4, 8, 8, BlockVoxelRed);
    Raymarch::Volume BlockGreen = Raymarch::VolumeFactory::CreateSolid(4, 8, 8, BlockVoxelGreen);
    Raymarch::Volume BlockBlue  = Raymarch::VolumeFactory::CreateSolid(4, 8, 8, BlockVoxelBlue);
    Raymarch::Volume BlockBlack = Raymarch::VolumeFactory::CreateSolid(4, 8, 8, BlockVoxelBlack);
    Raymarch::Volume BlockGrey  = Raymarch::VolumeFactory::CreateSolid(4, 8, 8, BlockVoxelGrey);
    Raymarch::Volume BlockWhite = Raymarch::VolumeFactory::CreateSolid(4, 8, 8, BlockVoxelWhite);
    State.AddToMap({{ 8 * 2 + 80, 8, 64}}, BlockRed  );
    State.AddToMap({{12 * 2 + 80, 8, 64}}, BlockGreen);
    State.AddToMap({{16 * 2 + 80, 8, 64}}, BlockBlue );
    State.AddToMap({{20 * 2 + 80, 8, 64}}, BlockBlack);
    State.AddToMap({{24 * 2 + 80, 8, 64}}, BlockGrey );
    State.AddToMap({{28 * 2 + 80, 8, 64}}, BlockWhite);

    std::cout << "Finished creating an environment." << std::endl;
    std::cout << "----------" << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    /// Create a renderer.                                                   //
    ///////////////////////////////////////////////////////////////////////////

    std::cout << "Creating a renderer..." << std::endl;

    Raymarch::Renderer Renderer(ScreenWidth, ScreenHeight);

    std::cout << "Finished creating a renderer." << std::endl;
    std::cout << "----------" << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    /// Attach the keyboard callback.                                        //
    ///////////////////////////////////////////////////////////////////////////

    std::cout << "Attaching the keyboard callback..." << std::endl;

    // To handle key presses during the rendering loop the gamestate is set as a user pointer in GLFW.
    glfwSetWindowUserPointer(WindowHandle, &State);

    // When "glfwPollEvents()" is called GLFW calls this callback which in turn sends input to the state.
    glfwSetKeyCallback(WindowHandle, [](GLFWwindow* WindowHandle, int Key, int ScanCode, int Action, int Mode){
        // Unused parameters.
        static_cast<void>(ScanCode);
        static_cast<void>(Mode);
        // Convert the GLFW key into the gamestate type.
        Raymarch::GameState::KeyType ConvertedKey;
        switch (Key) {
            default: return;
            case GLFW_KEY_UP: ConvertedKey = Raymarch::GameState::KeyType::Up; break;
            case GLFW_KEY_DOWN: ConvertedKey = Raymarch::GameState::KeyType::Down; break;
            case GLFW_KEY_LEFT: ConvertedKey = Raymarch::GameState::KeyType::Left; break;
            case GLFW_KEY_RIGHT: ConvertedKey = Raymarch::GameState::KeyType::Right; break;
        }
        // Convert the GLFW action into the gamestate type.
        Raymarch::GameState::KeyStateType ConvertedAction;
        switch (Action) {
            default: return;
            case GLFW_PRESS: ConvertedAction = Raymarch::GameState::KeyStateType::Press; break;
            case GLFW_RELEASE: ConvertedAction = Raymarch::GameState::KeyStateType::Release; break;
        }
        // Get the stored gamestate pointer.
        Raymarch::GameState& State = *static_cast<Raymarch::GameState*>(glfwGetWindowUserPointer(WindowHandle));
        // Input the pressed key to the gamestate.
        State.Input(ConvertedKey, ConvertedAction);
    });

    std::cout << "Finished attaching the keyboard callback." << std::endl;
    std::cout << "----------" << std::endl;

    ///////////////////////////////////////////////////////////////////////////
    /// Start the rendering loop.                                            //
    ///////////////////////////////////////////////////////////////////////////

    std::cout << "Starting the rendering loop..." << std::endl;

    // Run the update and render loop until the escape key is pressed or the window is closed.
    while ((glfwGetKey(WindowHandle, GLFW_KEY_ESCAPE) != GLFW_PRESS) && (glfwWindowShouldClose(WindowHandle) == 0)) {
        // Delta time
        static float LastFrameTime = static_cast<float>(glfwGetTime());
        float ThisFrameTime = static_cast<float>(glfwGetTime());
        float DeltaTime = ThisFrameTime - LastFrameTime;
        LastFrameTime = ThisFrameTime;

        #if 1
        {
            // Measure and output FPS.
            static std::size_t FrameCount = 0; ++FrameCount;
            static float LastFPSTime = static_cast<float>(glfwGetTime());
            if (ThisFrameTime - LastFPSTime >= 1.0) {
                std::cout << "  FPS: " << static_cast<std::size_t>(std::round(static_cast<float>(FrameCount) / (ThisFrameTime - LastFPSTime))) << std::endl;
                FrameCount = 0;
                LastFPSTime = ThisFrameTime;
            }
        }
        #endif

        //#define AVOID_OLD_GLFWBUG 1
        #ifdef AVOID_OLD_GLFWBUG
            // Call this 100 times to try and avoid a GLFW bug and prevent duplicate key presses.
            // This is only required if the FPS of the renderer is low ( < 30ish).
            // The bug has been fixed in modern versions of GLFW. ( https://github.com/glfw/glfw/issues/747 )
            for (std::size_t I = 0; I < 100; ++I) {
                // Poll for events.
                glfwPollEvents();
            }
        #else
            // Poll for events.
            glfwPollEvents();
        #endif

        // Update state.
        State.Update(DeltaTime);

        // Draw state scene, the scene is already linked by reference to the renderer.
        Renderer.Render(State);

        // Swap buffers.
        glfwSwapBuffers(WindowHandle);
    }

    std::cout << "Finished the rendering loop." << std::endl;
    std::cout << "----------" << std::endl;

    // Return a successful exit status.
    return EXIT_SUCCESS;
}
