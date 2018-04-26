#pragma once

#include <ee0/typedef.h>

#include <cu/cu_macro.h>
#include <SM_Vector.h>

#include <memory>
#include <string>

class wxWindow;

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

	void SetWindow(wxWindow* wnd) { m_wnd = wnd; }
	wxWindow* GetWindow() { return m_wnd; }

	void SetSubMgr(const ee0::SubjectMgrPtr& sub_mgr) { m_sub_mgr = sub_mgr; }
	const ee0::SubjectMgrPtr& GetSubMgr() { return m_sub_mgr; }
	
private:
	std::shared_ptr<Context> m_ctx = nullptr;

	sm::vec2 m_mouse_pos;

	std::string m_work_dir;

	// todo only for editor
	wxWindow* m_wnd;
	ee0::SubjectMgrPtr m_sub_mgr = nullptr;

	CU_SINGLETON_DECLARATION(Blackboard);

}; // Blackboard

}