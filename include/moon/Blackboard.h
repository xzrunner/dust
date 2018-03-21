#pragma once

#include <cu/cu_macro.h>
#include <SM_Vector.h>

#include <memory>
#include <string>

namespace moon
{

class Context;

class Blackboard
{
public:
	void SetContext(const std::shared_ptr<Context>& ctx) { m_ctx = ctx; }
	const std::shared_ptr<Context>& GetContext() const { return m_ctx; }

	void SetMousePos(const sm::vec2& mouse_pos) { m_mouse_pos = mouse_pos; }
	const sm::vec2& GetMousePos() const { return m_mouse_pos; }

	void SetWorkDir(const std::string& work_dir) { m_work_dir = work_dir; }
	const std::string& GetWorkDir() const { return m_work_dir; }
	
private:
	std::shared_ptr<Context> m_ctx = nullptr;

	sm::vec2 m_mouse_pos;

	std::string m_work_dir;

	CU_SINGLETON_DECLARATION(Blackboard);

}; // Blackboard

}