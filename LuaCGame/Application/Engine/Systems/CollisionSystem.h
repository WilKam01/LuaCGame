#pragma once

#include "../Dev/LuaHelper.h"
#include "raylib.h"
#include "../Components/Componentpch.h"
#include "System.h"
#include "../Resources.h"

class CollisionSystem : public System
{
private:
	lua_State* L;
	Resources* resource;
public:
	CollisionSystem(lua_State* L, Resources* resource) :
		L(L), resource(resource)
	{}

	bool update(entt::registry& reg, float deltaTime) final
	{
		auto view = reg.view<Behaviour, MeshComp, TransformComp>();
		view.each([&](Behaviour& script1, MeshComp& mesh1, TransformComp& transform1)
		{
			auto view2 = reg.view<Behaviour, MeshComp, TransformComp>();
			view2.each([&](Behaviour& script2, MeshComp& mesh2, TransformComp& transform2)
			{
				Model* model1 = resource->getModel(mesh1.modelName);
				Model* model2 = resource->getModel(mesh2.modelName);
				if (!model1 || !model2)
					return;

				BoundingBox box1 = GetModelBoundingBox(*model1);
				box1.min = Vector3Add(Vector3Multiply(box1.min, transform1.scale), transform1.position);
				box1.max = Vector3Add(Vector3Multiply(box1.max, transform1.scale), transform1.position);

				BoundingBox box2 = GetModelBoundingBox(*model2);
				box2.min = Vector3Add(Vector3Multiply(box2.min, transform2.scale), transform2.position);
				box2.max = Vector3Add(Vector3Multiply(box2.max, transform2.scale), transform2.position);

				if (&script1 == &script2 || !CheckCollisionBoxes(box1, box2))
					return;

				lua_rawgeti(L, LUA_REGISTRYINDEX, script1.luaRef);
				if (luaL_dofile(L, ("Scripts/Behaviour/" + std::string(script1.path)).c_str()) != LUA_OK)
					LuaHelper::dumpError(L);
				else
				{
					lua_getfield(L, -1, "collision"); // Get new collision function
					lua_setfield(L, -3, "collision"); // Set instance collision function to the new one
					lua_pop(L, 1);
				}

				lua_getfield(L, -1, "collision");
				if (lua_type(L, -1) == LUA_TNIL)
				{
					lua_pop(L, 1);
				}
				else
				{
					lua_pushvalue(L, -2);
					lua_rawgeti(L, LUA_REGISTRYINDEX, script2.luaRef);

					if (lua_pcall(L, 2, 0, 0) != LUA_OK)
						LuaHelper::dumpError(L);
					else
						lua_pop(L, 1);
				}

				lua_rawgeti(L, LUA_REGISTRYINDEX, script2.luaRef);
				if (luaL_dofile(L, ("Scripts/Behaviour/" + std::string(script2.path)).c_str()) != LUA_OK)
					LuaHelper::dumpError(L);
				else
				{
					lua_getfield(L, -1, "collision"); // Get new collision function
					lua_setfield(L, -3, "collision"); // Set instance collision function to the new one
					lua_pop(L, 1);
				}

				lua_getfield(L, -1, "collision");
				if (lua_type(L, -1) == LUA_TNIL)
				{
					lua_pop(L, 1);
				}
				else
				{
					lua_pushvalue(L, -2);
					lua_rawgeti(L, LUA_REGISTRYINDEX, script1.luaRef);

					if (lua_pcall(L, 2, 0, 0) != LUA_OK)
						LuaHelper::dumpError(L);
					else
						lua_pop(L, 1);
				}
			});
		});

		return false;
	}
};