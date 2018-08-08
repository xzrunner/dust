#include "moon/moon.h"
#include "moon/runtime.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

#include <vector>

extern "C" int luaopen_moon_graphics(lua_State* L);
extern "C" int luaopen_moon_scene(lua_State* L);
extern "C" int luaopen_moon_physics(lua_State* L);
extern "C" int luaopen_moon_mouse(lua_State* L);
extern "C" int luaopen_moon_filesystem(lua_State* L);
extern "C" int luaopen_moon_gui(lua_State* L);

namespace
{

std::vector<luaL_Reg> MODULES = {
	{ "moon.graphics", luaopen_moon_graphics },
	{ "moon.scene", luaopen_moon_scene },
	{ "moon.physics", luaopen_moon_physics },
	{ "moon.mouse", luaopen_moon_mouse },
	{ "moon.filesystem", luaopen_moon_filesystem },
	{ "moon.gui", luaopen_moon_gui },
};

}

int luaopen_moon(lua_State* L)
{
	lua_newtable(L);

	// Preload module loaders.
	for (auto& m : MODULES) {
		moon::luax_preload(L, m.func, m.name);
	}

	return 1;
}

void moon_add_module(const char* name, lua_CFunction func)
{
	luaL_Reg reg;
	reg.name = name;
	reg.func = func;
	MODULES.push_back(reg);
}