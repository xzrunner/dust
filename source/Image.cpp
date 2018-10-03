#include "moon/Image.h"

#include <painting2/RenderSystem.h>
#include <facade/Image.h>
#include <facade/ResPool.h>

namespace moon
{

Image::Image(const std::string& filepath)
{
	m_img = facade::ResPool::Instance().Fetch<facade::Image>(filepath);
}

void Image::Draw(const sm::Matrix2D& mt) const
{
	sm::rect pos(m_img->GetWidth(), m_img->GetHeight());
	pt2::RenderSystem::DrawTexture(*m_img->GetTexture(), pos, mt);
}

}