#pragma once

#include "moon/Object.h"

#include <SM_Vector.h>
#include <SM_Matrix2D.h>
#include <SM_Rect.h>
#include <node0/typedef.h>

#include <string>

namespace ur2 {
    class Device;
    class Context;
    struct RenderState;
}

namespace moon
{

class SceneNode : public Object
{
public:
	SceneNode(const n0::SceneNodePtr node);
	SceneNode(const ur2::Device& dev, const std::string& filepath);

	void Draw(const ur2::Device& dev, ur2::Context& ctx,
        ur2::RenderState& rs, const sm::Matrix2D& mt) const;

	const sm::vec2& GetPosition() const;
	void SetPosition(const sm::vec2& pos);

	const sm::rect& GetBounding() const;
	void SetSize(float w, float h);

	const std::string& GetFilepath() const;

	const n0::SceneNodePtr& GetNode() const { return m_node; }

private:
	n0::SceneNodePtr m_node = nullptr;

}; // SceneNode

}