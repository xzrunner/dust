#include "moon/ScriptHelper.h"

#include <guard/check.h>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

#include <string>

namespace moon
{

void ScriptHelper::AddSearchPath(lua_State* L, const char* path)
{
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "path");
	std::string cur_path = lua_tostring(L, -1);
	cur_path.append(";");
	cur_path.append(path);
	lua_pop(L, 1);
	lua_pushstring(L, cur_path.c_str());
	lua_setfield(L, -2, "path");
	lua_pop(L, 1);
}

const char* ScriptHelper::DoString(lua_State* L, const char* str)
{
	int err = luaL_dostring(L, str);
	if (err) {
		return lua_tostring(L, -1);
	} else {
		return nullptr;
	}
}

const char* ScriptHelper::DoFile(lua_State* L, const char* filename)
{
	int err = luaL_dofile(L, filename);
	if (err) {
		return lua_tostring(L, -1);
	} else {
		return nullptr;
	}
}

}