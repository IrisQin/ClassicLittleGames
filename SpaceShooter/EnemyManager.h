#pragma once
#include "Simple2D.h"
#include "Spaceship.h"
#include "Meteor.h"
#include "Helper.h"
#include <vector>
#include <list>
using namespace std;


// responsible for enemy ships' and meteors' spawn and movement path during their lifetime
class EnemyManager {

private:
	// each function related to frequency has a saperate timer
	float spawnEnemyRandomTimer = 0, spawnMeteorRandomTimer = 0, spawnRectFormTimer = 0, spawnArrowFormTimer = 0, stretchTimer = 0;
	float spawnX, spawnYMax;

public:
	EnemyManager();
	~EnemyManager();
	list<EnemyShip*> enemyShips;
	list<Meteor*> meteors;
	EnemyShip* boss = nullptr;
	// more low-level enemies, less high-level
	void spawnEnemyRandom(float enemyCD);
	// meteor groups
	// 0: rotate 
	// 1: 2*2
	// 2: has y direction
	// 3: big, 2 life
	void spawnMeteorRandom(float cd);
	// rect formation
	void spawnRectForm(int row, int column, float cd, int level);
	// arrow formation
	void spawnArrowForm(float cd, int level);
	void update();

};