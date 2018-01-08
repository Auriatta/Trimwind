#ifndef AI_H
#define AI_H
#include "Include.h"
#include "Scrain.h"
#include "Colision.h"

#define DETECT_SIZE 300
#define HUNTCHANNEL 5
#define ENDHUNTCHANEL 6

class AI
{
	class grid
	{
	public:
		grid();
		long x, y;
		int linkIndex[7];
		int N_links;
	};

	friend class Game;

	short status; // 0 szukanie najblizszego punktu, 1 patrol(szukanie gracza po siatce), 2 looking for(memory), 3 atak, 4 omijanie przeszkody po ominieciu wraca do 3
	grid* pathPoint = new grid[1];
	int n_pathPoints;
	short m_speedX, m_speedY, crrX, crrY;
	bool audioHunt;

	bool detection = true;

	#pragma region cicle Move Stuff
	double theta;
	int r = 0;
	int deley = 0, r_deley = 0;
	short endCicle;
	double minDist = -1;
	bool next = 0;
	double prevTheta = -1;
	int c_speed = 90;
	Point originGoal;
	#pragma endregion

	#pragma region PathFinding
	Point pf_goal_c; // x y celu
	int pf_goal_i; // index celu
	int* pf_checkedIds = new int[1]; // indexy punktow ktore zostaly sprawdzone
	int pf_n_checkedIds;
	int* pf_wayIds = new int[1]; // œciezka do celu
	int pf_n_wayIds;
	int pf_actualWayPoint = 0;
	int pf_nextId = -1;
	#pragma endregion

	#pragma region Detect Level System
	int detect_regionSize;
	Uint32 detect_deley = NULL;
	#pragma endregion

	#pragma region AI Stuff
	Uint32 DeleybtwCatch = NULL;
	int waittime = 0;
	SDL_Rect rgoal;
	SDL_Rect detectionLine;
	bool scanForAv = 0;
	short moveType = 0;
	int correction = 0;
	bool isSCTurnBack = 0;
	short irlichtLookSide = 0;
	bool irlichtTurn = 0;
	long prevMoveX = 0;
	Uint32 timeHock = NULL;
	Uint32 timePassed = NULL;
	#pragma endregion
	bool firstRun;
	bool ghostAchivement;
	bool setFullDetection = 0;
	bool isOffLevel = 0; // jesli poziom nie posiada wszedzie rozmieszczonych punktow ai
	float hypx, hypy, dist;
	int actualPPId;
	int prevPPid = 0;
	Point point; // point mouse/touchpad/ai
	Point spawn;
	Point WorldPoint; // point - cameracoord;
	Point goal;
	Scrain* scrain;
	Colision* col;
	Camera* cam;
	SDL_Rect* camr;
	SDL_Renderer* renderer;
	SettingsControl st;
	void SetCircleMoves();
	int GetRandPointFromPull();
	int PullSearch(int var, bool type);
	void setCorrection();
	void MakeWorldPoint();
public:
	AI();
	~AI();

	void CPUControl();
	void AICheckPlane();
	void AIcicleMove();
	void AIfindNearPoint(bool target); // 0 = near irlicht | 1 = near scrain
	void AiAvoidObstacles();
	void AImoveToPoint();
	bool AIFindPathTo(int index, int x, int y, bool mode); // 0 mode = index | 1 mode = x,y do zlozonego szukania na podstawie detekcji
	void AIQuickFindPathTo(); // do szukania po zgubieniu scraina za sciana

	int ComparePointsSearch(bool type); 

	Point* GetPoint();

	void loadGrid();
	void setPoint(int x, int y);

	void getPointers(Scrain* scrain, Colision* col, Camera* cam, SDL_Renderer* renderer);
};

#endif