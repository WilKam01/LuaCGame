#pragma once

#include <iostream>
#include "Scene.h"
#include "Input.h"
#include "UserInterface.h"

class Engine
{
private:
	Vector2 dimensions;
	Scene scene;
	Input input;
	UserInterface UI;
	lua_State* L;

	void lua_openmetatables(lua_State* L);
	void update();
	void render();
public:
	Engine(Vector2 dimensions);
	virtual ~Engine();

	void run();
};

