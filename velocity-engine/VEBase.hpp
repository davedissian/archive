/*
	Velocity Engine
	By SerialVelocity
*/
#pragma once

// Macros
#define VE_0_2_0
#define VE_VERSION "0.2.0"
#define VE_BEGIN namespace VE {
#define VE_END }
#define VE_RESCACHE_SIZE 2048
#define SAFE_RELEASE( x ) if( x ) { (x)->Release(); (x) = 0; }
#define SAFE_DELETE( x ) if( x ) { delete (x); (x) = 0; }
#ifndef _DEGUG
#define LOGWRITE( x ) { SYSTEMTIME time; GetSystemTime( &time ); std::string append; if( time.wHour < 10 ) append = "0"; else append = ""; std::cout << append << time.wHour << ":"; \
if( time.wMinute < 10 ) append = "0"; else append = ""; std::cout << append << time.wMinute << ":"; \
if( time.wSecond < 10 ) append = "0"; else append = ""; std::cout << append << time.wSecond << " > " << x << std::endl; }
#else
#define LOGWRITE( x )
#endif
#define VE_DIR_MODELS "models/"
#define VE_DIR_SHADERS "shaders/"
#define VE_DIR_SCRIPTS "scripts/"
#define VE_DIR_TEXTURES "textures/"
#define VE_WDIR_MODELS L"models/"
#define VE_WDIR_SHADERS L"shaders/"
#define VE_WDIR_SCRIPTS L"scripts/"
#define VE_WDIR_TEXTURES L"textures/"

// Support Headers
#include <iostream>
#include <fstream>
#include <sstream>

#include <list>
#include <vector>
#include <queue>
#include <map>
#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

VE_BEGIN

// Global Variables
extern class Engine *gEngine;
extern class EventDispatcher *gEventDispatcher;
extern class ResCache *gResCache;
extern class ScreenManager *gScreenManager;

VE_END

// Maths
#include "support\CMath.hpp"

// Globals
#include "VEEventDispatcher.hpp"
#include "VEResCache.hpp"
#include "VEScreen.hpp"
#include "VEScreenManager.hpp"

// Enable the console when in debug mode
#ifndef _DEBUG
#pragma comment( linker, "/subsystem:windows /ENTRY:mainCRTStartup" )
#else
#pragma comment( linker, "/subsystem:console" )
#endif
