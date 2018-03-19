#include "dust/ModuleMgr.h"

#include <guard/check.h>

namespace dust
{

ModuleMgr::~ModuleMgr()
{
	for (auto& module : m_modules) {
		delete module;
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