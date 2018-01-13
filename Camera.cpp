#include"Camera.h"

Camera::Camera(int LEVEL_WIDTH, int LEVEL_HEIGHT, int SCREEN_WIDTH, int SCREEN_HEIGHT, SDL_Rect* target)
{
	this->LEVEL_WIDTH = LEVEL_WIDTH;
	this->LEVEL_HEIGHT = LEVEL_HEIGHT;
	this->SCREEN_WIDTH = SCREEN_WIDTH;
	this->SCREEN_HEIGHT = SCREEN_HEIGHT;
	camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	DrawingRect = { camera.x, camera.y, SCREEN_WIDTH, SCREEN_HEIGHT };
	this->target = target;
	DrawingRect = { 0, 0, 0, 0 };
}

Camera::~Camera()
{
	target = nullptr;
}



void Camera::Set()
{
	
	camera.x = (target->x + target->w /2) - SCREEN_WIDTH / 2;
	camera.y = (target->y + target->h /2) - SCREEN_HEIGHT /2;
	
	if (camera.x <= 0)
	{
		camera.x = 0;
	}
	if (camera.y <= 0)
	{
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
	
	DrawingRect = { camera.x, camera.y, SCREEN_WIDTH, SCREEN_HEIGHT };
	
}

SDL_Rect* Camera::GetHeuraCoord() // zwraca wartosci jeszcze nie obliczone przez glowna funkcje
{
	HeuraCoord.x = (target->x + target->w / 2) - SCREEN_WIDTH / 2;
	HeuraCoord.y = (target->y + target->h / 2) - SCREEN_HEIGHT / 2;
	if (HeuraCoord.x <= 0)
	{
		HeuraCoord.x = 0;
	}
	if (HeuraCoord.y <= 0)
	{
		HeuraCoord.y = 0;
	}
	if (HeuraCoord.x > LEVEL_WIDTH - camera.w)
	{
		HeuraCoord.x = LEVEL_WIDTH - camera.w;
	}
	if (HeuraCoord.y > LEVEL_HEIGHT - camera.h)
	{
		HeuraCoord.y = LEVEL_HEIGHT - camera.h;
	}
	return &HeuraCoord;
}