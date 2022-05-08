local vector = require("Scripts/vector")
scene.loadResource("suzanne.obj", "Suzanne")

function printStats (t, indent)
	for k,v in pairs(t) do
	if type(v) == "table" then
		io.write(indent .. k .. "\n")
			printStats(v, indent .. " ")
		else
			io.write(indent .. k .. " = " .. tostring(v) .. "\n")
		end
	end
end

local offsetVec = vector(10, 10, 10)

for x = 0, 9 do
	for z = 0, 9 do
		local entity = scene.createEntity();
		scene.setComponent(entity, ComponentType.MeshComp, "Suzanne")
		transform = scene.getComponent(entity, ComponentType.Transform)
		transform.position = vector(x, 0, z) * 2 - offsetVec
		transform.rotation.y = math.random(0, 360)
		scene.setComponent(entity, ComponentType.Transform, transform)
	end
end