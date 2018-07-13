#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
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

const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453f;

Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
int level = 1;
int score = 0;
int life = 3;
int screen = 1;//1:main menu 2:playing  3:game over
sf::Font font;
sf::Text lifeMessage;
sf::Text menu;
sf::Text over;
sf::Text scoreMessage;

class Animation
{
public:
	float Frame, speed;
	Sprite sprite;
	std::vector<IntRect> frames;

	Animation() {}

	Animation(Texture &t, int x, int y, int w, int h, int count, float Speed)
	{
		Frame = 0;
		speed = Speed;

		for (int i = 0; i<count; i++)
			frames.push_back(IntRect(x + i*w, y, w, h));

		sprite.setTexture(t);
		sprite.setOrigin(w / 2, h / 2);
		sprite.setTextureRect(frames[0]);
	}


	void update()
	{
		Frame += speed;
		int n = frames.size();
		if (Frame >= n) Frame -= n;
		if (n>0) sprite.setTextureRect(frames[int(Frame)]);
	}

	bool isEnd()
	{
		return Frame + speed >= frames.size();
	}

};
Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);

class Entity
{
public:
	float speed = 1;
	bool justCollide = false;
	float x, y, dx, dy, R, angle;
	bool life;
	std::string name;
	Animation anim;
	Texture textu;
	Sprite sprite;

	Entity()
	{
		life = 1;
	}

	void settings(Animation &a, int X, int Y, float Angle = 0, int radius = 1)
	{
		anim = a;
		x = X; y = Y;
		angle = Angle;
		R = radius;
		
	}
	void settingsT(Texture &t, int X, int Y, float Angle = 0, int radius = 1)
	{
		textu = t;
		x = X; y = Y;
		angle = Angle;
		R = radius;
	}

	virtual void update() {};

	void draw(RenderWindow &app)
	{
		anim.sprite.setPosition(x, y);
		anim.sprite.setRotation(angle + 90);
		app.draw(anim.sprite);

		CircleShape circle(R);
		circle.setFillColor(Color(255, 0, 0, 170));
		circle.setOrigin(R, R);
		circle.setPosition(x, y);
		//app.draw(circle);
	}
	
	void drawT(RenderWindow &app)
	{
		sprite.setTexture(textu);
		sprite.setPosition(x, y);
		sprite.setRotation(angle);// + 90
		app.draw(sprite);

		CircleShape circle(R);
		circle.setFillColor(Color(255, 0, 0, 170));
		circle.setPosition(x + R, y + R);
		circle.setOrigin(R, R);
		//app.draw(circle);
	}

	virtual ~Entity() {};
};
std::list<Entity*> entities;


class asteroid : public Entity
{
public:
	asteroid()
	{
		
		dx = float(rand() % 2000 - 1000) / (float)1000; //-1~1
		dy = float(rand() % 2000 - 1000) / (float)1000; //rand() % 1000 / (float)500 - 1;
		name = "asteroid";
	}

	void  update()
	{
		x += dx*speed;
		y += dy*speed;

		if (x>W) x = 0;  if (x<0) x = W;
		if (y>H) y = 0;  if (y<0) y = H;
	}

};



class bullet : public Entity
{
public:
	bullet()
	{
		name = "bullet";
	}

	void  update()
	{
		dx = cos(angle*DEGTORAD) * 6;
		dy = sin(angle*DEGTORAD) * 6;
		// angle+=rand()%6-3;
		x += dx;
		y += dy;

		if (x>W || x<0 || y>H || y<0) life = 0;
	}

};


class player : public Entity
{
public:
	bool thrust;

	player()
	{
		name = "player";
	}

	void update()
	{
		if (thrust)
		{
			dx += cos(angle*DEGTORAD)*0.2;
			dy += sin(angle*DEGTORAD)*0.2;
		}
		else
		{
			dx *= 0.99;
			dy *= 0.99;
		}

		int maxSpeed = 15;
		float speed = sqrt(dx*dx + dy*dy);
		if (speed>maxSpeed)
		{
			dx *= maxSpeed / speed;
			dy *= maxSpeed / speed;
		}

		x += dx;
		y += dy;

		if (x>W) x = 0; if (x<0) x = W;
		if (y>H) y = 0; if (y<0) y = H;
	}

};
player *p; 


bool isCollide(Entity *a, Entity *b)
{
	float ax = a->x;
	float ay = a->y;
	float bx = b->x;
	float by = b->y;
	if (a->name == "asteroid") {
		ax = a->x + a->R;
		ay = a->y + a->R;
	}
	if (b->name == "asteroid") {
		bx = b->x + b->R;
		by = b->y + b->R;
	}
	return (bx - ax)*(bx - ax) +
		(by - ay)*(by - ay)<
		(a->R + b->R)*(a->R + b->R);
}

void generateAsteroid(int num) {
	int asteroidCount = 0;
	for (int i = 0; asteroidCount<num; i++)
	{
		bool bad = false;
		asteroid *a = new asteroid();
		a->settingsT(t10, rand() % (W / 3) + (W / 3), rand() % (H / 3) + (H / 3), 0, 25);//rand() % 360
		a->speed = level;
		//std::cout << a->speed;
		for (auto b : entities)
			if (isCollide(a, b)) {
				bad = true;
			}
		if (!bad) {
			entities.push_back(a);
			asteroidCount++;
		}
	}
}

void initialSetting() {
	generateAsteroid(3 * level);
	p = new player();
	p->settings(sPlayer, W / 2, H / 2, 0, 20);
	p->dx = 0; p->dy = 0;
	entities.push_back(p);

	scoreMessage.setFont(font);
	scoreMessage.setCharacterSize(30);
	scoreMessage.setPosition(W * 13 / 30, H / 10);
	scoreMessage.setFillColor(sf::Color::Red);
	scoreMessage.setString("Score:0");
	lifeMessage.setFont(font);
	lifeMessage.setCharacterSize(30);
	lifeMessage.setPosition(W * 13 / 30, H / 5);
	lifeMessage.setFillColor(sf::Color::Red);
	lifeMessage.setString("Life:3");
}

int main()
{
	srand(static_cast<unsigned int>(std::time(NULL)));

	RenderWindow app(VideoMode(W, H), "Asteroids!");
	app.setFramerateLimit(60);
	
	t1.loadFromFile("images/spaceship.png");
	t2.loadFromFile("images/space.jpg");
	t3.loadFromFile("images/explosions/type_C.png");
	t4.loadFromFile("images/rock.png");
	t5.loadFromFile("images/fire_blue.png");
	t6.loadFromFile("images/rock_small.png");
	t7.loadFromFile("images/explosions/type_B.png");
	t8.loadFromFile("images/rockS.png");
	t9.loadFromFile("images/rockM.png");
	t10.loadFromFile("images/rockL.png");
	if (!font.loadFromFile("resources/arial.ttf"))
		return EXIT_FAILURE;

	t1.setSmooth(true);
	t2.setSmooth(true);

	Sprite background(t2);

	menu.setFont(font);
	menu.setCharacterSize(40);
	menu.setPosition(W / 4, H / 4);
	menu.setFillColor(sf::Color::Red);
	menu.setString("Main Menu\n\n1. Start a new game: Press S\n\n2. Exit: Press esc");

	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("resources/ball_lost.wav"))
			return EXIT_FAILURE;
	sf::Sound thrust(buffer);
	sf::SoundBuffer buffer2;
	if (!buffer2.loadFromFile("resources/wall_bounce.wav"))
		return EXIT_FAILURE;
	sf::Sound biubiu(buffer2);
	sf::SoundBuffer buffer3;
	if (!buffer3.loadFromFile("resources/shatter.wav"))
		return EXIT_FAILURE;
	sf::Sound shatter(buffer3);
	sf::SoundBuffer buffer4;
	if (!buffer4.loadFromFile("resources/level_complete.wav"))
		return EXIT_FAILURE;
	sf::Sound level_complete(buffer4);
	
		


	/////main loop/////
	while (app.isOpen())
	{
		Event event;
		while (app.pollEvent(event))
		{
			if (event.type == Event::Closed || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)&&screen==1))
				app.close();
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)&&screen == 2) {
				screen = 1;
				for (auto i = entities.begin(); i != entities.end();)
				{
					Entity *e = *i;
					i = entities.erase(i); 
					delete e; 
				}
				life = 3;
				score = 0;
				level = 1;
			}
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::S&&screen == 1)
				{
					screen = 2;
					level = 1;
					for (auto i = entities.begin(); i != entities.end();)
					{
						Entity *e = *i;
						i = entities.erase(i);
						delete e;
					}
					initialSetting();
					life = 3;
					score = 0;
				}
				if (event.key.code == Keyboard::Space&&screen == 2)
				{
					bullet *b = new bullet();
					b->settings(sBullet, p->x, p->y, p->angle, 10);
					entities.push_back(b);
					biubiu.play();
				}
				if (event.key.code == Keyboard::S&&screen == 3)
				{
					screen = 1;
				}
		}
		
		app.draw(background);
		if (screen == 1) {
			app.draw(menu);
		}
		else if (screen == 2) {
			if (Keyboard::isKeyPressed(Keyboard::Right)) p->angle += 3;
			if (Keyboard::isKeyPressed(Keyboard::Left))  p->angle -= 3;
			if (Keyboard::isKeyPressed(Keyboard::Up)) p->thrust = true;
			else p->thrust = false;


			for (auto a : entities)
				for (auto b : entities)
				{

					if (a->name == "asteroid" && b->name == "asteroid" && !(a->x == b->x&&a->y == b->y) && a->life && b->life)
						if (isCollide(a, b))
						{
							if (!a->justCollide && !b->justCollide) {
								a->dx = -a->dx;
								a->dy = -a->dy;
								b->dx = -b->dx;
								b->dy = -b->dy;
								a->justCollide = true;
								b->justCollide = true;
							}
						}

					if (a->name == "asteroid" && b->name == "bullet" && b->life && a->life)
						if (isCollide(a, b))
						{
							score++;
							scoreMessage.setString("Score:" + std::to_string(score));
							a->life = false;
							b->life = false;

							Entity *e = new Entity();
							e->settings(sExplosion, a->x, a->y);
							e->name = "explosion";
							entities.push_back(e);

							if (a->R == 15) {
								Entity *e = new asteroid();
								e->settingsT(t8, a->x + 11, a->y, 0, 10);
								e->speed = level;
								float ta = e->dx;
								float tb = e->dy;
								entities.push_back(e);
								e = new asteroid();
								e->settingsT(t8, a->x - 11, a->y, 0, 10);
								e->speed = level;
								e->dx = -ta;
								e->dy = -tb;
								entities.push_back(e);
							}
							if (a->R == 25) {
								Entity *e = new asteroid();
								e->settingsT(t9, a->x + 16, a->y, 0, 15);
								e->speed = level;
								float ta = e->dx;
								float tb = e->dy;
								entities.push_back(e);
								e = new asteroid();
								e->settingsT(t9, a->x - 16, a->y, 0, 15);
								e->speed = level;
								e->dx = -ta;
								e->dy = -tb;
								entities.push_back(e);
							}
						}



					if (a->name == "player" && b->name == "asteroid")
						if (isCollide(a, b))
						{
							life--;
							lifeMessage.setString("Life:" + std::to_string(life));
							shatter.play();
							b->life = false;

							Entity *e = new Entity();
							e->settings(sExplosion_ship, a->x, a->y);
							e->name = "explosion";
							entities.push_back(e);

							p->settings(sPlayer, W / 2, H / 2, 0, 20);
							p->dx = 0; p->dy = 0;
						}
				}

			for (auto a : entities)
				for (auto b : entities) {
					a->justCollide = false;
					b->justCollide = false;
				}


			if (p->thrust) {
				p->anim = sPlayer_go; thrust.play();
			}
			else   p->anim = sPlayer;


			for (auto e : entities)
				if (e->name == "explosion")
					if (e->anim.isEnd()) e->life = 0;

			bool clearAll = true;
			for (auto e : entities)
				if (e->name == "asteroid"&&e->life)
					clearAll = false;
			if (clearAll) {
				//std::cout << "clearAll";
				level++;
				level_complete.play();
				generateAsteroid(3 * level);
			}
			/*if (rand() % 150 == 0)
			{
			asteroid *a = new asteroid();
			a->settings(sRock, 0, rand() % H, rand() % 360, 25);
			entities.push_back(a);
			}*/

			for (auto i = entities.begin(); i != entities.end();)
			{
				Entity *e = *i;
				e->update();
				e->anim.update();
				if (e->life == false) { i = entities.erase(i); delete e; }
				else i++;
			}

			for (auto i : entities) {
				if (i->name == "asteroid") i->drawT(app);
				else i->draw(app);
			}
			app.draw(scoreMessage);
			app.draw(lifeMessage);
			if (life <= 0) {
				for (auto i = entities.begin(); i != entities.end();)
				{
					Entity *e = *i;
					i = entities.erase(i);
					delete e;
				}
				screen = 3;
			}
		}
		else if (screen == 3) {
			app.draw(scoreMessage);
			app.draw(lifeMessage);
			over.setFont(font);
			over.setCharacterSize(40);
			over.setPosition(W / 4, H / 4);
			over.setFillColor(sf::Color::Red);
			over.setString("Game Over\n\nPress S to main menu.");
			app.draw(over);
		}

		
		
		app.display();
	}

	return 0;
}
