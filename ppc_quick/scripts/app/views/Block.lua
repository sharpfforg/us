-- local Levels = import("..data.Levels")
BLOCK_STATE_NOR = "nor"
BLOCK_STATE_RES = "res"
BLOCK_STATE_DIS = "dis"


local Block = class("Block", function()
    local index = 1
    -- if nodeType == Levels.NODE_IS_BLACK then
    --     index = 8
    -- end

    local id = math.random(0, GAME_BLOCK_TYPE_NUMS - 1)
    local sprite = display.newSprite(string.format("#icon_%s_%d.png", BLOCK_STATE_NOR, id))
    sprite.id = id
    sprite.state = BLOCK_STATE_NOR
    sprite.nor = sprite
    -- sprite.isWhite = index == 1
    return sprite
end)

function Block:ctor()
	self.frames = {}
    self.frames[BLOCK_STATE_NOR] = self.nor
    self.frames[BLOCK_STATE_RES] = display.newSprite(string.format("#icon_%s_%d.png", BLOCK_STATE_RES, self.id))
    self.frames[BLOCK_STATE_RES]:setVisible(false)
    self.frames[BLOCK_STATE_DIS] = display.newSprite(string.format("#icon_%s_%d.png", BLOCK_STATE_DIS, self.id))
    self.frames[BLOCK_STATE_DIS]:setVisible(false)

    self.removeFlag = false
end

function Block:get(state)
	return self.frames[state]
end

function Block:set(args)
	self.frames = clone(args.frames)
	self.id = args.id
	self.state = args.state
	self.nor = args.nor
	self.removeFlag = args.removeFlag
end

function Block:getId()
	return self.id
end

function Block:getCurerntOne()
	return self.frames[self.state]
end


function Block:getState()
	return self.state
end

function Block:setState(state)
	self.frames[self.state]:setVisible(false)
	self.state = state
	self.frames[state]:setVisible(true)
end


function Block:markRemove()
	-- self.frames[BLOCK_STATE_NOR]:setVisible(false)
	-- self.frames[BLOCK_STATE_RES]:setVisible(false)
	-- self.frames[BLOCK_STATE_DIS]:setVisible(false)
	self.removeFlag = true
end

function Block:isMarkRemove()
	return self.removeFlag
end

function Block:remove()
	self.frames[BLOCK_STATE_NOR]:setVisible(false)
	self.frames[BLOCK_STATE_RES]:setVisible(false)
	self.frames[BLOCK_STATE_DIS]:setVisible(false)

end

function Block:setPos(x, y)
	self.frames[BLOCK_STATE_NOR]:setPosition(x, y)
	self.frames[BLOCK_STATE_RES]:setPosition(x, y)
	self.frames[BLOCK_STATE_DIS]:setPosition(x, y)
end

function Block:move(x, y, cbMoved)
	transition.moveTo(self.frames[BLOCK_STATE_NOR], {x = x, y = y, time = 0.3, onComplete = cbMoved, easing = "BOUNCEOUT"})
	-- self.frames[BLOCK_STATE_NOR]:setPosition(x, y) cbMoved()
	self.frames[BLOCK_STATE_RES]:setPosition(x, y)
	self.frames[BLOCK_STATE_DIS]:setPosition(x, y)

end

-- function Block:moveEx(x, y, cbMoved, state)
-- 	if state ~= nil then
-- 		transition.moveTo(self.frames[state], {x = x, y = y, time = 0.3, onComplete = cbMoved, easing = "BOUNCEOUT"})
-- 	else
-- 		return
-- 	end

-- 	if state == BLOCK_STATE_NOR then
-- 		self.frames[BLOCK_STATE_RES]:setPosition(x, y)
-- 		self.frames[BLOCK_STATE_DIS]:setPosition(x, y)
-- 	elseif state == BLOCK_STATE_RES then
-- 		self.frames[BLOCK_STATE_NOR]:setPosition(x, y)
-- 		self.frames[BLOCK_STATE_DIS]:setPosition(x, y)
-- 	elseif state == BLOCK_STATE_DIS then
-- 		self.frames[BLOCK_STATE_NOR]:setPosition(x, y)
-- 		self.frames[BLOCK_STATE_RES]:setPosition(x, y)
-- 	end

-- end

function Block:outputInfo()
	print(self.frames[BLOCK_STATE_NOR]:isVisible(), 
		self.frames[BLOCK_STATE_RES]:isVisible(),
		self.frames[BLOCK_STATE_DIS]:isVisible(),
		self.removeFlag)
	-- echoInfo("nor v[%d], res v[%d], dis v[%d], removeFlag[%d]", 
	-- 	function() 
	-- 		if self.frames[BLOCK_STATE_NOR]:isVisible() == true then
	-- 			return 1
	-- 		else
	-- 			return 0
	-- 		end
	-- 	end, 
	-- 	function() 
	-- 		if self.frames[BLOCK_STATE_RES]:isVisible() == true then
	-- 			return 1
	-- 		else
	-- 			return 0
	-- 		end
	-- 	end, 
	-- 	function() 
	-- 		if self.frames[BLOCK_STATE_DIS]:isVisible() == true then
	-- 			return 1
	-- 		else
	-- 			return 0
	-- 		end
	-- 	end, 
	-- 	function() 
	-- 		if self.removeFlag == true then
	-- 			return 1
	-- 		else
	-- 			return 0
	-- 		end
	-- 	end)
end


-- function Coin:flip(onComplete)
--     local frames = display.newFrames("Coin%04d.png", 1, 8, not self.isWhite)
--     local animation = display.newAnimation(frames, 0.3 / 8)
--     self:playAnimationOnce(animation, false, onComplete)

--     self:runAction(transition.sequence({
--         CCScaleTo:create(0.15, 1.5),
--         CCScaleTo:create(0.1, 1.0),
--         CCCallFunc:create(function()
--             local actions = {}
--             local scale = 1.1
--             local time = 0.04
--             for i = 1, 5 do
--                 actions[#actions + 1] = CCScaleTo:create(time, scale, 1.0)
--                 actions[#actions + 1] = CCScaleTo:create(time, 1.0, scale)
--                 scale = scale * 0.95
--                 time = time * 0.8
--             end
--             actions[#actions + 1] = CCScaleTo:create(0, 1.0, 1.0)
--             self:runAction(transition.sequence(actions))
--         end)
--     }))

--     self.isWhite = not self.isWhite
-- end

return Block
