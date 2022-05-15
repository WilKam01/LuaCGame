local room = require("Scripts/room")
local map = {}

map.playerID = 0
map.startroom = room(9, 9, { vector(4, 1), vector(4, 9), vector(1, 4), vector(9, 4) }, {})
map.endroom = room(7, 7, { vector(3, 1), vector(3, 7), vector(1, 3), vector(7, 3) }, {})
map.roomCount = 10
map.template = room(6, 6, { vector(3, 1), vector(3, 6), vector(1, 3), vector(6, 3) }, { vector(3, 3) })
map.rooms = {}
map.curRoom = 0
map.curRoomIndex = 0
map.locked = true
map.IDs = {}
map.EnemyIDs = {}
map.DoorIDs = {}

function map:makelayout()
	self.rooms = {}
	self.startroom = room(self.startroom.width, self.startroom.height, self.startroom.doors, {})
	self.endroom = room(self.endroom.width, self.endroom.height, self.endroom.doors, {})

	self.rooms[1] = self.startroom
	self.IDs = {}
	self.EnemyIDs = {}
	self.DoorIDs = {}

	local i = 2
	while(i <= self.roomCount) do
		local connectRoomIndex = 0
		if (i == self.roomCount) then
			self.rooms[i] = self.endroom
			connectRoomIndex = math.random(2, i - 1)
		else
			local newR = self.roomTemplates[math.random(1, #self.roomTemplates)]
			self.rooms[i] = room(newR.width, newR.height, newR.doors, newR.enemies)
			connectRoomIndex = math.random(1, i - 1)
		end

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
	local offset = vector(r.width, 0, r.height) / 2 + vector(1, 0, 1) * 0.5
	for x = 1, r.width do
		for z = 1, r.height do
			local entity = scene.createEntity()
			local transform = scene.getComponent(entity, ComponentType.Transform)
			transform.position = vector(x, -1, z) - offset
			if (r[x][z] == "Floor") then
				scene.setComponent(entity, ComponentType.MeshComp, "Floor")
			elseif (r[x][z] == "Wall") then
				scene.setComponent(entity, ComponentType.MeshComp, "Wall")
			elseif (r[x][z] == "Door") then
				for i, v in ipairs(r.doors) do
					if(v.x == x and v.y == z) then
						if (v.z == 0) then
							scene.setComponent(entity, ComponentType.MeshComp, "Wall")
							r[x][z] = "Wall"
							table.remove(r.doors, i)
						else
							scene.setComponent(entity, ComponentType.MeshComp, "Door")
							table.insert(self.DoorIDs, entity)
						end
						break
					end
				end
			elseif (r[x][z] == "Enemy") then
				scene.setComponent(entity, ComponentType.MeshComp, "Floor")

				local enemy = scene.createEntity();
				local enemyTransform = scene.getComponent(enemy, ComponentType.Transform)
				enemyTransform.position = transform.position + vector(0, 1, 0)
				scene.setComponent(enemy, ComponentType.Transform, enemyTransform)

				scene.setComponent(enemy, ComponentType.MeshComp, "Enemy")
				scene.setComponent(enemy, ComponentType.Behaviour, "enemy.lua")

				table.insert(self.EnemyIDs, enemy)
			end
			table.insert(self.IDs, entity)
			scene.setComponent(entity, ComponentType.Transform, transform)
		end
	end
	if (index == self.roomCount) then
		local entity = scene.createEntity()
		scene.setComponent(entity, ComponentType.MeshComp, "Goal")
		table.insert(self.IDs, entity)
	end

	self.curRoom = r
	self.curRoomIndex = index
	self.locked = true
end

function map:despawnroom(index)
	for _, v in ipairs(self.IDs) do
		scene.removeEntity(v)
	end
	for _, v in ipairs(self.EnemyIDs) do
		scene.removeEntity(v)
	end
	self.IDs = {}
	self.EnemyIDs = {}
	self.DoorIDs = {}
end

function map:init()
	self.roomTemplates = {}
	table.insert(self.roomTemplates, self.template)
	local r = room.newFromFile("../Resources/Rooms/1.room")
	local counter = 2

	while(r ~= nil) do
		table.insert(self.roomTemplates, r)
		r = room.newFromFile("../Resources/Rooms/" .. tostring(counter) .. ".room")
		counter = counter + 1
	end
end

function map:update()
	local player = scene.getComponent(self.playerID, ComponentType.Behaviour)
	local playerTransform = scene.getComponent(self.playerID, ComponentType.Transform)
	local pos = playerTransform.position + vector(self.curRoom.width, 0, self.curRoom.height) / 2 - vector(1, 0, 1) * 0.5
	pos = vector(math.floor(pos.x + 0.5), 0, math.floor(pos.z + 0.5)) + vector(1, 0, 1)

	-- Collision with wall (edge)
	if(pos.x <= 1 or pos.z <= 1 or pos.x > self.curRoom.width - 1 or pos.z > self.curRoom.height - 1) then
		-- Collision with door
		if(self.curRoom[pos.x][pos.z] == "Door" and self.locked == false) then
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
	end

	for i, v in ipairs(self.EnemyIDs) do
		if (not scene.entityValid(v)) then
			table.remove(self.EnemyIDs, i)
		end
	end

	if (#self.EnemyIDs == 0 and self.locked == true) then
		self.locked = false
		for _, v in ipairs(self.DoorIDs) do
			scene.setComponent(v, ComponentType.MeshComp, "Floor")
		end
		for _, v in ipairs(self.curRoom.enemies) do
			self.curRoom[v.x][v.y] = "Floor" 
		end
		self.curRoom.enemies = {}
	end

	-- Found goal
	if (self.curRoomIndex == self.roomCount and pos.x == math.ceil(self.curRoom.width * 0.5) and pos.z == math.ceil(self.curRoom.height * 0.5)) then
		self:despawnroom(self.curRoomIndex)
		self:makelayout()
		self:spawnroom(1)
		playerTransform.position = vector()
	end
	scene.setComponent(self.playerID, ComponentType.Transform, playerTransform)
end

return map