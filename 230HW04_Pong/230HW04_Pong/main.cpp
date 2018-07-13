#include <SFML/Graphics.hpp>
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

// Define some constants
const float pi = 3.14159f;
const int gameWidth = 800;
const int gameHeight = 600;
sf::Vector2f paddleSize(25, 80);
float ballRadius = 10.f;
sf::Text scoreMessage;
// Define the paddles properties
sf::Clock AITimer;
const sf::Time AITime = sf::seconds(0.1f);
sf::Clock clockk;
bool isPlaying = false;
int scoreA = 0;
int scoreB = 0;


class Paddle: public sf::RectangleShape {	// tracking the paddle's location.
public:

	float speed = 0.f;
	Paddle(sf::Color);
	~Paddle();
};

class Ball: public sf::CircleShape {	//	tracking and updating the ball's location, speed, and direction
public:
	float speed = 300.f;
	float angle = 0.f; // to be changed later
	
	Ball();
	~Ball();
};


Ball::Ball()
{
	setRadius(ballRadius - 3);
	setOutlineThickness(3);
	setOutlineColor(sf::Color::Black);
	setFillColor(sf::Color::White);
	setOrigin(ballRadius / 2, ballRadius / 2);
}

Ball::~Ball()
{
}



int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	// Create the window of the application
	sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "SFML Pong",
		sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	Paddle leftPaddleUp = Paddle(sf::Color(100, 100, 200));
	Paddle leftPaddleDown = Paddle(sf::Color(100, 200, 100));
	Paddle rightPaddle = Paddle(sf::Color(200, 100, 100));
	Paddle midPaddle = Paddle(sf::Color(100, 100, 100));
	leftPaddleUp.speed = 300.f;
	leftPaddleDown.speed = 300.f;
	midPaddle.speed = 80.f;
	Ball ball = Ball();

	// Load the sounds used in the game
	sf::SoundBuffer ballSoundBuffer;
	if (!ballSoundBuffer.loadFromFile("resources/ball.wav"))
		return EXIT_FAILURE;
	sf::Sound ballSound(ballSoundBuffer);

	// Load the text font
	sf::Font font;
	if (!font.loadFromFile("resources/sansation.ttf"))
		return EXIT_FAILURE;

	// Load the back image
	sf::Texture back;
	if (!back.loadFromFile("resources/back.jpg"))
		return EXIT_FAILURE;
	// Create the left paddle
	sf::RectangleShape backR;
	backR.setSize(sf::Vector2f(gameWidth, gameHeight));
	backR.setTexture(&back);

	// Initialize the pause message
	sf::Text pauseMessage;
	pauseMessage.setFont(font);
	pauseMessage.setCharacterSize(30);
	pauseMessage.setPosition(100.f, 150.f);
	pauseMessage.setFillColor(sf::Color::White);
	pauseMessage.setString("Welcome to SFML pong!\nUse W/S to control left upper paddle.\nUse Up/Down to control left lower paddle.\n5 points is a round.\nPress space to start the game.");
	
	
	// Initialize the score message
	scoreMessage.setFont(font);
	scoreMessage.setCharacterSize(40);
	scoreMessage.setPosition(350.f, 50.f);
	scoreMessage.setFillColor(sf::Color::White);
	while (window.isOpen()) {
		
		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			
			if ((event.type == sf::Event::Closed) ||
				((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
			{
				window.close();
				break;
			}
			// Space key pressed: play
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
			{
				
				if (!isPlaying)
				{
					// (re)start the game
					isPlaying = true;
					scoreA = 0;
					scoreB = 0;
					clockk.restart();

					scoreMessage.setString(std::to_string(scoreA) + " : " + std::to_string(scoreB));

					// Reset the position of the paddles and ball
					leftPaddleDown.setPosition(10 + paddleSize.x / 2, gameHeight / 2 + 60);
					leftPaddleUp.setPosition(10 + paddleSize.x / 2, gameHeight / 2 - 60);
					rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
					midPaddle.setPosition(gameWidth/2 - paddleSize.x / 2, gameHeight / 2);
					ball.setPosition(gameWidth / 2, gameHeight / 2);
					ball.speed = 200.f;
					// Reset the ball angle
					do
					{
						// Make sure the ball initial angle is not too much vertical
						ball.angle = (std::rand() % 360) * 2 * pi / 360;
					} while (std::abs(std::cos(ball.angle)) < 0.7f);
				}
			}
		}
		if (isPlaying)
		{
			float deltaTime = clockk.restart().asSeconds();
			// Move the player's paddle
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
				(leftPaddleDown.getPosition().y - paddleSize.y / 2 > 5.f) && leftPaddleDown.getPosition().y>= (leftPaddleUp.getPosition().y+ paddleSize.y))
			{
				leftPaddleDown.move(0.f, -leftPaddleDown.speed * deltaTime);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
				(leftPaddleDown.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f))
			{
				leftPaddleDown.move(0.f, leftPaddleDown.speed * deltaTime);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
				(leftPaddleUp.getPosition().y - paddleSize.y / 2 > 5.f))
			{
				leftPaddleUp.move(0.f, -leftPaddleUp.speed * deltaTime);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
				(leftPaddleUp.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f) && leftPaddleDown.getPosition().y >= (leftPaddleUp.getPosition().y + paddleSize.y))
			{
				leftPaddleUp.move(0.f, leftPaddleDown.speed * deltaTime);
			}

			// Move the computer's paddle
			if (((rightPaddle.speed < 0.f) && (rightPaddle.getPosition().y - paddleSize.y / 2 > 5.f)) ||
				((rightPaddle.speed > 0.f) && (rightPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f)))
			{
				rightPaddle.move(0.f, rightPaddle.speed * deltaTime);
			}

			// Move the middle object
			if ((midPaddle.getPosition().y - paddleSize.y / 2) <= 5.f || (midPaddle.getPosition().y + paddleSize.y / 2) >= gameHeight - 5.f)
			{
				midPaddle.move(0.f, -midPaddle.speed * deltaTime);
				midPaddle.speed = -midPaddle.speed;
			}
			else {
				midPaddle.move(0.f, midPaddle.speed * deltaTime);
			}
			

			// Update the computer's paddle direction according to the ball position
			if (AITimer.getElapsedTime() > AITime)
			{
				AITimer.restart();
				if (ball.getPosition().y + ballRadius > rightPaddle.getPosition().y + paddleSize.y / 2)
					rightPaddle.speed = 150.f;
				else if (ball.getPosition().y - ballRadius < rightPaddle.getPosition().y - paddleSize.y / 2)
					rightPaddle.speed = -150.f;
				else
					rightPaddle.speed = 0.f;
			}

			// Move the ball
			float factor = ball.speed * deltaTime;
			ball.move(std::cos(ball.angle) * factor, std::sin(ball.angle) * factor);

			// Check collisions between the ball and the screen
			if (ball.getPosition().x - ballRadius < 0.f)
			{
				scoreB++;
				if (scoreB >= 5) {
					isPlaying = false;
					pauseMessage.setString("You lost!\nPress space to restart or\nescape to exit");
				}
				else {
					clockk.restart();
					scoreMessage.setString(std::to_string(scoreA) + " : " + std::to_string(scoreB));
					// Reset the position of the paddles and ball
					leftPaddleDown.setPosition(10 + paddleSize.x / 2, gameHeight / 2 + 60);
					leftPaddleUp.setPosition(10 + paddleSize.x / 2, gameHeight / 2 - 60);
					rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
					midPaddle.setPosition(gameWidth / 2 - paddleSize.x / 2, gameHeight / 2);
					ball.setPosition(gameWidth / 2, gameHeight / 2);
					ball.speed = 200.f;
					// Reset the ball angle
					do
					{
						// Make sure the ball initial angle is not too much vertical
						ball.angle = (std::rand() % 360) * 2 * pi / 360;
					} while (std::abs(std::cos(ball.angle)) < 0.7f);
				}
				
			}
			if (ball.getPosition().x + ballRadius > gameWidth)
			{
				scoreA++;
				if (scoreA >= 5) {
					isPlaying = false;
					pauseMessage.setString("You won!\nPress space to restart or\nescape to exit");
				}
				else {
					clockk.restart();
					scoreMessage.setString(std::to_string(scoreA) + " : " + std::to_string(scoreB));
					// Reset the position of the paddles and ball
					leftPaddleDown.setPosition(10 + paddleSize.x / 2, gameHeight / 2 + 60);
					leftPaddleUp.setPosition(10 + paddleSize.x / 2, gameHeight / 2 - 60);
					rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
					midPaddle.setPosition(gameWidth / 2 - paddleSize.x / 2, gameHeight / 2);
					ball.setPosition(gameWidth / 2, gameHeight / 2);
					ball.speed = 200.f;
					// Reset the ball angle
					do
					{
						// Make sure the ball initial angle is not too much vertical
						ball.angle = (std::rand() % 360) * 2 * pi / 360;
					} while (std::abs(std::cos(ball.angle)) < 0.7f);
				}
				
			}
			if (ball.getPosition().y - ballRadius < 0.f)
			{
				ballSound.play();
				ball.angle = -ball.angle;
				ball.setPosition(ball.getPosition().x, ballRadius + 0.1f);
			}
			if (ball.getPosition().y + ballRadius > gameHeight)
			{
				ballSound.play();
				ball.angle = -ball.angle;
				ball.setPosition(ball.getPosition().x, gameHeight - ballRadius - 0.1f);
			}

			// Check the collisions between the ball and the paddles
			// Left Paddle Up
			if (ball.getPosition().x - ballRadius < leftPaddleUp.getPosition().x + paddleSize.x / 2 &&
				ball.getPosition().x - ballRadius > leftPaddleUp.getPosition().x &&
				ball.getPosition().y + ballRadius >= leftPaddleUp.getPosition().y - paddleSize.y / 2 &&
				ball.getPosition().y - ballRadius <= leftPaddleUp.getPosition().y + paddleSize.y / 2)
			{
				float ratio = abs(leftPaddleUp.getPosition().y + paddleSize.y / 2 - ball.getPosition().y - ball.getRadius()) / (paddleSize.y / 2);
				ball.angle = pi / 4 * (1 - ratio);
				ball.speed += 20.f;
				ball.setPosition(leftPaddleUp.getPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball.getPosition().y);
			}
			// Left Paddle Down
			if (ball.getPosition().x - ballRadius < leftPaddleDown.getPosition().x + paddleSize.x / 2 &&
				ball.getPosition().x - ballRadius > leftPaddleDown.getPosition().x &&
				ball.getPosition().y + ballRadius >= leftPaddleDown.getPosition().y - paddleSize.y / 2 &&
				ball.getPosition().y - ballRadius <= leftPaddleDown.getPosition().y + paddleSize.y / 2)
			{
				float ratio = abs(leftPaddleDown.getPosition().y + paddleSize.y / 2 - ball.getPosition().y - ball.getRadius()) / (paddleSize.y / 2);
				ball.angle = pi / 4 * (1 - ratio);
				ball.speed += 20.f;
				ball.setPosition(leftPaddleDown.getPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball.getPosition().y);
			}

			// Right Paddle
			if (ball.getPosition().x + ballRadius > rightPaddle.getPosition().x - paddleSize.x / 2 &&
				ball.getPosition().x + ballRadius < rightPaddle.getPosition().x &&
				ball.getPosition().y + ballRadius >= rightPaddle.getPosition().y - paddleSize.y / 2 &&
				ball.getPosition().y - ballRadius <= rightPaddle.getPosition().y + paddleSize.y / 2)
			{
				float ratio = abs(rightPaddle.getPosition().y + paddleSize.y / 2 - ball.getPosition().y - ball.getRadius()) / (paddleSize.y / 2);
				ball.angle = pi - pi / 4 * (1 - ratio);
				ball.speed += 20.f;
				ball.setPosition(rightPaddle.getPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball.getPosition().y);
			}
			// Mid Paddle
			if (ball.getPosition().x + ballRadius > midPaddle.getPosition().x - paddleSize.x / 2 &&
				ball.getPosition().x + ballRadius < midPaddle.getPosition().x &&
				ball.getPosition().y + ballRadius >= midPaddle.getPosition().y - paddleSize.y / 2 &&
				ball.getPosition().y - ballRadius <= midPaddle.getPosition().y + paddleSize.y / 2)
			{
				float ratio = abs(midPaddle.getPosition().y + paddleSize.y / 2 - ball.getPosition().y - ball.getRadius()) / (paddleSize.y / 2);
				ball.angle = pi - pi / 4 * (1 - ratio);
				ball.setPosition(midPaddle.getPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball.getPosition().y);
			}
			// Mid Paddle
			if (ball.getPosition().x - ballRadius < midPaddle.getPosition().x + paddleSize.x / 2 &&
				ball.getPosition().x - ballRadius > midPaddle.getPosition().x &&
				ball.getPosition().y + ballRadius >= midPaddle.getPosition().y - paddleSize.y / 2 &&
				ball.getPosition().y - ballRadius <= midPaddle.getPosition().y + paddleSize.y / 2)
			{
				float ratio = abs(midPaddle.getPosition().y + paddleSize.y / 2 - ball.getPosition().y - ball.getRadius()) / (paddleSize.y / 2);
				ball.angle = pi / 4 * (1 - ratio);
				ball.setPosition(midPaddle.getPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball.getPosition().y);
			}
		}
		// Clear the window
		window.clear(sf::Color(50, 200, 50));
		backR.setPosition(sf::Vector2f(0, 0));
		window.draw(backR);
		if (isPlaying)
		{
			// Draw the paddles and the ball
			window.draw(leftPaddleUp);
			window.draw(leftPaddleDown);
			window.draw(rightPaddle);
			window.draw(midPaddle);
			window.draw(ball);
			window.draw(scoreMessage);
		}
		else
		{
			// Draw the pause message
			window.draw(pauseMessage);
		}
		// Display things on screen
		window.display();
	}

	return 0;
}

Paddle::Paddle(sf::Color color)
{
	setSize(paddleSize - sf::Vector2f(3, 3));
	setOutlineThickness(3);
	setOutlineColor(sf::Color::Black);
	setFillColor(color);
	setOrigin(paddleSize / 2.f);
}

Paddle::~Paddle()
{
}

// DEBUG
//std::ostringstream buffer;
//buffer << ball.angle;
//std::string str = buffer.str();
//LPCSTR a = (LPCSTR)str.c_str();
//OutputDebugString(a);