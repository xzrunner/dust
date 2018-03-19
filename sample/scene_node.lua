function load(self)
end

function update(self)
end

function draw(self)
	local x, y = self.v_node:get_pos()
	self.v_node:set_pos(x + 10, y)
end
