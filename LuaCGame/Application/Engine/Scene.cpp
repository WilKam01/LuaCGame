#include "Scene.h"

Scene::Scene()
{
	
}

Scene::~Scene()
{
	for (auto& s : this->systems)
		delete s;
}

void Scene::lua_openscene(lua_State* L, Scene* scene)
{
	lua_newtable(L);

	luaL_Reg methods[] = {
		{ "createSystem", lua_createSystem },
		{ "getEntityCount", lua_getEntityCount },
		{ "entityValid", lua_entityValid },
		{ "createEntity", lua_createEntity },
		{ "removeEntity", lua_removeEntity },
		{ "hasComponent", lua_hasComponent },
		{ "getComponent", lua_getComponent },
		{ "setComponent", lua_setComponent },
		{ "removeComponent", lua_removeComponent },
		{ NULL , NULL }
	};

	lua_pushlightuserdata(L, scene);
	luaL_setfuncs(L, methods, 1);
	lua_setglobal(L, "scene");

	lua_newtable(L);
	for (size_t i = 0; i < systemTypes.size(); i++)
	{
		lua_pushnumber(L, i);
		lua_setfield(L, -2, systemTypes[i].c_str());
	}
	lua_setglobal(L, "SystemType");

	lua_newtable(L);
	for (size_t i = 0; i < compTypes.size(); i++)
	{
		lua_pushnumber(L, i);
		lua_setfield(L, -2, compTypes[i].c_str());
	}
	lua_setglobal(L, "ComponentType");
}

void Scene::setCamera(Vector3 pos, Vector3 lookDir, float fov)
{
	this->cam.position = pos;
	this->cam.target = { pos.x + lookDir.x, pos.y + lookDir.y, pos.z + lookDir.z };
	this->cam.fovy = fov;

	float rad = PI * 0.5f;
	this->cam.up = { lookDir.x, lookDir.y * cos(rad) - lookDir.z * sin(rad), lookDir.y * sin(rad) + lookDir.z * cos(rad) };
	this->cam.projection = CameraProjection::CAMERA_PERSPECTIVE;
}

Vector3 Scene::getCameraPos() const
{
	return this->cam.position;
}

void Scene::setCameraPos(Vector3 pos)
{
	this->cam.position = pos;
}

void Scene::updateSystems(float deltaTime)
{
	for (auto it = this->systems.begin(); it != this->systems.end();)
	{
		if ((*it)->update(this->reg, deltaTime))
		{
			delete (*it);
			it = this->systems.erase(it);
		}
		else
		{
			it++;
		}
	}
}

int Scene::getEntityCount() const
{
	return this->reg.alive();
}

bool Scene::entityValid(int entity) const
{
	return this->reg.valid((entt::entity)entity);
}

int Scene::createEntity()
{
	int entity = (int)this->reg.create();
	this->addComponent<Transform>(entity);
	return entity;
}

bool Scene::removeEntity(int entity)
{
	bool valid = this->entityValid(entity);
	if (valid)
		this->reg.destroy((entt::entity)entity);
	return valid;
}

int Scene::lua_createSystem(lua_State* L)
{
	return 0;
}

int Scene::lua_getEntityCount(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	lua_pushnumber(L, scene->getEntityCount());
	return 1;
}

int Scene::lua_entityValid(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	lua_pushboolean(L, scene->entityValid(lua_tointeger(L, 1)));
	return 1;
}

int Scene::lua_createEntity(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	lua_pushnumber(L, scene->createEntity());
	return 1;
}

int Scene::lua_removeEntity(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	lua_pushboolean(L, scene->removeEntity(lua_tointeger(L, 1)));
	return 1;
}

int Scene::lua_hasComponent(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	return 1;
}

int Scene::lua_getComponent(lua_State* L)
{
	return 0;
}

int Scene::lua_setComponent(lua_State* L)
{
	return 0;
}

int Scene::lua_removeComponent(lua_State* L)
{
	return 0;
}
