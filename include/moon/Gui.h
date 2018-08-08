#pragma once

#include "moon/Module.h"

namespace moon
{

class Gui : public Module
{
public:
	virtual ModuleTypeID TypeID() const override { 
		return GetModuleTypeID<Gui>();
	}
	virtual const char* GetName() const override { 
		return "moon.gui"; 
	}

}; // Gui

}