#include "Engine.h"

Engine::Engine(Vector2 dimensions):
	dimensions(dimensions)
{
	// Raylib
	InitWindow(this->dimensions.x, this->dimensions.y, "Raylib Window");
	SetTargetFPS(60);

	this->L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dostring(L, "print('Hello Lua!')");

	lua_pushstring(L, "!");
	lua_pushstring(L, "World");
	lua_pushstring(L, "Hello");
	LuaHelper::dumpStack(L);
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
		BeginDrawing();
		ClearBackground(RAYWHITE);

		int fontSize = 20;
		std::string text = "Congrats! You created your first window!";
		Vector2 size = MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, 1);

		DrawText(text.c_str(), (this->dimensions.x - size.x) / 2.0f, (this->dimensions.y - size.y) / 2.0f, fontSize, LIGHTGRAY);

		EndDrawing();
	}
}
