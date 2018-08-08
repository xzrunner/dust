#pragma once

#include "moon/Module.h"

#include <node0/typedef.h>

#include <vector>
#include <functional>

namespace moon
{

class SceneGraph : public Module
{
public:
	using TRAVERSE_FUNC = std::function<bool(const n0::SceneNodePtr&)>;

public:
	virtual ModuleTypeID TypeID() const override { 
		return GetModuleTypeID<SceneGraph>();
	}
	virtual const char* GetName() const override { 
		return "moon.scene"; 
	}

	void SetTraverseAllNodesFunc(std::function<void(TRAVERSE_FUNC)> func) { m_trav_all_node = func; }
	void SetTraverseSelectionFunc(std::function<void(TRAVERSE_FUNC)> func) { m_trav_selection = func; }
	void SetSetSelectionFunc(std::function<void(const std::vector<n0::SceneNodePtr>&)> func) { m_set_selection = func; }

	void TraverseAllNodes(std::function<bool(const n0::SceneNodePtr&)> func) { m_trav_all_node(func); }
	void TraverseSelection(std::function<bool(const n0::SceneNodePtr&)> func) { m_trav_selection(func); }
	void SetSelection(const std::vector<n0::SceneNodePtr>& nodes) { m_set_selection(nodes); }

private:
	std::function<void(TRAVERSE_FUNC)> m_trav_all_node;
	std::function<void(TRAVERSE_FUNC)> m_trav_selection;

	std::function<void(const std::vector<n0::SceneNodePtr>&)> m_set_selection;

}; // SceneGraph

}