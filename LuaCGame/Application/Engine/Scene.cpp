#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
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
	return (int)this->reg.create();
}

void Scene::removeEntity(int entity)
{
	if (this->entityValid(entity))
		this->reg.destroy((entt::entity)entity);
}
