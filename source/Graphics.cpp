#include "moon/Graphics.h"

#include <painting2/RenderSystem.h>
#include <painting2/RenderColorCommon.h>
#include <painting2/Text.h>
#include <painting2/Blackboard.h>
#include <painting2/WindowContext.h>
#include <tessellation/Painter.h>

namespace
{

const pt2::Textbox DEFAULT_TB;

}

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
	//tess::Painter pt;
	//pt.AddPolyline((sm::vec2*)coords, count / 2, m_shader_color.ToABGR(), m_line_width);
	//pt2::RenderSystem::DrawPainter(pt);
}

void Graphics::Rectangle(DrawMode mode, float x, float y, float w, float h)
{
	//sm::vec2 min(x, y);
	//sm::vec2 max(x + w, y + h);

	//tess::Painter pt;
	//switch (mode)
	//{
	//case DRAW_LINE:
	//	pt.AddRect(min, max, m_shader_color.ToABGR(), m_line_width);
	//	break;
	//case DRAW_FILL:
	//	pt.AddRectFilled(min, max, m_shader_color.ToABGR());
	//	break;
	//}
	//pt2::RenderSystem::DrawPainter(pt);
}

void Graphics::Circle(DrawMode mode, float x, float y, float radius, int points)
{
	//tess::Painter pt;
	//switch (mode)
	//{
	//case DRAW_LINE:
	//	pt.AddCircle(sm::vec2(x, y), radius, m_shader_color.ToABGR(), m_line_width);
	//	break;
	//case DRAW_FILL:
	//	pt.AddCircleFilled(sm::vec2(x, y), radius, m_shader_color.ToABGR());
	//	break;
	//}
	//pt2::RenderSystem::DrawPainter(pt);
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

void Graphics::SetColor(const pt0::Color& color)
{
	m_shader_color = color;
}

const pt0::Color& Graphics::GetColor() const
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
	//auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	//ur_rc.SetClearColor((m_background_color.a << 24) |
	//	                (m_background_color.r << 16) |
	//	                (m_background_color.g << 8) |
	//	                (m_background_color.b));
	//ur_rc.Clear();
}

void Graphics::SetLineWidth(float width)
{
	m_line_width = width;
}

void Graphics::Print(const char* str, const sm::Matrix2D& mt, const pt2::RenderColorCommon& col) const
{
	pt2::RenderSystem::DrawText(str, DEFAULT_TB, mt, col.mul, col.add);
}

int Graphics::GetWidth() const
{
	//auto& wnd_ctx = pt2::Blackboard::Instance()->GetWindowContext();
	//return wnd_ctx->GetScreenWidth();

    return 0;
}

int Graphics::GetHeight() const
{
	//auto& wnd_ctx = pt2::Blackboard::Instance()->GetWindowContext();
	//return wnd_ctx->GetScreenHeight();

    return 0;
}

}