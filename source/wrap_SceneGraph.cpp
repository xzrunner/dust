#include "dust/SceneGraph.h"
#include "dust/Module.h"
#include "dust/runtime.h"
#include "dust/Blackboard.h"
#include "dust/Context.h"

namespace dust
{

#define INSTANCE() (Blackboard::Instance()->ctx->GetModuleMgr().GetModule<SceneGraph>(Module::M_SCENE_GRAPH))

// List of functions to wrap.
static const luaL_Reg functions[] =
{
	{ 0, 0 }
};

extern "C" int luaopen_scene_node(lua_State* L);

// Types for this module.
static const lua_CFunction types[] =
{
	luaopen_scene_node,
	0
};

extern "C" int luaopen_dust_scene_graph(lua_State* L)
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