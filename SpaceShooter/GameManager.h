#pragma once
#include "Simple2D.h"
#include "EnemyManager.h"
#include "Explosion.cpp"
#include "Helper.h"
#include <vector>
#include <list>
using namespace std;

class GameManager {
private:
	
	float startTime;
	float upgradeSpawnTimer = 0;
	vector<bool> isKeyDown;

	Sprite* backGroundCopy;
	list<Sprite*> hearts;
	Sprite* windowBoundary;
	Sprite* dieBoundary;
	list<PlayerShip*> playerShips;
	list<Sprite*> upgrades;
	EnemyManager *enemyManager;

	// support long press
	void input();
	// call active's "collide()", call passive's "beCollided()"
	template<class T1, class T2>
	void collision(list<T1*>* active, list<T2*>* passive);
	// update objects' movement, death, draw
	template<class T>
	void moveDieDraw(list<T*>* i);
	// update explosion
	void explode();
	// spawn upgrades
	void upgradeSpawn(float cd);
	// move background in loop by using a copy
	void moveBg();

public:
	GameManager();
	~GameManager();
	
	static float score;
	static float speed;
	static list<Sprite*> playerBullets;
	static list<Sprite*> enemyBullets;
	static list<Explosion*> explosions;
	// control all kinds of objects' behaviors according to game stage
	void update();
	
};