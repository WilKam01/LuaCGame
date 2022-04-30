#pragma once

#include <iostream>
#include "raylib.h"
#include "Lua/LuaHelper.h"
#include "Scene.h"

class Engine
{
private:
	Vector2 dimensions;
	Scene scene;
	lua_State* L;
public:
	Engine(Vector2 dimensions);
	virtual ~Engine();

	void run();
};

