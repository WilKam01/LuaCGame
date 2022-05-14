local room = {}

function room.new(width, height, doors)
	local t = {}
	t.width = width
	t.height = height
	t.doors = {}

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

	setmetatable(t, room)
	return t
end

function room.getdoor(r, num)
	if (num == 1) then
		return { r.doors[num], 0 }
	elseif (num == 2) then
		return { r.doors[num], r.height - 1 }
	elseif (num == 3) then
		return { 0, r.doors[num] }
	elseif (num == 4) then
		return {r.width - 1, r.doors[num]}
	end

	return
end

return setmetatable(room, {
	__call = function(_, ...)
	return room.new(...)
end
})