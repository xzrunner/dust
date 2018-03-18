#pragma once

#include <string>

struct lua_State;

namespace dust
{

class LuaVM;

class StageCanvasScript
{
public:
	StageCanvasScript(lua_State* L, const std::string& filepath);

	void OnLoad() const;
	void OnUpdate() const;
	void OnDraw() const;

	int LoadScript();

private:
	void RegistFunc(const char* name, const char* key);

private:
	std::string m_filepath;

	lua_State* L;

}; // StageCanvasScript

}