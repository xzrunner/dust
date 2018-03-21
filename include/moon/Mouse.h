#pragma once

#include "moon/Module.h"

#include <SM_Vector.h>

namespace moon
{

class Mouse : public Module
{
public:
//	Mouse();

	virtual ModuleType GetModuleType() const { return M_MOUSE; }
	virtual const char* GetName() const { return "moon.mouse"; }

	const sm::vec2& GetPosition() const;

}; // Mouse

}
