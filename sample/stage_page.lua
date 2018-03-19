local test

function dust.load()
	test = dust.scene_graph.new_scene_node("D:\\engine\\love\\demos\\lovalanche\\green_ball.png")
end

function dust.update()
end

function dust.draw()
	dust.graphics.circle('line', -150, 100, 10)
	dust.graphics.rectangle('line', 0, 0, 100, 200)

	dust.graphics.draw(test)
end