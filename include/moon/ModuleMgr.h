#pragma once

#include "moon/Module.h"

#include <vector>

namespace moon
{

class ModuleMgr
{
public:
	~ModuleMgr();

	void RegisterModule(Module* module);
	void UnregisterModule(Module* module);

	template <typename T>
	T* GetModule()
	{
		size_t type_id = moon::GetModuleTypeID<T>();
		if (type_id >= 0 && type_id < m_modules.size()) {
			return (T*)m_modules[type_id];
		} else {
			return nullptr;
		}
	}

private:
	std::vector<Module*> m_modules = {};

}; // ModuleMgr

}