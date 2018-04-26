#pragma once

struct lua_State;

namespace moon
{

class ScriptHelper
{
public:
	static const char* DoString(lua_State* L, const char* str);
	static const char* DoFile(lua_State* L, const char* filename);

}; // ScriptHelper

}