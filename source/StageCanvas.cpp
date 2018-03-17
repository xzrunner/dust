#include "dust/StageCanvas.h"
#include "dust/LuaVM.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

namespace dust
{

#define DUST_LOAD   "_dust_stage_load"
#define DUST_UPDATE "_dust_stage_update"
#define DUST_DRAW   "_dust_stage_draw"

StageCanvas::StageCanvas(lua_State* L, const char* filepath)
	: L(L)
{
	LoadScript(filepath);
}

void StageCanvas::Load() const
{
	lua_getfield(L, LUA_REGISTRYINDEX, DUST_LOAD);
	lua_pcall(L, 0, 0, 0);
}

void StageCanvas::Update() const
{
	lua_getfield(L, LUA_REGISTRYINDEX, DUST_UPDATE);
	lua_pcall(L, 0, 0, 0);
}

void StageCanvas::Draw() const
{
	lua_getfield(L, LUA_REGISTRYINDEX, DUST_DRAW);
	lua_pcall(L, 0, 0, 0);
}

int StageCanvas::LoadScript(const char* filepath)
{
	if (luaL_loadfile(L, filepath) || lua_pcall(L, 0, 0, 0)) {
		return luaL_error(L, "Fail to load %s.", filepath);
	}

	lua_getglobal(L, "dust");
	if (lua_isnil(L, -1)) {
		return luaL_error(L, "Could not find global dust.");
	}

	RegistFunc("load", DUST_LOAD);
	RegistFunc("update", DUST_UPDATE);
	RegistFunc("draw", DUST_DRAW);

	lua_pop(L, 1); // dust

	return 0;
}

void StageCanvas::RegistFunc(const char* name, const char* key)
{
	lua_getfield(L, -1, name);
	if (lua_isnil(L, -1)) {
		luaL_error(L, "Could not find dust.%s!", name);
	}

	lua_setfield(L, LUA_REGISTRYINDEX, key);
}

}