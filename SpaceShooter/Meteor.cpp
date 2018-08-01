#include "Meteor.h"
#include "GameManager.h"
#include <iostream>
using namespace Simple2D;

Meteor::Meteor(float _x, float _y, int _type) : Sprite(_x, _y, GameManager::speed, 1) {
	imageFiles.push_back("../../Content/Textures/Meteorite_1.png");
	imageFiles.push_back("../../Content/Textures/Meteorite_2.png");
	imageFiles.push_back("../../Content/Textures/Meteorite_3.png");
	imageFiles.push_back("../../Content/Textures/Meteorite_4.png");
	type = _type;
	birthTime = GetGameTime();
}

bool Meteor::beCollided() {
	GameManager::score++;
	return --life;

}
bool Meteor::collide(Sprite* s) {
	GameManager::score++;
	s->beCollided(); return --life;
}