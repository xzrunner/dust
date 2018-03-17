#include "dust/types.h"

namespace dust
{

static const TypeBits* CreateTypeFlags()
{
	static TypeBits b[TYPE_MAX_ENUM];
	TypeBits one = TypeBits(1);

	b[INVALID_ID] = one << INVALID_ID;

	b[OBJECT_ID] = one << OBJECT_ID;
	b[MODULE_ID] = (one << MODULE_ID) | b[OBJECT_ID];

	b[MODULE_GRAPHICS_ID] = (one << MODULE_GRAPHICS_ID) | b[MODULE_ID];

	return b;
}

const TypeBits* TypeFlags = CreateTypeFlags();

static std::string types[TYPE_MAX_ENUM];

void AddTypeName(Type type, const char* name)
{
	types[type] = name;
}

}