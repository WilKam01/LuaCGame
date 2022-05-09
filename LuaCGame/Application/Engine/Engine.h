#pragma once

#include <iostream>
#include "Scene.h"
#include "Input.h"

class Engine
{
private:
	Vector2 dimensions;
	Scene scene;
	Input input;
	lua_State* L;

	void update();
	void render();
public:
	Engine(Vector2 dimensions);
	virtual ~Engine();

	void run();
};

