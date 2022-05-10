#include "Engine.h"

void Engine::lua_openmetatables(lua_State* L)
{
	luaL_loadfile(L, "Scripts/vector.lua");
	lua_pcall(L, 0, 1, 0);
	lua_setglobal(L, "vector");
}

void Engine::update()
{
	this->scene.updateSystems(GetFrameTime());
}

void Engine::render()
{
	BeginDrawing();
	ClearBackground(BLACK);

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
	luaL_openlibs(this->L);
	this->lua_openmetatables(this->L);
	Scene::lua_openscene(this->L, &this->scene);
	Input::lua_openinput(this->L, &this->input);

	this->scene.getResources().loadPrimitives();
	this->scene.createSystem<BehaviourSystem>(this->L);
	this->scene.setScene(this->L, "scene.lua");
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
