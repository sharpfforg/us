
-- local AdBar = import("..views.AdBar")
local LevelsList = import("..views.LevelsList")

local ChooseLevelScene = class("ChooseLevelScene", function()
    return display.newScene("ChooseLevelScene")
end)

function ChooseLevelScene:ctor()
    local bg = display.newSprite("#bg_mainmenu.png")
    -- make background sprite always align top
    bg:setPosition(display.cx, display.top - bg:getContentSize().height / 2)
    self:addChild(bg)

    -- local title = display.newSprite("#Title.png", display.cx, display.top - 100)
    -- self:addChild(title)

    -- local adBar = AdBar.new()
    -- self:addChild(adBar)

    -- create levels list
    local rect = CCRect(display.left, display.bottom + 180, display.width, display.height - 280)
    self.levelsList = LevelsList.new(rect)
    echoInfo("levelsList:addEventListener START")
    self.levelsList:addEventListener("onTapLevelIcon2", handler(self, self.onTapLevelIcon2))
    echoInfo("levelsList:addEventListener END")
    self:addChild(self.levelsList)

    -- create menu
    local backButton = ui.newImageMenuItem({
        image = "#icon_nor_2.png",
        imageSelected = "#icon_res_2.png",
        x = display.right - 100,
        y = display.bottom + 120,
        -- sound = GAME_SFX.backButton,
        listener = function()
            app:enterMenuScene()
        end,
    })

    local menu = ui.newMenu({backButton})
    self:addChild(menu)
end

function ChooseLevelScene:onTapLevelIcon2(event)
    -- audio.playSound(GAME_SFX.tapButton)
    app:playLevel(event.levelIndex)
end

function ChooseLevelScene:onEnter()
    self.levelsList:setTouchEnabled(true)
end

return ChooseLevelScene
