#include "moon/Mouse.h"
#include "moon/Blackboard.h"

namespace moon
{

sm::vec2 Mouse::GetPosition() const
{
	return Blackboard::Instance()->GetMousePos();
}

}