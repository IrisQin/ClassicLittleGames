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

class Behaviour {
public:
	virtual float behaviour(sf::Time currT, sf::Time totalT, float k, float b) = 0;
};
class LinearGrow : public Behaviour {
public:
	virtual float behaviour(sf::Time currT, sf::Time totalT, float k, float b) {
		return k*(totalT.asSeconds() - currT.asSeconds()) / totalT.asSeconds() + b;
	}
};
class LinearShrink : public Behaviour {
public:
	virtual float behaviour(sf::Time currT, sf::Time totalT, float k, float b) {
		float res = -k*(totalT.asSeconds() - currT.asSeconds()) / totalT.asSeconds() + b;
		if (res < 0)return 0;
		return res;
	}
};
class CubicEaseIn : public Behaviour {
public:
	virtual float behaviour(sf::Time currT, sf::Time totalT, float k, float b) {
		float t = totalT.asSeconds() - currT.asSeconds();
		t /= totalT.asSeconds();
		return k*t*t*t + b;;
	}
};
class BounceEaseOut : public Behaviour {
public:
	virtual float behaviour(sf::Time currT, sf::Time totalT, float k, float b) {
		float p = totalT.asSeconds() - currT.asSeconds();
		p /= totalT.asSeconds();
		if (p < 4 / 11.0)
		{
			return (121 * p * p) / 16.0 *k+b ;
		}
		else if (p < 8 / 11.0)
		{
			return ((363 / 40.0 * p * p) - (99 / 10.0 * p) + 17 / 5.0)*k+b ;
		}
		else if (p < 9 / 10.0)
		{
			return ((4356 / 361.0 * p * p) - (35442 / 1805.0 * p) + 16061 / 1805.0)*k+b;
		}
		else
		{
			return ((54 / 5.0 * p * p) - (513 / 25.0 * p) + 268 / 25.0)*k+b;
		}
	}
};
class ElasticEaseIn : public Behaviour {
public:
	virtual float behaviour(sf::Time currT, sf::Time totalT, float k, float b) {
		float p = totalT.asSeconds() - currT.asSeconds();
		p /= totalT.asSeconds();
		return sin(13 * 3.14 * p) * pow(2, 10 * (p - 1)) * k ;
	}
};

class ParticleSystem
{
public:

	ParticleSystem(unsigned int count, sf::Time lifetime, sf::Vector2f emitter, float textureScale, float angleRange) :
		count(count),
		particles(0),
		lifetime(lifetime),
		emitter(emitter),
		textureScale(textureScale),
		angleRange(angleRange),
		scaleBehaviour(nullptr),
		speedBehaviour(nullptr),
		rotateBehaviour(nullptr)
	{
	}

	int count;

	struct Particle
	{
		sf::Sprite sprite;
		sf::Vector2f velocity;
		sf::Time currTime;
	};

	std::vector<Particle> particles;

	void setEmitter(sf::Vector2f position)
	{
		emitter = position;
	}

	bool setTexture(const std::string& texturefile)
	{
		if (!texture.loadFromFile(texturefile))
			return false;
	}

	void update(sf::Time elapsed)
	{
		for (std::size_t i = 0; i < particles.size(); ++i)
		{
			Particle& p = particles[i];
			p.currTime -= elapsed;
			if (p.currTime <= sf::Time::Zero)
				resetParticle(i);
			sf::Vector2f oldp = p.sprite.getPosition();

			float velocityFactor = 1;
			if (speedBehaviour != nullptr) {
				velocityFactor = speedBehaviour->behaviour(p.currTime, lifetime, 20, 0.01);
				
			}
			p.sprite.setPosition(oldp + p.velocity * elapsed.asSeconds() * velocityFactor);

			if (scaleBehaviour != nullptr) {
				float newScale = scaleBehaviour->behaviour(p.currTime, lifetime, 0.8, textureScale);
				p.sprite.setScale(newScale, newScale);
			}

			if (rotateBehaviour != nullptr) {
				float newAngle = rotateBehaviour->behaviour(p.currTime, lifetime, 360, 0);
				p.sprite.setRotation(newAngle);
			}
			
		}
	}
	
	void addParticle(int count) {
		for (int i = 0; i < count; i++) {
			Particle newp;
			newp.sprite.setTexture(texture);
			newp.sprite.setPosition(emitter);
			newp.sprite.setScale(textureScale, textureScale);
			newp.sprite.setOrigin(newp.sprite.getLocalBounds().width/2, newp.sprite.getLocalBounds().height / 2);
			float angle = - 3.14f/2 - angleRange*3.14f + (std::rand() % 360)*3.14f*angleRange / 180.f;
			float speed = (std::rand() % 50) + 150.f;//+50
			newp.velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
			newp.currTime = sf::milliseconds((std::rand() % 2000) + 1000);//(std::rand() % 2000) + 1000
			particles.push_back(newp);
		}
	}

	void deleteParticle(int count) {
		while (count--) {
			if(particles.empty()){
				return;
			}
			std::vector<Particle>::iterator k = particles.begin();
			particles.erase(k);	
		}
	}

	void setScaleBehaviour(std::string type) {
		if (type == "linearGrow") {
			scaleBehaviour = new LinearGrow();
		}else if (type == "linearShrink") {
			scaleBehaviour = new LinearShrink();
		}
		else if (type == "CubicEaseIn") {
			scaleBehaviour = new CubicEaseIn();
		}else if (type == "BounceEaseOut") {
			scaleBehaviour = new BounceEaseOut();
		}else if (type == "ElasticEaseIn") {
			scaleBehaviour = new ElasticEaseIn();
		}
		
	}

	void setSpeedBehaviour(std::string type) {
		if (type == "linearGrow") {
			speedBehaviour = new LinearGrow();
		}
		else if (type == "linearShrink") {
			speedBehaviour = new LinearShrink();
		}else if (type == "CubicEaseIn") {
			speedBehaviour = new CubicEaseIn();
		}
		else if (type == "BounceEaseOut") {
			speedBehaviour = new BounceEaseOut();
		}
		else if (type == "ElasticEaseIn") {
			speedBehaviour = new ElasticEaseIn();
		}
	}

	void setRotateBehaviour(std::string type) {
		if (type == "linearGrow") {
			rotateBehaviour = new LinearGrow();
		}
		else if (type == "linearShrink") {
			rotateBehaviour = new LinearShrink();
		}
		else if (type == "CubicEaseIn") {
			rotateBehaviour = new CubicEaseIn();
		}
		else if (type == "BounceEaseOut") {
			rotateBehaviour = new BounceEaseOut();
		}
		else if (type == "ElasticEaseIn") {
			rotateBehaviour = new ElasticEaseIn();
		}
	}

private:

	void resetParticle(std::size_t index)
	{
		float angle = -3.14f / 2 - angleRange*3.14f + (std::rand() % 360)*3.14f*angleRange / 180.f;
		float speed = (std::rand() % 50) + 50.f;
		particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
		particles[index].currTime = sf::milliseconds((std::rand() % 2000) + 1000);
		particles[index].sprite.setPosition(emitter);
	}

	sf::Time lifetime;
	sf::Vector2f emitter;
	sf::Texture texture;
	float angleRange;	// 0-1
	float textureScale;
	Behaviour* scaleBehaviour;
	Behaviour* speedBehaviour;
	Behaviour* rotateBehaviour;
};



int main()
{
	int windowWidth = 2048;
	int windowHeight = 1024;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Particles");
	

	//ParticleSystem fireParticles(10, sf::seconds(3), sf::Vector2f(windowWidth / 4, windowHeight / 2),0.5,0.1);
	//fireParticles.setTexture("resources/fire.png");
	//fireParticles.addParticle(fireParticles.count);
	//fireParticles.setScaleBehaviour("linearShrink");
	//fireParticles.setSpeedBehaviour("linearGrow");
	//ParticleSystem smallParticles(5, sf::seconds(3), sf::Vector2f(windowWidth*3 / 4, windowHeight /2), 0.001,1);
	//smallParticles.setTexture("resources/gun_target_cursor.jpg");//");sc-snowflakes11.png
	//smallParticles.addParticle(smallParticles.count);
	//smallParticles.setScaleBehaviour("linearGrow");
	//smallParticles.setSpeedBehaviour("linearShrink");
	ParticleSystem snowParticles1(10, sf::seconds(3), sf::Vector2f(windowWidth / 5, windowHeight / 2), 0.3, 1);
	snowParticles1.setTexture("resources/sc-snowflakes11.png");
	snowParticles1.addParticle(snowParticles1.count);
	//snowParticles1.setScaleBehaviour("CubicEaseIn");
	//snowParticles1.setSpeedBehaviour("CubicEaseIn");
	snowParticles1.setRotateBehaviour("CubicEaseIn");
	ParticleSystem snowParticles2(5, sf::seconds(3), sf::Vector2f(windowWidth*4 / 5, windowHeight / 5), 0.3, 1);
	snowParticles2.setTexture("resources/sc-snowflakes11.png");
	snowParticles2.addParticle(snowParticles2.count);
	//snowParticles2.setScaleBehaviour("BounceEaseOut");
	//snowParticles2.setSpeedBehaviour("BounceEaseOut");
	snowParticles2.setRotateBehaviour("BounceEaseOut");
	ParticleSystem snowParticles3(5, sf::seconds(3), sf::Vector2f(windowWidth * 4 / 5, windowHeight* 4/ 5), 0.3, 1);
	snowParticles3.setTexture("resources/sc-snowflakes11.png");
	snowParticles3.addParticle(snowParticles3.count);
	//snowParticles3.setScaleBehaviour("ElasticEaseIn");
	//snowParticles3.setSpeedBehaviour("ElasticEaseIn");
	snowParticles3.setRotateBehaviour("ElasticEaseIn");

	sf::Clock clock;

	sf::Font font;
	if (!font.loadFromFile("resources/arial.ttf"))
		return EXIT_FAILURE;
	sf::Text particleNumberMessage;
	particleNumberMessage.setFont(font);
	particleNumberMessage.setCharacterSize(20);
	particleNumberMessage.setPosition(windowWidth*2/5, windowHeight/10);
	particleNumberMessage.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			fireParticles.addParticle(1);
			snowParticles.addParticle(1);
			smallParticles.addParticle(1);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			fireParticles.deleteParticle(1);
			snowParticles.deleteParticle(1);
			smallParticles.deleteParticle(1);
		}*/

		sf::Time elapsed = clock.restart();
		//fireParticles.update(elapsed);
		snowParticles1.update(elapsed);
		snowParticles2.update(elapsed);
		snowParticles3.update(elapsed);
		//smallParticles.update(elapsed);

		window.clear();
		//for (int i = 0; i < fireParticles.particles.size(); i++) {
		//	window.draw(fireParticles.particles[i].sprite);
		//}
		for (int i = 0; i < snowParticles1.particles.size(); i++) {
			window.draw(snowParticles1.particles[i].sprite);
		}
		for (int i = 0; i < snowParticles2.particles.size(); i++) {
			window.draw(snowParticles2.particles[i].sprite);
		}
		for (int i = 0; i < snowParticles3.particles.size(); i++) {
			window.draw(snowParticles3.particles[i].sprite);
		}
		//for (int i = 0; i < smallParticles.particles.size(); i++) {
		//	window.draw(smallParticles.particles[i].sprite);
		//}
		
		//"Particle Count:"+ std::to_string(fireParticles.particles.size())+\n\nLeft particle:\nEmission angle range:0.1\nParticle size shrinking linearly\nvelocity growing linearly\n\nRight particle:\nEmission angle range:1.0\nParticle size growing linearly\nvelocity shrinking linearly ");
		particleNumberMessage.setString("Left: CubicEaseIn(Rotate)\nUpper right: BounceEaseOut(Rotate)\nLower right: ElasticEaseIn(Rotate)");
		window.draw(particleNumberMessage);
		window.display();
	}

	return 0;
}