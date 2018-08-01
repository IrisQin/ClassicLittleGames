#include "Simple2D.h"
#include "GameManager.h"
#include "Helper.h"
#include <iostream>
#include <vector> 
#include <type_traits>
using namespace Simple2D;

class SceneManager {
private:
	SceneManager() {
		font = CreateFont(string("../../Content/Fonts/AGENCYB.TTF"), 80);
		string backGFile = "../../Content/Textures/StarLayer.png";
		backGround = new Sprite(0, 0, backGFile, 1.f);	// here to adjust initial game speed
		initSpeed = backGround->speed;
		windowWidth = backGround->getWidth();
		windowHeight = backGround->getHeight();
		rightBoundary = windowWidth;
		string menuBgFile = "../../Content/Textures/Moon.png";
		menuBg = new Sprite(0, 0, menuBgFile);
		menuBg->setPosition(backGround);
		
	};
	~SceneManager() {
		DestroyFont(font);
		destroy(gameManager);
		destroy(backGround);
		destroy(menuBg);
	};

	
	GameManager* gameManager;

	enum Status
	{
		InMenu = 1,
		Ready = 2,
		InGame = 3,
		Die = 4,
		Win = 5,
	};

	void drawScore() {
		DrawString(font, to_string((int)GameManager::score), 20, windowHeight - 70, 0, 0.5);
	};

	void wait2Menu() {
		if (GetGameTime() - dieTime > 2) {
			destroy(gameManager);
			status = InMenu;
			backGround->setPosition(0, 0);
		}
	};

public:
	// singleton
	static SceneManager& getInstance() {
		static SceneManager instance;
		return instance;
	};

	int windowWidth, windowHeight;
	int rightBoundary;
	float initSpeed;
	float dieTime = 0;
	Sprite* backGround;
	Sprite* menuBg;
	Font* font;

	Status status = InMenu;
	// so that other class can set status
	void setStatus(int num) {
		status = Status(num);
	};

	void update() {
		switch (status) {
		case(InMenu):
			menuBg->draw();
			backGround->draw();
			DrawString(font, string("Space Shooter"), windowWidth/2, windowHeight/2);
			DrawString(font, string("Press Space"), windowWidth / 2, windowHeight / 2-200,0,0.5);
			if (IsKeyPressed(KEY_SPACE)) status = Ready;
			break;
		case(Ready):
			gameManager = new GameManager(); 
			status = InGame; 
			break;
		case(InGame):
			gameManager->update();
			drawScore();
			break;
		case(Die):
			gameManager->update();
			drawScore();
			DrawString(font, string("Fail"), windowWidth / 2, windowHeight / 2);
			wait2Menu();
			break;
		case(Win):
			gameManager->update();
			drawScore();
			DrawString(font, string("Win"), windowWidth / 2, windowHeight / 2);
			wait2Menu();
			break;
		default:
			cout << "status error" << endl; break;
		}
	}


};