#pragma once

#include "raylib.h"
#include "raymath.h"
#include "../Lua/LuaHelper.h"

struct TransformComp
{
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	TransformComp():
		position(Vector3Zero()), rotation(Vector3Zero()), scale(Vector3One())
	{}
};

static Vector3 lua_tovector(lua_State* L, int index)
{
	Vector3 vec = Vector3Zero();
	// Sanity check
	if (!lua_istable(L, index)) {
		std::cout << "Error: not table" << std::endl;
		return vec;
	}

	lua_getfield(L, index, "x");
	vec.x = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "y");
	vec.y = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "z");
	vec.z = lua_tonumber(L, -1);
	lua_pop(L, 1);

	return vec;
}

static void lua_pushvector(lua_State* L, const Vector3& vec)
{
	lua_newtable(L);

	lua_pushnumber(L, vec.x);
	lua_setfield(L, -2, "x");

	lua_pushnumber(L, vec.y);
	lua_setfield(L, -2, "y");

	lua_pushnumber(L, vec.z);
	lua_setfield(L, -2, "z");
}

static TransformComp lua_totransform(lua_State* L, int index)
{
	TransformComp transform;
	// Sanity check
	if (!lua_istable(L, index)) {
		std::cout << "Error: not table" << std::endl;
		return transform;
	}

	lua_getfield(L, index, "position");
	transform.position = lua_tovector(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "rotation");
	transform.rotation = lua_tovector(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "scale");
	transform.scale = lua_tovector(L, -1);
	lua_pop(L, 1);

	return transform;
}

static void lua_pushtransform(lua_State* L, const TransformComp& transform)
{
	lua_newtable(L);

	lua_pushvector(L, transform.position);
	lua_setfield(L, -2, "position");

	lua_pushvector(L, transform.rotation);
	lua_setfield(L, -2, "rotation");

	lua_pushvector(L, transform.scale);
	lua_setfield(L, -2, "scale");
}