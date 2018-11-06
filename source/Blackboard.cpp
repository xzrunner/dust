#include "moon/Blackboard.h"

#include <ee0/CameraHelper.h>
#include <ee0/WxStagePage.h>
#include <ee0/WxStageCanvas.h>

#ifdef EASYEDITOR
#include <wx/window.h>
#endif // EASYEDITOR

namespace moon
{

CU_SINGLETON_DEFINITION(Blackboard);

Blackboard::Blackboard()
{
}

sm::vec2 Blackboard::GetMousePos() const
{
#ifdef EASYEDITOR
	const wxPoint pt = wxGetMousePosition();
	int mouse_x = pt.x - m_stage->GetScreenPosition().x;
	int mouse_y = pt.y - m_stage->GetScreenPosition().y;
	return ee0::CameraHelper::TransPosScreenToProject(
		*m_stage->GetImpl().GetCanvas()->GetCamera(), mouse_x, mouse_y);
#else
	return sm::vec2();
#endif // EASYEDITOR
}

}