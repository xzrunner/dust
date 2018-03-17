#pragma once

struct lua_State;

namespace dust
{

class LuaVM;

class StageCanvas
{
public:
	StageCanvas(lua_State* L, const char* filepath);

	void Load() const;
	void Update() const;
	void Draw() const;

private:	
	int LoadScript(const char* filepath);

	void RegistFunc(const char* name, const char* key);

private:
	lua_State* L;

}; // StageCanvas

}