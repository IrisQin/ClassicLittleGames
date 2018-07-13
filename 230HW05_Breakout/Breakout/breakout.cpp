
#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <iostream>
using namespace sf;


Texture t1, t2, t3, t4,t5,t6;
int brickCount = 0;

class Paddle : public Sprite {
public:
	float speed = 0.f;
	Paddle(Texture t) :Sprite(t) {
	};
	~Paddle();
};

class Ball : public Sprite {
public:
	float speed = 0.5f;
	float angle = 0.f; 
	Ball(Texture t) :Sprite(t) {
	};
	~Ball();
};

class Brick : public Sprite {
public:
	Brick();
	~Brick();
};
Brick block[1000];

void level1() {
	brickCount = 0;
	for (int i = 1; i <= 2; i++)
		for (int j = 1; j <= 2; j++)
		{
			block[brickCount].setTexture(t1);
			block[brickCount].setPosition(i * 86, j * 40);
			brickCount++;
		}
}

void level2() {
	brickCount = 0;
	for (int i = 1; i <= 2; i++)
		for (int j = 1; j <= 2; j++)
		{
			if (i == 1 && j == 1) {
				block[brickCount].setTexture(t5);
			}
			else {
				block[brickCount].setTexture(t1);
				
			}
			
			block[brickCount].setPosition(i * 86+100, j * 20);
			brickCount++;
		}
}

void level3() {
	brickCount = 0;
	for (int i = 1; i <= 3; i++)
		for (int j = 1; j <= 3; j++)
		{
			block[brickCount].setTexture(t6);
			block[brickCount].setPosition(i * 100, j * 30);
			brickCount++;
		}
}

int main()
{
	int life = 3;

	std::srand(static_cast<unsigned int>(std::time(NULL)));
	RenderWindow app(VideoMode(520, 450), "Breakout");
	app.setFramerateLimit(60);

	
	t1.loadFromFile("resources/block06.png");
	t2.loadFromFile("resources/Xmas.jpg");
	t3.loadFromFile("resources/ball.png");
	t4.loadFromFile("resources/paddle.png");
	t5.loadFromFile("resources/block02.png");
	t6.loadFromFile("resources/block07.png");
	sf::SoundBuffer ballSoundBuffer1;
	sf::SoundBuffer ballSoundBuffer2;
	sf::SoundBuffer ballSoundBuffer3;
	sf::SoundBuffer ballSoundBuffer4;
	sf::SoundBuffer ballSoundBuffer5;
	sf::SoundBuffer ballSoundBuffer6;
	sf::SoundBuffer ballSoundBuffer7;
	if (!ballSoundBuffer1.loadFromFile("resources/ball_lost.wav"))
			return EXIT_FAILURE;
		sf::Sound ball_lost(ballSoundBuffer1);
	if (!ballSoundBuffer2.loadFromFile("resources/break.wav"))
			return EXIT_FAILURE;
		sf::Sound breakS(ballSoundBuffer2);
	if (!ballSoundBuffer3.loadFromFile("resources/gameover.wav"))
			return EXIT_FAILURE;
		sf::Sound gameover(ballSoundBuffer3);
	if (!ballSoundBuffer4.loadFromFile("resources/level_complete.wav"))
			return EXIT_FAILURE;
		sf::Sound level_complete(ballSoundBuffer4);
	if (!ballSoundBuffer5.loadFromFile("resources/paddle_bounce.wav"))
			return EXIT_FAILURE;
		sf::Sound paddle_bounce(ballSoundBuffer5);
	if (!ballSoundBuffer6.loadFromFile("resources/shatter.wav"))
			return EXIT_FAILURE;
		sf::Sound shatter(ballSoundBuffer6);
	if (!ballSoundBuffer7.loadFromFile("resources/wall_bounce.wav"))
			return EXIT_FAILURE;
		sf::Sound wall_bounce(ballSoundBuffer7);

	Load the text font
	sf::Font font;
	if (!font.loadFromFile("resources/arial.ttf"))
		return EXIT_FAILURE;
	sf::Text lifeMessage;
	lifeMessage.setFont(font);
	lifeMessage.setCharacterSize(30);
	lifeMessage.setPosition(50.f, 350.f);
	lifeMessage.setFillColor(sf::Color::Black);
	sf::Text overMessage;
	overMessage.setFont(font);
	overMessage.setCharacterSize(30);
	overMessage.setPosition(200.f, 300.f);
	overMessage.setFillColor(sf::Color::Red);
	sf::Text scoreMessage;
	scoreMessage.setFont(font);
	scoreMessage.setCharacterSize(30);
	scoreMessage.setPosition(380.f, 350.f);
	scoreMessage.setFillColor(sf::Color::Black);
	scoreMessage.setString("Score:0");


	Sprite sBackground(t2);
	Ball sBall(t3);
	sBall.setTexture(t3);
	Paddle sPaddle(t4);
	sPaddle.setTexture(t4);
	sPaddle.setPosition(300, 440);
	
	
	float dx, dy;
	float x = 340, y = 428;
	sBall.setPosition(x, y);
	bool launch = false;
	int score = 0;
	bool once = false;
	int level = 1;
	int moveAroundHeight = 0;

	level1();

	while (app.isOpen())
	{

		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Space) && launch == false && life > 0) {
			launch = true; 
			dx = -(std::rand() % 13 + 1) + 7;
			dy = -6.5f;
		}
		if (life <= 0 && once == false) {
			overMessage.setString("GAME OVER");
			gameover.play();
			once = true;
		}
		if (launch == true) {
			if (level == 3 ) {
				if (moveAroundHeight++ < 10) for (int i = 0; i < 100; i++) block[i].move(0, 1);
				if (moveAroundHeight++ >= 10)for (int i = 0; i < 100; i++) block[i].move(0, -1);
				if (moveAroundHeight == 20) moveAroundHeight = 0;
					
			}
			if (brickCount == 0 && level == 1) {
				sBall.speed += 0.2f;
				level_complete.play();
				level2();
				std::cout << brickCount;
				level = 2;
			}
			if (brickCount == 1 && level == 2) {
				sBall.speed += 0.2f;
				level_complete.play();
				level3();
				level = 3;
			}
			if (brickCount == 0 && level == 3) {
				sBall.speed += 0.2f;
				level_complete.play();
				level1();
				level = 1;
			}

			x += dx*sBall.speed;
			for (int i = 0; i < 100; i++) 
				if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
				{
					if (level == 2 && i == 0) continue;
					block[i].setPosition(-100, 0);
					brickCount--;
					std::cout << brickCount;
					score++;
					scoreMessage.setString("Score:" + std::to_string(score));
					dx = -dx;
					breakS.play();
				}

			y += dy*sBall.speed;
			for (int i = 0; i<100; i++)
				if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
				{
					if (level == 2 && i == 0) continue;
					block[i].setPosition(-100, 0);
					brickCount--;
					std::cout << brickCount;
					score++;
					scoreMessage.setString("Score:" + std::to_string(score));
					dy = -dy;
					breakS.play();
				}

			if (x < 0 || x>520) {
				dx = -dx; wall_bounce.play();
			}
			if (y<0 )  dy = -dy;
			if (y > 450) {
				x = sPaddle.getGlobalBounds().left + sPaddle.getGlobalBounds().width / 2.f - 6.f;
				y = 428.f;
				life--;
				launch = false;
				ball_lost.play();
				
			}
		}
		

		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			if (sPaddle.getGlobalBounds().left + sPaddle.getGlobalBounds().width * 2 >= 520) {
				sPaddle.setPosition(430, 440);
				if (launch == false) {
					x = 469;
				}
			}
			else {
				sPaddle.move(6, 0);
				if (launch == false) {
					x += 6;
				}
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			if (sPaddle.getGlobalBounds().left <= sPaddle.getGlobalBounds().width) {
				sPaddle.setPosition(0, 440);
				if (launch == false) {
					x = 39;
				}
			}
			else {
				sPaddle.move(-6, 0);
				if (launch == false) {
					x -= 6;
				}
			} 
		}

		dx 4 dy -3
		if (FloatRect(x, y, 12, 12).intersects(sPaddle.getGlobalBounds())) {
			dx =  ((x + 6.f) - (sPaddle.getGlobalBounds().left + 45.f) ) / 78.f * 12.f;
			dy = -dy;
			paddle_bounce.play();
		} 
		dx -4~4

		sBall.setPosition(x, y);


		lifeMessage.setString("Lives:" + std::to_string(life - 1));
		app.clear();
		app.draw(sBackground);
		app.draw(sBall);
		app.draw(sPaddle);
		app.draw(lifeMessage);
		app.draw(overMessage);
		app.draw(scoreMessage);
		for (int i = 0; i<100; i++) app.draw(block[i]);
		app.display();
	}

	return 0;
}

Ball::~Ball()
{
}
Paddle::~Paddle()
{
}
Brick::Brick()
{
}
Brick::~Brick()
{
}
