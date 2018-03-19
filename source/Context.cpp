#include "dust/Context.h"
#include "dust/runtime.h"
#include "dust/dust.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

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

}