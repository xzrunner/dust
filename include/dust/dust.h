#pragma once

struct lua_State;

#ifdef __cplusplus
extern "C"
{
#endif

int luaopen_dust(lua_State* L);

#ifdef __cplusplus
}
#endif
