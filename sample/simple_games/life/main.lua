function moon.load()
	-- moon._openConsole()
	moon.graphics.set_background_color(255, 255, 255)

	cellSize = 5

	gridXCount = 70
	gridYCount = 50

	grid = {}
	for y = 1, gridYCount do
		grid[y] = {}
		for x = 1, gridXCount do
			grid[y][x] = false
		end
	end

	-- moon.keyboard.setKeyRepeat(true)
end

function moon.update()
	local x, y = moon.mouse.get_position()
	selectedX = math.floor(x / cellSize) + 1
	selectedY = math.floor(y / cellSize) + 1

	-- if moon.mouse.isDown(1)
	-- and selectedX <= gridXCount
	-- and selectedY <= gridYCount then
	-- 	grid[selectedY][selectedX] = true
	-- elseif moon.mouse.isDown(2) then
	-- 	grid[selectedY][selectedX] = false
	-- end
end

function moon.draw()
	for y = 1, 50 do
		for x = 1, 70 do
			local cellDrawSize = cellSize - 1

			if x == selectedX and y == selectedY then
				moon.graphics.set_color(0, 255, 255)
			elseif grid[y][x] then
				moon.graphics.set_color(255, 0, 255)
			else
				moon.graphics.set_color(220, 220, 220)
			end

			moon.graphics.rectangle(
				'fill',
				(x - 1) * cellSize,
				(y - 1) * cellSize,
				cellDrawSize,
				cellDrawSize
			)
		end
	end
end

function moon.keypressed()
	local nextGrid = {}
	for y = 1, gridYCount do
		nextGrid[y] = {}
		for x = 1, gridXCount do
			local neighbours = 0

			for dy = -1, 1 do
				for dx = -1, 1 do
					if not (dy == 0 and dx == 0)
					and grid[y + dy]
					and grid[y + dy][x + dx] then
						neighbours = neighbours + 1
					end
				end
			end

			nextGrid[y][x] = neighbours == 3 or (grid[y][x] and neighbours == 2)
		end
	end

	grid = nextGrid
end

