#pragma once

#include "moon/Module.h"
#include "moon/StringMap.h"

#include <painting2/Color.h>
#include <SM_Matrix2D.h>

namespace pt2 { class RenderColorCommon; }

namespace moon
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

	virtual ModuleTypeID TypeID() const override {
		return GetModuleTypeID<Graphics>();
	}
	virtual const char* GetName() const override {
		return "moon.graphics";
	}

	void Polyline(const float* coords, size_t count);
	void Rectangle(DrawMode mode, float x, float y, float w, float h);
	void Circle(DrawMode mode, float x, float y, float radius, int points = 10);
	void Polygon(DrawMode mode, const float* coords, size_t count);

	void SetColor(const pt2::Color& color);
	const pt2::Color& GetColor() const;
	void SetBackgroundColor(const pt2::Color& color) { m_background_color = color; }
	void ClearColor();

	void SetLineWidth(float width);

	void Print(const char* str, const sm::Matrix2D& mt,
		const pt2::RenderColorCommon& col) const;

	int GetWidth() const;
	int GetHeight() const;

	static bool GetConstant(const char *in, DrawMode &out);
	static bool GetConstant(DrawMode in, const char *&out);

private:
	static StringMap<DrawMode, DRAW_MAX_ENUM>::Entry m_draw_mode_entries[];
	static StringMap<DrawMode, DRAW_MAX_ENUM> m_draw_modes;

	pt2::Color m_shader_color;
	pt2::Color m_background_color;

}; // Graphics

}