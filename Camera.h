#ifndef CAMERA_H
#define CAMERA_H

#include "Include.h"
#include "Events.h"

class Camera
{
	SDL_Rect camera;
	int LEVEL_WIDTH; 
	int LEVEL_HEIGHT;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	SDL_Rect* target;
	SDL_Rect DrawingRect;
	SDL_Rect HeuraCoord;

public:
	Camera(int LEVEL_WIDTH, int LEVEL_HEIGHT, int SCREEN_WIDTH, int SCREEN_HEIGHT, SDL_Rect* target);
	~Camera();
	void Set();
	
	SDL_Rect GetCameraCoord()
	{
		return DrawingRect;
	}
	SDL_Rect* GetNewCoord()
	{
		if (DrawingRect.h == 0)
			return NULL;
		else
			return &DrawingRect;
	}
	SDL_Rect* GetHeuraCoord(); // zwraca wartosci jeszcze nie obliczone przez glowna funkcje
};
#endif