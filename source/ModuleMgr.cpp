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

	auto type = module->GetModuleType();
	GD_ASSERT(!m_modules[type], "module exists");
	m_modules[type] = module;
}

void ModuleMgr::UnregisterModule(Module* module)
{
}

}