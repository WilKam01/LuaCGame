scene.loadResource("suzanne.obj", "Suzanne")
scene.setCamera(vector(0, 2, 0), vector(90, 0, 0), 90)

local gridSize = 10
local offsetScalar = 10
local offsetVec = vector(1, 1, 1) * gridSize * offsetScalar * 0.5

--[[for x = 0, gridSize - 1 do
	for z = 0, gridSize - 1 do
		local entity = scene.createEntity();
		scene.setComponent(entity, ComponentType.MeshComp, "Cube")
		transform = scene.getComponent(entity, ComponentType.Transform)
		transform.position = vector(x + 0.5, 0, z + 0.5) * offsetScalar - offsetVec
		transform.rotation = vector(math.random(0, 360), math.random(0, 360), math.random(0, 360))
		transform.scale = vector(5, 5, 5)
		scene.setComponent(entity, ComponentType.Transform, transform)
	end
end]]--

local playerEntity = scene.createEntity();
scene.setComponent(playerEntity, ComponentType.Behaviour, "player.lua")
scene.setComponent(playerEntity, ComponentType.MeshComp, "Suzanne")

local entity = scene.createEntity()
scene.setComponent(entity, ComponentType.Behaviour, "map.lua")

local map = scene.getComponent(entity, ComponentType.Behaviour)
map.playerID = playerEntity
map:makelayout()
map:spawnroom(1)