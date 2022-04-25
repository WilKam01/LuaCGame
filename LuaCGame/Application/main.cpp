#include <iostream>
#include "lua.hpp"

int main()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dostring(L, "print('Hello Lua!')");

	return 0;
}