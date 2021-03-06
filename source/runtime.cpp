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

#include "moon/runtime.h"
#include "moon/Module.h"
#include "moon/Blackboard.h"
#include "moon/Context.h"
#include "moon/Reference.h"

#include <iostream>

namespace moon
{

/**
 * Called when an object is collected. The object is released
 * once in this function, possibly deleting it.
 **/
static int w__gc(lua_State* L)
{
	Proxy* p = (Proxy* ) lua_touserdata(L, 1);
	p->object->Release();
	return 0;
}

static int w__tostring(lua_State* L)
{
	Proxy* p = (Proxy* )lua_touserdata(L, 1);
	const char *typname = lua_tostring(L, lua_upvalueindex(1));
	lua_pushfstring(L, "%s: %p", typname, p->object);
	return 1;
}

static int w__type(lua_State* L)
{
	lua_pushvalue(L, lua_upvalueindex(1));
	return 1;
}

static int w__typeOf(lua_State* L)
{
	Proxy* p = (Proxy* )lua_touserdata(L, 1);
	Type t = luax_type(L, 2);
	luax_pushboolean(L, TypeFlags[p->type][t]);
	return 1;
}

static int w__eq(lua_State* L)
{
	Proxy* p1 = (Proxy* )lua_touserdata(L, 1);
	Proxy* p2 = (Proxy* )lua_touserdata(L, 2);
	luax_pushboolean(L, p1->object == p2->object);
	return 1;
}

Reference *luax_refif(lua_State *L, int type)
{
	Reference *r = nullptr;

	// Create a reference only if the test succeeds.
	if (lua_type(L, -1) == type)
		r = new Reference(L);
	else // Pop the value even if it fails (but also if it succeeds).
		lua_pop(L, 1);

	return r;
}

void luax_printstack(lua_State *L)
{
	for (int i = 1; i <= lua_gettop(L); i++)
		std::cout << i << " - " << luaL_typename(L, i) << std::endl;
}

bool luax_toboolean(lua_State *L, int idx)
{
	return (lua_toboolean(L, idx) != 0);
}

void luax_pushboolean(lua_State *L, bool b)
{
	lua_pushboolean(L, b ? 1 : 0);
}

bool luax_optboolean(lua_State *L, int idx, bool b)
{
	if (lua_isboolean(L, idx) == 1)
		return (lua_toboolean(L, idx) == 1 ? true : false);
	return b;
}

int luax_assert_argc(lua_State *L, int min)
{
	int argc = lua_gettop(L);
	if (argc < min)
		return luaL_error(L, "Incorrect number of arguments. Got [%d], expected at least [%d]", argc, min);
	return 0;
}

int luax_assert_argc(lua_State *L, int min, int max)
{
	int argc = lua_gettop(L);
	if (argc < min || argc > max)
		return luaL_error(L, "Incorrect number of arguments. Got [%d], expected [%d-%d]", argc, min, max);
	return 0;
}

void luax_setfuncs(lua_State* L, const luaL_Reg *l)
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

	// Gets the moon table.
	luax_insistglobal(L, "moon");

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
	lua_setfield(L, -3, m.name); // moon.graphics = table
	lua_remove(L, -2); // moon

	// Register module instance

	Blackboard::Instance()->GetContext()->GetModuleMgr().RegisterModule(m.module);

	return 1;
}

void luax_rawnewtype(lua_State* L, Type type, Object *object)
{
	Proxy* u = (Proxy* )lua_newuserdata(L, sizeof(Proxy));

	object->Retain();

	u->object = object;
	u->type = type;

	const char *name = "Invalid";
	GetTypeName(type, name);

	luaL_newmetatable(L, name);
	lua_setmetatable(L, -2);
}

bool luax_istype(lua_State *L, int idx, Type type)
{
	if (lua_type(L, idx) != LUA_TUSERDATA) {
		return false;
	}

	Proxy* p = (Proxy* )lua_touserdata(L, idx);

	if (p->type > INVALID_ID && p->type < TYPE_MAX_ENUM) {
		return TypeFlags[p->type][type];
	} else {
		return false;
	}
}

int luax_insist(lua_State* L, int idx, const char *k)
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

int luax_insistglobal(lua_State* L, const char *k)
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

int luax_c_insistglobal(lua_State* L, const char *k)
{
	return luax_insistglobal(L, k);
}

int luax_insistmoon(lua_State* L, const char *k)
{
	luax_insistglobal(L, "moon");
	luax_insist(L, -1, k);

	// The moon table should be replaced with the top stack
	// item. Only the reqested table should remain on the stack.
	lua_replace(L, -2);

	return 1;
}

int luax_getmoon(lua_State* L, const char *k)
{
	lua_getglobal(L, "moon");

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

int luax_register_type(lua_State* L, Type type, const char *name, ...)
{
	AddTypeName(type, name);

	// Get the place for storing and re-using instantiated moon types.
	luax_getregistry(L, REGISTRY_OBJECTS);

	// Create registry._moonobjects if it doesn't exist yet.
	if (!lua_istable(L, -1))
	{
		lua_newtable(L);
		lua_replace(L, -2);

		// Create a metatable.
		lua_newtable(L);

		// metatable.__mode = "v". Weak userdata values.
		lua_pushliteral(L, "v");
		lua_setfield(L, -2, "__mode");

		// setmetatable(newtable, metatable)
		lua_setmetatable(L, -2);

		// registry._moonobjects = newtable
		lua_setfield(L, LUA_REGISTRYINDEX, "_moonobjects");
	}
	else
		lua_pop(L, 1);

	luaL_newmetatable(L, name);

	// m.__index = m
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	// setup gc
	lua_pushcfunction(L, w__gc);
	lua_setfield(L, -2, "__gc");

	// Add equality
	lua_pushcfunction(L, w__eq);
	lua_setfield(L, -2, "__eq");

	// Add tostring function.
	lua_pushstring(L, name);
	lua_pushcclosure(L, w__tostring, 1);
	lua_setfield(L, -2, "__tostring");

	// Add type
	lua_pushstring(L, name);
	lua_pushcclosure(L, w__type, 1);
	lua_setfield(L, -2, "type");

	// Add typeOf
	lua_pushcfunction(L, w__typeOf);
	lua_setfield(L, -2, "typeOf");

	va_list fs;
	va_start(fs, name);
	for (const luaL_Reg *f = va_arg(fs, const luaL_Reg *); f; f = va_arg(fs, const luaL_Reg *))
		luax_setfuncs(L, f);
	va_end(fs);

	lua_pop(L, 1); // Pops metatable.
	return 0;
}

void luax_pushtype(lua_State* L, Type type, Object *object)
{
	if (object == nullptr)
	{
		lua_pushnil(L);
		return;
	}

	// Fetch the registry table of instantiated objects.
	luax_getregistry(L, REGISTRY_OBJECTS);

	// The table might not exist - it should be insisted in luax_register_type.
	if (!lua_istable(L, -1))
	{
		lua_pop(L, 1);
		return luax_rawnewtype(L, type, object);
	}

	// Get the value of moonobjects[object] on the stack.
	lua_pushlightuserdata(L, object);
	lua_gettable(L, -2);

	// If the Proxy userdata isn't in the instantiated types table yet, add it.
	if (lua_type(L, -1) != LUA_TUSERDATA)
	{
		lua_pop(L, 1);

		luax_rawnewtype(L, type, object);

		lua_pushlightuserdata(L, object);
		lua_pushvalue(L, -2);

		// moonobjects[object] = Proxy.
		lua_settable(L, -4);
	}

	// Remove the moonobjects table from the stack.
	lua_remove(L, -2);

	// Keep the Proxy userdata on the stack.
}

int luax_insistregistry(lua_State* L, Registry r)
{
	switch (r)
	{
	case REGISTRY_MODULES:
		return luax_insistmoon(L, "_modules");
	case REGISTRY_OBJECTS:
		return luax_insist(L, LUA_REGISTRYINDEX, "_moonobjects");
	default:
		return luaL_error(L, "Attempted to use invalid registry.");
	}
}

int luax_getregistry(lua_State* L, Registry r)
{
	switch (r)
	{
	case REGISTRY_MODULES:
		return luax_getmoon(L, "_modules");
	case REGISTRY_OBJECTS:
		lua_getfield(L, LUA_REGISTRYINDEX, "_moonobjects");
		return 1;
	default:
		return luaL_error(L, "Attempted to use invalid registry.");
	}
}

static const char *MAIN_THREAD_KEY = "_moon_mainthread";

lua_State *luax_insistpinnedthread(lua_State *L)
{
	lua_getfield(L, LUA_REGISTRYINDEX, MAIN_THREAD_KEY);

	if (lua_isnoneornil(L, -1))
	{
		lua_pop(L, 1);

		// lua_pushthread returns 1 if it's actually the main thread, but we
		// can't actually get the real main thread if lua_pushthread doesn't
		// return it (in Lua 5.1 at least), so we ignore that for now...
		// We do store a strong reference to the current thread/coroutine in
		// the registry, however.
		lua_pushthread(L);
		lua_pushvalue(L, -1);
		lua_setfield(L, LUA_REGISTRYINDEX, MAIN_THREAD_KEY);
	}

	lua_State *thread = lua_tothread(L, -1);
	lua_pop(L, 1);
	return thread;
}

lua_State *luax_getpinnedthread(lua_State *L)
{
	lua_getfield(L, LUA_REGISTRYINDEX, MAIN_THREAD_KEY);
	lua_State *thread = lua_tothread(L, -1);
	lua_pop(L, 1);
	return thread;
}

extern "C" int luax_typerror(lua_State* L, int narg, const char *tname)
{
	int argtype = lua_type(L, narg);
	const char *argtname = 0;

	// We want to use the moon type name for userdata, if possible.
	if (argtype == LUA_TUSERDATA && luaL_getmetafield(L, narg, "type") != 0)
	{
		lua_pushvalue(L, narg);
		if (lua_pcall(L, 1, 1, 0) == 0 && lua_type(L, -1) == LUA_TSTRING)
		{
			argtname = lua_tostring(L, -1);

			// Non-moon userdata might have a type metamethod which doesn't
			// describe its type properly, so we only use it for moon types.
			Type t;
			if (!GetTypeName(argtname, t))
				argtname = 0;
		}
	}

	if (argtname == 0)
		argtname = lua_typename(L, argtype);

	const char *msg = lua_pushfstring(L, "%s expected, got %s", tname, argtname);
	return luaL_argerror(L, narg, msg);
}

size_t luax_objlen(lua_State *L, int ndx)
{
#if LUA_VERSION_NUM == 501
	return lua_objlen(L, ndx);
#else
	return lua_rawlen(L, ndx);
#endif
}

Type luax_type(lua_State* L, int idx)
{
	Type t = INVALID_ID;
	GetTypeName(luaL_checkstring(L, idx), t);
	return t;
}

}