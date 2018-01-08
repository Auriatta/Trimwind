#ifndef SCRAIN_H
#define SCRAIN_H

#include"Include.h"
#include"Effects.h"
#include"Settings.h"

class Scrain
{
	int hp, detect, m_speed, r_angle, lvl_speed;
	const int protectionTime = 3000;
	short* migniecie;
	Uint8 alpha = 0;
	Point lastSpawn;
	bool gothit;
	bool ressurection = 0;
	bool isHiden = 0;
	bool isfallen = 0;
	bool untouch = 0;
	short animSpeed;
	short AnimIl;
	SDL_Rect setting;
	SDL_Rect convSet;
	SDL_Rect drawingset;
	SDL_Rect animRect;
	int frameW, frameH;
	int textureW, textureH;
	SDL_Texture* spriteSheet;
	SDL_Texture* MoveSheet;
	SDL_Texture* IdleSheet;
	SDL_Texture* JumpSheet;
	SDL_Texture* IdleJumpSheet;
	short anim_type; // typ animacji
	short anim_cycle; 
	short jump_seq; // sekwencja ktora ma sie wykonac podczas skoku
	bool jumpKey_reduct = 0; // wymaga od gracza wcisniecia ponownie klawisza d b¹dz a po skoku
	int rendSpeed = 0;
	bool move = 1;
	int l_count; // loop counter
	SDL_Renderer* renderer;
	Point spawnPosition;
	SDL_Rect* CameraCoord;
	int* phforceY;
	SettingsControl st;

	void d_Level();
	void ifMove();

	friend class Events;
	friend class PhysicEngine;

	

public:

	Scrain(SDL_Renderer* rendererr, Events* evs, short* migniecie);
	~Scrain();
	void RenderScrain();
	void LoadScrain(int yFrames, int xFrames);

	void ifJump();

	void gotHit();
	bool isHit();
	bool isFallen();
	bool isHide();
	void setHide(bool var);
	Point getSpawn();
	bool isCheckPointSet();
	void setSpawn(int x, int y);
	void ressurectionEffect();
	bool isRessurected();

	void setStartLoc(SDL_Rect cmr, Point spawnPos);
	void getPointers(SDL_Rect *camCoord, int* phforceY);
	void setUntouch(bool var);
	bool isUntouch();
	void ResetScrain(bool side);
	void ResetAnimRect();
	void isDeath(SDL_Rect cameraPos, Effects* effects);
	void reductJumpKey();
	void stopMove();
	int GetJumpSeq();
	void SetJumpSeq(short seq);
	int GetSpeed();
	void SetSize(int w, int h);
	void GetSize(SDL_Rect* d);
	SDL_Rect GetConvPosition();
	SDL_Rect GetPosition();
	void DeleteScrain();
	void SetRotation(int angle);
	void SetPosition(int x, int y);
	void SetSpeed(int sp);
	void SetLvlSpeed(int lsp);
	int GetHP();
	void SetAnimType(int var);
	int GetAnimType();
	int GetDetect();
	void SetDetect(int var);
	void SetHP(int var);
	void resetAnimCycle(short var);
	void reductJumpKey(bool var);
	bool GetReductJumpKey_Status();
};

#endif