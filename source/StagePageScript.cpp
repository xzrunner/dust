#include "dust/StagePageScript.h"
#include "dust/Blackboard.h"
#include "dust/Graphics.h"
#include "dust/Context.h"

#include <guard/check.h>

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

StagePageScript::StagePageScript(lua_State* L, const std::string& filepath)
	: L(L)
	, m_filepath(filepath)
{
	LoadScript();
}

void StagePageScript::OnLoad() const
{
	lua_getfield(L, LUA_REGISTRYINDEX, DUST_LOAD);
	int err = lua_pcall(L, 0, 0, 0);
	if (err != LUA_OK) {
		GD_REPORT_ASSERT(lua_tostring(L, -1));
		lua_error(L);
	}
}

void StagePageScript::OnUpdate() const
{
	lua_getfield(L, LUA_REGISTRYINDEX, DUST_UPDATE);

	lua_pushnumber(L, 1.0f / 30);
	int err = lua_pcall(L, 1, 0, 0);
	if (err != LUA_OK) {
		GD_REPORT_ASSERT(lua_tostring(L, -1));
		lua_error(L);
	}
}

void StagePageScript::OnDraw() const
{
	Blackboard::Instance()->GetContext()->GetModuleMgr().
		GetModule<Graphics>(Module::M_GRAPHICS)->ClearColor();

	lua_getfield(L, LUA_REGISTRYINDEX, DUST_DRAW);
	int err = lua_pcall(L, 0, 0, 0);
	if (err != LUA_OK) {
		GD_REPORT_ASSERT(lua_tostring(L, -1));
		lua_error(L);
	}
}

int StagePageScript::LoadScript()
{
	if (luaL_loadfile(L, m_filepath.c_str()) || lua_pcall(L, 0, 0, 0)) {
		return luaL_error(L, "Fail to load %s.", m_filepath.c_str());
	}

	lua_getglobal(L, "dust");

	RegistFunc("load", DUST_LOAD);
	RegistFunc("update", DUST_UPDATE);
	RegistFunc("draw", DUST_DRAW);

	lua_pop(L, 1);	// dust

	return 0;
}

void StagePageScript::RegistFunc(const char* name, const char* key)
{
	lua_getfield(L, -1, name);
	if (lua_isnil(L, -1)) {
		luaL_error(L, "Could not find dust.%s!", name);
	}

	lua_setfield(L, LUA_REGISTRYINDEX, key);
}

}