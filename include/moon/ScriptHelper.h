#pragma once

struct lua_State;

namespace moon
{

class ScriptHelper
{
public:
	static const char* DoString(lua_State* L, const char* str);

}; // ScriptHelper

}