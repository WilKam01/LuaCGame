#pragma once

#include "entt.hpp"

class Scene
{
private:
	entt::registry reg;
public:
	Scene();
	virtual ~Scene();

	int getEntityCount() const;
	bool entityValid(int entity) const;

	int createEntity();
	void removeEntity(int entity);

	template <typename ...Args>
	bool hasComponents(int entity);

	template <typename T>
	T& getComponent(int entity);

	template <typename T>
	void addComponent(int entity, const T&);

	template <typename T, typename ...Args>
	void addComponent(int entity, Args... args);

	template <typename T>
	void removeComponent(int entity);
};

template < typename ... Args >
bool Scene::hasComponents(int entity)
{
	return this->reg.all_of <Args ...>(
		(entt::entity)entity);
}

template < typename T>
T& Scene::getComponent(int entity)
{
	return this->reg.get <T>(
		(entt::entity)entity);
}

template < typename T>
void Scene::addComponent(int entity,
	const T& component)
{
	this->reg.emplace_or_replace <T>(
		(entt::entity)entity, component);
}

template < typename T, typename ...Args >
void Scene::addComponent(int entity,
	Args ... args)
{
	this->reg.emplace_or_replace <T>(
		(entt::entity)entity, args ...);
}

template < typename T>
void Scene::removeComponent(int entity)
{
	this->reg.remove <T>((entt::entity)entity);
}


