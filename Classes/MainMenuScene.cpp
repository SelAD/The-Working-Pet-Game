#include "MainMenuScene.h"
#include "MainGame.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h"
#include "Pet.h"
#include "Ground.h"
#include "AdmobManager.h"
USING_NS_CC;
int playtimes = 0;
Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first

	if (!Layer::init())
	{
		return false;
	}
	this->setKeypadEnabled(true);
	initAudio();
	sdkbox::IAP::init();
	//Check if user purchased
	sdkbox::IAP::setListener(this);
	if (!UserDefault::sharedUserDefault()->getBoolForKey("NO_ADS"))
	{
		AdmobManager::showAd();
	}
	else { AdmobManager::hideAd(); }
	//-----
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto sprite = Sprite::create("Background0.jpg");
	sprite->setScale(1.3*visibleSize.width / sprite->getContentSize().width);
	sprite->setPosition(Point(visibleSize.width / 2+origin.x, visibleSize.height / 2+origin.y));
	this->addChild(sprite, 0);
	auto playBtn = MenuItemImage::create("nutplay.png", "nutplay.png",CC_CALLBACK_1(MainMenuScene::GoToMainGame, this));
	playBtn->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2.5 + origin.y));
	float playBtnTargetSize = visibleSize.width / 3;
	playBtn->setScale(playBtnTargetSize/playBtn->getContentSize().width);
	playBtn->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0), DelayTime::create(0.5), FadeIn::create(0), DelayTime::create(0.5),nullptr)));
	auto the = MenuItemImage::create("the.png", "the.png");
	auto working = MenuItemImage::create("working.png", "working.png");
	auto pet = MenuItemImage::create("pet.png", "pet.png");
	float workingScale = (visibleSize.width / 2) / working->getContentSize().width;
	working->setScale(workingScale);
	pet->setScale(workingScale);
	the->setScale(workingScale);
	auto TitleMenu = Menu::create(the,working,pet, NULL);
	TitleMenu->alignItemsVerticallyWithPadding(visibleSize.height / 60);
	this->addChild(TitleMenu,1);
	TitleMenu->setPosition(Point(playBtn->getPosition().x, visibleSize.height-(working->getContentSize().height+pet->getContentSize().height+the->getContentSize().height*2+visibleSize.height/30)/2));
	auto MainMenu = Menu::create(playBtn, NULL);
	MainMenu->setPosition(Point::ZERO);
	this->addChild(MainMenu, 1);
	auto RemoveAds = MenuItemImage::create("ads.png", "ads.png", [](Ref* pSender)
	{
		//AdmobManager::showToast("This feature will be available in the new update");
		sdkbox::IAP::purchase("remove_ads");
	});
	
	float optionsscale = (visibleSize.width/5 )/ RemoveAds->getContentSize().width;
	RemoveAds->setScale(optionsscale);
	RemoveAds->setPosition(Point(1.5*visibleSize.width/8 + origin.x,visibleSize.height/3.5));
	auto Rate = MenuItemImage::create("rate.png", "rate.png", [](Ref* pSender)
	{
		AdmobManager::openURL("market://details?id=com.dimstudio.workingpet");
	});
	Rate->setScale(optionsscale);
	Rate->setPosition(Point(3*visibleSize.width / 8 + origin.x, visibleSize.height / 5));
	SoundOn = MenuItemImage::create("muteactive.png", "muteactive.png", CC_CALLBACK_0(MainMenuScene::TurnSoundOn, this));
	SoundOff = MenuItemImage::create("mute.png", "mute.png", CC_CALLBACK_0(MainMenuScene::TurnSoundOff,this));
	if (UserDefault::sharedUserDefault()->getBoolForKey("SOUND_ON")) 
	{
		SoundOff->setVisible(true);
		SoundOn->setVisible(false);
	}
	else
	{
		SoundOff->setVisible(false);
		SoundOn->setVisible(true);
	}
	SoundOff->setScale(optionsscale);
	SoundOff->setPosition(Point(5 * visibleSize.width / 8 + origin.x, visibleSize.height / 5));
	SoundOn->setScale(optionsscale);
	SoundOn->setPosition(Point(5 * visibleSize.width / 8 + origin.x, visibleSize.height / 5));
	auto Like = MenuItemImage::create("like.png", "like.png", [](Ref* pSender)
	{
		AdmobManager::openURL("https://www.facebook.com/dimgames/");
	});
	Like->setScale(optionsscale);
	Like->setPosition(Point(6.5 * visibleSize.width / 8 + origin.x, visibleSize.height / 3.5));
	auto Options = Menu::create(RemoveAds, Rate, SoundOn,SoundOff, Like, NULL);
	Options->setPosition(Point::ZERO);
	this->addChild(Options, 1);
	__String* path = __String::create("Ground0.jpg");
	auto ground = new Ground(this, playBtn->getPosition().x, visibleSize.height/2, false, path);
	auto PlayerPreview = new Pet(this);
	PlayerPreview->getPet()->setScale(PlayerPreview->getPet()->getScale()*1.5);
	PlayerPreview->getPet()->setPosition(Point(playBtn->getPosition().x,visibleSize.height/2 + visibleSize.width/ (PART_AMOUNT*3) + PlayerPreview->getPet()->getContentSize().height*PlayerPreview->getPet()->getScale() / 2));
	
	PlayerPreview->getPet()->runAction(RepeatForever::create(Sequence::create(JumpBy::create(JUMP_TIME, Vec2(0, 0), visibleSize.width / (2 * PART_AMOUNT), 1),DelayTime::create(0.6),nullptr)));
	//PlayerPreview->getPet()->getAnimation()->playByIndex(0,0,1);
	
    return true;
}
void MainMenuScene::GoToMainGame(Ref* pSender) //Replace MainMenuScene with MainGame-Scene
{
	auto scene = MainGame::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
void MainMenuScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		count++;
		if (count == 1)
		{
			AdmobManager::showToast("Tap again to quit");
		}
		if (count == 2)
		{
			Director::getInstance()->end();
		}
	}
}
void MainMenuScene::initAudio()
{
	FileUtils::getInstance()->addSearchPath("sounds", true);
	if (UserDefault::sharedUserDefault()->getIntegerForKey("HIGH_SCORE") == 0)
	{
		UserDefault::sharedUserDefault()->setBoolForKey("SOUND_ON", true);
		UserDefault::sharedUserDefault()->flush();
	}
	
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BGM.mp3");	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("wrong.mp3");	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("jump.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("pickup.mp3");

	if (UserDefault::sharedUserDefault()->getBoolForKey("SOUND_ON"))
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("BGM.mp3", true);
	}
}
void MainMenuScene::TurnSoundOff()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	UserDefault::sharedUserDefault()->setBoolForKey("SOUND_ON", false);
	UserDefault::sharedUserDefault()->flush();
	SoundOff->setVisible(false);
	SoundOn->setVisible(true);
}
void MainMenuScene::TurnSoundOn()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("BGM.mp3");
	UserDefault::sharedUserDefault()->setBoolForKey("SOUND_ON", true);
	UserDefault::sharedUserDefault()->flush();
	SoundOff->setVisible(true);
	SoundOn->setVisible(false);
}
void MainMenuScene::onSuccess(const sdkbox::Product &p)
{
	if (p.name == "remove_ads") {
		UserDefault::sharedUserDefault()->setBoolForKey("NO_ADS", true);
		UserDefault::sharedUserDefault()->flush();
	}
	AdmobManager::showToast("Thanks for your purchasing!");
}
void MainMenuScene::onCanceled(const sdkbox::Product &p)
{
	AdmobManager::showToast("Purchase canceled");
}
void MainMenuScene::onFailure(const sdkbox::Product &p, const std::string &msg)
{
	AdmobManager::showToast("Purchase failed");
}
void MainMenuScene::onInitialized(bool ok) {}
void MainMenuScene::onRestoreComplete(bool ok, const std::string &msg) {}
void MainMenuScene::onRestored(sdkbox::Product const& p) {}
void MainMenuScene::onProductRequestSuccess(std::vector<sdkbox::Product> const &products) {}
void MainMenuScene::onProductRequestFailure(const std::string &msg) {}