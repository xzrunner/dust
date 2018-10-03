#pragma once

#include "moon/Object.h"

#include <SM_Matrix2D.h>
#include <facade/Image.h>

namespace moon
{

class Image : public Object
{
public:
	Image(const std::string& filepath);

	void Draw(const sm::Matrix2D& mt) const;

	auto& GetImage() const { return m_img; }

private:
	std::shared_ptr<facade::Image> m_img = nullptr;

}; // Image

}