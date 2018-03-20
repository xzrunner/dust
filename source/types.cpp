#include "moon/types.h"
#include "moon/StringMap.h"

namespace moon
{

static const TypeBits* CreateTypeFlags()
{
	static TypeBits b[TYPE_MAX_ENUM];
	TypeBits one = TypeBits(1);

	b[INVALID_ID] = one << INVALID_ID;

	b[OBJECT_ID] = one << OBJECT_ID;
	b[MODULE_ID] = (one << MODULE_ID) | b[OBJECT_ID];

	b[SCENE_NODE_ID] = (one << SCENE_NODE_ID) | b[OBJECT_ID];

	b[MODULE_GRAPHICS_ID] = (one << MODULE_GRAPHICS_ID) | b[MODULE_ID];
	b[MODULE_SCENE_GRAPH_ID] = (one << MODULE_SCENE_GRAPH_ID) | b[MODULE_ID];

	return b;
}

const TypeBits* TypeFlags = CreateTypeFlags();

static StringMap<Type, TYPE_MAX_ENUM> types(nullptr, 0);

void AddTypeName(Type type, const char* name)
{
	const char *n;
	if (!types.find(type, n))
		types.add(name, type);
}

bool GetTypeName(const char *in, Type &out)
{
	return types.find(in, out);
}

bool GetTypeName(Type in, const char *&out)
{
	return types.find(in, out);
}

}