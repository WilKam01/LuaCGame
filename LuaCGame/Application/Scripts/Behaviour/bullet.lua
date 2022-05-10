local bullet = {}

bullet.type = "Bullet"
bullet.timer = 1
bullet.speed = 35
bullet.velocity = vector(0, 0, 1)

function bullet:init()
	print("Bullet fired! (ID: " .. self.ID .. ")")
	scene.setComponent(self.ID, ComponentType.MeshComp, "Sphere")
end

function bullet:update(deltaTime)
	local transform = scene.getComponent(self.ID, ComponentType.Transform)
	transform.position = transform.position + self.velocity * self.speed * deltaTime
	transform.scale = vector(1, 1, 1) * 0.5
	scene.setComponent(self.ID, ComponentType.Transform, transform)

	self.timer = self.timer - deltaTime
	if(self.timer <= 0) then
		scene.removeEntity(self.ID)
	end
end

return bullet