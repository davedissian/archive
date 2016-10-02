/*
	Velocity Engine
	By SerialVelocity
*/
#pragma once

// Lua
#include <lua.hpp>
#pragma comment( lib, "lua51.lib" )

VE_BEGIN

// Lua State Manager
class LuaState
{
public:
						LuaState();
						~LuaState();

	// Functions
	void				registerFunc( const std::string &name, lua_CFunction function );

	// Execution
	void				executeChunk( const std::string &line );
	void				executeFile( const std::string &file );

	// Accessors
	lua_State			*getState();

private:
	// Lua State
	lua_State			*mLS;

};

VE_END
