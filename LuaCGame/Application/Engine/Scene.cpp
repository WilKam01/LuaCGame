#include "Scene.h"
#include "Dev/StringHelper.h"

Scene::Scene():
	quit(false)
{
	this->setCamera(Vector3Zero(), Vector3Zero(), 90.0f);
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
		{ "setScene", lua_setScene },
		{ "quit", lua_quit },
		{ "setCamera", lua_setCamera },
		{ "getCameraPos", lua_getCameraPos },
		{ "setCameraPos", lua_setCameraPos },
		{ "getCameraRot", lua_getCameraRot },
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
	BeginMode3D(this->cam.cam3D);

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
			DrawModelWiresEx(*model, transform.position, { 0.0f, 1.0f, 0.0f }, -transform.rotation.y, transform.scale, BLACK);

			for (int i = 0; i < model->meshCount; i++)
			{
				DrawMesh(model->meshes[i], model->materials[model->meshMaterial[i]], matTransform);
			}
		}
	});
	EndMode3D();

	auto buttonView = this->reg.view<UIElement>();
	buttonView.each([&](const UIElement& elem)
	{
		if(elem.visibleBG)
			DrawRectangle(elem.position.x, elem.position.y, elem.dimensions.x, elem.dimensions.y, elem.colour);
		Vector2 size = MeasureTextEx(GetFontDefault(), elem.text.c_str(), elem.fontSize, 1);
		DrawText(elem.text.c_str(), 
			elem.position.x + (elem.dimensions.x - size.x) * 0.5f, 
			elem.position.y + (elem.dimensions.y - size.y) * 0.5f, 
			elem.fontSize, WHITE);
	});
}

bool Scene::shouldQuit()
{
	return this->quit;
}

void Scene::setCamera(Vector3 pos, Vector3 rotation, float fov)
{
	this->cam.cam3D.position = pos;
	this->cam.rotation = rotation;
	this->cam.cam3D.fovy = fov;
	this->cam.cam3D.projection = CameraProjection::CAMERA_PERSPECTIVE;

	Vector3 forwardDir = { 0.0f, 0.0f, 1.0f };
	Vector3 upDir = { 0.0f, 1.0f, 0.0f };
	Quaternion quaternion = QuaternionFromEuler(rotation.x * DEG2RAD, rotation.y * DEG2RAD, rotation.z * DEG2RAD);

	this->cam.cam3D.target = Vector3Add(pos, Vector3RotateByQuaternion(forwardDir, quaternion));
	this->cam.cam3D.up = Vector3RotateByQuaternion(upDir, quaternion);
}

Vector3 Scene::getCameraPos() const
{
	return this->cam.cam3D.position;
}

void Scene::setCameraPos(Vector3 pos)
{
	this->setCamera(pos, this->cam.rotation, this->cam.cam3D.fovy);
}

void Scene::setCameraRot(Vector3 rot)
{
	this->setCamera(this->cam.cam3D.position, rot, this->cam.cam3D.fovy);
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

int Scene::lua_setScene(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	std::string path = lua_tostring(L, 1);
	scene->setScene(L, path);

	return 0;
}

int Scene::lua_quit(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	scene->quit = true;
	return 0;
}

int Scene::lua_setCamera(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	Vector3 pos = lua_tovector(L, 1);
	Vector3 rot = lua_tovector(L, 2);
	float fov = lua_tonumber(L, 3);
	scene->setCamera(pos, rot, fov);

	return 0;
}

int Scene::lua_getCameraPos(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	lua_pushvector(L, scene->cam.cam3D.position);
	return 1;
}

int Scene::lua_setCameraPos(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	Vector3 pos = lua_tovector(L, 1);
	scene->setCameraPos(pos);
	return 0;
}

int Scene::lua_getCameraRot(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	lua_pushvector(L, scene->cam.rotation);
	return 1;
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
	else if (compTypes.at(type) == "UIElement")
		hasComp = scene->hasComponents<UIElement>(entity);

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
	else if (compTypes.at(type) == "Behaviour" && scene->hasComponents<Behaviour>(entity))
		lua_rawgeti(L, LUA_REGISTRYINDEX, scene->getComponent<Behaviour>(entity).luaRef);
	else if (compTypes.at(type) == "UIElement" && scene->hasComponents<UIElement>(entity))
		lua_pushuielement(L, scene->getComponent<UIElement>(entity));
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
	else if (compTypes.at(type) == "UIElement")
	{
		UIElement elem = lua_touielement(L, 3);
		if(elem.dimensions.x == 0 && elem.dimensions.y == 0)
			scene->setComponent<UIElement>(entity);
		else
			scene->setComponent<UIElement>(entity, lua_touielement(L, 3));
	}
	else if (compTypes.at(type) == "Behaviour")
	{
		if (scene->hasComponents<Behaviour>(entity))
			scene->removeComponent<Behaviour>(entity);

		std::string path = lua_tostring(L, 3);
		if (luaL_dofile(L, ("Scripts/Behaviour/" + path).c_str()) != LUA_OK)
			LuaHelper::dumpError(L);
		else
		{
			lua_pushvalue(L, -1);
			int luaRef = luaL_ref(L, LUA_REGISTRYINDEX);

			lua_pushinteger(L, entity);
			lua_setfield(L, -2, "ID");

			lua_pushstring(L, path.c_str());
			lua_setfield(L, -2, "path");

			lua_getfield(L, -1, "init");
			if (lua_type(L, -1) == LUA_TNIL)
			{
				lua_pop(L, 1);
				return 0;
			}

			lua_pushvalue(L, -2);
			if (lua_pcall(L, 1, 0, 0) != LUA_OK)
				LuaHelper::dumpError(L);
			else
			{
				scene->setComponent<Behaviour>(entity, path.c_str(), luaRef);
				return 1;
			}
		}
	}

	return 0;
}

int Scene::lua_removeComponent(lua_State* L)
{
	Scene* scene = (Scene*)lua_touserdata(L, lua_upvalueindex(1));
	int entity = (int)lua_tointeger(L, 1);
	int type = (int)lua_tointeger(L, 2);

	if (compTypes.at(type) == "Behaviour" && scene->hasComponents<Behaviour>(entity))
		scene->removeComponent<Behaviour>(entity);
	else if (compTypes.at(type) == "MeshComp" && scene->hasComponents<MeshComp>(entity))
		scene->removeComponent<MeshComp>(entity);
	else if (compTypes.at(type) == "UIElement" && scene->hasComponents<UIElement>(entity))
		scene->removeComponent<UIElement>(entity);

	return 0;
}
