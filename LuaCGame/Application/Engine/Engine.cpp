#include "Engine.h"

void Engine::update()
{
	if(IsKeyPressed(KEY_R))
		this->scene.setScene(L, "scene.lua");

	this->scene.updateSystems(GetFrameTime());
}

void Engine::render()
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	this->scene.render();

	EndDrawing();
}

Engine::Engine(Vector2 dimensions):
	dimensions(dimensions)
{
	// Raylib
	InitWindow((int)this->dimensions.x, (int)this->dimensions.y, "Raylib Window");
	SetTargetFPS(60);

	this->L = luaL_newstate();
	luaL_openlibs(L);
	Scene::lua_openscene(L, &this->scene);
	Input::lua_openinput(L, &this->input);

	this->scene.setScene(L, "scene.lua");
}

Engine::~Engine()
{
	lua_close(L);
	CloseWindow();
}

void Engine::run()
{
	while (!WindowShouldClose())
	{
		this->update();
		this->render();
	}
}
