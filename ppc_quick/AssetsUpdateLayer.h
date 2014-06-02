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


    void requestLastestVersion();	//�������°汾
    void requestLastestVersionCallback(CCHTTPRequest* req);
    bool checkUpdate();		//�ж��Ƿ���Ҫ����
    void calcLocalLatestVersion();	//���㱾�����°汾
    void download();		//���ظ���
    void gameEnter();		//�汾���ok��������Ϸ
    void MD5Check();		//������ɺ����md5���
    void speedUpdate(CCObject* object);		//�����ٶ�
    //void GoToAppPage(CCObject *sender, CCControl::EventType controlEvent);
    //void updateLayerMessageBox();
private:
    AssetsManager* getAssetsManager();
    void createDownloadedDir();
    void removeDownloadedDir();	//�ڴ����µ�ʱ��ɾ�������ص���Դ
    void getVerNumber(int& num1, int& num2, int& num3, string versionStr);
    void createLayerItem();	//����������Ԫ��

    //LabelTTF* m_percentLabel;	//�������ı�
    //LabelTTF* m_packegNumberLabel;	//ʣ����Դ������
    //ProgressTimer* m_progressBar;	//���ؽ���
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
