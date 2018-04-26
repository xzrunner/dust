#include "moon/Blackboard.h"

namespace moon
{

CU_SINGLETON_DEFINITION(Blackboard);

Blackboard::Blackboard()
	: m_wnd(nullptr)
{
}

}