#ifndef __GROUND_H__
#define __GROUND_H__

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"

class Ground
{
public:
	Ground(cocos2d::Layer* layer, float x, float y,bool item, cocos2d::__String* path);
	void setItemStatus(bool status);
	cocos2d::Sprite* getSprite();
	void Destroy();
	bool isHavingItem();
	cocos2d::Sprite* getItemSprite();
private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Sprite* ground;
	cocos2d::Sprite* itemSprite;
	bool itemStatus;
};

#endif // __GROUND_H__
