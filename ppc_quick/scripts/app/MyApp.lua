
require("config")
require("framework.init")
require("framework.shortcodes")
require("framework.cc.init")

local MyApp = class("MyApp", cc.mvc.AppBase)

function MyApp:ctor()
    MyApp.super.ctor(self)
end

function MyApp:run()
	echoInfo("MyApp:running")

    -- CCUserDefault:sharedUserDefault():setStringForKey("string", "value1")
    -- CCUserDefault:sharedUserDefault():setIntegerForKey("integer", 10)
    -- CCUserDefault:sharedUserDefault():setFloatForKey("float", 2.3)
    -- CCUserDefault:sharedUserDefault():setDoubleForKey("double", 2.4)
    -- CCUserDefault:sharedUserDefault():setBoolForKey("bool", false)

    CCFileUtils:sharedFileUtils():addSearchPath("res/")
    -- display.addSpriteFramesWithFile(GAME_TEXTURE_DATA_FILENAME1, GAME_TEXTURE_IMAGE_FILENAME1)
    display.addSpriteFramesWithFile(GAME_TEXTURE_DATA_FILENAME, GAME_TEXTURE_IMAGE_FILENAME)
    -- echoInfo("self:playLevel -s")
    -- self:playLevel(2)
    -- echoInfo("self:playLevel -e")
    self:enterMenuScene()
end

function MyApp:enterMenuScene()
    self:enterScene("MenuScene", nil, "fade", 0.6, display.COLOR_WHITE)
end

function MyApp:enterMoreGamesScene()
    self:enterScene("MoreGamesScene", nil, "fade", 0.6, display.COLOR_WHITE)
end

function MyApp:enterChooseLevelScene()
    self:enterScene("ChooseLevelScene", nil, "fade", 0.6, display.COLOR_WHITE)
end

function MyApp:playLevel(levelIndex)
    echoInfo("MyApp:playLevel -s")
    self:enterScene("PlayLevelScene", {levelIndex}, "fade", 0.6, display.COLOR_WHITE)
    echoInfo("MyApp:playLevel -e")
end

return MyApp
