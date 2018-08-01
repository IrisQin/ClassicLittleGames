#include "Simple2D.h"
#include "EnemyManager.h"
#include "SceneManager.cpp";
#include "Helper.h"
#include <iostream>
#include <vector>
using namespace std;

EnemyManager::EnemyManager() {
	spawnX = SceneManager::getInstance().rightBoundary;
	spawnYMax = SceneManager::getInstance().windowHeight;
}

EnemyManager::~EnemyManager() {

	for (auto s : enemyShips) destroy(s);
	for (auto s : meteors) destroy(s);
}

void EnemyManager::update() {
	
	for (auto s : enemyShips ) {
		float pastTime = Simple2D::GetGameTime()-s->birthTime;
		if (pastTime > 1) { 
			s->shoot();
		}
	}

	for (auto s : meteors) {
		float pastTime = Simple2D::GetGameTime() - s->birthTime;
		if (s->type == 0) {
			s->rotation++;
		}
	}

	if (boss != nullptr) {
		float pastTime = Simple2D::GetGameTime() - boss->birthTime;
		if (((int)pastTime) % 2 == 0) {
			boss->scale += 0.03;
		}
		else {
			boss->scale -= 0.03;
		}
		if (((int)pastTime) % 6 < 3) {
			boss->dirX = -1.2;
		}
		else {
			boss->dirX = 1.2;
		}

	}
}


void EnemyManager::spawnEnemyRandom(float enemyCD) {
	if (Simple2D::GetGameTime() - spawnEnemyRandomTimer > enemyCD) {
		spawnEnemyRandomTimer = Simple2D::GetGameTime();

		EnemyShip* curr;
		int chance = randomInt(0, 100);
		if (chance > 80) {
			curr = new EnemyShip(spawnX, (float)randomInt(0, spawnYMax - 100.f), 2);
		}
		else if (chance > 40) {
			curr = new EnemyShip(spawnX, (float)randomInt(0, spawnYMax - 100.f), 1);
		}
		else {
			curr = new EnemyShip(spawnX, (float)randomInt(0, spawnYMax - 100.f), 0);
		}
		enemyShips.push_back(curr);

	}

}

void EnemyManager::spawnRectForm(int row, int column, float cd, int level){
	if (Simple2D::GetGameTime() - spawnRectFormTimer > cd) {
		spawnRectFormTimer = Simple2D::GetGameTime();
		float gap = 60.f;
		float y = randomInt(0, spawnYMax - gap*row);
		for (int i = 0; i < row; i++) 
			for (int j = 0; j < column; j++) {
				enemyShips.push_back(new EnemyShip(spawnX+ gap*j, y+i*gap, level));
			}
	}
}

void EnemyManager::spawnArrowForm(float cd, int level) {
	if (Simple2D::GetGameTime() - spawnArrowFormTimer > cd) {
		spawnArrowFormTimer = Simple2D::GetGameTime();
		float gap = 40.f;
		float y = randomInt(0, spawnYMax - gap*3);
		enemyShips.push_back(new EnemyShip(spawnX, y, level));
		enemyShips.push_back(new EnemyShip(spawnX+gap, y+gap, level));
		enemyShips.push_back(new EnemyShip(spawnX+ gap, y-gap, level));
		enemyShips.push_back(new EnemyShip(spawnX + gap*2, y + gap*2, level));
		enemyShips.push_back(new EnemyShip(spawnX + gap*2, y - gap*2, level));
	}


}


void EnemyManager::spawnMeteorRandom(float cd) {
	if (Simple2D::GetGameTime() - spawnMeteorRandomTimer > cd) {
		spawnMeteorRandomTimer = Simple2D::GetGameTime();
		int type = randomInt(0, 3);
		switch (type) {
			Meteor* m;
			float y;
		case 0:
			m = new Meteor(spawnX, (float)randomInt(0, spawnYMax), 0);
			m->setImage(m->imageFiles[type]);
			meteors.push_back(m);
			break;
		case 1:
			y = randomInt(0, spawnYMax - 240.f);
			for (int i = 0; i < 2; i++)
				for (int j = 0; j < 2;j++) {
				m = new Meteor(spawnX+j*60, y+i*60, 1);
				m->setImage(m->imageFiles[type]);
				meteors.push_back(m);
			}
			break;
		case 2:
			y = - (float)randomInt(2, 12)/10.f; 
			m = new Meteor(spawnX, (float)randomInt(spawnYMax*3/4, spawnYMax), 2);
			m->dirY = y;
			m->speed += 5;
			m->setImage(m->imageFiles[type]);
			meteors.push_back(m);
			break;
		case 3:
			m = new Meteor(spawnX, (float)randomInt(0, spawnYMax), 3);
			m->life = 3;
			m->setImage(m->imageFiles[type]);
			meteors.push_back(m);
			break;
		default:cout << "type error" << endl; break;
		}
	
	}


}
