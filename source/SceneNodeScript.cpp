#include "dust/SceneNodeScript.h"
#include "dust/SceneNode.h"
#include "dust/Context.h"
#include "dust/Blackboard.h"
#include "dust/runtime.h"

#include <guard/check.h>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

namespace dust
{

#define DUST_SCENE_NODE "_dust_scene_nodes"

#define LOAD_FUNC   "load"
#define UPDATE_FUNC "update"
#define DRAW_FUNC   "draw"

void SceneNodeScript::OnLoad() const
{
	CallFunc(LOAD_FUNC);
}

void SceneNodeScript::OnUpdate() const
{
	CallFunc(UPDATE_FUNC);
}

void SceneNodeScript::OnDraw() const
{
	CallFunc(DRAW_FUNC);
}

int SceneNodeScript::LoadScript(const std::string& filepath, const n0::SceneNodePtr& node)
{
	auto L = Blackboard::Instance()->ctx->GetState();
	if (luaL_loadfile(L, filepath.c_str()) || lua_pcall(L, 0, 0, 0)) {
		return luaL_error(L, "Fail to load %s.", filepath.c_str());
	}

	lua_getfield(L, LUA_REGISTRYINDEX, DUST_SCENE_NODE);
	if (!lua_istable(L, -1))
	{
		lua_pop(L, 1);
		lua_newtable(L);

		lua_newtable(L);
		lua_pushliteral(L, "v");
		lua_setfield(L, -2, "__mode");
		lua_setmetatable(L, -2);

		lua_setfield(L, LUA_REGISTRYINDEX, DUST_SCENE_NODE);

		lua_getfield(L, LUA_REGISTRYINDEX, DUST_SCENE_NODE);
	}

	// obj
	lua_newtable(L);

	// v_node = scene_node
	lua_pushliteral(L, "v_node");
	auto sn = new SceneNode(node);
	luax_pushtype(L, SCENE_NODE_ID, sn);
	sn->Release();
	lua_settable(L, -3);

	// local mt = {}
	lua_newtable(L);

	// mt.load = load()
	lua_pushliteral(L, LOAD_FUNC);
	lua_getglobal(L, LOAD_FUNC);
	lua_settable(L, -3);

	// mt.update = update()
	lua_pushliteral(L, UPDATE_FUNC);
	lua_getglobal(L, UPDATE_FUNC);
	lua_settable(L, -3);

	// mt.draw = draw()
	lua_pushliteral(L, DRAW_FUNC);
	lua_getglobal(L, DRAW_FUNC);
	lua_settable(L, -3);

	// mt.__index = mt
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	// setmetatable({v_node}, mt)
	lua_setmetatable(L, -2);

	// regist obj to DUST_SCENE_NODE
	lua_pushlightuserdata(L, this);
	lua_pushvalue(L, -2);
	lua_settable(L, -4);

	lua_pop(L, 2);

	// clear global
	lua_pushnil(L);
	lua_setglobal(L, LOAD_FUNC);
	lua_pushnil(L);
	lua_setglobal(L, UPDATE_FUNC);
	lua_pushnil(L);
	lua_setglobal(L, DRAW_FUNC);

	GD_ASSERT(lua_gettop(L) == 0, "not clean");

	return 0;
}

void SceneNodeScript::CallFunc(const char* func_name) const
{
	auto L = Blackboard::Instance()->ctx->GetState();

	lua_getfield(L, LUA_REGISTRYINDEX, DUST_SCENE_NODE);

	lua_pushlightuserdata(L, const_cast<SceneNodeScript*>(this));
	lua_gettable(L, -2);
	GD_ASSERT(lua_type(L, -1) == LUA_TTABLE, "err");

	lua_pushstring(L, func_name);
	lua_gettable(L, -2);
	GD_ASSERT(lua_isfunction(L, -1), "err");

	// push self
	lua_pushvalue(L, -2);

	int err = lua_pcall(L, 1, 0, 0);
	if (err != LUA_OK) {
		GD_REPORT_ASSERT(lua_tostring(L, -1));
		lua_error(L);
	}

	lua_pop(L, 2);

	GD_ASSERT(lua_gettop(L) == 0, "not clean");
}

}