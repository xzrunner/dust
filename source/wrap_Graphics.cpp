#include "dust/Graphics.h"
#include "dust/Module.h"
#include "dust/runtime.h"
#include "dust/Blackboard.h"
#include "dust/Context.h"
#include "dust/SceneNode.h"

#include <vector>
#include <algorithm>

struct lua_State;

namespace dust
{

#define INSTANCE() (Blackboard::Instance()->ctx->GetModuleMgr().GetModule<Graphics>(Module::M_GRAPHICS))

int w_line(lua_State* L)
{
	int args = lua_gettop(L);
	if (args % 2 != 0) {
		return luaL_error(L, "Number of vertex components must be a multiple of two");
	} else if (args < 4) {
		return luaL_error(L, "Need at least two vertices to draw a line");
	}

	std::vector<float> coords;
	coords.reserve(args);
	for (int i = 0; i < args; ++i) {
		coords.push_back(luax_tofloat(L, i + 1));
	}

	INSTANCE()->Polyline(&coords[0], coords.size());

	return 0;
}

int w_rectangle(lua_State* L)
{
	Graphics::DrawMode mode;
	const char *str = luaL_checkstring(L, 1);
	if (!Graphics::GetConstant(str, mode))
		return luaL_error(L, "Invalid draw mode: %s", str);

	float x = (float)luaL_checknumber(L, 2);
	float y = (float)luaL_checknumber(L, 3);
	float w = (float)luaL_checknumber(L, 4);
	float h = (float)luaL_checknumber(L, 5);

	if (lua_isnoneornil(L, 6))
	{
		INSTANCE()->Rectangle(mode, x, y, w, h);
		return 0;
	}

	float rx = (float)luaL_optnumber(L, 6, 0.0);
	float ry = (float)luaL_optnumber(L, 7, rx);

	int points;
	if (lua_isnoneornil(L, 8))
		points = std::max(rx, ry) > 20.0 ? (int)(std::max(rx, ry) / 2) : 10;
	else
		points = (int)luaL_checknumber(L, 8);

	// todo
	INSTANCE()->Rectangle(mode, x, y, w, h/*, rx, ry, points*/);
	return 0;
}

int w_circle(lua_State* L)
{
	Graphics::DrawMode mode;
	const char *str = luaL_checkstring(L, 1);
	if (!Graphics::GetConstant(str, mode)) {
		return luaL_error(L, "Invalid draw mode: %s", str);
	}

	float x = (float)luaL_checknumber(L, 2);
	float y = (float)luaL_checknumber(L, 3);
	float radius = (float)luaL_checknumber(L, 4);
	int points;
	if (lua_isnoneornil(L, 5))
		points = radius > 10 ? (int)(radius) : 10;
	else
		points = (int)luaL_checknumber(L, 5);

	INSTANCE()->Circle(mode, x, y, radius, points);
	return 0;
}

int w_draw(lua_State* L)
{
	if (!luax_istype(L, 1, SCENE_NODE_ID)) {
		return 0;
	}

	auto node = luax_checktype<SceneNode>(L, 1, SCENE_NODE_ID);
	node->Draw();
	return 0;
}

// List of functions to wrap.
static const luaL_Reg functions[] =
{
	// shape
	{ "line", w_line },
	{ "rectangle", w_rectangle },
	{ "circle", w_circle },

	{ "draw", w_draw },

	{ 0, 0 }
};

// Types for this module.
static const lua_CFunction types[] =
{
//	luaopen_image,
	0
};

extern "C" int luaopen_dust_graphics(lua_State* L)
{
	Graphics* instance = INSTANCE();
	if (instance == nullptr) {
		luax_catchexcept(L, [&](){ instance = new Graphics(); });
	} else {
		instance->Retain();
	}

	WrappedModule w;
	w.module = instance;
	w.name = "graphics";
	w.type = MODULE_GRAPHICS_ID;
	w.functions = functions;
	w.types = types;

	return luax_register_module(L, w);
}

}