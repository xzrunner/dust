#include "moon/runtime.h"
#include "moon/Image.h"
#include "moon/types.h"

namespace
{

moon::Image* luax_checkimg(lua_State* L, int idx)
{
	return moon::luax_checktype<moon::Image>(L, idx, moon::IMAGE_ID);
}

int w_get_width(lua_State* L)
{
	auto img = luax_checkimg(L, 1);
	lua_pushnumber(L, img->GetImage()->GetWidth());
	return 1;
}

int w_get_height(lua_State* L)
{
	auto img = luax_checkimg(L, 1);
	lua_pushnumber(L, img->GetImage()->GetHeight());
	return 1;
}

}

namespace moon
{

static const luaL_Reg w_functions[] =
{
	{ "get_width",  w_get_width },
	{ "get_height", w_get_height },

	{ 0, 0 }
};

extern "C" int luaopen_image(lua_State* L)
{
	return luax_register_type(L, IMAGE_ID, "Image", w_functions, nullptr);
}

}