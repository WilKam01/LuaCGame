local enemy = {}
enemy.time = 1
enemy.type = "Enemy"

function enemy:init()
	self.time = math.random(1, 3)
end

function enemy:update(deltaTime)
	self.time = self.time - deltaTime
	if (self.time <= 0) then
		scene.removeEntity(self.ID)
	end
end

return enemy