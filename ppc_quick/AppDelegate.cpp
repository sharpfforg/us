
#include "cocos2d.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "support/CCNotificationCenter.h"
#include "CCLuaEngine.h"
#include <string>

#include "AssetsUpdateLayer.h"

using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{
    // fixed me
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
}

AppDelegate::~AppDelegate()
{
    // end simple audio engine here, or it may crashed on win32
    SimpleAudioEngine::sharedEngine()->end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    pDirector->setProjection(kCCDirectorProjection2D);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    launch();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    CCDirector::sharedDirector()->pause();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("APP_ENTER_BACKGROUND");
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    CCDirector::sharedDirector()->resume();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("APP_ENTER_FOREGROUND");
}

void AppDelegate::setProjectConfig(const ProjectConfig& config)
{
    m_projectConfig = config;
}

void AppDelegate::launch()
{
    // register lua engine
    CCLuaEngine *pEngine = CCLuaEngine::defaultEngine();
    CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);

    CCLuaStack *pStack = pEngine->getLuaStack();

    AssetsUpdateLayer *assets = AssetsUpdateLayer::create();
    CCScene* scene = CCScene::create();
    scene->addChild(assets);
    CCDirector::sharedDirector()->runWithScene(scene);

    bool flag = false;
    std::string pathToSave = CCFileUtils::sharedFileUtils()->getWritablePath();
    pathToSave += "loaddir/";
    std::string tmp = pathToSave + "scripts/main.lua";
    FILE *fp = fopen(tmp.c_str(), "r");

    if (fp)
    {
        flag = true;
        fclose(fp);
    }
    int ret1, ret2;
    string path;
    if (flag)
    {
        
        vector<string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();
        for (vector<string>::iterator iter = searchPaths.begin(); iter != searchPaths.end(); iter++)
        {
            CCLog("start curr search path [%s]", iter->c_str());
        }
        /*
        searchPaths.insert(searchPaths.begin(), pathToSave + "scripts/");
        CCLog("added [%s] ", (pathToSave + "scripts").c_str());
        searchPaths.insert(searchPaths.begin(), pathToSave);
        CCLog("added [%s] ", pathToSave.c_str());
        path = pathToSave + "scripts/main.lua";
        CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
        */

        //string res = pathToSave + "res/";
        string scripts = pathToSave + "scripts/";
        //pStack->addSearchPath(pathToSave.substr(0, pathToSave.find_last_of("/")).c_str()); // download path "loaddir"
        //pStack->addSearchPath(res.substr(0, res.find_last_of("/")).c_str()); // download path "loaddir"
        //pStack->addSearchPath(scripts.substr(0, scripts.find_last_of("/")).c_str()); // download path "loaddir"
        
        //vector<string> searchPaths;
        searchPaths.clear();
        //searchPaths.push_back(res);
        searchPaths.push_back(scripts);
        searchPaths.push_back(pathToSave);
        CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
        //path = CCFileUtils::sharedFileUtils()->fullPathForFilename("scripts/main.lua");
        //CCLog("pathToSave[%s] path[%s]", pathToSave.c_str(), path.c_str());
        //pStack->addSearchPath(path.substr(0, path.find_last_of("/")).c_str());
        //CCLog("executeScriptFile path[%s]", path.c_str());
        //int ret = pEngine->executeScriptFile("main.lua");
        //CCLog("ret [%d]", ret);
        //CCLog("res path[%s]", res.c_str());
        //CCLog("scripts path[%s]", scripts.c_str());
        //CCLog("pathToSave path[%s]", pathToSave.c_str());
        for (vector<string>::iterator iter = searchPaths.begin(); iter != searchPaths.end(); iter++)
        {
            CCLog("end curr search path [%s]", iter->c_str());
        }


/*

        string env = "__LUA_STARTUP_FILE__=\"";
        env.append(path);
        env.append("\"");
        pEngine->executeString(env.c_str());
*/        
        // load framework
        std::string zip_path = pathToSave + "res/framework_precompiled.zip";
        ret1 = pStack->loadChunksFromZip(zip_path.c_str());
        //path = CCFileUtils::sharedFileUtils()->fullPathForFilename("scripts/main.lua");
        ret2 = pEngine->executeScriptFile("main.lua");
        CCLog("exec ret1[%d], ret2[%d]", ret1, ret2);
        //CCLog("------------------------------------------------");
        //CCLog("LOAD LUA FILE: exec %s, env %s", path.c_str(), env.c_str()*/);
        //CCLog("------------------------------------------------");
        return;
    }


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // set script path
    path = CCFileUtils::sharedFileUtils()->fullPathForFilename("scripts/main.lua");
    CCLog("XXX :) fullPathForFilename %s", path.c_str());
    
#else /* WIN32 */
    // load framework
    if (m_projectConfig.isLoadPrecompiledFramework())
    {
        const string precompiledFrameworkPath = SimulatorConfig::sharedDefaults()->getPrecompiledFrameworkPath();
        pStack->loadChunksFromZip(precompiledFrameworkPath.c_str());
    }

    // set script path
    path = CCFileUtils::sharedFileUtils()->fullPathForFilename(m_projectConfig.getScriptFileRealPath().c_str());
#endif

    size_t pos;
    while ((pos = path.find_first_of("\\")) != std::string::npos)
    {
        path.replace(pos, 1, "/");
    }
    size_t p = path.find_last_of("/\\");
    if (p != path.npos)
    {
        const string dir = path.substr(0, p);
        pStack->addSearchPath(dir.c_str());
        CCLog("added 1 %s", dir.c_str());

        p = dir.find_last_of("/\\");
        if (p != dir.npos)
        {
            pStack->addSearchPath(dir.substr(0, p).c_str());
            CCLog("added 2 %s", dir.substr(0, p).c_str());
        }
    }
    // load framework
    ret1 = pStack->loadChunksFromZip("res/framework_precompiled.zip");

    string env = "__LUA_STARTUP_FILE__=\"";
    env.append(path);
    env.append("\"");
    pEngine->executeString(env.c_str());
    ret2 = pEngine->executeScriptFile(path.c_str());

    CCLog("------------------------------------------------");
    CCLog("LOAD LUA FILE: exec %s, env %s, ret1 [%d], ret2[%d]", path.c_str(), env.c_str(), ret1, ret2);
    CCLog("------------------------------------------------");
     
}
