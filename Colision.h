#ifndef COLISION_H
#define COLISION_H

#include "Scrain.h"
#include"Include.h"
#include"Events.h"
#include"Camera.h"
#include"Effects.h"

/*COLISION NUMBER MEANINGS:
0 = No colide | 5 = Colide Down | 2 = Colide Up | 7 = Colide Right | 1 = Colide Left
*/

class Colision
{
	int Aline, Bline, Cline, Dline, side;
	long * colisionPozData = new long[4]; // {start x, start y, end x, end y}
	long * colisionPionData = new long[4]; // {start x, start y, end x, end y}
	long * colisionDeathLineData = new long[4];
	long * colisionHideLineData = new long[4];
	short walldetect; // 0 none | 1 left | right
	Camera* cam;
	Effects* effects;

public:
	Colision();
	~Colision();
	void AtCamera(Camera* camera);
	void loadColision(string LevelName);
	bool isIntersection(Point p1, Point p2, Point p3, Point p4);
	short isColidePlane(SDL_Rect Data, int* correction, bool type, int x1, int y1); // type 0 plane type 1 wall
	void isDeathLineColide(Scrain* scrain, Events* ev);
	void getPlaneColideCoord(SDL_Rect Data, bool type, int x1, int y1, int* side1, int* side2, int* side3); // if type 0 return x if type 1 return y
	bool isHideLineColide(int x1, int y1, SDL_Rect target);
	void checkCol(Scrain* scrain, Events* ev);
	void isWallColide(Scrain* scrain, Events* ev);
	bool isColideMob();
	short GetWallDetStatus();
	void loadPointers(Effects* effects);
};

#endif