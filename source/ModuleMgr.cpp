#include "moon/ModuleMgr.h"

#include <guard/check.h>

namespace moon
{

ModuleMgr::~ModuleMgr()
{
	for (auto& module : m_modules) {
		module->Release();
	}
}

void ModuleMgr::RegisterModule(Module* module)
{
	GD_ASSERT(module, "null module");

	ModuleTypeID id = module->TypeID();
	if (m_modules.size() <= id) {
		m_modules.resize(id + 1);
	} else {
		GD_ASSERT(!m_modules[id], "module exists");
	}

	m_modules[id] = module;
}

void ModuleMgr::UnregisterModule(Module* module)
{
}

}