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

	int fontSize = 20;
	std::string text = "Congrats! You created your first window!";
	Vector2 size = MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, 1);

	DrawText(text.c_str(), (this->dimensions.x - size.x) / 2, (this->dimensions.y - size.y) / 2, fontSize, LIGHTGRAY);

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
