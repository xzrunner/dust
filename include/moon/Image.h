#pragma once

#include "moon/Object.h"

#include <SM_Matrix2D.h>
#include <facade/Image.h>

namespace ur {
    class Device;
    class Context;
    struct RenderState;
}

namespace moon
{

class Image : public Object
{
public:
	Image(const ur::Device& dev, const std::string& filepath);

	void Draw(const ur::Device& dev, ur::Context& ctx,
        ur::RenderState& rs, const sm::Matrix2D& mt) const;

	auto& GetImage() const { return m_img; }

private:
	std::shared_ptr<facade::Image> m_img = nullptr;

}; // Image

}