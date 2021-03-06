/*
 * GL Framework
 * Copyright (c) David Avedissian 2014-2015
 */
#include "Common.h"
#include "Application.h"

Application::Application() : mWindow(nullptr), mWindowWidth(0), mWindowHeight(0)
{
}

Application::~Application()
{
    if (mWindow != nullptr)
        DestroyWindow();
}

int Application::CreateWindow(const string& windowTitle, uint width, uint height)
{
    INFO << "Starting " << windowTitle << endl;

    // Create a window
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        PrintSDLError();
        return 1;
    }

    // Set up GL context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create the window
    mWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (mWindow == nullptr)
    {
        PrintSDLError();
        return 1;
	}

	// Now the window exists without errors, update the member variables
	mWindowWidth = width;
	mWindowHeight = height;

    // Create the GL context
    mContext = SDL_GL_CreateContext(mWindow);
    SDL_GL_SetSwapInterval(1);

    // Initialise gl3w
    if (gl3wInit())
    {
        ERROR << "Failed to initialise OpenGL" << endl;
        return 1;
    }

    // Get version info
    INFO << "Renderer: " << glGetString(GL_RENDERER) << endl;
    INFO << "OpenGL " << glGetString(GL_VERSION)
         << ", GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    // Everything is ok
    return 0;
}

void Application::DestroyWindow()
{
    assert(mWindow != nullptr);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

int Application::Run(const string& windowTitle, uint width, uint height)
{
	try
	{
		if (CreateWindow(windowTitle, width, height) != 0)
			return 1;
		Startup();

		// Main loop
		SDL_Event e;
		bool quit = false;
		while (!quit)
		{
			// Handle message pump
			while (SDL_PollEvent(&e) != 0)
			{
				switch (e.type)
				{
				case SDL_QUIT:
					quit = true;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    OnMouseDown(e.button.button);
                    break;

                case SDL_MOUSEBUTTONUP:
                    OnMouseUp(e.button.button);
                    break;

                case SDL_MOUSEMOTION:
                    OnMouseMove(glm::vec2(e.motion.xrel, e.motion.yrel));
                    break;

				case SDL_KEYDOWN:
					OnKeyDown(e.key.keysym.sym);
					break;

                case SDL_KEYUP:
                    OnKeyUp(e.key.keysym.sym);
                    break;

				default:
					break;
				}
			}

            // Run one tick of the game logic
            if (!Update(1.0f / 60.0f))
                quit = true;

			// Render a frame
			Render();

			// Swap the front and back buffer
			SDL_GL_SwapWindow(mWindow);
		}

		Shutdown();
		DestroyWindow();
		return 0;
	}
	catch (std::exception& e)
	{
        ERROR << "Runtime Exception: " << e.what() << endl;
		SDL_ShowSimpleMessageBox(0, "Runtime Error", e.what(), NULL);
		return 1;
	}
}

void Application::OnMouseDown(int button)
{
    mCameraMan.OnMouseDown(button);
}

void Application::OnMouseUp(int button)
{
    mCameraMan.OnMouseUp(button);
}

void Application::OnMouseMove(const glm::vec2& offset)
{
    mCameraMan.OnMouseMove(offset);
}

void Application::OnKeyDown(SDL_Keycode kc)
{
    mCameraMan.OnKeyDown(kc);
}

void Application::OnKeyUp(SDL_Keycode kc)
{
    mCameraMan.OnKeyUp(kc);
}

void Application::PrintSDLError()
{
	ERROR << "SDL Error: " << SDL_GetError() << endl;
}
