#include "Effects.h"
#include"SDLStuff.h"


Effects::Effects(SDL_Renderer* rend, SDL_Rect* scrainPoz, SDL_Rect* camr)
{
	this->cam = camr;
	renderer = rend;
	curse.loadPointers(scrainPoz);
}
Effects::~Effects()
{
	renderer = nullptr;
	delete renderer;
	cam = nullptr;
	delete cam;
	
}

void Effects::loadEffect(int index, short target, SDL_Rect* poz)
{
	switch (index)
	{
	case 1: // curse
	{
		SDL_RWops* file = SDL_RWFromFile("graphics/curseAnim.png", "rb");
		if (target != 0)
			curse.setTarget(target);
		curse.load(110, 68, poz, file, renderer,11,1);
		break;
	}
	case 2: // resurrection
	{
		particleSpawnMenager.spawn(poz->x, poz->y, 0, 0, 1000, SCRAINW, 9, 2, renderer, cam);
		break;
	}
	default:
		break;
	}
}
void Effects::removeEffect(int index, bool timer)
{
	switch (index)
	{
	case 1:
	{
		curse.remove(timer);
		break;
	}
	default:
		break;
	}
}
void Effects::renderEffects()
{
	if (curse.isLoad == true || curse.rem_timer == true)
	{
		curse.runAnim();
		curse.render(renderer,cam);
	}
	particleSpawnMenager.update();
}



Effects::Effect::Effect()
{
	rendSpeed = 0;
	target = 0;
}
Effects::Effect::~Effect()
{
	SDL_DestroyTexture(texture);
	scrainPoz = nullptr;
	delete scrainPoz;
	trackPoz = nullptr;
	delete trackPoz;
}
void Effects::Effect::load(int w, int h, SDL_Rect* TrackPoz, SDL_RWops *file, SDL_Renderer* renderer,int num_animX,int num_animY)
{
	if (target == 0)
		trackPoz = TrackPoz;
	else
		loadPozFromTarget();
	setting.h = h;
	setting.w = w;
	texture = LoadTextureRW(file, renderer);
	SDL_QueryTexture(texture, NULL, NULL, &textureW, &textureH);
	frameW = textureW / num_animX;
	frameH = textureH / num_animY;
	textureW = frameW * num_animX;
	textureH = frameH * num_animY;
	animRect.w = frameW;
	animRect.h = frameH;
	animRect.x = animRect.y = 0;
	isLoad = true;
}
void Effects::Effect::render(SDL_Renderer* renderer, SDL_Rect* cam)
{
	if (texture != NULL)
	{
		setting.x = trackPoz->x+3 - cam->x;
		setting.y = trackPoz->y - cam->y-14;
		SDL_RenderCopyEx(renderer, texture, &animRect, &setting, NULL, NULL, SDL_FLIP_NONE);
	}
}
void Effects::Effect::remove(bool timer)
{
	if (timer == 0)
	{
		SDL_DestroyTexture(texture);
		frameH = NULL;
		frameW = NULL;
		textureH = NULL;
		textureW = NULL;
		animRect = {};
		isLoad = false;
		setting = {};
	}
	else
		rem_timer = true;
}
void Effects::Effect::runAnim()
{
	rendSpeed++;
	if (58 / rendSpeed == 4)
	{
		rendSpeed = 0;
		if (animRect.x + frameW >= textureW)
		{
			animRect.x = 0;

			if (animRect.y + frameH >= textureH)
			{
				animRect.y = 0;
				if (rem_timer == true)
				{
					rem_timer = 0;
					this->remove(0);
				}
			}
			else
				animRect.y += frameH;

		}
		else
			animRect.x += frameW;
	}
}
void Effects::Effect::loadPozFromTarget()
{
	if (target == 1)
		trackPoz = scrainPoz;
}


void Effects::Effect::setTarget(short v)
{
	target = v;
};
void Effects::Effect::loadPointers(SDL_Rect* ScrainPoz)
{
	scrainPoz = ScrainPoz;
}
bool Effects::isRun(int index)
{
	if (index == 1)
		return curse.isLoad;
}
bool Effects::isTimerRun(int index)
{
	if (index == 1)
		return curse.rem_timer;
}