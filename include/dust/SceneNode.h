#pragma once

#include "dust/Object.h"

#include <node0/typedef.h>

namespace dust
{

class SceneNode : public Object
{
public:
	SceneNode(const n0::SceneNodePtr node) 
		: node(node) {}

	n0::SceneNodePtr node = nullptr;

}; // SceneNode

}