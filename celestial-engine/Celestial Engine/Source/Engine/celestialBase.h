/*
	Celestial Engine
	Created by SerialVelocity
*/
#pragma once

/*
	External Dependencies
*/

// STL
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <list>
using namespace std;

// Windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/*
	Basic Macros
*/

// Debug
#if defined( DEBUG ) || defined( _DEBUG )
#define ceDEBUG
#endif

// Namespace Block
#define ceBEGIN namespace celestial {
#define ceEND }

/*
	Console and Logging
*/

// Display the console in debug more only
#ifdef ceDEBUG
#pragma comment( linker, "/subsystem:console" )
#define ceCONSOLE_ACTIVE
#else
#pragma comment( linker, "/subsystem:windows /ENTRY:mainCRTStartup" )
#endif

// Useful macros
#define ceSAFE_DELETE( x ) if( x ) { delete x; x = 0; }
#define ceSAFE_DELETE_ARRAY( x ) if( x ) { delete [] x; x = 0; }
#define ceSAFE_RELEASE( x ) if( x ) { x->Release(); x = 0; }

// Logging
#ifdef ceDEBUG
#define ceLOGWRITE( msg ) { SYSTEMTIME st; GetSystemTime( &st );  if( st.wHour > 9 ) { cout << st.wHour << ":"; } else { cout << "0" << st.wHour << ":"; } if( st.wMinute > 9 ) { cout << st.wMinute << ":"; } else { cout << "0" << st.wMinute << ":"; } if( st.wSecond > 9 ) { cout << st.wSecond; } else { cout << "0" << st.wSecond; } cout << " > " << msg << endl; }
#define ceLOGERROR( error ) { ceLOGWRITE( "Error: " << error ); ceLOGWRITE( "- line: " << __LINE__ ); ceLOGWRITE( "- file: " << __FILE__ ); }
#define ceSSLOGWRITE( msg ) ceLOGWRITE( ceSUBSYSTEM << ": " << msg );
#else
#define ceLOGWRITE( x )
#define ceLOGERROR( x )
#define ceSSLOGWRITE( x )
#endif

// Deprecation
#define CE_DEPRECATED __declspec( deprecated )

// Safe memory managment
#define CE_SAFE_DELETE( x ) if( x ) { delete x; x = 0; }

// Define the assert macro
#ifdef ceDEBUG
#define ceASSERT( expression ) if( !( expression ) ) { ceLOGWRITE( "Error: Assert Failed at line " << __LINE__ << " in file " << __FILE__ ); ceLOGWRITE( "- Expression: " << #expression ); abort(); }
#else
#define ceASSERT( expression )
#endif


/*
	Core Types
*/
// Integer Values
typedef unsigned __int8 u8;
typedef __int8 s8;
typedef unsigned __int16 u16;
typedef __int16 s16;
typedef unsigned __int32 u32;
typedef __int32 s32;
typedef unsigned __int64 u64;
typedef __int64 s64;

// Floationg Point Values
typedef float f32;
typedef double f64;

// Make sure the wide character type is defined
#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif

// Characters
typedef char c8;
typedef wchar_t c16;

/*
	Math Classes
*/
#include "ceMath.h"
#include "ceVector.h"
#include "ceQuaternion.h"
#include "ceMatrix.h"
#include "ceMatrixStack.h"

/*
	Base Class Hierarchy

	Game
		- ResourceManager
			- Resource
				- TextureResource
				- MeshResource
				- ShaderResource
				- ScriptResource
				- SoundResource
		- Renderer
			- SceneGraph
				- SceneNode
					-MeshNode
					-CameraNode
		- Audio
			- Sound
			- Music
		- Physics
			- PhysicalNode
		- ActorManager
			- Actor (Syncs a SceneNode with a PhysicalNode)
*/
#include "ceGame.h"
