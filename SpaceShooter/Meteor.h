#pragma once
#include "Sprite.h"
#include "Simple2D.h"
#include "Helper.h"
#include <iostream>
using namespace Simple2D;

class Meteor : public Sprite {

public:
	Meteor(float _x, float _y, int _type = 0);
	int type = 0;
	float birthTime;
	vector<string> imageFiles;
	bool beCollided();
	bool collide(Sprite* s);

};