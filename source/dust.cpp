#include "dust/dust.h"
#include "dust/runtime.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

extern "C" int luaopen_dust_graphics(lua_State* L);

static const luaL_Reg modules[] = {
	{ "dust.graphics", luaopen_dust_graphics },
	{ 0, 0 }
};

int luaopen_dust(lua_State* L)
{
	lua_newtable(L);

	// Preload module loaders.
	for (int i = 0; modules[i].name != 0; i++) {
		dust::luax_preload(L, modules[i].func, modules[i].name);
	}

	return 1;
}