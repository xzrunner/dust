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

	// Physics.
	b[PHYSICS_WORLD_ID] = (one << PHYSICS_WORLD_ID) | b[OBJECT_ID];
	b[PHYSICS_CONTACT_ID] = (one << PHYSICS_CONTACT_ID) | b[OBJECT_ID];
	b[PHYSICS_BODY_ID] = (one << PHYSICS_BODY_ID) | b[OBJECT_ID];
	b[PHYSICS_FIXTURE_ID] = (one << PHYSICS_FIXTURE_ID) | b[OBJECT_ID];
	b[PHYSICS_SHAPE_ID] = (one << PHYSICS_SHAPE_ID) | b[OBJECT_ID];
	b[PHYSICS_CIRCLE_SHAPE_ID] = (one << PHYSICS_CIRCLE_SHAPE_ID) | b[PHYSICS_SHAPE_ID];
	b[PHYSICS_POLYGON_SHAPE_ID] = (one << PHYSICS_POLYGON_SHAPE_ID) | b[PHYSICS_SHAPE_ID];
	b[PHYSICS_EDGE_SHAPE_ID] = (one << PHYSICS_EDGE_SHAPE_ID) | b[PHYSICS_SHAPE_ID];
	b[PHYSICS_CHAIN_SHAPE_ID] = (one << PHYSICS_CHAIN_SHAPE_ID) | b[PHYSICS_SHAPE_ID];
	b[PHYSICS_JOINT_ID] = (one << PHYSICS_JOINT_ID) | b[OBJECT_ID];
	b[PHYSICS_MOUSE_JOINT_ID] = (one << PHYSICS_MOUSE_JOINT_ID) | b[PHYSICS_JOINT_ID];
	b[PHYSICS_DISTANCE_JOINT_ID] = (one << PHYSICS_DISTANCE_JOINT_ID) | b[PHYSICS_JOINT_ID];
	b[PHYSICS_PRISMATIC_JOINT_ID] = (one << PHYSICS_PRISMATIC_JOINT_ID) | b[PHYSICS_JOINT_ID];
	b[PHYSICS_REVOLUTE_JOINT_ID] = (one << PHYSICS_REVOLUTE_JOINT_ID) | b[PHYSICS_JOINT_ID];
	b[PHYSICS_PULLEY_JOINT_ID] = (one << PHYSICS_PULLEY_JOINT_ID) | b[PHYSICS_JOINT_ID];
	b[PHYSICS_GEAR_JOINT_ID] = (one << PHYSICS_GEAR_JOINT_ID) | b[PHYSICS_JOINT_ID];
	b[PHYSICS_FRICTION_JOINT_ID] = (one << PHYSICS_FRICTION_JOINT_ID) | b[PHYSICS_JOINT_ID];
	b[PHYSICS_WELD_JOINT_ID] = (one << PHYSICS_WELD_JOINT_ID) | b[PHYSICS_JOINT_ID];
	b[PHYSICS_ROPE_JOINT_ID] = (one << PHYSICS_ROPE_JOINT_ID) | b[PHYSICS_JOINT_ID];
	b[PHYSICS_WHEEL_JOINT_ID] = (one << PHYSICS_WHEEL_JOINT_ID) | b[PHYSICS_JOINT_ID];
	b[PHYSICS_MOTOR_JOINT_ID] = (one << PHYSICS_MOTOR_JOINT_ID) | b[PHYSICS_JOINT_ID];

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