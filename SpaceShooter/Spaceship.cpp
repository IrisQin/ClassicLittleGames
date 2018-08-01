#include "Sprite.h"
#include "Spaceship.h"
#include "Simple2D.h"
#include "GameManager.h"
#include "SceneManager.cpp"
#include "Helper.h"
#include <iostream>
using namespace Simple2D;

Spaceship::Spaceship(float _x, float _y, float _speed, int _level, int _life) : Sprite(_x,_y,_speed, _life){
	level = _level;
}


PlayerShip::PlayerShip(float _x, float _y, float _speed, int _level, int _life) :Spaceship(_x, _y, _speed,_level,_life) {
	imageFiles.push_back("../../Content/Textures/Player_1.png");
	imageFiles.push_back("../../Content/Textures/Player_2.png");
	imageFiles.push_back("../../Content/Textures/Player_3.png");
	setImage(imageFiles[_level]);
	dirX = 1.f;
	lowBulletSpeed = 6.f;
	highBulletSpeed = 20.f;
}

void PlayerShip::shoot() {
	if (GetGameTime() - shootTimer > shootCD) {
		shootTimer = GetGameTime();
		
		string file = "../../Content/Textures/PlayerBullet.png";
		Sprite* bullet = new Sprite(0, 0, file, lowBulletSpeed);
		bullet->dirX = 1; bullet->dirY = 0;
		GameManager::playerBullets.push_back(bullet);

		float bulletX = x + width;
		float bulletY = y + height / 2 - bullet->getHeight() / 2;
		bullet->setPosition(bulletX, bulletY);
		if (level == 1) {
			bullet->speed = highBulletSpeed;
		}
		else if (level == 2) {
			bullet->speed = highBulletSpeed;
			bullet->setPosition(bulletX, y + height / 2 - bullet->getHeight() / 2 - 7);
			Sprite* bullet2 = new Sprite(bulletX, y + height / 2 + 7, file, highBulletSpeed);
			bullet2->dirX = 1; bullet2->dirY = 0;
			GameManager::playerBullets.push_back(bullet2);
		}
	}
}

bool PlayerShip::beCollided() {
	life--;
	if (life == 0) {
		// create explosion
		string file = "../../Content/Textures/Explosion_1.png";
		Explosion* e = new Explosion(0, 0, file);
		e->setPosition(this);
		GameManager::explosions.push_back(e);
		// means GAME OVER
		SceneManager::getInstance().setStatus(4);//Die
		SceneManager::getInstance().dieTime = GetGameTime();
	}
	else {
		if (level>0) setImage(imageFiles[--level]);
	}
	return life;
};

bool PlayerShip::collide(Sprite* s) {
	// NEED UPDATE: check sprite's type
	s->beCollided();
	if (s->rotation == 0) {
		if (level<2) setImage(imageFiles[++level]);
	}
	else {
		life++;
	}
	return life;
}

EnemyShip::EnemyShip(float _x, float _y,int _level) :Spaceship(_x, _y, GameManager::speed+2.f, _level) {
	imageFiles.push_back("../../Content/Textures/Enemy_1.png");
	imageFiles.push_back("../../Content/Textures/Enemy_2.png");
	imageFiles.push_back("../../Content/Textures/Enemy_3.png");
	imageFiles.push_back("../../Content/Textures/Enemy_4.png");
	setImage(imageFiles[_level]);
	birthTime = GetGameTime();
	life = level + 1;
	shootCD = 2.3f;
	lowBulletSpeed = 7.f;
	highBulletSpeed = 15.f;
}

void EnemyShip::shoot() {
	if (GetGameTime() - shootTimer > shootCD) {
		shootTimer = GetGameTime();

		string file = "../../Content/Textures/EnemyBullet.png";
		Sprite* bullet = new Sprite(0, 0, file, lowBulletSpeed);
		bullet->rotation = 180.f;
		GameManager::enemyBullets.push_back(bullet);

		float bulletX = x - bullet->getWidth();
		float bulletY = y + height / 2 - bullet->getHeight() / 2;
		bullet->setPosition(bulletX, bulletY);
		if (level == 1) {
			bullet->speed = highBulletSpeed;
		}
		else if (level >= 2) {
			bullet->speed = highBulletSpeed;
			bullet->setPosition(bulletX, y + height / 2 - bullet->getHeight() / 2 - 3);
			Sprite* bullet2 = new Sprite(bulletX, y + height / 2 + 3, file, highBulletSpeed);
			bullet2->rotation = 180.f;
			GameManager::enemyBullets.push_back(bullet2);
		}
	}
}

bool EnemyShip::collide(Sprite* s) { 
	s->beCollided(); 
	life--;
	if (life == 0) {
		GameManager::score++;
		// create explosion
		string file = "../../Content/Textures/Explosion_1.png";
		Explosion* e = new Explosion(0, 0, file);
		e->setPosition(this);
		GameManager::explosions.push_back(e);

		if (level == 3 && SceneManager::getInstance().status == 3) {	//InGame
			// means WIN
			e->scale = 5;
			SceneManager::getInstance().setStatus(5);//Win
			SceneManager::getInstance().dieTime = GetGameTime();
		}
	}
	return life; 
}

bool EnemyShip::beCollided() {

	life--;
	if (life == 0) {
		GameManager::score++;
		string file = "../../Content/Textures/Explosion_1.png";
		Explosion* e = new Explosion(0, 0, file);
		e->setPosition(this);
		GameManager::explosions.push_back(e);

		if (level == 3 && SceneManager::getInstance().status == 3) {	
			e->scale = 5;
			SceneManager::getInstance().setStatus(5);
			SceneManager::getInstance().dieTime = GetGameTime();
		}
	}
	return life;

}