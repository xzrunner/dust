#include "moon/SceneGraph.h"
#include "moon/SceneNode.h"
#include "moon/Module.h"
#include "moon/runtime.h"
#include "moon/Blackboard.h"
#include "moon/Context.h"
#include "moon/SceneNode.h"
#include "moon/runtime.h"

#include <ee0/CompNodeEditor.h>

#include <node0/SceneNode.h>
#include <node2/CompComplex.h>

#include <boost/filesystem.hpp>

namespace
{

#define INSTANCE() (moon::Blackboard::Instance()->GetContext()->GetModuleMgr().GetModule<moon::SceneGraph>(moon::Module::M_SCENE_GRAPH))

int w_get_selected_nodes(lua_State* L)
{
	lua_newtable(L);

	auto& selection = INSTANCE()->GetSelectioin();
	for (int i = 0, n = selection.size(); i < n; ++i) 
	{
		moon::SceneNode* node = nullptr;
		moon::luax_catchexcept(L, [&]() { 
			node = new moon::SceneNode(selection[i]); 
		});
		moon::luax_pushtype(L, moon::SCENE_NODE_ID, node);
		node->Release();
		lua_rawseti(L, -2, i + 1);
	}

	return 1;
}

int w_get_node(lua_State* L)
{
	auto& root = INSTANCE()->GetRoot();
	if (!root->HasSharedComp<n2::CompComplex>()) {
		return luaL_error(L, "root node is not complex.");
	}
	
	auto& nodes = root->GetSharedComp<n2::CompComplex>().GetAllChildren();
	if (lua_isstring(L, 1)) 
	{
		const char* name = lua_tostring(L, 1);
		for (auto& node : nodes) 
		{
			auto& ceditor = node->GetUniqueComp<ee0::CompNodeEditor>();
			if (ceditor.GetName() == name) 
			{
				moon::SceneNode* w_node = nullptr;
				moon::luax_catchexcept(L, [&]() {
					w_node = new moon::SceneNode(node);
				});
				moon::luax_pushtype(L, moon::SCENE_NODE_ID, w_node);
				w_node->Release();
				return 1;
			}
		}
	} 
	else if (lua_isinteger(L, 1)) 
	{
		auto id = lua_tointeger(L, 1);
		for (auto& node : nodes)
		{
			auto& ceditor = node->GetUniqueComp<ee0::CompNodeEditor>();
			if (ceditor.GetID() == id)
			{
				moon::SceneNode* w_node = nullptr;
				moon::luax_catchexcept(L, [&]() {
					w_node = new moon::SceneNode(node);
				});
				moon::luax_pushtype(L, moon::SCENE_NODE_ID, w_node);
				w_node->Release();
				return 1;
			}
		}
	} 
	else 
	{
		return luaL_error(L, "should pass name or id.");
	}
	return 0;
}

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
	{ "get_selected_nodes", w_get_selected_nodes },
	{ "get_node", w_get_node },

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

extern "C" int luaopen_moon_scene(lua_State* L)
{
	SceneGraph* instance = INSTANCE();
	if (instance == nullptr) {
		luax_catchexcept(L, [&](){ instance = new SceneGraph(); });
	} else {
		instance->Retain();
	}

	WrappedModule w;
	w.module = instance;
	w.name = "scene";
	w.type = MODULE_SCENE_GRAPH_ID;
	w.functions = functions;
	w.types = types;

	return luax_register_module(L, w);
}

}