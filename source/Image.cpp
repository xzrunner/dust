#include "moon/Image.h"

#include <painting2/RenderSystem.h>
#include <painting2/Texture.h>
#include <facade/Image.h>
#include <facade/ResPool.h>

namespace moon
{

Image::Image(const ur2::Device& dev, const std::string& filepath)
{
	m_img = facade::ResPool::Instance().Fetch<facade::Image>(filepath, &dev);
}

void Image::Draw(const ur2::Device& dev, ur2::Context& ctx,
                 ur2::RenderState& rs, const sm::Matrix2D& mt) const
{
	sm::rect pos(m_img->GetWidth(), m_img->GetHeight());
	pt2::RenderSystem::DrawTexture(dev, ctx, rs, m_img->GetTexture(), pos, mt);
}

}