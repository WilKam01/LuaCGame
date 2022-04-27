#include <iostream>
#include "lua.hpp"
#include "raylib.h"

int main()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dostring(L, "print('Hello Lua!')");

	InitWindow(800, 450, "raylib basic window");
	SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

	return 0;
}