#pragma once

#include "moon/Module.h"

namespace moon
{

class SceneGraph : public Module
{
public:
	virtual ModuleType GetModuleType() const { return M_SCENE_GRAPH; }
	virtual const char* GetName() const { return "moon.scenegraph"; }

}; // SceneGraph

}