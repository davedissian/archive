// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "Game.h"

int main(int argc, char** argv)
{
    /*
    try
    {
        return Game::run();
    }
    catch (std::runtime_error& e)
    {
        MessageBoxA(NULL, e.what(), "Error", MB_ICONERROR | MB_OK);
    }*/
    return Game::run();
}

// Win32 Main
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    return main(__argc, __argv);
}