#include "Irlicht.h"
#include "SDLStuff.h"
#include <cstdlib>
#define IRLICHTCHANNEL 3

Irlicht::Irlicht(SDL_Renderer* renderer)
{
	this->renderer = renderer;
	n_spawns = 0;
	animRectCore = {};
	animRectTong = {};
	rendSpeed = 0;
	memset(&prevAngle, 0, 2 * sizeof(double));
	for (int i = 0; i < 6; i++)
	{
		parts_angle[i] = 90;
	}
}
Irlicht::~Irlicht()
{
	destroy();
}
void Irlicht::load()
{
	SDL_RWops *file = SDL_RWFromFile("graphics/SpineCoreDown.png", "rb");
	core_down = LoadTextureRW(file, renderer);
	file = SDL_RWFromFile("graphics/SpineCoreMid.png", "rb");
	core_mid = LoadTextureRW(file, renderer);
	file = SDL_RWFromFile("graphics/SpineCoreTop.png", "rb");
	core_top = LoadTextureRW(file, renderer);
	file = SDL_RWFromFile("graphics/SpineTongDown.png", "rb");
	tong_down = LoadTextureRW(file, renderer);
	file = SDL_RWFromFile("graphics/SpineTongMid.png", "rb");
	tong_mid = LoadTextureRW(file, renderer);
	file = SDL_RWFromFile("graphics/SpineTongTop.png", "rb");
	tong_top = LoadTextureRW(file, renderer);
	sm.loadSound("Irlicht", 0);

	SDL_QueryTexture(core_down, NULL, NULL, &coreTextureS.w, &coreTextureS.h);
	SDL_QueryTexture(tong_down, NULL, NULL, &tongTextureS.w, &tongTextureS.h);
	coreFrame.w = coreTextureS.w / 3; 
	coreTextureS.w = coreFrame.w * 3;
	coreFrame.h = coreTextureS.h;
	animRectCore.w = coreFrame.w;
	animRectCore.h = coreFrame.h;
	tongFrame.w = tongTextureS.w / 3; 
	tongTextureS.w = tongFrame.w * 3;
	tongFrame.h = tongTextureS.h;
	animRectTong.w = tongFrame.w;
	animRectTong.h = tongFrame.h;
	SoundMenager sm;
	sm.loadSound("IrlichtCatch", 0);
}
void Irlicht::destroy()
{
	SDL_DestroyTexture(core_down);
	SDL_DestroyTexture(core_mid);
	SDL_DestroyTexture(core_top);
	SDL_DestroyTexture(tong_down);
	SDL_DestroyTexture(tong_mid);
	SDL_DestroyTexture(tong_top);
	renderer = nullptr;
	delete renderer;
	cam = nullptr;
	delete cam;
	delete[] spawnDataX;
	spawnDataX = nullptr;
	delete[] spawnDataY;
	spawnDataY = nullptr;
}
void Irlicht::loadSpawnsPoints(long* enemyX, long* enemyY, int nSpawn)
{
	delete[] spawnDataX;
	spawnDataX = nullptr;
	delete[] spawnDataY;
	spawnDataY = nullptr;
	spawnDataX = new long[nSpawn];
	spawnDataY = new long[nSpawn];
	n_spawns = nSpawn;
	for (int i = 0; i < nSpawn; i++)
	{
		spawnDataY[i] = enemyY[i];
		spawnDataX[i] = enemyX[i];
	}
	
}
void Irlicht::spawn(int id)
{
	if (id == -1)
		id = (rand() % n_spawns) + 0;
	
	
	for (int i = 0; i < 6; i++)
	{
		if (i > -1 && i < 3)
		{
			parts_poz[i].w = SPINECW;
			parts_poz[i].h = SPINECH;
		}
		else if (i > 2 && i < 6)
		{
			parts_poz[i].w = SPINETW;
			parts_poz[i].h = SPINETH;
		}
		if (i != 0)
		{
			dist += 17;
		}
		if (i == 4)
			dist -= 26;
		if (i == 2)
			dist -= 7;
		
		parts_poz[i].x = spawnDataX[id];
		parts_poz[i].y = spawnDataY[id] - dist;
	}
	RectPoint.x = spawnDataX[id];
	RectPoint.y = spawnDataY[id];
	dist = 15;

	
}
void Irlicht::followPoint()
{
	sm.addSound("Irlicht", IRLICHTCHANNEL, -1);
	RectPoint.x = point->x;
	RectPoint.y = point->y;
	prevAngle[0] = parts_angle[0];
	prev[0].x = parts_poz[0].x;
	prev[0].y = parts_poz[0].y;
	if (abs(parts_poz[0].x - point->x) > 1|| abs(parts_poz[0].y - point->y) > 1)
	{
		if (abs(prevPoint.x - point->x) > 9 || abs(prevPoint.y - point->y) > 9)
		{
			prevPoint = *point;
			parts_angle[0] = (atan2(point->y - parts_poz[0].y, point->x - parts_poz[0].x) * 180) / 3.14;
		}
		parts_poz[0].x = point->x;
		parts_poz[0].y = point->y;
	}
	for (int i = 1; i < 6; i++)
	{
		if (abs(parts_poz[i-1].x - parts_poz[i].x) > dist || abs(parts_poz[i-1].y - parts_poz[i].y) > dist)
		{
			prevAngle[1] = parts_angle[i];
			parts_angle[i] = prevAngle[0];
			prevAngle[0] = prevAngle[1];
			prev[1].x = parts_poz[i].x;
			prev[1].y = parts_poz[i].y;
			parts_poz[i].x = prev[0].x;
			parts_poz[i].y = prev[0].y;
			prev[0] = prev[1];
		}
	}
	sm.changeSoundPos(IRLICHTCHANNEL, point->x-cam->GetNewCoord()->x); // do zoptymalizowania
}
void Irlicht::runAnim()
{
	rendSpeed++;
	if (64 / rendSpeed == 4)
	{
		rendSpeed = 0;
		animRectCore.x += coreFrame.w;
		if (animRectCore.x >= coreTextureS.w)
			animRectCore.x = 0;
		animRectTong.x += tongFrame.w;
		if (animRectTong.x >= tongTextureS.w)
			animRectTong.x = 0;
	}
}
void Irlicht::render(SDL_Rect cam)
{
	for (int i = 0; i < 6; i++)
	{
		drawn_setting[i] = parts_poz[i];
		drawn_setting[i].x -= cam.x;
		drawn_setting[i].y -= drawn_setting[i].h / 2 + cam.y;
		drawn_setting[i].x += drawn_setting[i].w / 2;
	}
	SDL_RenderCopyEx(renderer, core_mid, &animRectCore, &drawn_setting[2], parts_angle[2] + 88, NULL, SDL_FLIP_NONE);
	SDL_RenderCopyEx(renderer, tong_top, &animRectTong, &drawn_setting[5], parts_angle[5] + 88, NULL, SDL_FLIP_NONE);
	SDL_RenderCopyEx(renderer, tong_mid, &animRectTong, &drawn_setting[4], parts_angle[4] + 88, NULL, SDL_FLIP_NONE);
	SDL_RenderCopyEx(renderer, tong_down, &animRectTong, &drawn_setting[3], parts_angle[3] + 88, NULL, SDL_FLIP_NONE);
	SDL_RenderCopyEx(renderer, core_top, &animRectCore, &drawn_setting[1], parts_angle[1] + 88, NULL, SDL_FLIP_NONE);
	SDL_RenderCopyEx(renderer, core_down, &animRectCore, &drawn_setting[0], parts_angle[0] -92, NULL, SDL_FLIP_NONE);
}



void Irlicht::GetPointers(Camera* cam)
{
	this->cam = cam;
}
void Irlicht::SetPointerToPoint(Point* point)
{
	this->point = point;
};
SDL_Rect* Irlicht::GetPositionPointer()
{
	return &RectPoint;
}