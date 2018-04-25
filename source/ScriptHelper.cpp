#include "moon/ScriptHelper.h"

#include <guard/check.h>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

namespace moon
{

const char* ScriptHelper::DoString(lua_State* L, const char* str)
{
	int err = luaL_dostring(L, str);
	if (err) {
		return lua_tostring(L, -1);
	} else {
		return nullptr;
	}
}

}