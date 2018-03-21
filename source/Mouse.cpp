#include "moon/Mouse.h"
#include "moon/Blackboard.h"

namespace moon
{

const sm::vec2& Mouse::GetPosition() const
{
	return Blackboard::Instance()->GetMousePos();
}

}