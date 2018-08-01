#include "Sprite.h"
#include "Simple2D.h"
#include "Helper.h"
#include <iostream>
using namespace Simple2D;


class Explosion : public Sprite {
private:
	string file1 = "../../Content/Textures/Explosion_1.png";
	string file2 = "../../Content/Textures/Explosion_2.png";
	
public:
	Explosion(float _x, float _y, string _file): Sprite(_x,_y, _file) {
	
	};
	float birthTime = GetGameTime();
	int stage = 1;
	void change2next() {
		setImage(file2);
		stage = 2;
	};
};

