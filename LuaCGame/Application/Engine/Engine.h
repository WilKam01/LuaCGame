#pragma once

#include <iostream>
#include "Scene.h"

class Engine
{
private:
	Vector2 dimensions;
	Scene scene;
	lua_State* L;

	void update();
	void render();
public:
	Engine(Vector2 dimensions);
	virtual ~Engine();

	void run();
};

