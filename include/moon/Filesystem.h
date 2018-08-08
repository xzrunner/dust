#pragma once

#include "moon/Module.h"

#include <vector>

namespace moon
{

class Filesystem : public Module
{
public:
	virtual ModuleTypeID TypeID() const override { 
		return GetModuleTypeID<Filesystem>();
	}
	virtual const char* GetName() const override {
		return "moon.filesystem"; 
	}

	void GetDirectoryItems(const char* dir, std::vector<std::string>& items) const;

	bool IsDirectory(const char* dir) const;

}; // Filesystem

}