#include "moon/Graphics.h"
#include "moon/runtime.h"
#include "moon/Blackboard.h"
#include "moon/Context.h"
#include "moon/Image.h"
#include "moon/SceneNode.h"

#include <painting2/RenderColorCommon.h>

#include <boost/filesystem.hpp>

#include <vector>
#include <algorithm>

struct lua_State;

namespace
{

#define INSTANCE() (moon::Blackboard::Instance()->GetContext()->GetModuleMgr().GetModule<moon::Graphics>())

int w_line(lua_State* L)
{
	int args = lua_gettop(L);
	bool is_table = false;
	if (args == 1 && lua_istable(L, 1))
	{
		args = (int)moon::luax_objlen(L, 1);
		is_table = true;
	}

	if (args % 2 != 0) {
		return luaL_error(L, "Number of vertex components must be a multiple of two");
	} else if (args < 4) {
		return luaL_error(L, "Need at least two vertices to draw a line");
	}

	std::vector<float> coords;
	coords.reserve(args);
	if (is_table)
	{
		for (int i = 0; i < args; ++i)
		{
			lua_rawgeti(L, 1, i + 1);
			coords.push_back(moon::luax_tofloat(L, -1));
			lua_pop(L, 1);
		}
	}
	else
	{
		for (int i = 0; i < args; ++i) {
			coords.push_back(moon::luax_tofloat(L, i + 1));
		}
	}
	for (int i = 0; i < args; ++i) {
		coords.push_back(moon::luax_tofloat(L, i + 1));
	}

	// reverse y
	for (int i = 1; i < args; i += 2) {
		coords[i] = -coords[i];
	}

	INSTANCE()->Polyline(&coords[0], coords.size());

	return 0;
}

int w_rectangle(lua_State* L)
{
	moon::Graphics::DrawMode mode;
	const char *str = luaL_checkstring(L, 1);
	if (!moon::Graphics::GetConstant(str, mode))
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
	moon::Graphics::DrawMode mode;
	const char *str = luaL_checkstring(L, 1);
	if (!moon::Graphics::GetConstant(str, mode)) {
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

int w_polygon(lua_State* L)
{
	int args = lua_gettop(L) - 1;

	moon::Graphics::DrawMode mode;
	const char *str = luaL_checkstring(L, 1);
	if (!moon::Graphics::GetConstant(str, mode)) {
		return luaL_error(L, "Invalid draw mode: %s", str);
	}

	bool is_table = false;
	float *coords;
	if (args == 1 && lua_istable(L, 2))
	{
		args = (int)moon::luax_objlen(L, 2);
		is_table = true;
	}

	if (args % 2 != 0) {
		return luaL_error(L, "Number of vertex components must be a multiple of two");
	} else if (args < 6) {
		return luaL_error(L, "Need at least three vertices to draw a polygon");
	}

	// fetch coords
	coords = new float[args + 2];
	if (is_table)
	{
		for (int i = 0; i < args; ++i)
		{
			lua_rawgeti(L, 2, i + 1);
			coords[i] = moon::luax_tofloat(L, -1);
			lua_pop(L, 1);
		}
	}
	else
	{
		for (int i = 0; i < args; ++i) {
			coords[i] = moon::luax_tofloat(L, i + 2);
		}
	}

	// make a closed loop
	coords[args]   = coords[0];
	coords[args+1] = coords[1];

	// reverse y
	for (int i = 1; i < args + 2; i += 2) {
		coords[i] = -coords[i];
	}

	INSTANCE()->Polygon(mode, coords, args+2);
	delete[] coords;

	return 0;
}

int w_set_color(lua_State* L)
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
	INSTANCE()->SetColor(c);
	return 0;
}

int w_get_color(lua_State* L)
{
	auto& c = INSTANCE()->GetColor();
	lua_pushnumber(L, c.r);
	lua_pushnumber(L, c.g);
	lua_pushnumber(L, c.b);
	lua_pushnumber(L, c.a);
	return 4;
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

int w_set_line_width(lua_State* L)
{
	float width = (float)luaL_checknumber(L, 1);
	INSTANCE()->SetLineWidth(width);
	return 0;
}

int w_get_font(lua_State* L)
{
	return 0;
}

int w_set_font(lua_State* L)
{
	return 0;
}

int w_draw(lua_State* L)
{
	if (!moon::luax_istype(L, 1, moon::SCENE_NODE_ID) &&
		!moon::luax_istype(L, 1, moon::IMAGE_ID)) {
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

	sm::Matrix2D mt;
	mt.SetTransformation(x, y, a, sx, sy, ox, oy, kx, ky);

	if (moon::luax_istype(L, 1, moon::SCENE_NODE_ID))
	{
		auto node = moon::luax_checktype<moon::SceneNode>(L, 1, moon::SCENE_NODE_ID);
		moon::luax_catchexcept(L, [&]() {
			node->Draw(mt);
		});
	}
	else if (moon::luax_istype(L, 1, moon::IMAGE_ID))
	{
		auto img = moon::luax_checktype<moon::Image>(L, 1, moon::IMAGE_ID);
		moon::luax_catchexcept(L, [&]() {
			img->Draw(mt);
		});
	}

	return 0;
}

int w_print(lua_State* L)
{
	const char* str = luaL_checkstring(L, 1);
	float x  = (float)luaL_optnumber(L, 2, 0.0);
	float y  = (float)luaL_optnumber(L, 3, 0.0);
	float a  = (float)luaL_optnumber(L, 4, 0.0f);
	float sx = (float)luaL_optnumber(L, 5, 1.0f);
	float sy = (float)luaL_optnumber(L, 6, sx);
	float ox = (float)luaL_optnumber(L, 7, 0.0f);
	float oy = (float)luaL_optnumber(L, 8, 0.0f);
	float kx = (float)luaL_optnumber(L, 9, 0.0f);
	float ky = (float)luaL_optnumber(L, 10, 0.0f);

	sm::Matrix2D mt;
	mt.SetTransformation(x, -y, -a, sx, sy, ox, -oy, kx, ky);
	moon::luax_catchexcept(L, [&]() {
		INSTANCE()->Print(str, mt, pt2::RenderColorCommon());
	});
	return 0;
}

int w_printf(lua_State* L)
{
	const char* str = luaL_checkstring(L, 1);
	float x  = (float)luaL_optnumber(L, 2, 0.0);
	float y  = (float)luaL_optnumber(L, 3, 0.0);
	float wrap = (float)luaL_checknumber(L, 4);

	float a = 0.0f;
	float sx = 1.0f, sy = 1.0f;
	float ox = 0.0f, oy = 0.0f;
	float kx = 0.0f, ky = 0.0f;

	//Font::AlignMode align = Font::ALIGN_LEFT;
	if (lua_gettop(L) >= 5)
	{
		if (!lua_isnil(L, 5))
		{
			const char *str = luaL_checkstring(L, 5);
			//if (!Font::getConstant(str, align))
			//	return luaL_error(L, "Incorrect alignment: %s", str);
		}

		a  = (float) luaL_optnumber(L, 6, 0.0f);
		sx = (float) luaL_optnumber(L, 7, 1.0f);
		sy = (float) luaL_optnumber(L, 8, sx);
		ox = (float) luaL_optnumber(L, 9, 0.0f);
		oy = (float) luaL_optnumber(L, 10, 0.0f);
		kx = (float) luaL_optnumber(L, 11, 0.0f);
		ky = (float) luaL_optnumber(L, 12, 0.0f);
	}

	sm::Matrix2D mt;
	mt.SetTransformation(x, -y, -a, sx, sy, ox, -oy, kx, ky);
	moon::luax_catchexcept(L, [&]() {
		INSTANCE()->Print(str, mt, pt2::RenderColorCommon());
	});
	return 0;
}

int w_get_width(lua_State* L)
{
	int w;
	moon::luax_catchexcept(L, [&]() {
		w = INSTANCE()->GetWidth();
	});
	lua_pushinteger(L, w);
	return 1;
}

int w_get_height(lua_State* L)
{
	int h;
	moon::luax_catchexcept(L, [&]() {
		h = INSTANCE()->GetHeight();
	});
	lua_pushinteger(L, h);
	return 1;
}

int w_new_image(lua_State* L)
{
	auto bb = moon::Blackboard::Instance();

	const char* filepath = luaL_checkstring(L, 1);
	auto real_path = boost::filesystem::absolute(filepath, bb->GetWorkDir());

	moon::Image* img = nullptr;
	moon::luax_catchexcept(L, [&]() { img = new moon::Image(real_path.string()); });
	moon::luax_pushtype(L, moon::IMAGE_ID, img);
	img->Release();

	return 1;
}

}

namespace moon
{

// List of functions to wrap.
static const luaL_Reg functions[] =
{
	// shape
	{ "line", w_line },
	{ "rectangle", w_rectangle },
	{ "circle", w_circle },
	{ "polygon", w_polygon },

	// state
	{ "set_color", w_set_color },
	{ "get_color", w_get_color },
	{ "set_background_color", w_set_background_color },
	{ "set_line_width", w_set_line_width },
	{ "get_font", w_get_font },
	{ "set_font", w_set_font },

	{ "draw", w_draw },

	{ "print", w_print },
	{ "printf", w_printf },

	{ "get_width", w_get_width },
	{ "get_height", w_get_height },

	{ "new_image", w_new_image },

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