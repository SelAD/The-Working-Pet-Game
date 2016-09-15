#include "SplashScreen.h"
#include "MainMenuScene.h"
#include "Definitions.h"
#include "AdmobManager.h"
USING_NS_CC;

Scene* SplashScreen::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = SplashScreen::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScreen::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	AdmobManager::hideAd();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto splashLogo = Sprite::create("DIMLogo.png");

	splashLogo->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(splashLogo, 0);
	this->scheduleOnce(schedule_selector(SplashScreen::GoToMainMenuScene), SPLASHSCREEN_TIME);
    return true;
}
void SplashScreen::GoToMainMenuScene(float dt)
{
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
