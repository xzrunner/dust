#pragma once

struct lua_State;

#ifdef __cplusplus
extern "C"
{
#endif

#include <lua.h>

int luaopen_moon(lua_State* L);

void moon_add_module(const char* name, lua_CFunction func);

#ifdef __cplusplus
}
#endif
