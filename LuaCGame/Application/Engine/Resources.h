#pragma once

#include <unordered_map>
#include "raylib.h"
#include "raymath.h"

class Resources
{
private:
	std::unordered_map<std::string, Model> models;
public:
	Resources();
	virtual ~Resources();

	bool hasModel(std::string name);
	Model* getModel(std::string name);
	void loadModel(std::string path, std::string name);
};

