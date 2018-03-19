#pragma once

#include <string>

struct lua_State;

namespace dust
{

class Context;

class StagePageScript
{
public:
	StagePageScript(lua_State* L, const std::string& filepath);

	void OnLoad() const;
	void OnUpdate() const;
	void OnDraw() const;

	int LoadScript();

private:
	void RegistFunc(const char* name, const char* key);

private:
	std::string m_filepath;

	lua_State* L;

}; // StagePageScript

}