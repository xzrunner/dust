#pragma once

#include <string>

struct lua_State;

namespace moon
{

class Context;

class StagePageScript
{
public:
	StagePageScript(lua_State* L, const std::string& filepath);

	void OnLoad() const;
	void OnUpdate() const;
	void OnDraw() const;

	void OnMousePressed(int x, int y, int button) const;
	void OnMouseReleased(int x, int y, int button) const;

	void OnKeyPressed(const char* key) const;

	int LoadScript();

private:
	void RegistFunc(const char* name, const char* key);

private:
	std::string m_filepath;

	lua_State* L;

	mutable bool m_loaded;

}; // StagePageScript

}