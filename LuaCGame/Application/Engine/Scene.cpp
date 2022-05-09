#include "Scene.h"
#include "Dev/StringHelper.h"

Scene::Scene()
{
	this->setCamera(Vector3Zero(), { 0.0f, 0.0f, 1.0f }, 90.0f);
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
		{ "loadResource", lua_loadResource },
		{ "setCamera", lua_setCamera },
		{ "getCameraPos", lua_getCameraPos },
		{ "setCameraPos", lua_setCameraPos },
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
		lua_pushnumber(L, (int)i);
		lua_setfield(L, -2, systemTypes[i].c_str());
	}
	lua_setglobal(L, "SystemType");

	lua_newtable(L);
	for (size_t i = 0; i < compTypes.size(); i++)
	{
		lua_pushnumber(L, (int)i);
		lua_setfield(L, -2, compTypes[i].c_str());
	}
	lua_setglobal(L, "ComponentType");
}

void Scene::setScene(lua_State* L, std::string path)
{
	this->reg.clear();
	if (luaL_dofile(L, ("Scripts/Scenes/" + path).c_str()) != LUA_OK)
		LuaHelper::dumpError(L);
}

void Scene::render()
{
	BeginMode3D(this->cam);

	auto view = this->reg.view<TransformComp, MeshComp>();
	view.each([&](const TransformComp& transform, const MeshComp& meshComp)
	{
		Model* model = this->resources.getModel(meshComp.modelName);
		if (model)
		{
			Matrix matTranslation = MatrixTranslate(transform.position.x, transform.position.y, transform.position.z);
			Matrix matRotation = MatrixRotateXYZ(Vector3Scale(transform.rotation, DEG2RAD));
			Matrix matScale = MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z);
			Matrix matTransform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

			for (int i = 0; i < model->meshCount; i++)
			{
				DrawMesh(model->meshes[i], model->materials[model->meshMaterial[i]], matTransform);
			}
		}
	});

	EndMode3D();
}

void Scene::setCamera(Vector3 pos, Vector3 lookDir, float fov)
{
	this->cam.position = pos;
	this->cam.target = { pos.x + lookDir.x, pos.y + lookDir.y, pos.z + lookDir.z };
	this->cam.fovy = fov;

	float rad = 90.0f * DEG2RAD;
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
	return (int)this->reg.alive();
}

bool Scene::entityValid(int entity) const
{
	return this->reg.valid((entt::entity)entity);
}

int Scene::createEntity()
{
	int entity = (int)this->reg.create();
	this->setComponent<TransformComp>(entity);
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
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	int type = (int)lua_tointeger(L, 1);

	/*if (systemTypes.at(type) == "CleanUp")
		scene->createSystem<CleanUp>();*/
	/*else if (systemTypes.at(type) == "Info")
		scene->createSystem<CleanUp>();*/

	return 0;
}

int Scene::lua_loadResource(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	std::string path = lua_tostring(L, 1);
	std::string name = lua_tostring(L, 2);
	scene->resources.loadModel(path, name);

	return 0;
}

int Scene::lua_setCamera(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	Vector3 pos = lua_tovector(L, 1);
	Vector3 lookDir = lua_tovector(L, 2);
	float fov = lua_tonumber(L, 3);
	scene->setCamera(pos, lookDir, fov);

	return 0;
}

int Scene::lua_getCameraPos(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	lua_pushvector(L, scene->cam.position);
	return 1;
}

int Scene::lua_setCameraPos(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	Vector3 pos = lua_tovector(L, 1);
	scene->cam.position = pos;
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
	lua_pushboolean(L, scene->entityValid((int)lua_tointeger(L, 1)));
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
	lua_pushboolean(L, scene->removeEntity((int)lua_tointeger(L, 1)));
	return 1;
}

int Scene::lua_hasComponent(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	int entity = (int)lua_tointeger(L, 1);
	int type = (int)lua_tointeger(L, 2);
	bool hasComp = false;

	if (compTypes.at(type) == "Transform")
		hasComp = scene->hasComponents<TransformComp>(entity);
	else if (compTypes.at(type) == "Behaviour")
		hasComp = scene->hasComponents<Behaviour>(entity);

	lua_pushboolean(L, hasComp);
	return 1;
}

int Scene::lua_getComponent(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	int entity = (int)lua_tointeger(L, 1);
	int type = (int)lua_tointeger(L, 2);

	if (compTypes.at(type) == "Transform" && scene->hasComponents<TransformComp>(entity))
		lua_pushtransform(L, scene->getComponent<TransformComp>(entity));
	//else if (compTypes.at(type) == "Behaviour" && scene->hasComponents<Behaviour>(entity))
	else
		lua_pushnil(L);

	return 1;
}

int Scene::lua_setComponent(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	int entity = (int)lua_tointeger(L, 1);
	int type = (int)lua_tointeger(L, 2);

	if (compTypes.at(type) == "Transform")
		scene->setComponent<TransformComp>(entity, lua_totransform(L, 3));
	else if (compTypes.at(type) == "MeshComp")
		scene->setComponent<MeshComp>(entity, lua_tostring(L, 3));
	/*else if (compTypes.at(type) == "Behaviour")
		scene->setComponent<Behaviour>(entity);*/

	return 0;
}

int Scene::lua_removeComponent(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	int entity = (int)lua_tointeger(L, 1);
	int type = (int)lua_tointeger(L, 2);

	if (compTypes.at(type) == "Transform" && scene->hasComponents<TransformComp>(entity))
		scene->removeComponent<TransformComp>(entity);
	else if (compTypes.at(type) == "Behaviour" && scene->hasComponents<Behaviour>(entity))
		scene->removeComponent<Behaviour>(entity);

	return 0;
}
