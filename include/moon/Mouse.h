#pragma once

#include "moon/Module.h"

#include <SM_Vector.h>

namespace moon
{

class Mouse : public Module
{
public:
//	Mouse();

	virtual ModuleTypeID TypeID() const override { 
		return GetModuleTypeID<Mouse>();
	}
	virtual const char* GetName() const override {
		return "moon.mouse"; 
	}

	const sm::vec2& GetPosition() const;

}; // Mouse

}
