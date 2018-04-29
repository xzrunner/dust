#include "moon/runtime.h"
#include "moon/SceneNode.h"

#include <ee0/CompCustomProperties.h>

#include <guard/check.h>
#include <SM_Vector.h>
#include <node0/SceneNode.h>
#include <painting2/Color.h>

namespace
{

moon::SceneNode* luax_checknode(lua_State* L, int idx)
{
	return moon::luax_checktype<moon::SceneNode>(L, idx, moon::SCENE_NODE_ID);
}

int w_get_pos(lua_State* L)
{
	auto node = luax_checknode(L, 1);
	auto& pos = node->GetPosition();
	lua_pushnumber(L, pos.x);
	lua_pushnumber(L, pos.y);
	return 2;
}

int w_set_pos(lua_State* L)
{
	auto node = luax_checknode(L, 1);
	float x = static_cast<float>(luaL_optnumber(L, 2, 0));
	float y = static_cast<float>(luaL_optnumber(L, 3, 0));
	node->SetPosition(sm::vec2(x, y));
	return 0;
}

int w_get_width(lua_State* L)
{
	auto node = luax_checknode(L, 1);
	auto& rect = node->GetBounding();
	lua_pushnumber(L, rect.Width());
	return 1;
}

int w_get_height(lua_State* L)
{
	auto node = luax_checknode(L, 1);
	auto& rect = node->GetBounding();
	lua_pushnumber(L, rect.Height());
	return 1;
}

int w_get_filepath(lua_State* L)
{
	auto node = luax_checknode(L, 1);
	auto& filepath = node->GetFilepath();
	lua_pushstring(L, filepath.c_str());
	return 1;
}

int w_set_ud(lua_State* L)
{
	auto node = luax_checknode(L, 1);
	auto impl = node->GetNode();
	if (!impl->HasUniqueComp<ee0::CompCustomProperties>()) {
		return 0;
	}

	auto key = lua_tostring(L, 2);
	auto& cprop = impl->GetUniqueComp<ee0::CompCustomProperties>();
	auto& props = cprop.GetAllProp();
	for (auto& prop : props) 
	{
		if (prop.key == key) 
		{
			switch (prop.type)
			{ 
			case ee0::CompCustomProperties::PROP_BOOL:
				GD_ASSERT(lua_isboolean(L, 3), "err val");
				prop.val.m_val.bl = lua_toboolean(L, 3);
				break;
			case ee0::CompCustomProperties::PROP_INT:
				prop.val.m_val.l = static_cast<int32_t>(luaL_checkinteger(L, 3));
				break;
			case ee0::CompCustomProperties::PROP_FLOAT:
				prop.val.m_val.flt = static_cast<float>(luaL_checknumber(L, 3));
				break;
			case ee0::CompCustomProperties::PROP_STRING:
				{
					delete[] prop.val.m_val.pc;
					auto str = luaL_checkstring(L, 3);
					char* tmp = new char[strlen(str) + 1];
					strcpy(tmp, str);
					prop.val.m_val.pc = tmp;
				}
				break;
			case ee0::CompCustomProperties::PROP_VEC2:
				{
					auto vec2 = static_cast<sm::vec2*>(prop.val.m_val.pv);
					vec2->x = static_cast<float>(luaL_checknumber(L, 3));
					vec2->y = static_cast<float>(luaL_checknumber(L, 4));
				}
				break;
			case ee0::CompCustomProperties::PROP_COLOR:
				{
					auto col = static_cast<pt2::Color*>(prop.val.m_val.pv);
					col->FromRGBA(static_cast<uint32_t>(luaL_checkinteger(L, 3)));
				}
				break;
			}
			break;
		}
	}

	return 0;
}

int w_get_ud(lua_State* L)
{
	auto node = luax_checknode(L, 1);

	auto impl = node->GetNode();
	if (!impl->HasUniqueComp<ee0::CompCustomProperties>()) {
		return 0;
	}

	auto key = lua_tostring(L, 2);
	auto& cprop = impl->GetUniqueComp<ee0::CompCustomProperties>();
	auto& props = cprop.GetAllProp();
	for (auto& prop : props) 
	{
		if (prop.key == key) 
		{
			int ret = 0;
			switch (prop.type)
			{
			case ee0::CompCustomProperties::PROP_BOOL:
				lua_pushboolean(L, prop.val.m_val.bl);
				ret = 1;
				break;
			case ee0::CompCustomProperties::PROP_INT:
				lua_pushinteger(L, prop.val.m_val.l);
				ret = 1;
				break;
			case ee0::CompCustomProperties::PROP_FLOAT:
				lua_pushnumber(L, prop.val.m_val.flt);
				ret = 1;
				break;
			case ee0::CompCustomProperties::PROP_STRING:
				lua_pushstring(L, prop.val.m_val.pc);
				ret = 1;
				break;
			case ee0::CompCustomProperties::PROP_VEC2:
				{
					auto vec2 = static_cast<sm::vec2*>(prop.val.m_val.pv);
					lua_pushnumber(L, vec2->x);
					lua_pushnumber(L, vec2->y);
					ret = 2;
				}
				break;
			case ee0::CompCustomProperties::PROP_COLOR:
				{
					auto col = static_cast<pt2::Color*>(prop.val.m_val.pv);
					lua_pushinteger(L, col->ToRGBA());
					ret = 1;
				}
				break;
			}
			return ret;
		}
	}

	return 0;
}

}
namespace moon
{

static const luaL_Reg w_functions[] =
{
	{ "get_pos", w_get_pos },
	{ "set_pos", w_set_pos },

	{ "get_width", w_get_width },
	{ "get_height", w_get_height },	

	{ "get_filepath", w_get_filepath },

	{ "set_ud", w_set_ud },
	{ "get_ud", w_get_ud },

	{ 0, 0 }
};

extern "C" int luaopen_scene_node(lua_State* L)
{
	return luax_register_type(L, SCENE_NODE_ID, "SceneNode", w_functions, nullptr);
}

}