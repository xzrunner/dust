#include "moon/SceneGraph.h"
#include "moon/SceneNode.h"
#include "moon/Module.h"
#include "moon/runtime.h"
#include "moon/Blackboard.h"
#include "moon/Context.h"
#include "moon/SceneNode.h"
#include "moon/runtime.h"

#include <ee0/CompNodeEditor.h>
#include <ee0/MsgHelper.h>

#include <node0/SceneNode.h>
#include <node2/CompComplex.h>

#include <boost/filesystem.hpp>

namespace
{

#define INSTANCE() (moon::Blackboard::Instance()->GetContext()->GetModuleMgr().GetModule<moon::SceneGraph>(moon::Module::M_SCENE_GRAPH))

void return_node(lua_State* L, const n0::SceneNodePtr node)
{
	moon::SceneNode* w_node = nullptr;
	moon::luax_catchexcept(L, [&]() {
		w_node = new moon::SceneNode(node);
	});
	moon::luax_pushtype(L, moon::SCENE_NODE_ID, w_node);
	w_node->Release();
}

int w_get_node(lua_State* L)
{
	int ret = 0;
	if (lua_isstring(L, 1)) 
	{
		const char* name = lua_tostring(L, 1);
		INSTANCE()->TraverseAllNodes([&](const n0::SceneNodePtr& node)->bool 
		{
			auto& ceditor = node->GetUniqueComp<ee0::CompNodeEditor>();
			if (ceditor.GetName() == name) {
				return_node(L, node);
				ret = 1;
				return false;
			}
			return true;
		});
	} 
	else if (lua_isinteger(L, 1)) 
	{
		auto id = lua_tointeger(L, 1);
		INSTANCE()->TraverseAllNodes([&](const n0::SceneNodePtr& node)->bool 
		{
			auto& ceditor = node->GetUniqueComp<ee0::CompNodeEditor>();
			if (ceditor.GetID() == id) {
				return_node(L, node);
				ret = 1;
				return false;
			}
			return true;
		});
	} 
	else 
	{
		return luaL_error(L, "should pass name or id.");
	}
	return ret;
}

int w_get_selection(lua_State* L)
{
	lua_newtable(L);

	int idx = 0;
	INSTANCE()->TraverseSelection([&](const n0::SceneNodePtr& node)->bool
	{
		return_node(L, node);
		lua_rawseti(L, -2, idx + 1);
		++idx;
		return true;
	});

	return 1;
}

int w_get_all_nodes(lua_State* L)
{
	lua_newtable(L);

	int idx = 0;
	INSTANCE()->TraverseAllNodes([&](const n0::SceneNodePtr& node)->bool
	{
		return_node(L, node);
		lua_rawseti(L, -2, idx + 1);
		++idx;
		return true;
	});

	return 1;
}

int w_set_selection(lua_State* L)
{
	if (!lua_istable(L, 1)) {
		return 0;
	}

	int n = (int)moon::luax_objlen(L, 1);
	if (n == 0) {
		return 0;
	}

	std::vector<n0::SceneNodePtr> nodes;
	nodes.reserve(n);
	for (int i = 1; i <= n; ++i)
	{
		lua_rawgeti(L, 1, i);
		
		auto sn = moon::luax_checktype<moon::SceneNode>(L, -1, moon::SCENE_NODE_ID);
		nodes.push_back(sn->GetNode());

		lua_pop(L, 1);
	}
	INSTANCE()->SetSelection(nodes);

	return 0;
}

int w_new_node(lua_State* L)
{
	auto bb = moon::Blackboard::Instance();

	const char* filepath = luaL_checkstring(L, 1);
	auto real_path = boost::filesystem::absolute(filepath, bb->GetWorkDir());

	moon::SceneNode* node = nullptr;
	moon::luax_catchexcept(L, [&]() { node = new moon::SceneNode(real_path.string()); });
	moon::luax_pushtype(L, moon::SCENE_NODE_ID, node);
	node->Release();

	ee0::MsgHelper::InsertNode(*bb->GetSubMgr(), node->GetNode());

	return 1;
}

}

namespace moon
{

// List of functions to wrap.
static const luaL_Reg functions[] =
{
	{ "get_node", w_get_node },
	{ "get_selection", w_get_selection },
	{ "get_all_nodes", w_get_all_nodes },
	
	{ "set_selection", w_set_selection },

	{ "new_node", w_new_node },

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