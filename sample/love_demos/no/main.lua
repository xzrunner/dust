-----------------------
-- NO: A game of numbers
-- Created: 23.08.08 by Michael Enger
-- Version: 0.2
-- Website: http://www.facemeandscream.com
-- Licence: ZLIB
-----------------------
-- This is the main file where we enter the game.
-----------------------

require("lua/button")
require("lua/states")

function moon.load()

	-- Resources
	color =	 {	background = {240,243,247},
				main = {63,193,245},
				text = {76,77,78},
				overlay = {255,255,255,235} }
	-- font = {	default = moon.graphics.newFont(24),
	-- 			large = moon.graphics.newFont(32),
	-- 			huge = moon.graphics.newFont(72),
	-- 			small = moon.graphics.newFont(22) }
	graphics = {logo = moon.scene_graph.new_scene_node("media/logo.png"),
				fmas = moon.scene_graph.new_scene_node("media/fmas.png"),
				set = moon.scene_graph.new_scene_node("media/set.png"),
				notset = moon.scene_graph.new_scene_node("media/notset.png") }
	-- music =	{	default = moon.audio.newSource("media/sawng.ogg") }
	-- sound =	{	click = moon.audio.newSource("media/click.ogg", "static"),
	-- 			shush = moon.audio.newSource("media/shh.ogg", "static"),
	-- 			pling = moon.audio.newSource("media/pling.ogg", "static") }
	
	-- Variables
	size = 6				-- size of the grid
	audio = true			-- whether audio should be on or off
	state = Menu.create()	-- current game state
	
	-- Setup
	moon.graphics.set_background_color(table.unpack(color["background"]))
	-- moon.audio.play(music["default"], 0)

end

function moon.draw()
	
	state:draw()
	
	moon.graphics.draw(graphics["fmas"], 700, 590, 0, 1, 1, 100, 10)
	
end

function moon.update(dt)

	state:update(dt)

end

function moon.mousepressed(x, y, button)

	state:mousepressed(x,y,button)

end

function moon.keypressed(key)
	
	if key == "f4" and (moon.keyboard.isDown("ralt") or moon.keyboard.isDown("lalt")) then
		moon.event.push("quit")
	end
	
	state:keypressed(key)

end