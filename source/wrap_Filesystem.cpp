#include "moon/Filesystem.h"
#include "moon/runtime.h"
#include "moon/Module.h"
#include "moon/Blackboard.h"
#include "moon/Context.h"

#include <boost/filesystem.hpp>

namespace
{

#define INSTANCE() (moon::Blackboard::Instance()->GetContext()->GetModuleMgr().GetModule<moon::Filesystem>(moon::Module::M_FILESYSTEM))

int w_is_directory(lua_State* L)
{
	const char* dir = luaL_checkstring(L, 1);
	auto& work_dir = moon::Blackboard::Instance()->GetWorkDir();
	auto real_dir = boost::filesystem::absolute(dir, work_dir);

	moon::luax_pushboolean(L, INSTANCE()->IsDirectory(real_dir.string().c_str()));
	return 1;
}

int w_get_directory_items(lua_State* L)
{
	const char* dir = luaL_checkstring(L, 1);
	auto& work_dir = moon::Blackboard::Instance()->GetWorkDir();
	auto real_dir = boost::filesystem::absolute(dir, work_dir);

	std::vector<std::string> items;
	INSTANCE()->GetDirectoryItems(real_dir.string().c_str(), items);

	lua_createtable(L, (int)items.size(), 0);

	for (int i = 0; i < (int)items.size(); i++)
	{
		lua_pushstring(L, items[i].c_str());
		lua_rawseti(L, -2, i + 1);
	}

	// Return the table.
	return 1;
}

}

namespace moon
{

// List of functions to wrap.
static const luaL_Reg functions[] =
{
	{ "is_directory", w_is_directory },
	{ "get_directory_items", w_get_directory_items },

	{ 0, 0 }
};

// Types for this module.
static const lua_CFunction types[] =
{
	0
};

extern "C" int luaopen_moon_filesystem(lua_State* L)
{
	Filesystem* instance = INSTANCE();
	if (instance == nullptr) {
		luax_catchexcept(L, [&](){ instance = new Filesystem(); });
	} else {
		instance->Retain();
	}

	WrappedModule w;
	w.module = instance;
	w.name = "filesystem";
	w.type = MODULE_FILESYSTEM_ID;
	w.functions = functions;
	w.types = types;

	return luax_register_module(L, w);
}

}