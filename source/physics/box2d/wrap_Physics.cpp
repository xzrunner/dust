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

// MOON
#include "moon/physics/box2d/wrap_Physics.h"
#include "moon/physics/box2d/wrap_World.h"
#include "moon/physics/box2d/wrap_Contact.h"
#include "moon/physics/box2d/wrap_Body.h"
#include "moon/physics/box2d/wrap_Fixture.h"
#include "moon/physics/box2d/wrap_Shape.h"
#include "moon/physics/box2d/wrap_CircleShape.h"
#include "moon/physics/box2d/wrap_PolygonShape.h"
#include "moon/physics/box2d/wrap_EdgeShape.h"
#include "moon/physics/box2d/wrap_ChainShape.h"
#include "moon/physics/box2d/wrap_Joint.h"
#include "moon/physics/box2d/wrap_MouseJoint.h"
#include "moon/physics/box2d/wrap_DistanceJoint.h"
#include "moon/physics/box2d/wrap_PrismaticJoint.h"
#include "moon/physics/box2d/wrap_RevoluteJoint.h"
#include "moon/physics/box2d/wrap_PulleyJoint.h"
#include "moon/physics/box2d/wrap_GearJoint.h"
#include "moon/physics/box2d/wrap_FrictionJoint.h"
#include "moon/physics/box2d/wrap_WeldJoint.h"
#include "moon/physics/box2d/wrap_WheelJoint.h"
#include "moon/physics/box2d/wrap_RopeJoint.h"
#include "moon/physics/box2d/wrap_MotorJoint.h"

#include "moon/Blackboard.h"
#include "moon/Context.h"

namespace moon
{
namespace physics
{
namespace box2d
{

#define INSTANCE() (Blackboard::Instance()->GetContext()->GetModuleMgr().GetModule<Physics>())

int w_newWorld(lua_State *L)
{
	float gx = (float)luaL_optnumber(L, 1, 0);
	float gy = (float)luaL_optnumber(L, 2, 0);
	bool sleep = luax_optboolean(L, 3, true);

	World *w;
	luax_catchexcept(L, [&](){ w = INSTANCE()->newWorld(gx, gy, sleep); });
	luax_pushtype(L, PHYSICS_WORLD_ID, w);
	w->Release();

	return 1;
}

int w_newBody(lua_State *L)
{
	World *world = luax_checkworld(L, 1);
	float x = (float)luaL_optnumber(L, 2, 0.0);
	float y = (float)luaL_optnumber(L, 3, 0.0);

	Body::Type btype = Body::BODY_STATIC;
	const char *typestr = lua_isnoneornil(L, 4) ? 0 : lua_tostring(L, 4);
	if (typestr && !Body::getConstant(typestr, btype))
		return luaL_error(L, "Invalid Body type: %s", typestr);

	Body *body;
	luax_catchexcept(L, [&](){ body = INSTANCE()->newBody(world, x, y, btype); });
	luax_pushtype(L, PHYSICS_BODY_ID, body);
	body->Release();
	return 1;
}

int w_newFixture(lua_State *L)
{
	Body *body = luax_checkbody(L, 1);
	Shape *shape = luax_checkshape(L, 2);
	float density = (float)luaL_optnumber(L, 3, 1.0f);
	Fixture *fixture;
	luax_catchexcept(L, [&](){ fixture = INSTANCE()->newFixture(body, shape, density); });
	luax_pushtype(L, PHYSICS_FIXTURE_ID, fixture);
	fixture->Release();
	return 1;
}

int w_newCircleShape(lua_State *L)
{
	int top = lua_gettop(L);

	if (top == 1)
	{
		float radius = (float)luaL_checknumber(L, 1);
		CircleShape *shape;
		luax_catchexcept(L, [&](){ shape = INSTANCE()->newCircleShape(radius); });
		luax_pushtype(L, PHYSICS_CIRCLE_SHAPE_ID, shape);
		shape->Release();
		return 1;
	}
	else if (top == 3)
	{
		float x = (float)luaL_checknumber(L, 1);
		float y = (float)luaL_checknumber(L, 2);
		float radius = (float)luaL_checknumber(L, 3);
		CircleShape *shape;
		luax_catchexcept(L, [&](){ shape = INSTANCE()->newCircleShape(x, y, radius); });
		luax_pushtype(L, PHYSICS_CIRCLE_SHAPE_ID, shape);
		shape->Release();
		return 1;
	}
	else
		return luaL_error(L, "Incorrect number of parameters");
}

int w_newRectangleShape(lua_State *L)
{
	int top = lua_gettop(L);

	if (top == 2)
	{
		float w = (float)luaL_checknumber(L, 1);
		float h = (float)luaL_checknumber(L, 2);
		PolygonShape *shape;
		luax_catchexcept(L, [&](){ shape = INSTANCE()->newRectangleShape(w, h); });
		luax_pushtype(L, PHYSICS_POLYGON_SHAPE_ID, shape);
		shape->Release();
		return 1;
	}
	else if (top == 4 || top == 5)
	{
		float x = (float)luaL_checknumber(L, 1);
		float y = (float)luaL_checknumber(L, 2);
		float w = (float)luaL_checknumber(L, 3);
		float h = (float)luaL_checknumber(L, 4);
		float angle = (float)luaL_optnumber(L, 5, 0);
		PolygonShape *shape;
		luax_catchexcept(L, [&](){ shape = INSTANCE()->newRectangleShape(x, y, w, h, angle); });
		luax_pushtype(L, PHYSICS_POLYGON_SHAPE_ID, shape);
		shape->Release();
		return 1;
	}
	else
		return luaL_error(L, "Incorrect number of parameters");
}

int w_newEdgeShape(lua_State *L)
{
	float x1 = (float)luaL_checknumber(L, 1);
	float y1 = (float)luaL_checknumber(L, 2);
	float x2 = (float)luaL_checknumber(L, 3);
	float y2 = (float)luaL_checknumber(L, 4);
	EdgeShape *shape;
	luax_catchexcept(L, [&](){ shape = INSTANCE()->newEdgeShape(x1, y1, x2, y2); });
	luax_pushtype(L, PHYSICS_EDGE_SHAPE_ID, shape);
	shape->Release();
	return 1;
}

int w_newPolygonShape(lua_State *L)
{
	int ret = 0;
	luax_catchexcept(L, [&](){ ret = INSTANCE()->newPolygonShape(L); });
	return ret;
}

int w_newChainShape(lua_State *L)
{
	int ret = 0;
	luax_catchexcept(L, [&](){ ret = INSTANCE()->newChainShape(L); });
	return ret;
}

int w_newDistanceJoint(lua_State *L)
{
	Body *body1 = luax_checkbody(L, 1);
	Body *body2 = luax_checkbody(L, 2);
	float x1 = (float)luaL_checknumber(L, 3);
	float y1 = (float)luaL_checknumber(L, 4);
	float x2 = (float)luaL_checknumber(L, 5);
	float y2 = (float)luaL_checknumber(L, 6);
	bool collideConnected = luax_optboolean(L, 7, false);
	DistanceJoint *j;
	luax_catchexcept(L, [&]() {
		j = INSTANCE()->newDistanceJoint(body1, body2, x1, y1, x2, y2, collideConnected);
	});
	luax_pushtype(L, PHYSICS_DISTANCE_JOINT_ID, j);
	j->Release();
	return 1;
}

int w_newMouseJoint(lua_State *L)
{
	Body *body = luax_checkbody(L, 1);
	float x = (float)luaL_checknumber(L, 2);
	float y = (float)luaL_checknumber(L, 3);
	MouseJoint *j;
	luax_catchexcept(L, [&](){ j = INSTANCE()->newMouseJoint(body, x, y); });
	luax_pushtype(L, PHYSICS_MOUSE_JOINT_ID, j);
	j->Release();
	return 1;
}

int w_newRevoluteJoint(lua_State *L)
{
	Body *body1 = luax_checkbody(L, 1);
	Body *body2 = luax_checkbody(L, 2);
	float xA = (float)luaL_checknumber(L, 3);
	float yA = (float)luaL_checknumber(L, 4);
	float xB, yB;
	bool collideConnected;
	float referenceAngle = 0.0f;
	if (lua_gettop(L) >= 6)
	{
		xB = (float)luaL_checknumber(L, 5);
		yB = (float)luaL_checknumber(L, 6);
		collideConnected = luax_optboolean(L, 7, false);
		referenceAngle = (float)luaL_optnumber(L, 8, referenceAngle);
	}
	else
	{
		xB = xA;
		yB = yA;
		collideConnected = luax_optboolean(L, 5, false);
	}
	RevoluteJoint *j;
	luax_catchexcept(L, [&]() {
		j = INSTANCE()->newRevoluteJoint(body1, body2, xA, yA, xB, yB, collideConnected, referenceAngle);
	});
	luax_pushtype(L, PHYSICS_REVOLUTE_JOINT_ID, j);
	j->Release();
	return 1;
}

int w_newPrismaticJoint(lua_State *L)
{
	Body *body1 = luax_checkbody(L, 1);
	Body *body2 = luax_checkbody(L, 2);
	float xA = (float)luaL_checknumber(L, 3);
	float yA = (float)luaL_checknumber(L, 4);
	float xB, yB, ax, ay;
	bool collideConnected;
	float referenceAngle = 0.0f;
	if (lua_gettop(L) >= 8)
	{
		xB = (float)luaL_checknumber(L, 5);
		yB = (float)luaL_checknumber(L, 6);
		ax = (float)luaL_checknumber(L, 7);
		ay = (float)luaL_checknumber(L, 8);
		collideConnected = luax_optboolean(L, 9, false);
		referenceAngle = (float)luaL_optnumber(L, 10, referenceAngle);
	}
	else
	{
		xB = xA;
		yB = yA;
		ax = (float)luaL_checknumber(L, 5);
		ay = (float)luaL_checknumber(L, 6);
		collideConnected = luax_optboolean(L, 7, false);
	}
	PrismaticJoint *j;
	luax_catchexcept(L, [&]() {
		j = INSTANCE()->newPrismaticJoint(body1, body2, xA, yA, xB, yB, ax, ay, collideConnected, referenceAngle);
	});
	luax_pushtype(L, PHYSICS_PRISMATIC_JOINT_ID, j);
	j->Release();
	return 1;
}

int w_newPulleyJoint(lua_State *L)
{
	Body *body1 = luax_checkbody(L, 1);
	Body *body2 = luax_checkbody(L, 2);
	float gx1 = (float)luaL_checknumber(L, 3);
	float gy1 = (float)luaL_checknumber(L, 4);
	float gx2 = (float)luaL_checknumber(L, 5);
	float gy2 = (float)luaL_checknumber(L, 6);
	float x1 = (float)luaL_checknumber(L, 7);
	float y1 = (float)luaL_checknumber(L, 8);
	float x2 = (float)luaL_checknumber(L, 9);
	float y2 = (float)luaL_checknumber(L, 10);
	float ratio = (float)luaL_optnumber(L, 11, 1.0);
	bool collideConnected = luax_optboolean(L, 12, true); // PulleyJoints default to colliding connected bodies, see b2PulleyJoint.h

	PulleyJoint *j;
	luax_catchexcept(L, [&]() {
		j = INSTANCE()->newPulleyJoint(body1, body2, b2Vec2(gx1,gy1), b2Vec2(gx2,gy2), b2Vec2(x1,y1), b2Vec2(x2,y2), ratio, collideConnected);
	});
	luax_pushtype(L, PHYSICS_PULLEY_JOINT_ID, j);
	j->Release();
	return 1;
}

int w_newGearJoint(lua_State *L)
{
	Joint *joint1 = luax_checkjoint(L, 1);
	Joint *joint2 = luax_checkjoint(L, 2);
	float ratio = (float)luaL_optnumber(L, 3, 1.0);
	bool collideConnected = luax_optboolean(L, 4, false);

	GearJoint *j;
	luax_catchexcept(L, [&]() {
		j = INSTANCE()->newGearJoint(joint1, joint2, ratio, collideConnected);
	});
	luax_pushtype(L, PHYSICS_GEAR_JOINT_ID, j);
	j->Release();
	return 1;
}

int w_newFrictionJoint(lua_State *L)
{
	Body *body1 = luax_checkbody(L, 1);
	Body *body2 = luax_checkbody(L, 2);
	float xA = (float)luaL_checknumber(L, 3);
	float yA = (float)luaL_checknumber(L, 4);
	float xB, yB;
	bool collideConnected;
	if (lua_gettop(L) >= 6)
	{
		xB = (float)luaL_checknumber(L, 5);
		yB = (float)luaL_checknumber(L, 6);
		collideConnected = luax_optboolean(L, 7, false);
	}
	else
	{
		xB = xA;
		yB = yA;
		collideConnected = luax_optboolean(L, 5, false);
	}
	FrictionJoint *j;
	luax_catchexcept(L, [&]() {
		j = INSTANCE()->newFrictionJoint(body1, body2, xA, yA, xB, yB, collideConnected);
	});
	luax_pushtype(L, PHYSICS_FRICTION_JOINT_ID, j);
	j->Release();
	return 1;
}

int w_newWeldJoint(lua_State *L)
{
	Body *body1 = luax_checkbody(L, 1);
	Body *body2 = luax_checkbody(L, 2);
	float xA = (float)luaL_checknumber(L, 3);
	float yA = (float)luaL_checknumber(L, 4);
	float xB, yB;
	bool collideConnected;
	float referenceAngle = 0.0f;
	if (lua_gettop(L) >= 6)
	{
		xB = (float)luaL_checknumber(L, 5);
		yB = (float)luaL_checknumber(L, 6);
		collideConnected = luax_optboolean(L, 7, false);
		referenceAngle = (float)luaL_optnumber(L, 8, referenceAngle);
	}
	else
	{
		xB = xA;
		yB = yA;
		collideConnected = luax_optboolean(L, 5, false);
	}
	WeldJoint *j;
	luax_catchexcept(L, [&]() {
		j = INSTANCE()->newWeldJoint(body1, body2, xA, yA, xB, yB, collideConnected, referenceAngle);
	});
	luax_pushtype(L, PHYSICS_WELD_JOINT_ID, j);
	j->Release();
	return 1;
}

int w_newWheelJoint(lua_State *L)
{
	Body *body1 = luax_checkbody(L, 1);
	Body *body2 = luax_checkbody(L, 2);
	float xA = (float)luaL_checknumber(L, 3);
	float yA = (float)luaL_checknumber(L, 4);
	float xB, yB, ax, ay;
	bool collideConnected;
	if (lua_gettop(L) >= 8)
	{
		xB = (float)luaL_checknumber(L, 5);
		yB = (float)luaL_checknumber(L, 6);
		ax = (float)luaL_checknumber(L, 7);
		ay = (float)luaL_checknumber(L, 8);
		collideConnected = luax_optboolean(L, 9, false);
	}
	else
	{
		xB = xA;
		yB = yA;
		ax = (float)luaL_checknumber(L, 5);
		ay = (float)luaL_checknumber(L, 6);
		collideConnected = luax_optboolean(L, 7, false);
	}

	WheelJoint *j;
	luax_catchexcept(L, [&]() {
		j = INSTANCE()->newWheelJoint(body1, body2, xA, yA, xB, yB, ax, ay, collideConnected);
	});
	luax_pushtype(L, PHYSICS_WHEEL_JOINT_ID, j);
	j->Release();
	return 1;
}

int w_newRopeJoint(lua_State *L)
{
	Body *body1 = luax_checkbody(L, 1);
	Body *body2 = luax_checkbody(L, 2);
	float x1 = (float)luaL_checknumber(L, 3);
	float y1 = (float)luaL_checknumber(L, 4);
	float x2 = (float)luaL_checknumber(L, 5);
	float y2 = (float)luaL_checknumber(L, 6);
	float maxLength = (float)luaL_checknumber(L, 7);
	bool collideConnected = luax_optboolean(L, 8, false);
	RopeJoint *j;
	luax_catchexcept(L, [&]() {
		j = INSTANCE()->newRopeJoint(body1, body2, x1, y1, x2, y2, maxLength, collideConnected);
	});
	luax_pushtype(L, PHYSICS_ROPE_JOINT_ID, j);
	j->Release();
	return 1;
}

int w_newMotorJoint(lua_State *L)
{
	Body *body1 = luax_checkbody(L, 1);
	Body *body2 = luax_checkbody(L, 2);
	MotorJoint *j = 0;
	if (!lua_isnoneornil(L, 3))
	{
		float correctionFactor = (float)luaL_checknumber(L, 3);
		bool collideConnected = luax_optboolean(L, 4, false);
		luax_catchexcept(L, [&]() {
			j = INSTANCE()->newMotorJoint(body1, body2, correctionFactor, collideConnected);
		});
	}
	else
	{
		luax_catchexcept(L, [&](){ j = INSTANCE()->newMotorJoint(body1, body2); });
	}
	luax_pushtype(L, PHYSICS_MOTOR_JOINT_ID, j);
	j->Release();
	return 1;
}

int w_getDistance(lua_State *L)
{
	return INSTANCE()->getDistance(L);
}

int w_setMeter(lua_State *L)
{
	int arg1 = (int) luaL_checknumber(L, 1);
	luax_catchexcept(L, [&](){ Physics::setMeter(arg1); });
	return 0;

}
int w_getMeter(lua_State *L)
{
	lua_pushinteger(L, Physics::getMeter());
	return 1;
}

// List of functions to wrap.
static const luaL_Reg functions[] =
{
	{ "newWorld", w_newWorld },
	{ "newBody", w_newBody },
	{ "newFixture", w_newFixture },
	{ "newCircleShape", w_newCircleShape },
	{ "newRectangleShape", w_newRectangleShape },
	{ "newPolygonShape", w_newPolygonShape },
	{ "newEdgeShape", w_newEdgeShape },
	{ "newChainShape", w_newChainShape },
	{ "newDistanceJoint", w_newDistanceJoint },
	{ "newMouseJoint", w_newMouseJoint },
	{ "newRevoluteJoint", w_newRevoluteJoint },
	{ "newPrismaticJoint", w_newPrismaticJoint },
	{ "newPulleyJoint", w_newPulleyJoint },
	{ "newGearJoint", w_newGearJoint },
	{ "newFrictionJoint", w_newFrictionJoint },
	{ "newWeldJoint", w_newWeldJoint },
	{ "newWheelJoint", w_newWheelJoint },
	{ "newRopeJoint", w_newRopeJoint },
	{ "newMotorJoint", w_newMotorJoint },
	{ "getDistance", w_getDistance },
	{ "getMeter", w_getMeter },
	{ "setMeter", w_setMeter },
	{ 0, 0 },
};

static const lua_CFunction types[] =
{
	luaopen_world,
	luaopen_contact,
	luaopen_body,
	luaopen_fixture,
	luaopen_shape,
	luaopen_circleshape,
	luaopen_polygonshape,
	luaopen_edgeshape,
	luaopen_chainshape,
	luaopen_joint,
	luaopen_mousejoint,
	luaopen_distancejoint,
	luaopen_prismaticjoint,
	luaopen_revolutejoint,
	luaopen_pulleyjoint,
	luaopen_gearjoint,
	luaopen_frictionjoint,
	luaopen_weldjoint,
	luaopen_wheeljoint,
	luaopen_ropejoint,
	luaopen_motorjoint,
	0
};

extern "C" int luaopen_moon_physics(lua_State *L)
{
	Physics *INSTANCE = INSTANCE();
	if (INSTANCE == nullptr)
	{
		luax_catchexcept(L, [&](){ INSTANCE = new box2d::Physics(); });
	}
	else
		INSTANCE->Retain();

	WrappedModule w;
	w.module = INSTANCE;
	w.name = "physics";
	w.type = MODULE_ID;
	w.functions = functions;
	w.types = types;

	return luax_register_module(L, w);
}

} // box2d
} // physics
} // moon
