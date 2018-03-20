#include "moon/Graphics.h"
#include "moon/Module.h"
#include "moon/runtime.h"
#include "moon/Blackboard.h"
#include "moon/Context.h"
#include "moon/SceneNode.h"

#include <vector>
#include <algorithm>

struct lua_State;

namespace moon
{

#define INSTANCE() (Blackboard::Instance()->GetContext()->GetModuleMgr().GetModule<Graphics>(Module::M_GRAPHICS))

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

int w_set_background_color(lua_State* L)
{
	pt2::Color c;
	if (lua_istable(L, 1))
	{
		for (int i = 1; i <= 4; i++)
			lua_rawgeti(L, 1, i);

		c.r = (uint8_t)luaL_checknumber(L, -4);
		c.g = (uint8_t)luaL_checknumber(L, -3);
		c.b = (uint8_t)luaL_checknumber(L, -2);
		c.a = (uint8_t)luaL_optnumber(L, -1, 255);

		lua_pop(L, 4);
	}
	else
	{
		c.r = (uint8_t)luaL_checknumber(L, 1);
		c.g = (uint8_t)luaL_checknumber(L, 2);
		c.b = (uint8_t)luaL_checknumber(L, 3);
		c.a = (uint8_t)luaL_optnumber(L, 4, 255);
	}
	INSTANCE()->SetBackgroundColor(c);
	return 0;
}

int w_draw(lua_State* L)
{
	if (!luax_istype(L, 1, SCENE_NODE_ID)) {
		return 0;
	}

	int startidx = 2;
	float x  = (float)luaL_optnumber(L, startidx + 0, 0.0);
	float y  = (float)luaL_optnumber(L, startidx + 1, 0.0);
	float a  = (float)luaL_optnumber(L, startidx + 2, 0.0);
	float sx = (float)luaL_optnumber(L, startidx + 3, 1.0);
	float sy = (float)luaL_optnumber(L, startidx + 4, sx);
	float ox = (float)luaL_optnumber(L, startidx + 5, 0.0);
	float oy = (float)luaL_optnumber(L, startidx + 6, 0.0);
	float kx = (float)luaL_optnumber(L, startidx + 7, 0.0);
	float ky = (float)luaL_optnumber(L, startidx + 8, 0.0);

	auto node = luax_checktype<SceneNode>(L, 1, SCENE_NODE_ID);
	sm::Matrix2D mt;
	mt.SetTransformation(x, -y, -a, sx, sy, ox, -oy, kx, ky);
	node->Draw(mt);
	return 0;
}

// List of functions to wrap.
static const luaL_Reg functions[] =
{
	// shape
	{ "line", w_line },
	{ "rectangle", w_rectangle },
	{ "circle", w_circle },

	// state
	{ "set_background_color", w_set_background_color },

	{ "draw", w_draw },

	{ 0, 0 }
};

// Types for this module.
static const lua_CFunction types[] =
{
//	luaopen_image,
	0
};

extern "C" int luaopen_moon_graphics(lua_State* L)
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