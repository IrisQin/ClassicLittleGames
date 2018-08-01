#include "Simple2D.h"
#include "GameManager.h"
#include "SceneManager.cpp"
#include "Helper.h"
#include "Sprite.h"
#include <iostream>
#include <vector> 
#include <type_traits>


using namespace Simple2D;

float GameManager::speed;
float GameManager::score;
list<Sprite*> GameManager::playerBullets;
list<Sprite*> GameManager::enemyBullets;
list<Explosion*>  GameManager::explosions;

GameManager::GameManager():isKeyDown(170, false) {
	score = 0;
	startTime = GetGameTime();
	string backGFile = "../../Content/Textures/StarLayer.png";
	speed = SceneManager::getInstance().initSpeed;
	backGroundCopy = new Sprite((float)SceneManager::getInstance().windowWidth, 0, backGFile, speed);
	windowBoundary = new Sprite(0, 0, SceneManager::getInstance().rightBoundary, SceneManager::getInstance().windowHeight);
	dieBoundary = new Sprite(0, 0, SceneManager::getInstance().rightBoundary + 500, SceneManager::getInstance().windowHeight);
	int playerLife = 3;
	playerShips.push_back(new PlayerShip(0.f, 100.f, 7.f,0, playerLife));
	enemyManager = new EnemyManager();
	string heartFile = "../../Content/Textures/heart.png";
	for (int i = 0; i < playerLife; i++) {
		hearts.push_back(new Sprite(10+50*i, SceneManager::getInstance().windowHeight-50,heartFile));
	}
}

GameManager::~GameManager() {
	destroy(windowBoundary);
	destroy(dieBoundary);
	destroy(backGroundCopy);
	destroy(enemyManager);
	
	for (auto s : playerShips) destroy(s);
	playerShips.clear();
	for (auto s : playerBullets ) destroy(s);
	playerBullets.clear();
	for (auto s : enemyBullets) destroy(s);
	enemyBullets.clear();
	for (auto s : explosions) destroy(s);
	explosions.clear();
	for (auto s : upgrades) destroy(s);
	upgrades.clear();
	for (auto s : hearts) destroy(s);
	hearts.clear();

 }

void GameManager::update() {

	speed += 0.00012;
	moveBg();
	input();	

	// deal with object spawn
	float pastTime = GetGameTime() - startTime;
	
	if (pastTime > 40) {
		// generate boss
		if (enemyManager->boss == nullptr) {
			enemyManager->boss = new EnemyShip(0, 0, 3);
			enemyManager->boss->life = 12;
			enemyManager->boss->setPosition(windowBoundary->getWidth() + 200, windowBoundary->getHeight() / 2 - enemyManager->boss->getHeight() / 2);
			enemyManager->boss->scale = 0.5f;
			enemyManager->enemyShips.push_back(enemyManager->boss);
		}
		enemyManager->spawnEnemyRandom(8);
		enemyManager->spawnArrowForm(8, 1);
		enemyManager->spawnRectForm(2, 2, 10, 0);
	}
	else if (pastTime > 25) {
		enemyManager->spawnEnemyRandom(5);
		enemyManager->spawnArrowForm(10, 1);
	}
	else if (pastTime > 10) {
		enemyManager->spawnEnemyRandom(5);
		enemyManager->spawnRectForm(3, 3, 8, 0);
	}
	else if (pastTime > 2) {
		enemyManager->spawnEnemyRandom(5);
	}
	upgradeSpawn(20);
	enemyManager->spawnMeteorRandom(5);
	enemyManager->update();
	

	// deal with collision
	collision(&playerBullets,&enemyManager->enemyShips);
	collision(&enemyBullets, &playerShips);
	collision(&enemyManager->enemyShips, &playerShips);
	collision(&playerShips, &upgrades);
	collision(&playerBullets, &enemyManager->meteors);
	collision(&enemyManager->meteors, &playerShips);
	collision(&enemyManager->meteors, &enemyManager->enemyShips);

	
	explode();
  	if (playerShips.front()->life > 0) {
		playerShips.front()->draw();
	}


	// death, move, delete outside, draw
	moveDieDraw(&playerBullets);
	moveDieDraw(&enemyBullets);
	moveDieDraw(&enemyManager->enemyShips);
	moveDieDraw(&enemyManager->meteors);
	moveDieDraw(&upgrades);
}

template<class T1, class T2>
void GameManager::collision(list<T1*>* active, list<T2*>* passive) {
	for (auto i : *active) 
		for (auto j : *passive)
		{
			Sprite* is = i, *js = j;
			if (i->life && j->life && i->intersect(j) && i->intersect(windowBoundary) && j->intersect(windowBoundary))	// collision only happens inside window! 
				i->collide(j);
		}
}

template<class T>
void GameManager::moveDieDraw(list<T*>* i) {
	for (auto s = (*i).begin(); s != (*i).end(); ) {
		Sprite* curr = *s;
		curr->move();
		if (!curr->life || !curr->intersect(dieBoundary)) {		// die automatically outside dieBoundary
			s = (*i).erase(s);
			destroy(curr);
		}
		else {
			s++;
			curr->draw();
		}
	}
}

void GameManager::explode() {
	float currTime = GetGameTime();
	for (auto s = explosions.begin(); s != explosions.end(); ) {
		Explosion* curr = *s;
		float pastTime = currTime - curr->birthTime;
		if (pastTime >0.1f) {
			s = explosions.erase(s);
			destroy(curr);
		}
		else {
			s++;
			if (pastTime > 0.05f && curr->stage ==1) 
				curr->change2next();
			curr->draw();
		}
	}
}

void GameManager:: moveBg() {
	
	Sprite* backGround = SceneManager::getInstance().backGround;
	backGround->speed = speed;
	backGround->move();
	backGroundCopy->speed = speed;
	backGroundCopy->move();
	int width = backGround->getWidth();
	if (backGround->getX() < -width + 20) backGround->setPosition((float)width, 0);
	if (backGroundCopy->getX() < -width + 20) backGroundCopy->setPosition((float)width, 0);
	backGround->draw();
	backGroundCopy->draw();
	
	if (hearts.empty())return;
	else if (hearts.size() > playerShips.front()->life) {
		while (hearts.size() != playerShips.front()->life) {
			destroy(hearts.back());
			hearts.pop_back();
		}
	}
	else if (hearts.size() < playerShips.front()->life) {
		while (hearts.size() != playerShips.front()->life) {
			string heartFile = "../../Content/Textures/heart.png";
			hearts.push_back(new Sprite(10 + 50 * hearts.size(), SceneManager::getInstance().windowHeight - 50, heartFile));
		}
	}
	for (auto s : hearts) s->draw();
}

void GameManager::upgradeSpawn(float cd) {
	if (Simple2D::GetGameTime() - upgradeSpawnTimer > cd) {
		upgradeSpawnTimer = Simple2D::GetGameTime();
		if (randomInt(0, 2)) {
			string file = "../../Content/Textures/Upgrade.png";
			Sprite* curr = new Sprite(SceneManager::getInstance().rightBoundary, (float)randomInt(0, SceneManager::getInstance().windowHeight - 50), file, speed);
			upgrades.push_back(curr);
		}
		else {
			string file = "../../Content/Textures/UpgradeLife.png";
			Sprite* curr = new Sprite(SceneManager::getInstance().rightBoundary, (float)randomInt(0, SceneManager::getInstance().windowHeight - 50), file, speed);
			curr->rotation = 90;
			upgrades.push_back(curr);
		}
		
	}

}

void GameManager::input() {
	PlayerShip* playerShip = playerShips.front();
	if (IsKeyPressed(KEY_SPACE) || isKeyDown[KEY_SPACE]) {
		playerShip->shoot();
		isKeyDown[KEY_SPACE] = true;
	}
	if (IsKeyPressed(KEY_UP_ARROW) || isKeyDown[KEY_UP_ARROW]) {
		playerShip->dirX = 0; playerShip->dirY = 1;
		playerShip->move(windowBoundary);
		isKeyDown[KEY_UP_ARROW] = true;
	}
	if (IsKeyPressed(KEY_DOWN_ARROW) || isKeyDown[KEY_DOWN_ARROW]) {
		playerShip->dirX = 0; playerShip->dirY = -1;
		playerShip->move(windowBoundary);
		isKeyDown[KEY_DOWN_ARROW] = true;
	}
	if (IsKeyPressed(KEY_LEFT_ARROW) || isKeyDown[KEY_LEFT_ARROW]) {
		playerShip->dirX = -1; playerShip->dirY = 0;
		playerShip->move(windowBoundary);
		isKeyDown[KEY_LEFT_ARROW] = true;
	}
	if (IsKeyPressed(KEY_RIGHT_ARROW) || isKeyDown[KEY_RIGHT_ARROW]) {
		playerShip->dirX = 1; playerShip->dirY = 0;
		playerShip->move(windowBoundary);
		isKeyDown[KEY_RIGHT_ARROW] = true;
	}
	if (IsKeyReleased(KEY_SPACE)) {
		isKeyDown[KEY_SPACE] = false;
	}
	if (IsKeyReleased(KEY_UP_ARROW)) {
		isKeyDown[KEY_UP_ARROW] = false;
	}
	if (IsKeyReleased(KEY_DOWN_ARROW)) {
		isKeyDown[KEY_DOWN_ARROW] = false;
	}
	if (IsKeyReleased(KEY_LEFT_ARROW)) {
		isKeyDown[KEY_LEFT_ARROW] = false;
	}
	if (IsKeyReleased(KEY_RIGHT_ARROW)) {
		isKeyDown[KEY_RIGHT_ARROW] = false;
	}
}
