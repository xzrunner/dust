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

	void SetWorkDir(const std::string& dir) { m_work_dir = dir; }
	const std::string& GetWorkDir() const { return m_work_dir; }

	void Init();

private:
	lua_State* L;

	ModuleMgr m_module_mgr;

	std::string m_work_dir;

}; // Context

}