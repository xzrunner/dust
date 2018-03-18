#pragma once

#include "dust/Module.h"

namespace dust
{

class SceneGraph : public Module
{
public:
	virtual ModuleType GetModuleType() const { return M_SCENE_GRAPH; }
	virtual const char* GetName() const { return "dust.scenegraph"; }

}; // SceneGraph

}