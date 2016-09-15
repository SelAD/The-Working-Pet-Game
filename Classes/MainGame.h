#ifndef __MAIN_GAME_H__
#define __MAIN_GAME_H__

#include "cocos2d.h"
#include "Pet.h"
#include "Ground.h"
#include "pluginiap/PluginIAP.h"
//#include "proj.android-studio\app\jni\pluginiap\PluginIAP.h"
class MainGame : public cocos2d::Layer, public sdkbox::IAPListener
{
public:
    static cocos2d::Scene* createScene();
	bool OnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void RemoveFromGroundList();
	void GenerateGround();
    virtual bool init();
    // implement the "static create()" method manually
    CREATE_FUNC(MainGame);
private:
	unsigned int score;
	virtual void onSuccess(sdkbox::Product const& p) override;
	virtual void onInitialized(bool ok) override;
	virtual void onFailure(sdkbox::Product const& p, const std::string &msg) override;
	virtual void onRestoreComplete(bool ok, const std::string &msg) override;
	virtual void onCanceled(sdkbox::Product const& p) override;
	virtual void onRestored(sdkbox::Product const& p) override;
	virtual void onProductRequestSuccess(std::vector<sdkbox::Product> const &products) override;
	virtual void onProductRequestFailure(const std::string &msg) override;
	cocos2d::Label* scoreDisplay;
	Pet* Player;
	double GroundSize;
	bool isGameOver = false;
	cocos2d::MenuItemImage* playagainBtn;
	float oldPosx, oldcameraPosx;
	float lifetimeafterstepped, groundPosx, groundPosy;
	void JumpForward();
	int style;
	void GameOver(bool wrongtouch);
	void PlayAgain();
	void update(float dt);
	void UpdateParallax();
	void ShowGameOverMenu();
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);
	void DestroyAll();
	cocos2d::Sprite* tutorial1;
	cocos2d::Sprite* tutorial2;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Sprite* groundSprite;
	cocos2d::Node* cameraCenter;
	std::vector<Ground*> GroundList;
	cocos2d::ParallaxNode* parallax1;
	cocos2d::ParallaxNode* parallax2;
	cocos2d::Sprite* parallaxBg1;
	cocos2d::Sprite* parallaxBg2;
	cocos2d::Sprite* wrongtap;
	cocos2d::MenuItemImage* SoundOn;
	cocos2d::MenuItemImage* SoundOff;
	void TurnSoundOn();
	void TurnSoundOff();
	int PlayerPos;
	int count = 0;
};

#endif // __MAIN_GAME_H__
