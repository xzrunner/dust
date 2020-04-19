#pragma once

#include "moon/Object.h"

#include <SM_Matrix2D.h>
#include <facade/Image.h>

namespace ur2 {
    class Device;
    class Context;
    struct RenderState;
}

namespace moon
{

class Image : public Object
{
public:
	Image(const ur2::Device& dev, const std::string& filepath);

	void Draw(const ur2::Device& dev, ur2::Context& ctx,
        ur2::RenderState& rs, const sm::Matrix2D& mt) const;

	auto& GetImage() const { return m_img; }

private:
	std::shared_ptr<facade::Image> m_img = nullptr;

}; // Image

}