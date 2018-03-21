#include "moon/SceneGraph.h"
#include "moon/Module.h"
#include "moon/runtime.h"
#include "moon/Blackboard.h"
#include "moon/Context.h"
#include "moon/SceneNode.h"

#include <boost/filesystem.hpp>

namespace
{

#define INSTANCE() (moon::Blackboard::Instance()->GetContext()->GetModuleMgr().GetModule<moon::SceneGraph>(moon::Module::M_SCENE_GRAPH))

int w_new_scene_node(lua_State* L)
{
	const char* filepath = luaL_checkstring(L, 1);
	auto& work_dir = moon::Blackboard::Instance()->GetWorkDir();
	auto real_path = boost::filesystem::absolute(filepath, work_dir);

	moon::SceneNode* node = nullptr;
	moon::luax_catchexcept(L, [&]() { node = new moon::SceneNode(real_path.string()); });
	moon::luax_pushtype(L, moon::SCENE_NODE_ID, node);
	node->Release();

	return 1;
}

}

namespace moon
{

// List of functions to wrap.
static const luaL_Reg functions[] =
{
	{ "new_scene_node", w_new_scene_node },
	{ 0, 0 }
};

extern "C" int luaopen_scene_node(lua_State* L);

// Types for this module.
static const lua_CFunction types[] =
{
	luaopen_scene_node,
	0
};

extern "C" int luaopen_moon_scene_graph(lua_State* L)
{
	SceneGraph* instance = INSTANCE();
	if (instance == nullptr) {
		luax_catchexcept(L, [&](){ instance = new SceneGraph(); });
	} else {
		instance->Retain();
	}

	WrappedModule w;
	w.module = instance;
	w.name = "scene_graph";
	w.type = MODULE_SCENE_GRAPH_ID;
	w.functions = functions;
	w.types = types;

	return luax_register_module(L, w);
}

}