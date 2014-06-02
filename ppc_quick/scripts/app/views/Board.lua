
-- local Levels = import("..data.Levels")
local Block   = import("..views.Block")

local Board = class("Board", function()
    return display.newLayer()
end)

local NODE_PADDING   = 64
local NODE_ZORDER    = 0
local BLOCK_ZORDER    = 1000



function Board:ctor(levelData)
    cc.GameObject.extend(self):addComponent("components.behavior.EventProtocol"):exportMethods()

    self.rows = levelData.rows
    self.cols = levelData.cols
    self.grid = {}

    -- make all block in one batch
    self.batch = display.newBatchNode(GAME_TEXTURE_IMAGE_FILENAME)
    self.batch:setPosition(display.cx, display.cy)
    self:addChild(self.batch)

    -- init grid    
    for i = 1, self.rows do
        self.grid[#self.grid + 1] = Block.new()
        self.grid[i] = {}
        for j = 1, self.cols do
            -- echoInfo("(%d.%d)", self.rows, self.cols)
            -- print(self.grid[i], self.grid)
            self.grid[i][#self.grid[i] + 1] = Block.new()
        end
    end

    -- add blocks to board
    local offsetX = -math.floor(NODE_PADDING * self.cols / 2) - NODE_PADDING / 2
    local offsetY = -math.floor(NODE_PADDING * self.rows / 2) - NODE_PADDING / 2
    for i = 1, self.rows do
        local y = i * NODE_PADDING + offsetY
        for j = 1, self.cols do
            local x = j * NODE_PADDING + offsetX
            self.grid[i][j]:setPos(x, y)
            -- self.grid[i][j]:get(BLOCK_STATE_NOR):setPosition({x, y})
            self.batch:addChild(self.grid[i][j]:get(BLOCK_STATE_NOR), BLOCK_ZORDER)
            -- -- echoInfo("POS (%d,%d)", self.grid[i][j]:get(BLOCK_STATE_NOR):getPosition())

            -- self.grid[i][j]:get(BLOCK_STATE_RES):setPosition({x, y})
            self.batch:addChild(self.grid[i][j]:get(BLOCK_STATE_RES), BLOCK_ZORDER)

            -- self.grid[i][j]:get(BLOCK_STATE_DIS):setPosition({x, y})
            self.batch:addChild(self.grid[i][j]:get(BLOCK_STATE_DIS), BLOCK_ZORDER)
        end
    end


    self.isProcessing = 0



    -- local nodeSprite = display.newSprite("#icon_nor_3.png", x, y)
    -- self.batch:addChild(nodeSprite, NODE_ZORDER + 1)



    self:addTouchEventListener(handler(self, self.onTouch))
    self:setNodeEventEnabled(true)
end

function Board:checkLevelCompleted()
    local count = 0
    for _, coin in ipairs(self.coins) do
        if coin.isWhite then count = count + 1 end
    end
    if count == #self.coins then
        -- completed
        self:setTouchEnabled(false)
        self:dispatchEvent({name = "LEVEL_COMPLETED"})
    end
end

function Board:getBlock(row, col)
    if self.grid[row] then
        return self.grid[row][col]
    end
end

function Board:onMovedV()
    echoInfo("onMovedV")
    self.isProcessing = self.isProcessing - 1
    if 0 == self.isProcessing then
        if false == self:onAdjustH() then
            self:setTouchEnabled(true)
        end
    end
end

function Board:onMovedH()
    echoInfo("onMovedH")
    self.isProcessing = self.isProcessing - 1
    if 0 == self.isProcessing then
        self:setTouchEnabled(true)
    end
end

function Board:onAdjustV()
    local ret = false
    for row = 1, self.rows do
        for col = 1, self.cols do
            if self.grid[row][col]:isMarkRemove() == true then -- every row in checking with only removed ... 
                echoInfo("remove [%d.%d]", row, col)
                if not MY_DEBUG then
                    self.grid[row][col]:remove() -- Debug only
                end
                local i = row
                local hasOneOnTop = false
                while i + 1 <= self.rows do 
                    i = i + 1        
                    if self.grid[i][col]:isMarkRemove() == false then  
                        echoInfo("from [%d.%d]", i, col)
                        hasOneOnTop = true
                        break
                    end
                end
                if hasOneOnTop == true then
                    local x1, y1 = self.grid[i][col]:getPosition()
                    local x2, y2 = self.grid[row][col]:getPosition()

                    echoInfo("from [%d.%d](%d,%d) to [%d][%d](%d,%d)", i, col, x1, y1, row, col, x2, y2)

                    -- touch be disabled
                    self.isProcessing = self.isProcessing + 1
                    self:setTouchEnabled(false)
                    -- move it
                    -- self.grid[i][col]:moveEx(x2, y2, handler(self, self.onMovedV), BLOCK_STATE_NOR)
                    self.grid[i][col]:move(x2, y2, handler(self, self.onMovedV))
                    ret = true
                    -- re-refer the grid
                    -- self.grid[row][col]:moveEx(x1, y1, handler(self, self.onMovedV), BLOCK_STATE_DIS)
                    self.grid[row][col]:setPos(x1, y1)
                    local tmpBlock = self.grid[row][col]
                    self.grid[row][col] = self.grid[i][col]
                    self.grid[i][col] = tmpBlock
                    
                end

            end
        end
    end
    return ret
end

function Board:onAdjustH()
    local ret = false
    -- if ret == false then
    --     return ret
    -- end
    for col = 1, self.cols do
        for row = 1, self.rows do
            if self.grid[row][col]:isMarkRemove() == true then -- every row in checking with only removed ... 
                echoInfo("remove [%d.%d]", row, col)
                if not MY_DEBUG then
                    self.grid[row][col]:remove() -- Debug only
                end
                local j = col
                local hasOneOnTop = false
                while j + 1 <= self.cols do 
                    j = j + 1        
                    if self.grid[row][j]:isMarkRemove() == false then  
                        echoInfo("from [%d.%d]", row, j)
                        hasOneOnTop = true
                        break
                    end
                end
                if hasOneOnTop == true then
                    local x1, y1 = self.grid[row][j]:getPosition()
                    local x2, y2 = self.grid[row][col]:getPosition()

                    echoInfo("from [%d.%d](%d,%d) to [%d][%d](%d,%d)", row, j, x1, y1, row, col, x2, y2)

                    -- touch be disabled
                    self.isProcessing = self.isProcessing + 1
                    self:setTouchEnabled(false)
                    -- move it
                    self.grid[row][j]:move(x2, y2, handler(self, self.onMovedH))
                    ret = true

                    -- re-refer the grid
                    self.grid[row][col]:setPos(x1, y1)
                    -- self.grid[row][col]:moveEx(x1, y1, handler(self, self.onMovedH), BLOCK_STATE_DIS)

                    local tmpBlock = self.grid[row][col]
                    self.grid[row][col] = self.grid[row][j]
                    self.grid[row][j] = tmpBlock
                    
                end

            end
        end
    end
    return ret
end
function Board:onAdjust()
    echoInfo("onAdjust")

    if self:onAdjustV() == false then
        self:onAdjustH()
    end
end

function Board:onDropped()
    for row = 1, self.rows do
        for col = 1, self.cols do
            if self.grid[row][col]:getState() == BLOCK_STATE_DIS then
                
                -- self.batch:removeChild(self.grid[row][col])
                self.grid[row][col]:markRemove()

            end
        end
    end

    self:onAdjust()
    -- self:performWithDelay(function()
    --     self:onAdjust()
    -- end, 0.2)

end

function Board:resetState(state1, state2, stack) 
    if stack == 0 then
        for row = 1, self.rows do
            for col = 1, self.cols do
                if self.grid[row][col]:getState() == state1 and self.grid[row][col]:isMarkRemove() == false then
                    
                    -- self.batch:removeChild(self.grid[row][col])
                    self.grid[row][col]:setState(state2)

                end
            end
        end
    end
end

function Board:findFriend(block, i, j, stack)

    echoInfo("block id[%d] (%d, %d) stack[%d]", block.id, i, j, stack)

    if block:getState() == BLOCK_STATE_NOR then
        self:resetState(BLOCK_STATE_RES, BLOCK_STATE_NOR, stack)
        block:setState(BLOCK_STATE_RES)  
    elseif block:getState() == BLOCK_STATE_RES then 
        block:setState(BLOCK_STATE_DIS) 
    else
        return
    end

    stack = stack + 1
    if i + 1 <= self.rows and 
        self:getBlock(i + 1, j).id == block.id and  
        self:getBlock(i + 1, j):getState() ~= BLOCK_STATE_DIS and 
        self:getBlock(i + 1, j):getState() ~= block:getState() then
        self:findFriend(self.grid[i + 1][j], i + 1, j, stack)
    end
    if i - 1 >= 1 and 
        self:getBlock(i - 1, j).id == block.id and 
        self:getBlock(i - 1, j):getState() ~= BLOCK_STATE_DIS and 
        self:getBlock(i - 1, j):getState() ~= block:getState() then
        self:findFriend(self.grid[i - 1][j], i - 1, j, stack)
    end
    if j + 1 <= self.cols and 
        self:getBlock(i, j + 1).id == block.id and 
        self:getBlock(i, j + 1):getState() ~= BLOCK_STATE_DIS and 
        self:getBlock(i, j + 1):getState() ~= block:getState() then
        self:findFriend(self.grid[i][j + 1], i, j + 1, stack)
    end
    if j - 1 >= 1 and 
        self:getBlock(i, j - 1).id == block.id and 
        self:getBlock(i, j - 1):getState() ~= BLOCK_STATE_DIS and 
        self:getBlock(i, j - 1):getState() ~= block:getState() then
        self:findFriend(self.grid[i][j - 1], i, j - 1, stack)
    end
    stack = stack - 1

    echoInfo("DONE id[%d] (%d, %d) stack[%d]", block.id, i, j, stack)

    if stack == 0 and block:getState() == BLOCK_STATE_DIS then
        self:onDropped()
    end
end

function Board:isValid(block, i, j)
    if i + 1 <= self.rows and 
        self:getBlock(i + 1, j):getId() == block:getId() and 
        self:getBlock(i + 1, j):getState() == block:getState() then
        return true
    end
    if i - 1 >= 1 and 
        self:getBlock(i - 1, j):getId() == block:getId() and
        self:getBlock(i - 1, j):getState() == block:getState() then
        return true
    end
    if j + 1 <= self.cols and 
        self:getBlock(i, j + 1):getId() == block:getId() and
        self:getBlock(i, j + 1):getState() == block:getState() then
        return true
    end
    if j - 1 >= 1 and 
        self:getBlock(i, j - 1):getId() == block:getId() and
        self:getBlock(i, j - 1):getState() == block:getState() then
        return true
    end
    return false
end

function Board:pressedBlock(block, i, j)

    if self:isValid(block, i, j) == false then
        return
    end

    self:findFriend(block, i, j, 0)


end

function Board:onTouchBegan(x, y)
    if self.grid == nil then return end
    local padding = NODE_PADDING / 2
    -- for _, b in ipairs(self.grid) do
    for i = 1, self.rows do
        for j =  1, self.cols do
            local block = self.grid[i][j]
            local cx, cy = block:getPosition()
            cx = cx + display.cx
            cy = cy + display.cy
            if x >= cx - padding
                and x <= cx + padding
                and y >= cy - padding
                and y <= cy + padding then
                self:pressedBlock(block, i, j)
                break
            end
        end
    end
end

function Board:onTouch(event, x, y)
    if event == "began" then
        self:onTouchBegan(x, y)
        echoInfo("Board:onTouch 1 %s [%d, %d]", event, x, y)
        return true
    elseif event == "moved" then
        echoInfo("Board:onTouch 2 %s [%d, %d]", event, x, y)
    elseif event == "ended" then
        echoInfo("Board:onTouch 3 %s [%d, %d]", event, x, y)
        -- self:setTouchEnabled(false)
        -- self:dispatchEvent({name = "LEVEL_COMPLETED"})
        
    end
end

function Board:onEnter()
    self:setTouchEnabled(true)
end

function Board:onExit()
    self:removeAllEventListeners()
end

return Board
