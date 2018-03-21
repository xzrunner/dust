#pragma once

#include "moon/Module.h"

#include <vector>

namespace moon
{

class Filesystem : public Module
{
public:
	virtual ModuleType GetModuleType() const { return M_FILESYSTEM; }
	virtual const char* GetName() const { return "moon.filesystem"; }

	void GetDirectoryItems(const char* dir, std::vector<std::string>& items) const;

	bool IsDirectory(const char* dir) const;

}; // Filesystem

}