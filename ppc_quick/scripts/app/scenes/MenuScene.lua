
-- local AdBar = import("..views.AdBar")
local BubbleButton = import("..views.BubbleButton")

local MenuScene = class("MenuScene", function()
    return display.newScene("MenuScene")
end)

function MenuScene:ctor()
    self.bg = display.newSprite("#bg_mainmenu.png", display.cx, display.cy)
    self:addChild(self.bg)

    -- self.adBar = AdBar.new()
    -- self:addChild(self.adBar)

    -- create menu
    self.moreGamesButton = BubbleButton.new({
        image = "#mainmenu_more.png",
        x = display.left + 150,
        y = display.bottom + 300,
        -- sound = GAME_SFX.tapButton,
        prepare = function()
            self.menu:setEnabled(false)
            self.layer:setKeypadEnabled(false)
        end,
        listener = function()
            app:enterMoreGamesScene()
        end,
    })

    self.startButton = BubbleButton.new({
        image = "#mainmenu_start.png",
        x = display.right - 150,
        y = display.bottom + 300,
        -- sound = GAME_SFX.tapButton,
        prepare = function()
            self.menu:setEnabled(false)
            self.layer:setKeypadEnabled(false)
        end,
        listener = function()
            app:enterChooseLevelScene()
        end,
    })

    self.menu = ui.newMenu({self.moreGamesButton, self.startButton})
    self:addChild(self.menu)

    -- keypad layer, for android
    self.layer = display.newLayer()
    self.layer:addKeypadEventListener(function(event)
        if event == "back" then
            audio.playSound(GAME_SFX.backButton)
            app:exit()
        end
    end)
    self:addChild(self.layer)
end

function MenuScene:onEnter()
    -- avoid unmeant back
    if device.platform == "android" then
        -- avoid unmeant back
        self:performWithDelay(function()
            -- keypad layer, for android
            local layer = display.newLayer()
            layer:addKeypadEventListener(function(event)
                if event == "back" then app.exit() end
            end)
            self:addChild(layer)

            layer:setKeypadEnabled(true)
        end, 0.5)
    end
end

return MenuScene
