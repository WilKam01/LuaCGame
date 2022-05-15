#pragma once

#include "../Dev/LuaHelper.h"
#include "raylib.h"
#include "TransformComp.h"

struct UIElement
{
	bool visibleBG;
	std::string text;
	Vector2 position;
	Vector2 dimensions;
	Color colour;

	UIElement():
		visibleBG(true), position({ 0.0f, 0.0f }), dimensions({ 100.0f, 100.0f }), colour(RAYWHITE)
	{}
};

static UIElement lua_touielement(lua_State* L, int index)
{
	UIElement elem;
	if (!lua_istable(L, index)) {
		std::cout << "Error: not table" << std::endl;
		elem.dimensions = { 0.0f, 0.0f };
		return elem;
	}
	Vector3 vec;

	lua_getfield(L, index, "visibleBG");
	elem.visibleBG = lua_toboolean(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "text");
	elem.text = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "position");
	vec = lua_tovector(L, -1);
	elem.position = { vec.x, vec.y };
	lua_pop(L, 1);

	lua_getfield(L, index, "dimensions");
	vec = lua_tovector(L, -1);
	elem.dimensions = { vec.x, vec.y };
	lua_pop(L, 1);

	lua_getfield(L, index, "colour");
	vec = lua_tovector(L, -1);
	elem.colour.r = vec.x;
	elem.colour.g = vec.y;
	elem.colour.b = vec.z;
	lua_pop(L, 1);

	return elem;
}

static void lua_pushuielement(lua_State* L, const UIElement& element)
{
	lua_newtable(L);

	lua_pushboolean(L, element.visibleBG);
	lua_setfield(L, -2, "visibleBG");

	lua_pushstring(L, element.text.c_str());
	lua_setfield(L, -2, "text");

	lua_pushvector(L, { element.position.x, element.position.y });
	lua_setfield(L, -2, "position");

	lua_pushvector(L, { element.dimensions.x, element.dimensions.y });
	lua_setfield(L, -2, "dimensions");

	Vector3 col;
	col.x = element.colour.r;
	col.y = element.colour.g;
	col.z = element.colour.b;
	lua_pushvector(L, col);
	lua_setfield(L, -2, "colour");
}