#include "dust/runtime.h"
#include "dust/SceneNode.h"

#include <SM_Vector.h>

namespace dust
{

SceneNode* luax_checkcanvas(lua_State* L, int idx)
{
	return luax_checktype<SceneNode>(L, idx, SCENE_NODE_ID);
}

int w_get_pos(lua_State* L)
{
	auto node = luax_checkcanvas(L, 1);
	auto& pos = node->GetPosition();
	lua_pushnumber(L, pos.x);
	lua_pushnumber(L, pos.y);
	return 2;
}

int w_set_pos(lua_State* L)
{
	auto node = luax_checkcanvas(L, 1);
	float x = static_cast<float>(luaL_optnumber(L, 2, 0));
	float y = static_cast<float>(luaL_optnumber(L, 3, 0));
	node->SetPosition(sm::vec2(x, y));
	return 0;
}

static const luaL_Reg w_functions[] =
{
	{ "get_pos", w_get_pos },
	{ "set_pos", w_set_pos },
	{ 0, 0 }
};

extern "C" int luaopen_scene_node(lua_State* L)
{
	return luax_register_type(L, SCENE_NODE_ID, "SceneNode", w_functions, nullptr);
}

}