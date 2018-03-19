#include "dust/Context.h"
#include "dust/runtime.h"
#include "dust/dust.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

const char* INIT_SCRIPT = R"(
	require "dust"
	require "dust.graphics"
	require "dust.scene_graph"
)";

namespace dust
{

Context::Context()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	// Add dust to package.preload for easy requiring.
	luax_preload(L, luaopen_dust, "dust");
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