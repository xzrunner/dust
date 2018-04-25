#pragma once

#include "moon/Module.h"

#include <node0/typedef.h>

#include <vector>

namespace moon
{

class SceneGraph : public Module
{
public:
	virtual ModuleType GetModuleType() const { return M_SCENE_GRAPH; }
	virtual const char* GetName() const { return "moon.scene"; }

	void SetSelection(const std::vector<n0::SceneNodePtr>& selection) { m_selection = selection; }
	const std::vector<n0::SceneNodePtr>& GetSelectioin() const { return m_selection; }

	const n0::SceneNodePtr& GetRoot() const { return m_root; }
	void SetRoot(const n0::SceneNodePtr& root) { m_root = root; }
	
private:
	std::vector<n0::SceneNodePtr> m_selection;

	n0::SceneNodePtr m_root = nullptr;

}; // SceneGraph

}