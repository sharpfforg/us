#include "AssetsUpdateLayer.h"
//#include "GameScene.h"
//#include "Battle/BattleScene.h"
#include "CCLuaEngine.h"
//#include "Utils/Localization.h"
//#include "Battle/BattleInitData.h"
//#include "Data/DataManager.h"
#include "AssetsUpdateLayer.h"
//#include "Utils/Localization.h"
//#include "Utils/MD5.h"
//#include "platform/DevicePlatform.h"


#if defined(COCOS2D_DEBUG) && COCOS2D_DEBUG == 1
#warning "COCOS2D_DEBUG == 1"
#else
#warning "COCOS2D_DEBUG == NONE"
#endif

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

USING_NS_CC;
USING_NS_CC_EXTRA;
USING_NS_CC_EXT;

#define ASSETS_SERVER_PATH "static/"
#define ASSETS_VERSION_FILE "game/version"

#define KEY_OF_VERSION   "current-version-code"

AssetsUpdateLayer::AssetsUpdateLayer()
: /*m_percentLabel(NULL)
  , m_packegNumberLabel(NULL)
  , */m_backgroundSpriteCreated(false)
  , /*m_progressBar(NULL)
    , */m_curBigVersion(0)
    , m_curMidVersion(0)
    , m_curSmallVersion(0)
    , m_endBigVersion(0)
    , m_endMidVersion(0)
    , m_endSmallVersion(0)
    , m_pathToSave("")
    , m_defaultPath("")
    , m_isDownloading(false)
    , m_oneSecondPackageLength(0)
    , m_downloadSpeed(0.0f)
    , m_packageLength(0)
    , m_curPackageLength(0)
{
    m_lastTime.tv_sec = 0;
    m_lastTime.tv_usec = 0;
}

AssetsUpdateLayer::~AssetsUpdateLayer()
{

}

bool AssetsUpdateLayer::init()
{
    CCLayer::init();
    CCLOG("AssetsUpdateLayer::init");

    createDownloadedDir();
    //m_defaultPath = CCFileUtils::sharedFileUtils()->getSearchPaths().at(0);

    // localization init
    //Localization::getInstance()->init();
    download();    
    return true;
}


void AssetsUpdateLayer::onEnter()
{
    CCLayer::onEnter();
    CCLOG("AssetsUpdateLayer::onEnter");

    //CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(AssetsUpdateLayer::speedUpdate), "NotifycationDownloadSpeed", NULL);

    //requestLastestVersion();
}

void AssetsUpdateLayer::onExit()
{
    //CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "NotifycationDownloadSpeed");

    CCLayer::onExit();
}

void AssetsUpdateLayer::update(float delta)
{
    if (1 || m_curBigVersion >= m_endBigVersion && m_curSmallVersion >= m_endSmallVersion)
    {
        CCLOG("Version check Ok");
        unscheduleUpdate();
        //MD5Check();
        gameEnter();
        return;
    }
    if (!m_isDownloading && checkUpdate())
    {
        createLayerItem();

        char str[128];
        sprintf(str, "%s%s%d.%d.%d.zip", SERVER_ADDRESS, ASSETS_SERVER_PATH, m_curBigVersion, m_curMidVersion, m_curSmallVersion + 1);
        getAssetsManager()->setPackageUrl(str);
        sprintf(str, "%d.%d.%d", m_curBigVersion, m_curMidVersion, m_curSmallVersion + 1);
        getAssetsManager()->setVersionFileUrl(str);
        getAssetsManager()->setStoragePath(m_pathToSave.c_str());

        //sprintf(str, Localization::getInstance()->getValueByKey("Loading_download_restBag_num"), m_endSmallVersion - m_curSmallVersion);
        //m_packegNumberLabel->setString(str);

        m_curPackageLength = 0;
        //getAssetsManager()->getPackageLength((long&)m_packageLength);

        m_isDownloading = true;
        download();
    }
}

void AssetsUpdateLayer::createLayerItem()
{
    if (!m_backgroundSpriteCreated)
    {
        /*		m_backgroundSpriteCreated = true;
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ccb/ccbResources/LoginLayer.plist");
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ccb/ccbResources/login_background.plist");
        Size winSize = Director::getInstance()->getWinSize();

        Sprite* bgSprite = Sprite::createWithSpriteFrameName("login_background.png");
        bgSprite->setPosition(Point(winSize.width * 0.5, winSize.height * 0.5));
        bgSprite->setScale(winSize.height / bgSprite->getContentSize().height);
        addChild(bgSprite);

        Sprite* progressBarBg = Sprite::createWithSpriteFrameName("loadingProcess_bg.png");
        progressBarBg->setPosition(Point(winSize.width * 0.5, winSize.height * 0.1));
        progressBarBg->setScale(winSize.width / 1800);
        addChild(progressBarBg);

        m_progressBar = ProgressTimer::create(Sprite::createWithSpriteFrameName("loadingProcess_fg.png"));
        m_progressBar->setType(ProgressTimer::Type::BAR);
        m_progressBar->setBarChangeRate(Point(1, 0));
        m_progressBar->setPercentage(0);
        m_progressBar->setMidpoint(Point(0, 0));
        progressBarBg->addChild(m_progressBar);
        m_progressBar->setPosition(Point(progressBarBg->getContentSize().width * 0.5, progressBarBg->getContentSize().height * 0.5));

        CCLayer* layer = CCLayer::create();
        layer->ignoreAnchorPointForPosition(false);
        layer->setContentSize(Size(960, 640));
        layer->setPosition(Point(winSize.width * 0.5, winSize.height * 0.2));
        layer->setScale(winSize.width / 960);
        addChild(layer);
        */
        //m_percentLabel = LabelTTF::create("", "Arial", 20);
        //m_percentLabel->setPosition(Point(layer->getContentSize().width * 0.5, layer->getContentSize().height * 0.5 - 15));
        //layer->addChild(m_percentLabel);

        //m_packegNumberLabel = LabelTTF::create("", "Arial", 20);
        //m_packegNumberLabel->setPosition(Point(layer->getContentSize().width * 0.5, layer->getContentSize().height * 0.5 + 15));
        //layer->addChild(m_packegNumberLabel);
    }
}

const int RESOURCE_CHECK_NUMBER = 4;
const char RESOURCE_CHECK_LIST[RESOURCE_CHECK_NUMBER][32] = {"data/jsonconf.json", "lua/BattleLayer.lua", "lua/ChooseToolLayer.lua", "lua/MainLayer.lua"};

void AssetsUpdateLayer::MD5Check()
{
#if 0
    string md5Final;
    string md5;
    unsigned long size = 0;

    for (int i = 0; i < RESOURCE_CHECK_NUMBER; i++)
    {
        char* pBuffer = (char*)CCFileUtils::sharedFileUtils()->getFileData(RESOURCE_CHECK_LIST[i], "rt", &size);
        md5 = MD5(pBuffer, size).toString();
        md5Final += md5;
        //CCLOG("Crypt MD5 is %s", md5Final.c_str());
        CC_SAFE_DELETE_ARRAY(pBuffer);
    }

    md5Final = MD5((char*)md5Final.c_str(), md5Final.length()).toString();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    CCLOG("Crypt MD5 is %s", md5Final.c_str());
#endif
#endif
}

void AssetsUpdateLayer::gameEnter()
{
    //CCFileUtils::getInstance()->addSearchPath("ccb/");

    // localization init
    //Localization::getInstance()->init();
    // CCLayer init
    //GameScene::getInstance();
    //Data init
    //DataManager::getInstance()->init();
    // Battle init 
    //BattleInitData::getInstance();

    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("scripts/main.lua");
    CCLOG("* path [%s] *", path.c_str());
    //pEngine->executeScriptFile(path.c_str());

    CCLuaStack *pStack = pEngine->getLuaStack();

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

        p = dir.find_last_of("/\\");
        if (p != dir.npos)
        {
            pStack->addSearchPath(dir.substr(0, p).c_str());
            CCLog("added %s", dir.substr(0, p).c_str());
        }
    }

    string env = "__LUA_STARTUP_FILE__=\"";
    env.append(path);
    env.append("\"");
    pEngine->executeString(env.c_str());

    CCLog("------------------------------------------------");
    CCLog("LOAD LUA FILE: exec %s, env %s", path.c_str(), env.c_str());
    CCLog("------------------------------------------------");
    pEngine->executeScriptFile(path.c_str());
    
}

void AssetsUpdateLayer::requestLastestVersion()
{
    char reqStr[128];
    sprintf(reqStr, "%s%s", SERVER_ADDRESS, ASSETS_VERSION_FILE);
    CCHTTPRequest* req = CCHTTPRequest::createWithUrl(this, /*HTTPRequest_selector(AssetsUpdateLayer::requestLastestVersionCallback), */
        reqStr);
    req->start();
}

void AssetsUpdateLayer::requestLastestVersionCallback(CCHTTPRequest* req)
{
    //得到服务器最新版本
    string endVersion = req->getResponseString();
    getVerNumber(m_endBigVersion, m_endMidVersion, m_endSmallVersion, endVersion);
    CCLOG("server version %d  %d  %d", m_endBigVersion, m_endMidVersion, m_endSmallVersion);

    scheduleUpdate();
}

void AssetsUpdateLayer::speedUpdate(CCObject* object)
{
    /*
    struct timeval now;
    if (gettimeofday(&now, NULL) != 0)
    {
    CCLOG("error in gettimeofday");
    return;
    }
    if (m_lastTime.tv_sec == 0 && m_lastTime.tv_usec == 0)
    {
    m_lastTime.tv_sec = now.tv_sec;
    m_lastTime.tv_usec = now.tv_usec;
    return;
    }
    double deltaTime = (now.tv_sec - m_lastTime.tv_sec) * 1000 + (now.tv_usec - m_lastTime.tv_usec) / 1000.0f;
    m_oneSecondPackageLength += ((Integer*)object)->getValue();
    m_curPackageLength += ((Integer*)object)->getValue();
    if (deltaTime > 1000)
    {
    m_downloadSpeed = (m_oneSecondPackageLength / 1024) / (deltaTime / 1000);
    CCLOG("Download speed is %.2f KB/s", m_downloadSpeed);
    m_lastTime.tv_sec = now.tv_sec;
    m_lastTime.tv_usec = now.tv_usec;
    m_oneSecondPackageLength = 0;
    }
    */
}

bool AssetsUpdateLayer::checkUpdate()
{	
    calcLocalLatestVersion();
    if (m_endBigVersion > m_curBigVersion)
    {
        CCLOG("Need to update app!");
        unscheduleUpdate();
        removeDownloadedDir();
        //updateLayerMessageBox();
        //MessageBox(Localization::getInstance()->getValueByKey("Loading_warning_need_update"), Localization::getInstance()->getValueByKey("Loading_warning_version_old"));
        return true;
    }
    if (m_endSmallVersion > m_curSmallVersion)
    {
        return true;
    }
    return false;
}

void AssetsUpdateLayer::calcLocalLatestVersion()
{
    //得到缓存的版本号
    string recordedVersion = CCUserDefault::sharedUserDefault()->getStringForKey(KEY_OF_VERSION);
    //得到本地app中的版本号
    unsigned long nSize = 0;
    unsigned char appVersion[32];
    unsigned char* tmpAppVersion = CCFileUtils::sharedFileUtils()->getFileData("version", "rb", &nSize);
    int recVersion1 = 0, recVersion2 = 0, recVersion3 = 0;
    getVerNumber(recVersion1, recVersion2, recVersion3, recordedVersion);
    int appVersion1 = 0, appVersion2 = 0, appVersion3 = 0;
    for (int i = 0; i < nSize; i++)
        appVersion[i] = tmpAppVersion[i];
    appVersion[nSize] = '\0';
    CC_SAFE_DELETE_ARRAY(tmpAppVersion);
    getVerNumber(appVersion1, appVersion2, appVersion3, string((char*)appVersion));

    CCLOG("version number %s %d  %d  %d, %s %d  %d  %d", recordedVersion.c_str(), recVersion1, recVersion2, recVersion3, appVersion, appVersion1, appVersion2, appVersion3);

    vector<string> searchPath;
    string recordPath = m_pathToSave;
    string recordPathCCB = m_pathToSave + "ccb";
    string appPath = m_defaultPath;
    string appPathCCB = m_defaultPath + "ccb";
    if (recVersion1 > appVersion1)
    {
        m_curBigVersion = recVersion1;
        m_curMidVersion = recVersion2;
        m_curSmallVersion = recVersion3;
        searchPath.push_back(recordPath);
        searchPath.push_back(recordPathCCB);
        searchPath.push_back(appPath);
        searchPath.push_back(appPathCCB);
    }
    else if (recVersion1 < appVersion1)
    {
        m_curBigVersion = appVersion1;
        m_curMidVersion = appVersion2;
        m_curSmallVersion = appVersion3;
        searchPath.push_back(appPath);
        searchPath.push_back(appPathCCB);
        searchPath.push_back(recordPath);
        searchPath.push_back(recordPathCCB);
        removeDownloadedDir();
    }
    else
    {
        m_curBigVersion = appVersion1;
        m_curMidVersion = appVersion2;

        if (recVersion3 > appVersion3)
        {
            m_curSmallVersion = recVersion3;
            searchPath.push_back(recordPath);
            searchPath.push_back(recordPathCCB);
            searchPath.push_back(appPath);
            searchPath.push_back(appPathCCB);
        }
        else
        {
            m_curSmallVersion = appVersion3;
            searchPath.push_back(appPath);
            searchPath.push_back(appPathCCB);
            searchPath.push_back(recordPath);
            searchPath.push_back(recordPathCCB);
            removeDownloadedDir();
        }
    }
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
}

void AssetsUpdateLayer::download()
{
    getAssetsManager()->update();
}

void AssetsUpdateLayer::getVerNumber(int& num1, int& num2, int& num3, string versionStr)
{
    if (versionStr == "")
        return;
    num1 = atoi(versionStr.substr(0, versionStr.find('.')).c_str());
    num2 = atoi(versionStr.substr(versionStr.find('.') + 1, versionStr.rfind('.')).c_str());
    num3 = atoi(versionStr.substr(versionStr.rfind('.') + 1, versionStr.length()).c_str());
}

void AssetsUpdateLayer::createDownloadedDir()
{
    m_pathToSave = CCFileUtils::sharedFileUtils()->getWritablePath();
    m_pathToSave += "loaddir/";
    CCLOG("writable path[%s]", m_pathToSave.c_str());
    getAssetsManager()->setStoragePath(m_pathToSave.c_str());

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    DIR *pDir = NULL;

    pDir = opendir (m_pathToSave.c_str());
    if (! pDir)
    {
        mkdir(m_pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    }
#else
    if ((GetFileAttributesA(m_pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES)
    {
        CreateDirectoryA(m_pathToSave.c_str(), 0);
    }
#endif
}

void delete_folder_tree(const char *dirname)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];

    if (path == NULL) {
        CCLOG("Out of memory error\n");
        return;
    }
    dir = opendir(dirname);
    if (dir == NULL) {
        CCLOG("Error opendir()");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            snprintf(path, (size_t) PATH_MAX, "%s/%s", dirname, entry->d_name);
            if (entry->d_type == DT_DIR) {
                delete_folder_tree(path);
            } else {
                unlink(path);
            }
        }

    }
    closedir(dir);

    CCLOG("(not really) Deleting: %s\n", dirname);

    return;
#endif
}

void AssetsUpdateLayer::removeDownloadedDir()
{
    CCLOG("version remove download dir");
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    DIR *pDir = NULL;
    pDir = opendir (m_pathToSave.c_str());
    if (pDir)
    {
        //char command[128];
        //sprintf(command, "rm -rf %s", m_pathToSave.c_str());
        //system(command);
        delete_folder_tree(m_pathToSave.c_str());
    }
#else
#endif
}

AssetsManager* AssetsUpdateLayer::getAssetsManager()
{
    static AssetsManager *pAssetsManager = NULL;

    if (!pAssetsManager)
    {

        pAssetsManager = new AssetsManager("https://raw.githubusercontent.com/sharpfforg/us/master/ppc_quick/ppc_quick.zip",
            "https://raw.githubusercontent.com/sharpfforg/us/master/ppc_quick/version",
            m_pathToSave.c_str());
        /*
        pAssetsManager = new AssetsManager("https://raw.githubusercontent.com/sharpfforg/ppc_quick/master/ppc_quick.zip",
            "https://raw.githubusercontent.com/sharpfforg/ppc_quick/master/version",
            m_pathToSave.c_str());            //site https://github.com/sharpfforg/ppc_quick/
        */
            //https://raw.github.com/minggo/AssetsManagerTest/master/version
        /*	
        pAssetsManager = AssetsManager::getInstance()*/;
        pAssetsManager->setDelegate(this);
        pAssetsManager->setConnectionTimeout(3);
    }

    return pAssetsManager;
}

void AssetsUpdateLayer::onError(AssetsManager::ErrorCode errorCode)
{
    CCLOG("AssetsUpdateLayer::onError [%d]", errorCode);

    if (errorCode == AssetsManager::kNoNewVersion)
    {
        //m_percentLabel->setString((char*)Localization::getInstance()->getValueByKey("Loading_version_is_new"));
    }

    if (errorCode == AssetsManager::kNetwork)
    {
        //m_percentLabel->setString((char*)Localization::getInstance()->getValueByKey("Public_netWork_wrong"));
    }
}

void AssetsUpdateLayer::onProgress(int percent)
{
    CCLOG("AssetsUpdateLayer::onProgress");
/*
    char progress[128];
    char packageDownloaded[32];
    char packageTotal[32];
    if (m_packageLength > 1024 * 1024)
        snprintf(packageTotal, 32, "%.2fMB", ((double)m_packageLength) / (1024.0 * 1024.0));
    else
        snprintf(packageTotal, 32, "%.2fKB", ((double)m_packageLength) / 1024.0);
    if (m_curPackageLength > 1024 * 1024)
        snprintf(packageDownloaded, 32, "%.2fMB", (double)m_curPackageLength / (1024.0 * 1024.0));
    else
        snprintf(packageDownloaded, 32, "%.2fKB", (double)m_curPackageLength / 1024.0);
    if (percent != 100)
        ; //snprintf(progress, 128, (char*)Localization::getInstance()->getValueByKey("Loading_load_percentAndSpeed"), percent, m_downloadSpeed, packageDownloaded, packageTotal);
    else
        ; //strncpy(progress, (char*)Localization::getInstance()->getValueByKey("Loading_isUnzip"), 128);
    //m_percentLabel->setString(progress);
    //	m_progressBar->setPercentage(percent);
*/
}

void AssetsUpdateLayer::onSuccess()
{
    //m_percentLabel->setString(Localization::getInstance()->getValueByKey("Loading_update_done"));
    CCLOG("AssetsUpdateLayer::onSuccess");
    m_isDownloading = false;
}

void AssetsUpdateLayer::requestFinished(CCHTTPRequest* request) 
{
    CCLOG("AssetsUpdateLayer::requestFinished");
    CCLOG("HEADER [%s]", request->getResponseHeadersString().c_str());
    scheduleUpdate();

}
void AssetsUpdateLayer::requestFailed(CCHTTPRequest* request)
{
    CCLOG("AssetsUpdateLayer::requestFailed");
    CCLOG("HEADER [%s]", request->getResponseHeadersString().c_str());
    scheduleUpdate();

}

#if 0
void AssetsUpdateLayer::GoToAppPage(CCObject *sender, CCControl::EventType controlEvent)
{
    //DevicePlatform::getInstance()->openAppUrl();
}

void AssetsUpdateLayer::updateLayerMessageBox()
{

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ccb/ccbResources/SystemLoading.plist");

    CCLayer* layer = CCLayer::create();
    layer->ignoreAnchorPointForPosition(false);
    layer->setContentSize(Size(960, 640));
    layer->setPosition(Point(Director::getInstance()->getWinSize().width * 0.5, Director::getInstance()->getWinSize().height * 0.5));
    layer->setScale(Director::getInstance()->getWinSize().width / 960);
    addChild(layer, 1);

    Sprite* messageboxSprite = Sprite::createWithSpriteFrameName("set_floor_v1.png");
    messageboxSprite->setPosition(Point(480, 320));
    layer->addChild(messageboxSprite, 1);

    LabelTTF* labelContent = LabelTTF::create(Localization::getInstance()->getValueByKey("Loading_warning_need_update"), "Helvetica",32);
    labelContent->setPosition(Point(480, 420));
    layer->addChild(labelContent, 1);

    ControlButton* btn = ControlButton::create(LabelTTF::create(Localization::getInstance()->getValueByKey("BUTTON_DONE"), "Helvetica", 32), Scale9Sprite::createWithSpriteFrameName("set_button2.png"));
    btn->setPreferredSize(CCSize(191,70));
    btn->setPosition(Point(480, 220));
    btn->addTargetWithActionForControlEvents(this, cccontrol_selector(AssetsUpdateLayer::GoToAppPage), Control::EventType::TOUCH_UP_INSIDE);
    layer->addChild(btn, 1);

}
#endif
