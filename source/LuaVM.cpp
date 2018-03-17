#include "dust/LuaVM.h"
#include "dust/runtime.h"
#include "dust/dust.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

namespace dust
{

LuaVM::LuaVM()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	// Add dust to package.preload for easy requiring.
	luax_preload(L, luaopen_dust, "dust");
}

LuaVM::~LuaVM()
{
	lua_close(L);
}

void LuaVM::DoString(const char* str)
{
	luaL_dostring(L, str);
}

void LuaVM::DoFile(const char* filename)
{
	luaL_dofile(L, filename);
}

}