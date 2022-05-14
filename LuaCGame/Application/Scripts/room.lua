local room = {}

function room.new(width, height, doors, enemies)
	local t = {}
	t.width = width
	t.height = height
	t.doors = {}
	t.enemies = {}

	for x = 1, width or 0 do
		t[x] = {}
		for y = 1, height or 0 do
			t[x][y] = "Floor"
		end
	end

	for i = 1, width or 0 do
		t[i][1] = "Wall"
		t[i][height] = "Wall"
	end

	for i = 1, height or 0 do
		t[1][i] = "Wall"
		t[width][i] = "Wall"
	end

	for _, v in ipairs(doors) do
		table.insert(t.doors, vector(v.x, v.y, 0))
		t[v.x][v.y] = "Door"
	end

	for _, v in ipairs(enemies) do
		table.insert(t.enemies, vector(v.x, v.y, 0))
		t[v.x][v.y] = "Enemy"
	end

	setmetatable(t, room)
	return t
end

return setmetatable(room, {
	__call = function(_, ...)
	return room.new(...)
end
})