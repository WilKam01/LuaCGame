#include "UserInterface.h"

int UserInterface::lua_hover(lua_State* L)
{
	UIElement elem = lua_touielement(L, 1);
	bool result = false;
	Vector2 mouse = GetMousePosition();

	if (mouse.x >= elem.position.x && mouse.x <= elem.position.x + elem.dimensions.x &&
		mouse.y >= elem.position.y && mouse.y <= elem.position.y + elem.dimensions.y)
		result = true;

	lua_pushboolean(L, result);
	return 1;
}

UserInterface::UserInterface()
{
}

UserInterface::~UserInterface()
{
}

void UserInterface::lua_openui(lua_State* L, UserInterface* UI)
{
	lua_newtable(L);

	luaL_Reg methods[] = {
		{ "isHover", lua_hover },
		{ NULL , NULL }
	};

	lua_pushlightuserdata(L, UI);
	luaL_setfuncs(L, methods, 1);
	lua_setglobal(L, "UI");
}
