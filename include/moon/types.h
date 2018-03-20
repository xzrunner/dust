#pragma once

#include <bitset>

namespace moon
{

enum Type
{
	INVALID_ID = 0,

	OBJECT_ID,
	MODULE_ID,

	SCENE_NODE_ID,

	// Modules
	MODULE_GRAPHICS_ID,
	MODULE_SCENE_GRAPH_ID,

	TYPE_MAX_ENUM
};

typedef std::bitset<TYPE_MAX_ENUM> TypeBits;

extern const TypeBits* TypeFlags;

void AddTypeName(Type type, const char* name);
bool GetTypeName(const char *in, Type &out);
bool GetTypeName(Type in, const char *&out);

}