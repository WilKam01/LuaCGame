#pragma once

#include "Dev/LuaHelper.h"
#include "raylib.h"
#include "raymath.h"
#include "Systems/System.h"
#include "Resources.h"
#include "Components/Componentpch.h";

class Scene
{
private:
	entt::registry reg;
	Resources resources;
	std::vector<System*> systems;
	Camera3D cam;

	inline static const std::vector<std::string> compTypes{
		"Transform",
		"MeshComp",
		"Behaviour"
	};

	inline static const std::vector<std::string> systemTypes{
		"CleanUp",
		"Info"
	};

	// Lua wrappers
	static int lua_createSystem(lua_State* L);
	static int lua_loadResource(lua_State* L);

	static int lua_setCamera(lua_State* L);
	static int lua_getCameraPos(lua_State* L);
	static int lua_setCameraPos(lua_State* L);

	static int lua_getEntityCount(lua_State* L);
	static int lua_entityValid(lua_State* L);
	static int lua_createEntity(lua_State* L);
	static int lua_removeEntity(lua_State* L);

	static int lua_hasComponent(lua_State* L);
	static int lua_getComponent(lua_State* L);
	static int lua_setComponent(lua_State* L);
	static int lua_removeComponent(lua_State* L);
public:
	Scene();
	virtual ~Scene();

	static void lua_openscene(lua_State* L, Scene* scene);

	// @param Name of lua script to open scene
	void setScene(lua_State* L, std::string path);
	void render();

	void setCamera(Vector3 pos, Vector3 lookDir, float fov);
	Vector3 getCameraPos() const;
	void setCameraPos(Vector3 pos);

	template <typename T, typename ...Args>
	void createSystem(Args... args);

	void updateSystems(float deltaTime);

	int getEntityCount() const;
	bool entityValid(int entity) const;

	int createEntity();
	bool removeEntity(int entity);

	template <typename ...Args>
	bool hasComponents(int entity);

	template <typename T>
	T& getComponent(int entity);

	template <typename T>
	void setComponent(int entity, const T&);

	template <typename T, typename ...Args>
	void setComponent(int entity, Args... args);

	template <typename T>
	void removeComponent(int entity);
};

template<typename T, typename ...Args>
inline void Scene::createSystem(Args ...args)
{
	this->systems.emplace_back(new T(args...));
}

template <typename ...Args>
bool Scene::hasComponents(int entity)
{
	return this->reg.all_of<Args...>(
		(entt::entity)entity);
}

template <typename T>
T& Scene::getComponent(int entity)
{
	return this->reg.get<T>(
		(entt::entity)entity);
}

template <typename T>
void Scene::setComponent(int entity,
	const T& component)
{
	this->reg.emplace_or_replace<T>(
		(entt::entity)entity, component);
}

template <typename T, typename ...Args>
void Scene::setComponent(int entity,
	Args... args)
{
	this->reg.emplace_or_replace<T>(
		(entt::entity)entity, args...);
}

template <typename T>
void Scene::removeComponent(int entity)
{
	this->reg.remove<T>((entt::entity)entity);
}


