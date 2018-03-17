#pragma once

struct lua_State;

namespace dust
{

class LuaVM
{
public:
	LuaVM();
	~LuaVM();

	void DoString(const char* str);
	void DoFile(const char* filename);

	lua_State* GetState() const { return L; }

private:
	lua_State* L;

}; // LuaVM

}