#include "Resources.h"
#include <iostream>

Resources::Resources()
{
	/*this->models.insert(std::pair<std::string, Model>("Cube", LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f))));
	this->models.insert(std::pair<std::string, Model>("Sphere", LoadModelFromMesh(GenMeshSphere(1.0f, 16, 16))));
	this->models.insert(std::pair<std::string, Model>("Cylinder", LoadModelFromMesh(GenMeshCylinder(1.0f, 2.0f, 16))));
	this->models.insert(std::pair<std::string, Model>("Plane", LoadModelFromMesh(GenMeshPlane(1.0f, 1.0f, 1, 1))));*/
}

Resources::~Resources()
{
	for (auto& element : this->models)
		UnloadModel(element.second);
}

bool Resources::hasModel(std::string name)
{
	return this->models.count(name) > 0;
}

Model* Resources::getModel(std::string name)
{
	Model* model = nullptr;
	if (this->hasModel(name))
		model = &models[name];

	return model;
}

void Resources::loadModel(std::string path, std::string name)
{
	if (this->hasModel(name))
	{
		std::cout << name << " has already been loaded\n";
		return;
	}

	Model model = LoadModel(("../Resources/Models/" + path).c_str());
	this->models.insert(std::pair<std::string, Model>(name, model));
}
