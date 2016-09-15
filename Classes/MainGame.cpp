#include "MainGame.h"
#include "Pet.h"
#include "cocostudio\CocoStudio.h"
#include "Definitions.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
#include "AdmobManager.h"
USING_NS_CC;
extern int playtimes;
Scene* MainGame::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainGame::create();

	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MainGame::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	//Check if user purchased
	sdkbox::IAP::init();
	sdkbox::IAP::setListener(this);
	if (!UserDefault::sharedUserDefault()->getBoolForKey("NO_ADS"))
	{
		AdmobManager::showAd();
	}
	else { AdmobManager::hideAd(); }
	//-----
	sdkbox::IAP::setListener(this);
	this->setKeypadEnabled(true);
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	GroundSize = visibleSize.width / PART_AMOUNT;
	lifetimeafterstepped = 1.5;
	isGameOver = false;
	score = 0;
	style = RandomHelper::random_int(0, STYLE_AMOUNT-1);
	__String* _bgPath = __String::createWithFormat("Background%d.jpg", style);
	PlayerPos = 1; //Start on 3. Ground
	GroundList.clear();
	groundPosx = GroundSize*1.5;
	groundPosy = visibleSize.height / 2;
	Player = new Pet(this);
	Player->getPet()->setPosition(Point(2.5*GroundSize, visibleSize.height / 2 + GroundSize / 3 + Player->getPet()->getContentSize().height*Player->getPet()->getScale()/2));
	cameraCenter = Node::create();
	cameraCenter->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(cameraCenter, 0);
	auto sprite = Sprite::create(_bgPath->getCString());
	sprite->setScale(1.3*visibleSize.width / sprite->getContentSize().width);
	sprite->setPosition(Point(0,0));
	cameraCenter->setAnchorPoint(Point(0, 0));
	cameraCenter->addChild(sprite, 0);
	this->runAction(Follow::create(cameraCenter, Rect::ZERO)); // Follow cameraCenter
	for (int i = 0;i < PART_AMOUNT - 2;i++) //Load first 8 grounds
	{
		GenerateGround();
	}
	//-----Game Process------//
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(MainGame::OnTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	//-----End Game Process------//
	oldPosx = Player->getPet()->getPosition().x;
	oldcameraPosx = cameraCenter->getPosition().x;
	//Parallax Background
	parallax1 = ParallaxNode::create();
	parallaxBg1 = Sprite::create("Parallax.png");
	parallaxBg1->setScale(visibleSize.width / parallaxBg1->getContentSize().width);
	parallax2 = ParallaxNode::create();
	parallax1->setAnchorPoint(Point(0.5, 0.5));
	parallaxBg1->setPosition(Point(0, 0));
	parallax2->setAnchorPoint(Point(0.5, 0.5));
	parallax1->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	parallax2->setPosition(Point(visibleSize.width / 2 + parallaxBg1->getContentSize().width / PARALLAX_SCROLL_RATIO, visibleSize.height / 2));
	parallaxBg2 = Sprite::create("Parallax1.png");
	parallaxBg2->setScale(visibleSize.width / parallaxBg2->getContentSize().width);
	parallaxBg2->setAnchorPoint(Point(0.5, 0.5));
	parallaxBg2->setPosition(Point(parallaxBg1->getContentSize().width, 0));
	this->addChild(parallax1, 1);
	this->addChild(parallax2, 1);
	parallax1->addChild(parallaxBg1, 1, Vec2(PARALLAX_SCROLL_RATIO, 0), Vec2(0, visibleSize.height / 2));
	parallax2->addChild(parallaxBg2, 1, Vec2(PARALLAX_SCROLL_RATIO, 0), Vec2(0, visibleSize.height / 2));
	//parallax1->runAction(RepeatForever::create(Sequence::create(CallFunc::create(CC_CALLBACK_0(MainGame::UpdateParallax, this)), nullptr)));
	//End Parallax
	//Scoring
	__String* scoreText = __String::createWithFormat("%d", score);

	scoreDisplay = Label::createWithTTF(scoreText->getCString(), "fonts/BebasNeue.otf", visibleSize.width / PART_AMOUNT);
	this->addChild(scoreDisplay, 6);
	scoreDisplay->setPosition(Point(visibleSize.width / 2, visibleSize.height - scoreDisplay->getContentSize().height));
	tutorial1 = Sprite::create("tapleft.png");
	tutorial2 = Sprite::create("tapright.png");
	tutorial1->setScale((visibleSize.width / 3) / tutorial1->getContentSize().width);
	tutorial2->setScale((visibleSize.width / 3) / tutorial2->getContentSize().width);
	this->addChild(tutorial1, 2);
	this->addChild(tutorial2, 2);
	tutorial1->setPosition(Point(visibleSize.width / 4, visibleSize.height / 4));
	tutorial2->setPosition(Point(3 * visibleSize.width / 4, visibleSize.height / 4));
	this->scheduleUpdate();
	return true;
}
void MainGame::GenerateGround()
{
	float random = CCRANDOM_0_1();
	bool item = false;
	if (random < ITEM_APPEAR_PROBABILITY && score!=0)
	{
		item = true;
	}
	__String* _groundPath = __String::createWithFormat("Ground%d.jpg", style);
	GroundList.push_back(new Ground(this, groundPosx, groundPosy, item, _groundPath));
	groundPosx += GroundSize;
}
void MainGame::GameOver(bool wrongtouch)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("wrong.mp3");
	isGameOver = true;
	cameraCenter->stopAllActions();
	scoreDisplay->stopAllActions();
	Player->getPet()->stopAllActions();
	for (auto i : GroundList)
	{
		i->getSprite()->stopAllActions();
	}
	if (wrongtouch)
	{
		wrongtap = Sprite::create("wrongtap.png");
		wrongtap->setScale((visibleSize.width / 3) / wrongtap->getContentSize().width);
		wrongtap->setPosition(cameraCenter->getPosition().x, visibleSize.height / 3);
		auto tintaction = TintBy::create(0.2, 200, -255, -255);
		GroundList[PlayerPos + 1]->getSprite()->runAction(RepeatForever::create(Sequence::create(tintaction, tintaction->reverse(), nullptr)));
	}
	else { wrongtap = Sprite::create(); }
	this->addChild(wrongtap, 1);
	this->runAction(Sequence::create(DelayTime::create(1.5), CallFunc::create(CC_CALLBACK_0(MainGame::DestroyAll, this)), DelayTime::create(2), CallFunc::create(CC_CALLBACK_0(MainGame::ShowGameOverMenu, this)), nullptr));
	if (UserDefault::sharedUserDefault()->getIntegerForKey("HIGH_SCORE") < score)
	{
		UserDefault::sharedUserDefault()->setIntegerForKey("HIGH_SCORE", score);
		UserDefault::sharedUserDefault()->flush();
	}
}
void MainGame::DestroyAll()
{
	auto easebackin = EaseBackIn::create(MoveBy::create(1, Vec2(0, -visibleSize.height)));
	wrongtap->runAction(FadeOut::create(0.5));
	for (auto i : GroundList)
	{
		i->Destroy();
	}
	Player->getPet()->runAction(easebackin);
	Player->getPet()->runAction(RotateBy::create(1, 180));
}
void MainGame::ShowGameOverMenu()
{
	auto bgLayer = LayerColor::create(Color4B(0, 0, 0, 200), visibleSize.width * 2, visibleSize.height * 2);
	bgLayer->setPosition(cameraCenter->getPosition().x - bgLayer->getContentSize().width / 2, 0);
	this->addChild(bgLayer, 4);
	playagainBtn = MenuItemImage::create("nutrestart.png", "nutrestart.png", CC_CALLBACK_0(MainGame::PlayAgain, this));
	playagainBtn->setPosition(Point(cameraCenter->getPosition().x, visibleSize.height / 2.5));
	float playBtnTargetSize = (visibleSize.width / 3) / playagainBtn->getContentSize().width;
	playagainBtn->setScale(0);
	auto thunho = ScaleTo::create(playBtnTargetSize,1);
	auto ease = EaseElasticOut::create(thunho->clone());
	playagainBtn->runAction(Sequence::create(DelayTime::create(0.3), ease, nullptr));
	auto MainMenu = Menu::create(playagainBtn, NULL);
	MainMenu->setPosition(Point::ZERO);
	this->addChild(MainMenu, 5);
	auto RemoveAds = MenuItemImage::create("ads.png", "ads.png", [](Ref* pSender)
	{
		sdkbox::IAP::purchase("remove_ads");
	});
	float optionsscale = (visibleSize.width / 5) / (RemoveAds->getContentSize().width);

	auto dichuyen1 = MoveTo::create(0.3, Vec2(1.5*visibleSize.width / 8 + cameraCenter->getPosition().x - visibleSize.width / 2, visibleSize.height / 3.5));
	auto dichuyen2 = MoveTo::create(0.3, Vec2(6.5 * visibleSize.width / 8 + cameraCenter->getPosition().x - visibleSize.width / 2, visibleSize.height / 3.5));
	RemoveAds->setScale(optionsscale);
	RemoveAds->setPosition(Point(1.5*visibleSize.width / 8-visibleSize.width/4+cameraCenter->getPosition().x-visibleSize.width/2, visibleSize.height / 3.5));
	RemoveAds->runAction(dichuyen1);
	auto Rate = MenuItemImage::create("rate.png", "rate.png", [](Ref* pSender)
	{
		AdmobManager::openURL("market://details?id=com.dimstudio.workingpet");
	});
	float optionsscale2 = (visibleSize.width / 5) / (Rate->getContentSize().width);
	auto thunho2 = ScaleTo::create(1,optionsscale);
	auto ease2 = EaseElasticOut::create(thunho2->clone());
	Rate->setScale(0);
	Rate->setPosition(Point(3 * visibleSize.width / 8 + cameraCenter->getPosition().x - visibleSize.width / 2, visibleSize.height / 5));
	Rate->runAction(Sequence::create(DelayTime::create(0.3), ease2, nullptr));

	SoundOn = MenuItemImage::create("muteactive.png", "muteactive.png", CC_CALLBACK_0(MainGame::TurnSoundOn, this));
	SoundOff = MenuItemImage::create("mute.png", "mute.png", CC_CALLBACK_0(MainGame::TurnSoundOff, this));
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
	SoundOff->setScale(0);
	SoundOff->setPosition(Point(5 * visibleSize.width / 8 + cameraCenter->getPosition().x - visibleSize.width / 2, visibleSize.height / 5));
	SoundOn->setScale(0);
	SoundOn->setPosition(Point(5 * visibleSize.width / 8 + cameraCenter->getPosition().x - visibleSize.width / 2, visibleSize.height / 5));
	SoundOn->runAction(Sequence::create(DelayTime::create(0.3), ease2->clone(), nullptr));
	SoundOff->runAction(Sequence::create(DelayTime::create(0.3), ease2->clone(), nullptr));
	auto Home = MenuItemImage::create("home.png", "home.png", [](Ref* pSender) {
		auto scene = MainMenuScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
	});
	Home->setScale(optionsscale);
	Home->setPosition(Point(6.5 * visibleSize.width / 8+visibleSize.width/4 + cameraCenter->getPosition().x - visibleSize.width / 2, visibleSize.height / 3.5));
	Home->runAction(dichuyen2);
	auto Options = Menu::create(RemoveAds, Rate, SoundOn,SoundOff, Home, NULL);
	Options->setPosition(Point::ZERO);
	this->addChild(Options, 5);
	scoreDisplay->runAction(Spawn::createWithTwoActions(MoveTo::create(0.3, Vec2(playagainBtn->getPosition().x, 2*visibleSize.height/3)), ScaleTo::create(0.3, 3)));
	__String* bestscore = __String::createWithFormat("BEST SCORE: %d", UserDefault::sharedUserDefault()->getIntegerForKey("HIGH_SCORE"));
	auto bestDisplay = Label::createWithTTF(bestscore->getCString(), "fonts/BebasNeue.otf", playagainBtn->getContentSize().height);
	this->addChild(bestDisplay, 5);
	bestDisplay->setPosition(cameraCenter->getPosition());
}
void MainGame::PlayAgain()
{
	playagainBtn->runAction(Sequence::create(Spawn::createWithTwoActions(ScaleTo::create(0.2, 1.2), RotateBy::create(0.2, -10.0f)), Spawn::createWithTwoActions(RotateBy::create(0.1, 10.0f), ScaleTo::create(0.1, 0)), nullptr));
	auto scene = MainGame::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
	playtimes += 1;
	if (playtimes % 3 == 0 && !UserDefault::sharedUserDefault()->getBoolForKey("NO_ADS"))
	{
		AdmobManager::showStartappAd();
	}
}
void MainGame::RemoveFromGroundList()
{
	GroundList.erase(GroundList.begin());
	PlayerPos -= 1;
	if (PlayerPos < 0 && !isGameOver)
	{
		GameOver(false);
	}
}
bool MainGame::OnTouchBegan(Touch* touch, Event* event) // <---- Game Process
{
	if (!isGameOver)
	{
		if (score == 0)
		{
			tutorial1->runAction(FadeOut::create(0.3));
			tutorial2->runAction(FadeOut::create(0.4));
		}
		float touchx = touch->getLocation().x;
		if (touchx >= visibleSize.width / 2)  // die rechte seite wurde getoucht. Springen ohne Karrote zu sammeln
		{
			if (!GroundList[PlayerPos + 1]->isHavingItem()) { JumpForward(); }
			else { GameOver(true); }
		}
		else //die linke seite wurde getoucht. Karrote sammeln
		{
			if (GroundList[PlayerPos + 1]->isHavingItem()) { JumpForward(); }
			else { GameOver(true); }
		}
	}
	return true;

}
void MainGame::JumpForward()
{
	PlayerPos += 1;
	score += 1;
	__String* scoreText = __String::createWithFormat("%d", score);
	scoreDisplay->setString(scoreText->getCString());
	Player->getPet()->getAnimation()->playByIndex(0, JUMP_TIME, 0);
	Player->getPet()->runAction(JumpBy::create(JUMP_TIME, Vec2(GroundSize, 0), GroundSize / 2, 1));
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("jump.mp3");
	cameraCenter->runAction(MoveBy::create(JUMP_TIME, Vec2(GroundSize, 0)));
	scoreDisplay->runAction(MoveBy::create(JUMP_TIME, Vec2(GroundSize, 0)));
	if (PlayerPos >= GroundList.size() - 4) { GenerateGround(); }
	if (PlayerPos > 1)
	{
		GroundList[0]->Destroy();
		RemoveFromGroundList();
	}
	oldPosx += GroundSize;

	if (GroundList[PlayerPos]->isHavingItem())
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pickup.mp3");
		GroundList[PlayerPos]->getItemSprite()->runAction(MoveBy::create(1, Vec2(0, GroundSize * 2)));
		GroundList[PlayerPos]->getItemSprite()->runAction(FadeOut::create(0.5));
	}
	if (score > 0)
	{
		CallFunc *runCallback = CallFunc::create(CC_CALLBACK_0(Ground::Destroy, GroundList[PlayerPos]));
		CallFunc *removeCallback = CallFunc::create(CC_CALLBACK_0(MainGame::RemoveFromGroundList, this));
		auto delay = DelayTime::create(lifetimeafterstepped);
		GroundList[PlayerPos]->getSprite()->runAction(Sequence::create(delay, runCallback, removeCallback, nullptr));
	}
	if (score == 1)
	{

		for (int i = 0; i < PlayerPos; i++)
		{
			GroundList[i]->Destroy();
			RemoveFromGroundList();
		}
	}
	if (score % 5 == 0 && lifetimeafterstepped > 0.4)
	{
		lifetimeafterstepped -= 0.1;
	}
	if (score == 100)
	{
		lifetimeafterstepped = 0.3;
	}
	if (score == 150)
	{
		lifetimeafterstepped = 0.275;
	}
}
void MainGame::update(float dt)
{
	if (parallax1->getPosition().x + parallaxBg1->getPosition().x <= cameraCenter->getPosition().x - (parallaxBg1->getContentSize().width / 2 + visibleSize.width / 2))
	{
		float distanz = cameraCenter->getPosition().x + visibleSize.width / 2 + parallaxBg1->getContentSize().width / 2 - (parallax1->getPosition().x + parallaxBg1->getPosition().x);
		parallax1->setPosition(Point(parallax1->getPosition().x + distanz / PARALLAX_SCROLL_RATIO, parallax1->getPosition().y));
	}
	if (parallax2->getPosition().x + parallaxBg2->getPosition().x <= cameraCenter->getPosition().x - (parallaxBg2->getContentSize().width / 2 + visibleSize.width / 2))
	{
		float distanz = cameraCenter->getPosition().x + visibleSize.width / 2 + parallaxBg2->getContentSize().width / 2 - (parallax2->getPosition().x + parallaxBg2->getPosition().x);
		parallax2->setPosition(Point(parallax2->getPosition().x + distanz / PARALLAX_SCROLL_RATIO, parallax2->getPosition().y));
	}
}
void MainGame::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		count++;
		if (count == 1)
		{
			AdmobManager::showToast("Tap again to go back");
		}
		if (count == 2)
		{
			auto scene = MainMenuScene::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
		}
	}
}
void MainGame::TurnSoundOff()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	UserDefault::sharedUserDefault()->setBoolForKey("SOUND_ON", false);
	UserDefault::sharedUserDefault()->flush();
	SoundOff->setVisible(false);
	SoundOn->setVisible(true);
}
void MainGame::TurnSoundOn()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("BGM.mp3");
	UserDefault::sharedUserDefault()->setBoolForKey("SOUND_ON", true);
	UserDefault::sharedUserDefault()->flush();
	SoundOff->setVisible(true);
	SoundOn->setVisible(false);
}
void MainGame::onSuccess(const sdkbox::Product &p)
{
	if (p.name == "remove_ads") {
		UserDefault::sharedUserDefault()->setBoolForKey("NO_ADS", true);
		UserDefault::sharedUserDefault()->flush();
	}
	AdmobManager::showToast("Thanks for your purchasing!");
}
void MainGame::onCanceled(const sdkbox::Product &p)
{
	AdmobManager::showToast("Purchase Canceled.");
}
void MainGame::onFailure(const sdkbox::Product &p, const std::string &msg)
{
	AdmobManager::showToast("Purchase failed");
}
void MainGame::onInitialized(bool ok) {}
void MainGame::onRestoreComplete(bool ok, const std::string &msg) {}
void MainGame::onRestored(sdkbox::Product const& p) {}
void MainGame::onProductRequestSuccess(std::vector<sdkbox::Product> const &products) {}
void MainGame::onProductRequestFailure(const std::string &msg) {}