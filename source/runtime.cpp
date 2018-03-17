/**
* Copyright (c) 2006-2016 LOVE Development Team
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
**/

#include "dust/runtime.h"
#include "dust/Module.h"

namespace dust
{

/**
 * Called when an object is collected. The object is released
 * once in this function, possibly deleting it.
 **/
static int w__gc(lua_State *L)
{
	Proxy *p = (Proxy *) lua_touserdata(L, 1);
	p->object->Release();
	return 0;
}

void luax_setfuncs(lua_State *L, const luaL_Reg *l)
{
	if (l == nullptr)
		return;

	for (; l->name != nullptr; l++)
	{
		lua_pushcfunction(L, l->func);
		lua_setfield(L, -2, l->name);
	}
}

int luax_register_module(lua_State* L, const WrappedModule& m)
{
	AddTypeName(m.type, m.name);

	// Put a reference to the C++ module in Lua.
	luax_insistregistry(L, REGISTRY_MODULES);

	Proxy* p = (Proxy*)lua_newuserdata(L, sizeof(Proxy));
	p->object = m.module;
	p->type = m.type;

	luaL_newmetatable(L, m.module->GetName());

	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, w__gc);
	lua_setfield(L, -2, "__gc");

	lua_setmetatable(L, -2);
	lua_setfield(L, -2, m.name); // _modules[name] = proxy
	lua_pop(L, 1);

	// Gets the love table.
	luax_insistglobal(L, "love");

	// Create new table for module.
	lua_newtable(L);

	// Register all the functions.
	if (m.functions != nullptr)
		luax_setfuncs(L, m.functions);

	// Register types.
	if (m.types != nullptr)
	{
		for (const lua_CFunction *t = m.types; *t != nullptr; t++)
			(*t)(L);
	}

	lua_pushvalue(L, -1);
	lua_setfield(L, -2, m.name); // love.graphics = table
	lua_remove(L, -2); // love

	// Register module instance
	Module::RegisterInstance(m.module);

	return 1;
}

int luax_insist(lua_State *L, int idx, const char *k)
{
	// Convert to absolute index if necessary.
	if (idx < 0 && idx > LUA_REGISTRYINDEX)
		idx += lua_gettop(L) + 1;

	lua_getfield(L, idx, k);

	// Create if necessary.
	if (!lua_istable(L, -1))
	{
		lua_pop(L, 1); // Pop the non-table.
		lua_newtable(L);
		lua_pushvalue(L, -1); // Duplicate the table to leave on top.
		lua_setfield(L, idx, k); // lua_stack[idx][k] = lua_stack[-1] (table)
	}

	return 1;
}

int luax_insistglobal(lua_State *L, const char *k)
{
	lua_getglobal(L, k);

	if (!lua_istable(L, -1))
	{
		lua_pop(L, 1); // Pop the non-table.
		lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setglobal(L, k);
	}

	return 1;
}

int luax_c_insistglobal(lua_State *L, const char *k)
{
	return luax_insistglobal(L, k);
}

int luax_insistdust(lua_State *L, const char *k)
{
	luax_insistglobal(L, "dust");
	luax_insist(L, -1, k);

	// The dust table should be replaced with the top stack
	// item. Only the reqested table should remain on the stack.
	lua_replace(L, -2);

	return 1;
}

int luax_getdust(lua_State *L, const char *k)
{
	lua_getglobal(L, "dust");

	if (!lua_isnil(L, -1))
	{
		lua_getfield(L, -1, k);
		lua_replace(L, -2);
	}

	return 1;
}

int luax_preload(lua_State* L, lua_CFunction f, const char* name)
{
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "preload");
	lua_pushcfunction(L, f);
	lua_setfield(L, -2, name);
	lua_pop(L, 2);
	return 0;
}

int luax_insistregistry(lua_State *L, Registry r)
{
	switch (r)
	{
	case REGISTRY_MODULES:
		return luax_insistdust(L, "_modules");
	case REGISTRY_OBJECTS:
		return luax_insist(L, LUA_REGISTRYINDEX, "_dustobjects");
	default:
		return luaL_error(L, "Attempted to use invalid registry.");
	}
}

int luax_getregistry(lua_State *L, Registry r)
{
	switch (r)
	{
	case REGISTRY_MODULES:
		return luax_getdust(L, "_modules");
	case REGISTRY_OBJECTS:
		lua_getfield(L, LUA_REGISTRYINDEX, "_dustobjects");
		return 1;
	default:
		return luaL_error(L, "Attempted to use invalid registry.");
	}
}

}