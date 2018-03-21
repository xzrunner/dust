#include "moon/Mouse.h"
#include "moon/runtime.h"
#include "moon/Module.h"
#include "moon/Blackboard.h"
#include "moon/Context.h"

namespace moon
{

#define INSTANCE() (Blackboard::Instance()->GetContext()->GetModuleMgr().GetModule<Mouse>(Module::M_MOUSE))

int w_get_position(lua_State* L)
{
	auto& pos = INSTANCE()->GetPosition();
	lua_pushnumber(L, pos.x);
	lua_pushnumber(L, -pos.y);
	return 2;
}

// List of functions to wrap.
static const luaL_Reg functions[] =
{
	{ "get_position", w_get_position },

	{ 0, 0 }
};

// Types for this module.
static const lua_CFunction types[] =
{
	0
};

extern "C" int luaopen_moon_mouse(lua_State* L)
{
	Mouse* instance = INSTANCE();
	if (instance == nullptr) {
		luax_catchexcept(L, [&](){ instance = new Mouse(); });
	} else {
		instance->Retain();
	}

	WrappedModule w;
	w.module = instance;
	w.name = "mouse";
	w.type = MODULE_ID;
	w.functions = functions;
	w.types = types;

	return luax_register_module(L, w);
}

}