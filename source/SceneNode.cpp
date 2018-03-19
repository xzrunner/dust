#include "dust/SceneNode.h"

#include <node0/SceneNode.h>
#include <node2/RenderSystem.h>
#include <node2/CompTransform.h>
#include <ns/NodeFactory.h>

namespace dust
{

SceneNode::SceneNode(const n0::SceneNodePtr node)
	: m_node(node) 
{
}

SceneNode::SceneNode(const std::string& filepath)
{
	m_node = ns::NodeFactory::Create(filepath);
}

void SceneNode::Draw() const
{
	n2::RenderSystem::Draw(m_node);
}

const sm::vec2& SceneNode::GetPosition() const
{
	return m_node->GetUniqueComp<n2::CompTransform>()
		.GetTrans().GetPosition();
}

void SceneNode::SetPosition(const sm::vec2& pos)
{
	m_node->GetUniqueComp<n2::CompTransform>().SetPosition(
		*m_node, sm::vec2(pos.x, pos.y));
}

}