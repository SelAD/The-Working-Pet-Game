#ifndef __SPLASH_SCREEN_H__
#define __SPLASH_SCREEN_H__

#include "cocos2d.h"

class SplashScreen : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    // implement the "static create()" method manually
    CREATE_FUNC(SplashScreen);
private:
	void GoToMainMenuScene(float dt);
};

#endif // __SPLASH_SCREEN_H__
