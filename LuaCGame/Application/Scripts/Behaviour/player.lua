local player = {}

player.type = "Player"
player.speed = 10
player.lastMove = vector(0, 0, 0)

function boolToInt(t)
	if(t) then
		return 1
	end
	return 0
end

function player:init()
	print("Player created! (ID: " .. self.ID .. ")")
end

function player:update(deltaTime)
	--print("Player updated! (ID: " .. self.ID .. ") " .. deltaTime .. " sec")

	local dir = vector(
		boolToInt(input.isKeyDown(Keys.A)) - boolToInt(input.isKeyDown(Keys.D)), 0,
		boolToInt(input.isKeyDown(Keys.W)) - boolToInt(input.isKeyDown(Keys.S)))

	dir = dir:normalize()
	local transform = scene.getComponent(self.ID, ComponentType.Transform)
	self.lastMove = dir * self.speed * deltaTime
	transform.position = transform.position + self.lastMove
	scene.setCameraPos(transform.position + vector(0, 3, 0))

	local centerVec = input.getMousePositionCenter():normalize()
	transform.rotation = vector(0, math.acos(centerVec.x / centerVec:length()) * (180 / math.pi) + 90, 0)

	if(centerVec.y < 0) then
		transform.rotation.y = 180 - transform.rotation.y
	end
	local lookDir = vector(-centerVec.x, 0, -centerVec.y)

	transform.scale = vector(1, 1, 1) * 0.25
	scene.setComponent(self.ID, ComponentType.Transform, transform)

	if(input.isMouseButtonPressed(Mouse.LEFT)) then
		local entity = scene.createEntity()
		scene.setComponent(entity, ComponentType.Behaviour, "bullet.lua")
		local bullet = scene.getComponent(entity, ComponentType.Behaviour)
		bullet.velocity = lookDir

		local t = scene.getComponent(entity, ComponentType.Transform)
		t.position = transform.position
		scene.setComponent(entity, ComponentType.Transform, t)
	end

	if(input.isKeyPressed(Keys.R)) then
		scene.setScene("scene.lua")
	end
end

function player:collision(other)
	print(other.type)
end

return player