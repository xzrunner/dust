#include "moon/Context.h"
#include "moon/runtime.h"
#include "moon/moon.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

const char* INIT_SCRIPT = R"(
	require "moon"
	require "moon.graphics"
	require "moon.scene"
	require "moon.physics"
	require "moon.mouse"
	require "moon.filesystem"
)";

namespace moon
{

Context::Context()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	// Add moon to package.preload for easy requiring.
	luax_preload(L, luaopen_moon, "moon");
}

Context::~Context()
{
	lua_close(L);
}

void Context::Init()
{
	luaL_dostring(L, INIT_SCRIPT);
}

}