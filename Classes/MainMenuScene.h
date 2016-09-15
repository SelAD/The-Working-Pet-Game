#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__
#include "pluginiap/PluginIAP.h"
//#include "proj.android-studio\app\jni\pluginiap\PluginIAP.h"
#include "cocos2d.h"
class MainMenuScene : public cocos2d::Layer, public sdkbox::IAPListener
{
public:
    static cocos2d::Scene* createScene();
	void TurnSoundOn();
	void TurnSoundOff();
    virtual bool init();
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);
private:
	void GoToMainGame(Ref* pSender);
	void initAudio();
	cocos2d::MenuItemImage* SoundOn;
	cocos2d::MenuItemImage* SoundOff;
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);
	virtual void onSuccess(sdkbox::Product const& p) override;
	virtual void onInitialized(bool ok) override;
	virtual void onFailure(sdkbox::Product const& p, const std::string &msg) override;
	virtual void onRestoreComplete(bool ok, const std::string &msg) override;
	virtual void onCanceled(sdkbox::Product const& p) override;
	virtual void onRestored(sdkbox::Product const& p) override;
	virtual void onProductRequestSuccess(std::vector<sdkbox::Product> const &products) override;
	virtual void onProductRequestFailure(const std::string &msg) override;
	int count = 0;
};

#endif // __MAINMENU_SCENE_H__
