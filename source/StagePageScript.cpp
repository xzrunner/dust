#include "moon/StagePageScript.h"
#include "moon/Blackboard.h"
#include "moon/Graphics.h"
#include "moon/Context.h"

#include <guard/check.h>

#include <boost/filesystem.hpp>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

namespace moon
{

#define MOON_LOAD   "_moon_stage_load"
#define MOON_UPDATE "_moon_stage_update"
#define MOON_DRAW   "_moon_stage_draw"
#define MOON_MOUSE_PRESSED  "_moon_stage_mouse_pressed"
#define MOON_MOUSE_RELEASED "_moon_stage_mouse_released"
#define MOON_KEY_PRESSED    "_moon_stage_key_pressed"

StagePageScript::StagePageScript(lua_State* L, const std::string& filepath)
	: L(L)
	, m_filepath(filepath)
	, m_loaded(false)
{
	LoadScript();
}

void StagePageScript::OnLoad() const
{
	lua_getfield(L, LUA_REGISTRYINDEX, MOON_LOAD);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		m_loaded = true;
		return;
	}

	try {
		int err = lua_pcall(L, 0, 0, 0);
		if (err != LUA_OK) {
			GD_REPORT_ASSERT(lua_tostring(L, -1));
			lua_error(L);
		}
	} catch (std::exception& e) {
		GD_REPORT_ASSERT(e.what());
	}

	m_loaded = true;
}

void StagePageScript::OnUpdate() const
{
	if (!m_loaded) {
		return;
	}

	lua_getfield(L, LUA_REGISTRYINDEX, MOON_UPDATE);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}

	try {
		lua_pushnumber(L, 1.0f / 30);
		int err = lua_pcall(L, 1, 0, 0);
		if (err != LUA_OK) {
			GD_REPORT_ASSERT(lua_tostring(L, -1));
			lua_error(L);
		}
	} catch (std::exception& e) {
		GD_REPORT_ASSERT(e.what());
	}
}

void StagePageScript::OnDraw() const
{
	if (!m_loaded) {
		return;
	}

	Blackboard::Instance()->GetContext()->GetModuleMgr().
		GetModule<Graphics>(Module::M_GRAPHICS)->ClearColor();

	lua_getfield(L, LUA_REGISTRYINDEX, MOON_DRAW);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}
	try {
		int err = lua_pcall(L, 0, 0, 0);
		if (err != LUA_OK) {
			GD_REPORT_ASSERT(lua_tostring(L, -1));
			lua_error(L);
		}
	} catch (std::exception& e) {
		GD_REPORT_ASSERT(e.what());
	}
}

void StagePageScript::OnMousePressed(int x, int y, int button) const
{
	if (!m_loaded) {
		return;
	}

	lua_getfield(L, LUA_REGISTRYINDEX, MOON_MOUSE_PRESSED);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}

	try {
		lua_pushinteger(L, x);
		lua_pushinteger(L, y);
		lua_pushinteger(L, button);
		int err = lua_pcall(L, 3, 0, 0);
		if (err != LUA_OK) {
			GD_REPORT_ASSERT(lua_tostring(L, -1));
			lua_error(L);
		}
	} catch (std::exception& e) {
		GD_REPORT_ASSERT(e.what());
	}
}

void StagePageScript::OnMouseReleased(int x, int y, int button) const
{
	if (!m_loaded) {
		return;
	}

	lua_getfield(L, LUA_REGISTRYINDEX, MOON_MOUSE_RELEASED);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}

	try {
		lua_pushinteger(L, x);
		lua_pushinteger(L, y);
		lua_pushinteger(L, button);
		int err = lua_pcall(L, 3, 0, 0);
		if (err != LUA_OK) {
			GD_REPORT_ASSERT(lua_tostring(L, -1));
			lua_error(L);
		}
	} catch (std::exception& e) {
		GD_REPORT_ASSERT(e.what());
	}
}

void StagePageScript::OnKeyPressed(const char* key) const
{
	if (!m_loaded) {
		return;
	}

	lua_getfield(L, LUA_REGISTRYINDEX, MOON_KEY_PRESSED);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}
	try {
		lua_pushstring(L, key);
		int err = lua_pcall(L, 1, 0, 0);
		if (err != LUA_OK) {
			GD_REPORT_ASSERT(lua_tostring(L, -1));
			lua_error(L);
		}
	} catch (std::exception& e) {
		GD_REPORT_ASSERT(e.what());
	}
}

int StagePageScript::LoadScript()
{
	auto dir = boost::filesystem::path(m_filepath).parent_path().string();
	SetLuaPath((dir + "\\?.lua" + ";" + dir + "\\?\\init.lua").c_str());

	try {
		if (luaL_loadfile(L, m_filepath.c_str()) || lua_pcall(L, 0, 0, 0)) {
			GD_REPORT_ASSERT(lua_tostring(L, -1));
			lua_error(L);
		}
	} catch (std::exception& e) {
		GD_REPORT_ASSERT(e.what());
	}

	lua_getglobal(L, "moon");

	RegistFunc("load", MOON_LOAD);
	RegistFunc("update", MOON_UPDATE);
	RegistFunc("draw", MOON_DRAW);

	RegistFunc("mousepressed", MOON_MOUSE_PRESSED);
	RegistFunc("mousereleased", MOON_MOUSE_RELEASED);
	RegistFunc("keypressed", MOON_KEY_PRESSED);

	lua_pop(L, 1);	// moon

	return 0;
}

void StagePageScript::SetLuaPath(const char* path)
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

void StagePageScript::RegistFunc(const char* name, const char* key)
{
	lua_getfield(L, -1, name);
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return;
	}

	lua_setfield(L, LUA_REGISTRYINDEX, key);
}

}