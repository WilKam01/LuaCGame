local room = require("Scripts/room")
local map = {}

local directions = { vector(0, 0, 1), vector(0, 0, -1), vector(-1, 0, 0), vector(1, 0, 0) }

map.playerID = 0
map.startroom = room(9, 9, { vector(4, 1), vector(4, 9), vector(1, 4), vector(9, 4) })
map.endroom = room(7, 7, { vector(3, 1), vector(3, 7), vector(1, 3), vector(7, 3) })
map.roomTemplates = {};
map.rooms = {}
map.curRoom = 0
map.curRoomIndex = 0
map.IDs = {}

map.roomTemplates[1] = room(5, 7, { vector(3, 1), vector(3, 7), vector(1, 3), vector(5, 3) })
map.roomTemplates[2] = map.roomTemplates[1]

function map:makelayout()
	self.rooms = {}
	self.rooms[1] = self.startroom

	local i = 2
	while(i <= 10) do
		local newR = self.roomTemplates[math.random(1, #self.roomTemplates)]
		self.rooms[i] = room(newR.width, newR.height, newR.doors)

		local connectRoomIndex = math.random(1, i - 1)
		local ownDoor = math.random(1, #self.rooms[i].doors)
		if (self.rooms[i].doors[ownDoor].z == 0) then
			local r1 = self.rooms[i]
			local v1 = r1.doors[ownDoor]
			local r2 = self.rooms[connectRoomIndex]
			for j, v2 in ipairs(r2.doors) do
				if (v2.z == 0 and v1.z == 0 and (
				(v1.x == 1 and v2.x == r2.width) or
				(v1.y == 1 and v2.y == r2.height) or 
				(v2.x == 1 and v1.x == r1.width or
				(v2.y == 1 and v1.y == r1.height)))) then
					-- Using z value for connectingIndex
					self.rooms[i].doors[ownDoor].z = connectRoomIndex
					self.rooms[connectRoomIndex].doors[j].z = i
					i = i + 1
					break
				end
			end
		end
	end


end

function map:spawnroom(index)
	local r = self.rooms[index]
	r.IDs = {}
	local offset = vector(r.width, 0, r.height) / 2 + vector(1, 0, 1) * 0.5
	for x = 1, r.width do
		for z = 1, r.height do
			local entity = scene.createEntity()
			local transform = scene.getComponent(entity, ComponentType.Transform)
			transform.position = vector(x, -1, z) - offset
			if (r[x][z] == "Floor") then
				scene.setComponent(entity, ComponentType.MeshComp, "Cube")
			elseif (r[x][z] == "Wall") then
				scene.setComponent(entity, ComponentType.MeshComp, "Sphere")
			elseif (r[x][z] == "Door") then
				for _, v in ipairs(r.doors) do
					if(v.x == x and v.y == z and v.z == 0) then
						scene.setComponent(entity, ComponentType.MeshComp, "Sphere")
						r[x][z] = "Wall"
						break
					end
				end
			end
			table.insert(self.IDs, entity)
			scene.setComponent(entity, ComponentType.Transform, transform)
		end
	end
	self.curRoom = r
	self.curRoomIndex = index
	print(self.curRoomIndex)
end

function map:despawnroom(index)
	for _, v in ipairs(self.IDs) do
		scene.removeEntity(v)
	end
	self.IDs = {}
end

function map:init()

end

function map:update()

	local player = scene.getComponent(self.playerID, ComponentType.Behaviour)
	local playerTransform = scene.getComponent(self.playerID, ComponentType.Transform)
	local pos = playerTransform.position + vector(self.curRoom.width, 0, self.curRoom.height) / 2 - vector(1, 0, 1) * 0.5
	pos = vector(math.floor(pos.x + 0.5), 0, math.floor(pos.z + 0.5)) + vector(1, 0, 1)

	-- Collision with wall (edge)
	if(pos.x <= 1 or pos.z <= 1 or pos.x > self.curRoom.width - 1 or pos.z > self.curRoom.height - 1) then
		
		-- Collision with door
		if(self.curRoom[pos.x][pos.z] == "Door") then
			-- Find right door
			for _, v in ipairs(self.curRoom.doors) do
				if(v.x == pos.x and v.y == pos.z) then
					-- Set player pos next to door in new room
					local spawnPos = vector()
					for __, v1 in ipairs(self.rooms[v.z].doors) do
						if(v1.z == self.curRoomIndex) then
							spawnPos = vector(v1.x, 0, v1.y) + player.lastMove * 5
							break
						end
					end
					playerTransform.position = spawnPos - vector(self.rooms[v.z].width, 0, self.rooms[v.z].height) / 2 - vector(1, 0, 1) * 0.5

					-- Spawn new room
					self:despawnroom(self.curRoomIndex)
					self:spawnroom(v.z)
					break
				end
			end
		else
			playerTransform.position = playerTransform.position - player.lastMove
		end
		scene.setComponent(self.playerID, ComponentType.Transform, playerTransform)
	end
end

return map