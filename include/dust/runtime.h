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

#pragma once

#include "dust/types.h"
#include "dust/Object.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

namespace dust
{

class Module;

/**
 * Registries represent special tables which can be accessed with
 * luax_insistregistry and luax_getregistry.
 **/
enum Registry
{
	REGISTRY_MODULES,
	REGISTRY_OBJECTS
};

/**
 * This structure wraps all Lua-exposed objects. It exists in the
 * Lua state as a full userdata (so we can catch __gc "events"),
 * though the Object it refers to is light userdata in the sense
 * that it is not allocated by the Lua VM.
 **/
struct Proxy
{
	// Holds type information (see types.h).
	Type type;

	// Pointer to the actual object.
	Object* object;
};

/**
 * A Module with Lua wrapper functions and other data.
 **/
struct WrappedModule
{
	// The module containing the functions.
	Module* module;

	// The name for the table to put the functions in, without the 'dust'-prefix.
	const char* name;

	// The type of this module.
	dust::Type type;

	// The functions of the module (last element {0,0}).
	const luaL_Reg* functions;

	// A list of functions which expose the types of the modules (last element 0).
	const lua_CFunction* types;
};

/**
* Pushes a bool onto the stack. It's the same as lua_pushboolean,
* but with bool instead of int.
* @param L The Lua state.
* @param b The bool to push.
**/
void luax_pushboolean(lua_State *L, bool b);

/**
* Convert the value at the specified index to an Lua number, and then
* convert to a float.
*
* @param L The Lua state.
* @param idx The index on the stack.
*/
inline float luax_tofloat(lua_State *L, int idx)
{
	return static_cast<float>(lua_tonumber(L, idx));
}

/**
* Like luax_tofloat, but checks that the value is a number.
*
* @see luax_tofloat
*/
inline float luax_checkfloat(lua_State *L, int idx)
{
	return static_cast<float>(luaL_checknumber(L, idx));
}

/**
* Registers all functions in the array l (see luaL_Reg) into the table at the
* top of the stack.
* Similar to Lua 5.2's luaL_setfuncs without the upvalues, and to Lua 5.1's
* luaL_register without the library name.
**/
void luax_setfuncs(lua_State *L, const luaL_Reg *l);

/**
* Register a module in the dust table. The dust table will be created if it does not exist.
* NOTE: The module-object is expected to have a +1 reference count before calling
* this function, as it doesn't retain the object itself but Lua will release it
* upon garbage collection.
* @param L The Lua state.
**/
int luax_register_module(lua_State* L, const WrappedModule& m);

/**
 * Inserts a module with 'name' into the package.preloaded table.
 * @param f The function to be called when the module is opened.
 * @param name The name of the module, with 'dust'-prefix, for instance 'dust.graphics'.
 **/
int luax_preload(lua_State* L, lua_CFunction f, const char* name);

/**
* Register a new type.
* @param type The type.
* @param name The type's human-readable name
* @param ... The list of lists of member functions for the type. (of type luaL_Reg*)
**/
int luax_register_type(lua_State *L, dust::Type type, const char *name, ...);

/**
* Pushes a Lua representation of the given object onto the stack, creating and
* storing the Lua representation in a weak table if it doesn't exist yet.
* NOTE: The object will be retained by Lua and released upon garbage collection.
* @param L The Lua state.
* @param type The type information of the object.
* @param object The pointer to the actual object.
**/
void luax_pushtype(lua_State *L, const Type type, Object *object);

/**
 * Creates a new Lua representation of the given object *without* checking if it
 * exists yet, and *without* storing it in a weak table.
 * This should only be used when performance is an extreme concern and the
 * object is not ever expected to be pushed to Lua again, as it prevents the
 * Lua-side objects from working in some cases when used as keys in tables.
 * NOTE: The object will be retained by Lua and released upon garbage collection.
 * @param L The Lua state.
 * @param type The type information of the object.
 * @param object The pointer to the actual object.
 **/
void luax_rawnewtype(lua_State *L, Type type, Object *object);

/**
* 'Insist' that a table 'k' exists in the table at idx. Insistence involves that the
* table (k) is created if it does not exist in the table at idx. The table at idx must
* pre-exist, however. Also note that if the a non-table value exists at the specified
* location, it will be overwritten with a new table. The insisted table, and only the
* insisted table, will be placed on top of the stack.
*
* @param idx The index on the stack containing a table.
* @param k The name of the table we are insisting exist.
**/
int luax_insist(lua_State *L, int idx, const char *k);

/**
* Insist that a global table 'k' exists. See luax_insist.
* @param k The name of the table we are insisting exist.
**/
int luax_insistglobal(lua_State *L, const char *k);

/**
* Insists that a table 'k' exists inside the 'dust' table. See luax_insist.
* @param k The name of the table we are insisting exist.
**/
int luax_insistdust(lua_State *L, const char *k);

/**
* Pushes the table 'k' in the dust table onto the stack. Pushes nil if the
* table doesn't exist.
* @param k The name of the table we want to get.
**/
int luax_getdust(lua_State *L, const char *k);

/**
* Gets (creates if needed) the specified Registry, and pushes it into the
* stack.
* @param L The Lua state.
* @param r The Registry to get.
**/
int luax_insistregistry(lua_State *L, Registry r);

/**
* Gets the specified Registry, and pushes it onto the stack. Pushes nil if the
* registry hasn't been created (see luax_insistregistry.)
* @param L The Lua state.
* @param r The Registry to get.
**/
int luax_getregistry(lua_State *L, Registry r);

extern "C" { // Also called from luasocket
	int luax_typerror(lua_State *L, int narg, const char *tname);
}

/**
 * Like luax_totype, but causes an error if the value at idx is not Proxy,
 * or is not the specified type.
 * @param L The Lua state.
 * @param idx The index on the stack.
 * @param type The type bit.
 **/
template <typename T>
T *luax_checktype(lua_State *L, int idx, Type type)
{
	if (lua_type(L, idx) != LUA_TUSERDATA)
	{
		const char *name = "Invalid";
		GetTypeName(type, name);
		luax_typerror(L, idx, name);
	}

	Proxy *u = (Proxy *)lua_touserdata(L, idx);

	if (u->type <= INVALID_ID || u->type >= TYPE_MAX_ENUM || !TypeFlags[u->type][type])
	{
		const char *name = "Invalid";
		GetTypeName(type, name);
		luax_typerror(L, idx, name);
	}

	return (T *)u->object;
}

Type luax_type(lua_State *L, int idx);

/**
 * Converts any exceptions thrown by the passed lambda function into a Lua error.
 * lua_error (and luaL_error) cannot be called from inside the exception handler
 * because they use longjmp, which causes undefined behaviour when the
 * destructor of the exception would have been called.
 **/
template <typename T>
int luax_catchexcept(lua_State *L, const T& func)
{
	bool should_error = false;

	try
	{
		func();
	}
	catch (const std::exception &e)
	{
		should_error = true;
		lua_pushstring(L, e.what());
	}

	if (should_error)
		return luaL_error(L, "%s", lua_tostring(L, -1));

	return 0;
}

template <typename T, typename F>
int luax_catchexcept(lua_State *L, const T& func, const F& finallyfunc)
{
	bool should_error = false;

	try
	{
		func();
	}
	catch (const std::exception &e)
	{
		should_error = true;
		lua_pushstring(L, e.what());
	}

	finallyfunc(should_error);

	if (should_error)
		return luaL_error(L, "%s", lua_tostring(L, -1));

	return 0;
}

}