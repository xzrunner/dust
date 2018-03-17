#include "dust/Graphics.h"

#include <painting2/PrimitiveDraw.h>

namespace dust
{

StringMap<Graphics::DrawMode, Graphics::DRAW_MAX_ENUM>::Entry Graphics::m_draw_mode_entries[] =
{
	{ "line", DRAW_LINE },
	{ "fill", DRAW_FILL },
};

StringMap<Graphics::DrawMode, Graphics::DRAW_MAX_ENUM> Graphics::m_draw_modes(
	Graphics::m_draw_mode_entries, sizeof(Graphics::m_draw_mode_entries));

void Graphics::Polyline(const float* coords, size_t count)
{
	pt2::PrimitiveDraw::Polyline(nullptr, coords, count, false);
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

bool Graphics::GetConstant(const char *in, DrawMode &out)
{
	return m_draw_modes.find(in, out);
}

bool Graphics::GetConstant(DrawMode in, const char *&out)
{
	return m_draw_modes.find(in, out);
}

}