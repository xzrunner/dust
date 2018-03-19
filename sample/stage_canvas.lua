require 'dust'
local graphics = require 'dust.graphics'

function dust.load()
end

function dust.update()
end

function dust.draw()
	graphics.circle('line', -150, 100, 10)

	graphics.rectangle('line', 0, 0, 100, 200)	
end

