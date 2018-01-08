#include"Include.h"
#include"Camera.h"

class Irlicht
{
	Point* point;
	SDL_Rect RectPoint;
	Point prevPoint;
	SDL_Rect parts_poz[6];
	SDL_Rect drawn_setting[6];
	double parts_angle[6];
	double prevAngle[2];
	int dist;
	long* spawnDataX = new long[1];
	long* spawnDataY = new long[1];
	int n_spawns;
	SDL_Texture* core_down, *core_mid, *core_top,
		*tong_down, *tong_mid, *tong_top;
	SizeRect coreTextureS, coreFrame;
	SizeRect tongTextureS, tongFrame;
	SDL_Rect animRectCore, animRectTong;
	SDL_Renderer* renderer;
	int rendSpeed;
	int state = 0;
	Point prev[2];
	Camera* cam;
	SoundMenager sm;

public:
	
	Irlicht(SDL_Renderer* renderer);
	~Irlicht();
	void followPoint();
	void spawn(int spId); // if -1 then random
	void destroy();
	void load();
	void runAnim();
	void render(SDL_Rect cam);
	void loadSpawnsPoints(long* enemyX, long* enemyY, int nSpawn);
	void GetPointers(Camera* cam);
	void SetPointerToPoint(Point* point);
	SDL_Rect* GetPositionPointer();
};