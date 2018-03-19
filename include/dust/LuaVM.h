#pragma once

#include "dust/ModuleMgr.h"

struct lua_State;

namespace dust
{

class LuaVM
{
public:
	LuaVM();
	~LuaVM();

	void DoString(const char* str);
	void DoFile(const char* filename);

	lua_State* GetState() const { return L; }

	ModuleMgr& GetModuleMgr() { return m_module_mgr; }

private:
	lua_State* L;

	ModuleMgr m_module_mgr;

}; // LuaVM

}