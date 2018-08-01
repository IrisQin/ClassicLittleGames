#pragma once
#include "Sprite.h"
#include "Simple2D.h"
#include "Helper.h"
#include <vector>
using namespace std;

class Spaceship: public Sprite
{
protected:
	vector<string> imageFiles;
	int level = 0;
	float shootCD = 0.2f;
	float shootTimer = 0;
	float lowBulletSpeed = 0;
	float highBulletSpeed = 0;
public:
	Spaceship(float _x, float _y, float _speed, int _level = 0, int _life = 1);
	virtual ~Spaceship() {};
	virtual void shoot()=0;
	virtual bool beCollided()=0;
	
};


class PlayerShip : public Spaceship {
public:
	PlayerShip(float _x, float _y, float _speed, int _level = 0, int _life = 1);
	void shoot();
	// game fails here
	bool beCollided();
	// when player collide with "upgrade" actively, its HP remains
	bool collide(Sprite* s);

};

class EnemyShip : public Spaceship {
public:
	EnemyShip(float _x, float _y, int _level = 0);
	float birthTime;
	void shoot();
	// game wins here
	bool collide(Sprite* s);
	// game wins here
	bool beCollided();
};