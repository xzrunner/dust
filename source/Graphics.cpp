#include "moon/Graphics.h"

#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>
#include <painting2/PrimitiveDraw.h>
#include <painting2/RenderSystem.h>
#include <painting2/RenderColorCommon.h>
#include <painting2/Text.h>
#include <painting2/Blackboard.h>
#include <painting2/WindowContext.h>

namespace moon
{

StringMap<Graphics::DrawMode, Graphics::DRAW_MAX_ENUM>::Entry Graphics::m_draw_mode_entries[] =
{
	{ "line", DRAW_LINE },
	{ "fill", DRAW_FILL },
};

StringMap<Graphics::DrawMode, Graphics::DRAW_MAX_ENUM> Graphics::m_draw_modes(
	Graphics::m_draw_mode_entries, sizeof(Graphics::m_draw_mode_entries));

Graphics::Graphics()
	: m_background_color(0x88, 0x88, 0x88, 0x88)
{
}

void Graphics::Polyline(const float* coords, size_t count)
{
	pt2::PrimitiveDraw::Polyline(nullptr, coords, count / 2, false);
}

void Graphics::Rectangle(DrawMode mode, float x, float y, float w, float h)
{
	sm::rect rect;
	rect.xmin = x;
	rect.ymin = y;
	rect.xmax = rect.xmin + w;
	rect.ymax = rect.ymin + h;

	pt2::PrimitiveDraw::Rect(nullptr, rect, mode == DRAW_FILL);
}

void Graphics::Circle(DrawMode mode, float x, float y, float radius, int points)
{
	pt2::PrimitiveDraw::Circle(nullptr, sm::vec2(x, y), radius, mode == DRAW_FILL, points);
}

void Graphics::Polygon(DrawMode mode, const float* coords, size_t count)
{
	// coords is an array of a closed loop of vertices, i.e.
	// coords[count-2] = coords[0], coords[count-1] = coords[1]
	if (mode == DRAW_LINE)
	{
		Polyline(coords, count);
	}
}

void Graphics::SetColor(const pt2::Color& color)
{
	m_shader_color = color;

	pt2::RenderColorCommon rc;
	rc.mul = color;
	pt2::RenderSystem::SetColor(rc);

	pt2::PrimitiveDraw::SetColor(color);
}

const pt2::Color& Graphics::GetColor() const
{
	return m_shader_color;
}

bool Graphics::GetConstant(const char *in, DrawMode &out)
{
	return m_draw_modes.find(in, out);
}

bool Graphics::GetConstant(DrawMode in, const char *&out)
{
	return m_draw_modes.find(in, out);
}

void Graphics::ClearColor()
{
	ur::Blackboard::Instance()->GetRenderContext().Clear(
		(m_background_color.a << 24) | 
		(m_background_color.r << 16) | 
		(m_background_color.g << 8)  | 
		(m_background_color.b)
	);
}

void Graphics::SetLineWidth(float width)
{
	pt2::PrimitiveDraw::LineWidth(width);
}

void Graphics::Print(const char* str, const sm::Matrix2D& mt) const
{
	pt2::Text text;
	text.text = str;
	pt2::RenderSystem::DrawText(text, mt);
}

int Graphics::GetWidth() const
{
	auto& wnd_ctx = pt2::Blackboard::Instance()->GetWindowContext();
	return wnd_ctx->GetScreenWidth();
}

int Graphics::GetHeight() const
{
	auto& wnd_ctx = pt2::Blackboard::Instance()->GetWindowContext();
	return wnd_ctx->GetScreenHeight();
}

}