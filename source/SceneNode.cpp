#include "moon/SceneNode.h"

#include <node0/SceneNode.h>
#include <node0/CompIdentity.h>
#include <node2/RenderSystem.h>
#include <node2/CompTransform.h>
#include <node2/CompBoundingBox.h>
#include <ns/NodeFactory.h>

namespace moon
{

SceneNode::SceneNode(const n0::SceneNodePtr node)
	: m_node(node)
{
}

SceneNode::SceneNode(const std::string& filepath)
{
	m_node = ns::NodeFactory::Create(filepath);
}

void SceneNode::Draw(const sm::Matrix2D& mt) const
{
	n2::RenderParams rp;
	rp.SetMatrix(mt);
	rp.QuadBaseLeftTop(true);
	n2::RenderSystem::Draw(m_node, rp);
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

const sm::rect& SceneNode::GetBounding() const
{
	return m_node->GetUniqueComp<n2::CompBoundingBox>().GetSize();
}

const std::string& SceneNode::GetFilepath() const
{
	return m_node->GetUniqueComp<n0::CompIdentity>().GetFilepath();
}

}