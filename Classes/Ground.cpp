#include "Ground.h"
#include "Definitions.h"
#include "MainGame.h"
USING_NS_CC;

Ground::Ground(Layer* layer,float x, float y, bool item, __String* path)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	ground = Sprite::create(path->getCString());
	ground->setPosition(Point(x,visibleSize.height/4));
	ground->setOpacity(0);
	Size groundTargetSize = Size(visibleSize.width / PART_AMOUNT, visibleSize.height / PART_AMOUNT);
	ground->setScale(groundTargetSize.width / ground->getContentSize().width);
	layer->addChild(ground, 2);
	this->setItemStatus(item);
	if (item)
	{
		itemSprite = Sprite::create("Item.png");
	}
	else { itemSprite = Sprite::create(); }
	
	
	itemSprite->setScale((3*groundTargetSize.width) / (4*itemSprite->getContentSize().width));
	itemSprite->setPosition(Point(x, y/2+3*groundTargetSize.width/4));
	layer->addChild(itemSprite, 2);
	auto moveup = MoveBy::create(0.4, Vec2(0, y/2));
	itemSprite->setOpacity(0);
	itemSprite->runAction(Spawn::createWithTwoActions(EaseIn::create(moveup->clone(), 0.4), FadeIn::create(0.4)));
	ground->runAction(Spawn::createWithTwoActions(EaseIn::create(moveup->clone(),0.4),FadeIn::create(0.4)));
	//layer->scheduleOnce(schedule_selector(Ground::Destroy), lifetime);
}
Sprite* Ground::getSprite()
{
	return ground;
}
Sprite* Ground::getItemSprite()
{
	return itemSprite;
}
void Ground::setItemStatus(bool status)
{
	itemStatus = status;
}
bool Ground::isHavingItem()
{
	return itemStatus;
}
void Ground::Destroy()
{
	this->getSprite()->stopAllActions();
	MoveBy* move = MoveBy::create(1.5, Vec2(0, -visibleSize.height));
	auto moveeasein = EaseOut::create(move->clone(), 0.5);
	auto destroyaction = Sequence::create(moveeasein, RemoveSelf::create(true),nullptr);
	auto fadeaction = FadeOut::create(0.5);
	this->getItemSprite()->runAction(Spawn::createWithTwoActions(destroyaction->clone(), Sequence::create(DelayTime::create(0.4), fadeaction->clone(), nullptr)));
	this->getSprite()->runAction(Spawn::createWithTwoActions(destroyaction,Sequence::create(DelayTime::create(0.4),fadeaction,nullptr)));

}
