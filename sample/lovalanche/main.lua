-- Example: Avalanche of LOVE

-- Contains all the balls.
balls = {}

-- Contains all the boxes. (Terrain)
boxes = {}

function moon.load()

	-- -- Fat lines.
	-- moon.graphics.setLineWidth(2)

	-- Load images.
	images = {
		green = moon.scene_graph.new_scene_node("green_ball.png"),
		big_love = moon.scene_graph.new_scene_node("big_love_ball.png"),
		moon = moon.scene_graph.new_scene_node("love_ball.png"),
	}

	-- Image / radius pairs.
	balldefs = {
		{ i = images.green, 	r = 32 , ox = 36, oy = 36},
		{ i = images.big_love, 	r = 46 , ox = 48, oy = 48},
		{ i = images.moon, 		r = 28 , ox = 32, oy = 32},
	}

	-- Create the world.
	world = moon.physics.newWorld(0, 50)

	-- Create ground body.
	ground = moon.physics.newBody(world, 0, 0, "static")

	-- Add all the balls.
	addball(balldefs[1], 50) -- Add 100 green.
	addball(balldefs[2], 5) -- Add 5 big.
	addball(balldefs[3], 25) -- Add 50 pink.

	-- This generates the terrain.
	for i = 0, 10 do
		addbox(i*50, i*50+100)
	end

end

function moon.update(dt)

	-- Update the world.
	world:update(dt)

	-- Check whether we need to reset some balls.
	-- When they move out of the screen, they
	-- respawn above the screen.
	for i,v in ipairs(balls) do
		local x, y = v.b:getPosition()
		if x > 850 or y > 650 then
			v.b:setPosition(math.random(0, 400), -math.random(100, 1500))
			v.b:setLinearVelocity(0, 0)
		end
	end

end

function moon.draw()
	-- Draw all the balls.
	for i, v in ipairs(balls) do
		moon.graphics.draw(v.i, v.b:getX(), v.b:getY(), v.b:getAngle(), 1, 1, v.ox, v.oy)
	end
	-- -- Draw all the boxes.
	-- for i, v in ipairs(boxes) do
	-- 	moon.graphics.polygon("line", v.s:getPoints())
	-- end
end

-- Adds a static box.
function addbox(x, y)
	local t = {}
	t.b = ground
	t.s = moon.physics.newRectangleShape(x, y, 50, 50)
	t.f = moon.physics.newFixture(t.b, t.s)
	table.insert(boxes, t)
end


-- Adds X balls.
function addball(def, num)
	for i = 1, num do
		local x, y = math.random(0, 400), -math.random(100, 1500)
		local t = {}
		t.b = moon.physics.newBody(world, x, y, "dynamic")
		t.s = moon.physics.newCircleShape(def.r)
		t.f = moon.physics.newFixture(t.b, t.s)
		t.i = def.i
		t.ox = def.ox
		t.oy = def.oy
		table.insert(balls, t)
	end
end
