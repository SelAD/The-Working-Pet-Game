#ifndef __PET_H__
#define __PET_H__

#include "cocos2d.h"
#include "cocostudio\CocoStudio.h"

class Pet
{
public:
	Pet(cocos2d::Layer* layer);
	void ChangePet(int petIndex);
	void ChangeAnim(int animIndex);
	cocostudio::Armature* getPet();
private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocostudio::Armature* Player;
};

#endif // __PET_H__
