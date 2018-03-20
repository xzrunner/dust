local test

function moon.load()
	test = moon.scene_graph.new_scene_node("D:\\engine\\love\\demos\\lovalanche\\green_ball.png")
end

function moon.update()
end

function moon.draw()
	moon.graphics.circle('line', -150, 100, 10)
	moon.graphics.rectangle('line', 0, 0, 100, 200)

	moon.graphics.draw(test)
end