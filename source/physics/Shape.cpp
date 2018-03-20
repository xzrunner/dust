/**
 * Copyright (c) 2006-2016 LOVE Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

#include "moon/physics/box2d/Shape.h"

namespace moon
{
namespace physics
{

Shape::~Shape()
{
}

bool Shape::getConstant(const char *in, Type &out)
{
	return types.find(in, out);
}

bool Shape::getConstant(Type in, const char  *&out)
{
	return types.find(in, out);
}

StringMap<Shape::Type, Shape::SHAPE_MAX_ENUM>::Entry Shape::typeEntries[] =
{
	{"circle", Shape::SHAPE_CIRCLE},
	{"polygon", Shape::SHAPE_POLYGON},
	{"edge", Shape::SHAPE_EDGE},
	{"chain", Shape::SHAPE_CHAIN},
};

StringMap<Shape::Type, Shape::SHAPE_MAX_ENUM> Shape::types(Shape::typeEntries, sizeof(Shape::typeEntries));

} // physics
} // moon
