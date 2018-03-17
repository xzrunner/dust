#pragma once

#include <bitset>

namespace dust
{

enum Type
{
	INVALID_ID = 0,

	OBJECT_ID,
	MODULE_ID,

	MODULE_GRAPHICS_ID,

	TYPE_MAX_ENUM
};

typedef std::bitset<TYPE_MAX_ENUM> TypeBits;

extern const TypeBits* TypeFlags;

void AddTypeName(Type type, const char* name);

}