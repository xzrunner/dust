#include "moon/moon.h"
#include "moon/runtime.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

extern "C" int luaopen_moon_graphics(lua_State* L);
extern "C" int luaopen_moon_scene_graph(lua_State* L);

static const luaL_Reg modules[] = {
	{ "moon.graphics", luaopen_moon_graphics },
	{ "moon.scene_graph", luaopen_moon_scene_graph },
	{ 0, 0 }
};

int luaopen_moon(lua_State* L)
{
	lua_newtable(L);

	// Preload module loaders.
	for (int i = 0; modules[i].name != 0; i++) {
		moon::luax_preload(L, modules[i].func, modules[i].name);
	}

	return 1;
}