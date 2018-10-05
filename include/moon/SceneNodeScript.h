#pragma once

#include <node0/typedef.h>

#include <string>

struct lua_State;

namespace moon
{

class SceneNodeScript
{
public:
	void Init() const;
	void Update() const;
	void Draw() const;

	int Reload(const std::string& filepath, const n0::SceneNodePtr& node);

	int AddFunc(const std::string& name, const std::string& body) const;
	void CallMouseFunc(const char* func_name, float x, float y) const;

private:
	void CallFunc(const char* func_name, int n_params = 0,
		std::function<void(lua_State* L)> pass_params_func = nullptr) const;


}; // SceneNodeScript

}