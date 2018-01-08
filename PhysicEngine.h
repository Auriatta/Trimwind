#ifndef PHYSENGINE_H
#define PHYSENGINE_H

#include"Include.h"
#include "Colision.h"
#include "Events.h"

struct Force
{
	bool script;
	bool side;
	int power;
	bool type; // 0 gwaltowny 1 cykliczny
};

class PhysicEngine
{
	Force forceY;
	Force forceX;
	bool hidePlaneDetect; // hide plane colision thats near by
	Point prevScrPos;
	bool rotatedMove=0;
	int correct;

	Scrain* scrain;
	Events* ev;

	friend void ifGroundFJump(SDL_Rect* r, Scrain* scrain, Colision* colision, Force* forceX, Events* ev, PhysicEngine* ph);
	friend class Game;
	void rotate(Scrain* scrain, Force* forceY);
public:
	PhysicEngine();
	~PhysicEngine();
	void loadPointers(Scrain* scr, Events* events);

	void check( Colision* colision);
	void grav();
	
	void scriptJump(int xPw, int yPw, bool side);

	bool isHidePlaneDetect();
};
#endif