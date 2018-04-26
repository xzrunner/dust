#pragma once

#include "moon/Module.h"

namespace moon
{

class Gui : public Module
{
public:
	virtual ModuleType GetModuleType() const { return M_GUI; }
	virtual const char* GetName() const { return "moon.gui"; }

}; // Gui

}