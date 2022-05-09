local vector = require("Scripts/vector")

scene.loadResource("suzanne.obj", "Suzanne")
scene.setCamera(vector(0, 5, 0), vector(0, -1, 0), math.random(30, 90))

local gridSize = 10
local offsetScalar = 3
local offsetVec = vector(gridSize * offsetScalar * 0.5, gridSize * offsetScalar * 0.5, gridSize * offsetScalar * 0.5)

for x = 0, gridSize - 1 do
	for z = 0, gridSize - 1 do
		local entity = scene.createEntity();
		scene.setComponent(entity, ComponentType.MeshComp, "Suzanne")
		transform = scene.getComponent(entity, ComponentType.Transform)
		transform.position = vector(x + 0.5, 0, z + 0.5) * offsetScalar - offsetVec
		transform.rotation = vector(math.random(0, 360), math.random(0, 360), math.random(0, 360))
		scene.setComponent(entity, ComponentType.Transform, transform)
	end
end

print(input.getCharPressed())