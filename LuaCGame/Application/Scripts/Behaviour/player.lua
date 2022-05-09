local player = {}
local speed = 25

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
	scene.setCameraPos(scene.getCameraPos() + dir * speed * deltaTime)

	if(input.isKeyPressed(Keys.R)) then
		scene.setScene("scene.lua")
	end
end

return player