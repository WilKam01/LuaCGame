scene.loadResource("suzanne.obj", "Suzanne")
scene.setCamera(vector(0, 5, 0), vector(90, 0, 0), 90)

local gridSize = 10
local offsetScalar = 10
local offsetVec = vector(1, 1, 1) * gridSize * offsetScalar * 0.5

for x = 0, gridSize - 1 do
	for z = 0, gridSize - 1 do
		local entity = scene.createEntity();
		scene.setComponent(entity, ComponentType.MeshComp, "Cube")
		transform = scene.getComponent(entity, ComponentType.Transform)
		transform.position = vector(x + 0.5, 0, z + 0.5) * offsetScalar - offsetVec
		transform.rotation = vector(math.random(0, 360), math.random(0, 360), math.random(0, 360))
		transform.scale = vector(5, 5, 5)
		scene.setComponent(entity, ComponentType.Transform, transform)
	end
end

local entity = scene.createEntity();
scene.setComponent(entity, ComponentType.Behaviour, "player.lua")
scene.setComponent(entity, ComponentType.MeshComp, "Suzanne")