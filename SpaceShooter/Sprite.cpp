#include "Sprite.h"
#include "Simple2D.h"
#include "Helper.h"
#include <iostream>
using namespace Simple2D;


Sprite::Sprite(float _x, float _y, float _speed, int _life) :x(_x), y(_y), speed(_speed), life(_life) {
}


Sprite::Sprite(float _x, float _y, std::string& sImageFileName, float _speed, int _life) :x(_x), y(_y), speed(_speed), life(_life) {
	image = CreateImage(sImageFileName);
	GetImageSize(image, &width, &height);
}


Sprite::Sprite(float _x, float _y, int _width, int _height, float _speed) : x(_x), y(_y),width(_width), height(_height), speed(_speed) {
}

Sprite::~Sprite() {
	if (image != nullptr) {
		DestroyImage(image);
	}
	
}


void Sprite::setPosition(float _x, float _y) {
	x = _x;
	y = _y;
}

void Sprite::setPosition(Sprite* s) {
	x = s->getX() + s->getWidth() / 2 - width/2;
	y = s->getY() + s->getHeight() / 2 - height / 2;
}

void Sprite::move(){
	setPosition(x + dirX*speed, y + dirY*speed);
}

void Sprite::move(Sprite* boundary) {
	float p1 = boundary->getX();
	float p2 = boundary->getY();
	float p3 = p1 + boundary->getWidth() - width;
	float p4 = p2 + boundary->getHeight() - height;
	float targetX = x + dirX*speed;
	float targetY = y + dirY*speed;
	if(targetX>p1 && targetY>p2 && targetX<p3 && targetY<p4) setPosition(targetX, targetY);
}

void Sprite::draw(){
	DrawImage(image, x+ width/2, y+ height/2, rotation, scale);
}

void Sprite::setImage(std::string& sImageFileName){
	if (image == nullptr) {
		image = CreateImage(sImageFileName);
		GetImageSize(image, &width, &height);
	}
	else {
		Image* newimage = CreateImage(sImageFileName);
		int newWidth, newHeight;
		GetImageSize(newimage, &newWidth, &newHeight);
		DestroyImage(image);
		image = newimage;
		setPosition(x + width / 2 - newWidth / 2, y + height / 2 - newHeight / 2);
		width = newWidth;
		height = newHeight;
	
	}
}

bool Sprite::intersect(Sprite* s) {
	return !((x + width < s->x) || (y > s->y+s->height) || (s->x+s->width < x) || (s->y > y+height));
}



float Sprite::getX() {
	return x;
}

float Sprite::getY() {
	return y;
}

int Sprite::getWidth() {
	return width;
}

int Sprite::getHeight() {
	return height;
}
