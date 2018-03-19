#pragma once

#include "dust/Module.h"

#include <array>

namespace dust
{

class ModuleMgr
{
public:
	~ModuleMgr();

	void RegisterModule(Module* module);
	void UnregisterModule(Module* module);

	template <typename T>
	T* GetModule(Module::ModuleType type) {
		return (T*)m_modules[type];
	}

private:
	std::array<Module*, Module::M_MAX_ENUM> m_modules = {};

}; // ModuleMgr

}