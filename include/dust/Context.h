#pragma once

#include "dust/ModuleMgr.h"

struct lua_State;

namespace dust
{

class Context
{
public:
	Context();
	~Context();

	lua_State* GetState() const { return L; }

	ModuleMgr& GetModuleMgr() { return m_module_mgr; }

	void Init();

private:
	lua_State* L;

	ModuleMgr m_module_mgr;

}; // Context

}