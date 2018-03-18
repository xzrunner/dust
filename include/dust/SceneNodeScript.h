#pragma once

#include <node0/typedef.h>

#include <string>

struct lua_State;

namespace dust
{

class SceneNodeScript
{
public:
	void OnLoad() const;
	void OnUpdate() const;
	void OnDraw() const;

	int LoadScript(const std::string& filepath, const n0::SceneNodePtr& node);

private:
	void CallFunc(const char* func_name) const;

}; // SceneNodeScript

}