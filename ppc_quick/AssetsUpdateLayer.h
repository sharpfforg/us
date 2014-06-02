#ifndef __ASSETS_UPDATE_LAYER_H__
#define __ASSETS_UPDATE_LAYER_H__

#include "cocos2d.h"
#include "AssetsManager/AssetsManager.h"
#include "network/CCHTTPRequest.h"
#include "cocos-ext.h"

#define SERVER_ADDRESS "192.168.3.100"

USING_NS_CC;
USING_NS_CC_EXTRA;
USING_NS_CC_EXT;

class AssetsUpdateLayer : public CCLayer, public CCHTTPRequestDelegate, AssetsManagerDelegateProtocol
{
public:
    CREATE_FUNC(AssetsUpdateLayer)
        AssetsUpdateLayer();
    ~AssetsUpdateLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void update(float delta);

    virtual void onError(AssetsManager::ErrorCode errorCode);
    virtual void onProgress(int percent);
    virtual void onSuccess();

    virtual void requestFinished(CCHTTPRequest* request);
    virtual void requestFailed(CCHTTPRequest* request);


    void requestLastestVersion();	//请求最新版本
    void requestLastestVersionCallback(CCHTTPRequest* req);
    bool checkUpdate();		//判断是否需要更新
    void calcLocalLatestVersion();	//计算本地最新版本
    void download();		//下载更新
    void gameEnter();		//版本检测ok，进入游戏
    void MD5Check();		//下载完成后进行md5检测
    void speedUpdate(CCObject* object);		//计算速度
    //void GoToAppPage(CCObject *sender, CCControl::EventType controlEvent);
    //void updateLayerMessageBox();
private:
    AssetsManager* getAssetsManager();
    void createDownloadedDir();
    void removeDownloadedDir();	//在大版更新的时候删除掉下载的资源
    void getVerNumber(int& num1, int& num2, int& num3, string versionStr);
    void createLayerItem();	//创建本界面元素

    //LabelTTF* m_percentLabel;	//进度条文本
    //LabelTTF* m_packegNumberLabel;	//剩余资源包数量
    //ProgressTimer* m_progressBar;	//下载进度
    int m_curBigVersion;
    int m_curMidVersion;
    int m_curSmallVersion;
    int m_endBigVersion;
    int m_endMidVersion;
    int m_endSmallVersion;
    string m_pathToSave;
    string m_defaultPath;
    bool m_isDownloading;
    bool m_backgroundSpriteCreated;
    size_t m_oneSecondPackageLength;	//one second download package length for calculate download speed
    struct timeval m_lastTime;
    float m_downloadSpeed;
    double m_packageLength;
    double m_curPackageLength;
};

#endif
