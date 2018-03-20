-------------------------------------------------
-- LOVE: Passing Clouds Demo								
-- Website: http://moon.sourceforge.net			
-- Licence: ZLIB/libpng									
-- Copyright (c) 2006-2009 LOVE Development Team
-------------------------------------------------

function moon.load()
	
	-- -- The amazing music.
	-- music = moon.audio.newSource("prondisk.xm")
	
	-- The various images used.
	body = moon.scene_graph.new_scene_node("body.png")
	ear = moon.scene_graph.new_scene_node("ear.png")
	face = moon.scene_graph.new_scene_node("face.png")
	logo = moon.scene_graph.new_scene_node("love.png")
	cloud = moon.scene_graph.new_scene_node("cloud_plain.png")

	-- Set the background color to soothing pink.
	moon.graphics.set_background_color(0xff, 0xf1, 0xf7)
	
	-- Spawn some clouds.
	for i=1,5 do
		spawn_cloud(math.random(-100, 900), math.random(-100, 700), 80 + math.random(0, 50))
	end
	
	moon.graphics.set_color(255, 255, 255, 200)
	
	-- moon.audio.play(music, 0)
	
end

function moon.update(dt)
	try_spawn_cloud(dt)
	
	nekochan:update(dt)
	
	-- Update clouds.
	for k, c in ipairs(clouds) do
		c.x = c.x + c.s * dt
	end
end

function moon.draw()

	moon.graphics.draw(logo, 400, 380, 0, 1, 1, 128, 64)
	
	for k, c in ipairs(clouds) do
		moon.graphics.draw(cloud, c.x, c.y)
	end
	
	nekochan:render()
	
end

function moon.keypressed(k)
	if k == "r" then
		moon.filesystem.load("main.lua")()
	end
end


nekochan = {
	x = 400, 
	y = 250, 
	a = 0
}

function nekochan:update(dt)
		self.a = self.a + 10 * dt	
end

function nekochan:render()
	moon.graphics.draw(body, self.x, self.y, 0, 1, 1, 64, 64)
	moon.graphics.draw(face, self.x, self.y + math.sin(self.a/5) * 3, 0, 1, 1, 64, 64)
	local r = 1 + math.sin(self.a*math.pi/20)
	for i = 1,10 do
		moon.graphics.draw(ear, self.x, self.y, (i * math.pi*2/10) + self.a/10, -1, 1, 16, 64+10*r)
	end
	
end

-- Holds the passing clouds.
clouds = {}

cloud_buffer = 0
cloud_interval = 1

-- Inserts a new cloud.
function try_spawn_cloud(dt)

	cloud_buffer = cloud_buffer + dt
	
	if cloud_buffer > cloud_interval then
		cloud_buffer = 0
		spawn_cloud(-512, math.random(-50, 500), 80 + math.random(0, 50))
	end
		
end

function spawn_cloud(xpos, ypos, speed)
	table.insert(clouds, { x = xpos, y = ypos, s = speed } )
end
