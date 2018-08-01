#include "Simple2D.h"
#include "SceneManager.cpp"
#include <iostream>
#include <time.h>  
using namespace Simple2D;


int main(int argc, char *argv[])
{

	srand((unsigned)time(NULL));
	SceneManager& sceneManager = SceneManager::getInstance();
	Window* pWindow = CreateWindow("ShooterGame", sceneManager.windowWidth, sceneManager.windowHeight);
	while (!ShouldWindowClose(pWindow))
	{
		sceneManager.update();
		RefreshWindowBuffer(pWindow);
	}

	DestroyWindow(pWindow);
	

}


