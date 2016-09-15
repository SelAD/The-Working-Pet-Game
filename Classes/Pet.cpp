#include "Pet.h"
#include "Definitions.h"
USING_NS_CC;

Pet::Pet(Layer* layer)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	std::string petFilePNG = "ConTho.png"; //Die PNG-Datei
	std::string petFilePLIST = "TheWorkingPet.plist"; //Die PLIST-Datei
	std::string petFileEJSON = "TheWorkingPet.ExportJson"; //Die ExportJson-Datei
	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo(petFilePNG, petFilePLIST, petFileEJSON);
	Player = cocostudio::Armature::create("TheWorkingPet");
	Size PlayerTargetSize = Size(visibleSize.width / PART_AMOUNT, visibleSize.height / PART_AMOUNT);
	Player->setScale(1.2*PlayerTargetSize.width/(Player->getContentSize().width));
	layer->addChild(Player, 3);
}
void Pet::ChangePet(int petIndex)
{
	//auto nuttest = Sprite::create("PlayButton.png"); Day la cac ham thay sprite cho bone
	//armature->getBone("ConTho")->addDisplay(nuttest, 1);
	//armature->getBone("ConTho")->changeDisplayWithIndex(1, true);
}
void Pet::ChangeAnim(int animIndex)
{
	//code thay animation
}
cocostudio::Armature* Pet::getPet()
{
	return Player;
}