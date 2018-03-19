#pragma once

#include "dust/Module.h"
#include "dust/StringMap.h"

#include <painting2/Color.h>

namespace dust
{

class Graphics : public Module
{
public:
	enum DrawMode
	{
		DRAW_LINE,
		DRAW_FILL,
		DRAW_MAX_ENUM
	};

public:
	Graphics();

	virtual ModuleType GetModuleType() const { return M_GRAPHICS; }
	virtual const char* GetName() const { return "dust.graphics"; }

	void Polyline(const float* coords, size_t count);
	void Rectangle(DrawMode mode, float x, float y, float w, float h);
	void Circle(DrawMode mode, float x, float y, float radius, int points = 10);

	void SetBackgroundColor(const pt2::Color& color) { m_background_color = color; }
	void ClearColor();

	static bool GetConstant(const char *in, DrawMode &out);
	static bool GetConstant(DrawMode in, const char *&out);

private:
	static StringMap<DrawMode, DRAW_MAX_ENUM>::Entry m_draw_mode_entries[];
	static StringMap<DrawMode, DRAW_MAX_ENUM> m_draw_modes;

	pt2::Color m_background_color;

}; // Graphics

}