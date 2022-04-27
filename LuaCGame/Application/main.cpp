#include <iostream>
#include "lua.hpp"
#include "raylib.h"
#include "entt.hpp"

int main()
{
    srand((unsigned int)time(0));

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dostring(L, "print('Hello Lua!')");

    entt::registry registry;
    for (int i = 0; i < 100; i++)
    {
        entt::entity entity = registry.create();
        registry.emplace<int>(entity, rand() % 25);
    }

    auto view = registry.view<int>();
    view.each([](int num) {
        std::cout << num << std::endl;
    });

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