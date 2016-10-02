/*
	Velocity Engine
	By SerialVelocity
*/
#include "VEBase.hpp"
#include "VELuaState.hpp"
using namespace VE;

// Lua State Manager
LuaState::LuaState()
{
	// Open Lua
	mLS = lua_open();
	luaL_openlibs( mLS );
}

LuaState::~LuaState()
{
	// Close Lua
	lua_close( mLS );
}

void LuaState::registerFunc( const std::string &name, lua_CFunction function )
{
	// Register the function
	lua_register( mLS, name.c_str(), function );
}

void LuaState::executeChunk( const std::string &line )
{
	// Execute the chunk
	luaL_dostring( mLS, line.c_str() );
}

void LuaState::executeFile( const std::string &file )
{
	// Execute the file
	luaL_dofile( mLS, file.c_str() );
}

lua_State *LuaState::getState()
{
	// Return the lua state
	return mLS;
}
