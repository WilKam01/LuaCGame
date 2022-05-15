local menu = {}
local element = {}

function menu:init()
	element.visibleBG = true
	element.text = ""
	element.position = vector(490, 200)
	element.dimensions = vector(300, 100)
	element.colour = vector(200, 200, 200)

	element.text = "Play!"
	self.playButton = scene.createEntity()
	scene.setComponent(self.playButton, ComponentType.UIElement, element)

	element.text = "Create Room"
	element.position.y = 350
	self.roomButton = scene.createEntity()
	scene.setComponent(self.roomButton, ComponentType.UIElement, element)

	element.text = "Exit"
	element.position.y = 500
	self.exitButton = scene.createEntity()
	scene.setComponent(self.exitButton, ComponentType.UIElement, element)
end

function menu:update(deltaTime)
	local button = scene.getComponent(self.playButton, ComponentType.UIElement)
	if (UI.isHover(button)) then
		button.colour = vector(225, 225, 225)
		scene.setComponent(self.playButton, ComponentType.UIElement, button)
		if (input.isMouseButtonPressed(Mouse.LEFT)) then
			scene.setScene("gameScene.lua")
			return
		end
	elseif (button.colour == vector(225, 225, 225)) then
		button.colour = vector(200, 200, 200)
		scene.setComponent(self.playButton, ComponentType.UIElement, button)
	end

	button = scene.getComponent(self.roomButton, ComponentType.UIElement)
	if (UI.isHover(button)) then
		button.colour = vector(225, 225, 225)
		scene.setComponent(self.roomButton, ComponentType.UIElement, button)
		if (input.isMouseButtonPressed(Mouse.LEFT)) then
			--scene.setScene("gameScene.lua")
			--return
		end
	elseif (button.colour == vector(225, 225, 225)) then
		button.colour = vector(200, 200, 200)
		scene.setComponent(self.roomButton, ComponentType.UIElement, button)
	end

	button = scene.getComponent(self.exitButton, ComponentType.UIElement)
	if (UI.isHover(button)) then
		button.colour = vector(225, 225, 225)
		scene.setComponent(self.exitButton, ComponentType.UIElement, button)
		if (input.isMouseButtonPressed(Mouse.LEFT)) then
			scene.quit()
		end
	elseif (button.colour == vector(225, 225, 225)) then
		button.colour = vector(200, 200, 200)
		scene.setComponent(self.exitButton, ComponentType.UIElement, button)
	end
end

return menu