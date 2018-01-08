#ifndef OBJECT_H
#define OBJECT_H

#include"Include.h"
#include"Scrain.h"
#include"PhysicEngine.h"
#include "Events.h"
#include "ParticleSystem.h"
#include "SoundSystem.h"

class Objects
{
	class ObjectTypeRender   // odrebne renderowanie dla kazdego typu obiektu z powodu roznej ilosci klatek
	{
		bool frameLoc; // czy obiekt znajduje sie w ramce okna
		bool isTActive, t_TActive; // pierwsza dotyczy czy postaæ jest w obrêbie obiektu, druga czy skonczyla sie animacja
		int rendSpeed;
		int animSpeed = 74;
		bool passiveAnimLock = 0;
		bool darkShading = 0;
		struct specific
		{
			int id = 0;
			bool isHide = 0;
			bool endTimer = 0;
			int var1 = 0;
			int offsetY = 0;
			int c_sound = -1;
			int soundTimer = 0;
		};
		double angle;
		specific* data = new specific[1];
		int SDSize;
		bool t_a_Hnon, t_p_Hnon; // czy jest to animacja jednolinijna
		SDL_Texture* textureActive;
		SDL_Texture* texturePassive;
		SDL_Texture* textureAlt;
		SizeRect frameASize, textureASize;
		SizeRect framePSize, texturePSize;
		SDL_Rect animRect_active;
		SDL_Rect animRect_passive;
		friend class Objects;
	public:
		ObjectTypeRender();
		~ObjectTypeRender();

		void renderAlternative(SDL_Rect setting, SDL_Renderer* renderer, int id, short type);
		void renderPassive(SDL_Rect setting, SDL_Renderer* renderer, int id, short type);
		void renderActive(SDL_Rect setting, SDL_Renderer* renderer, int id, short type);
		void runAnim();
		void activeDarkShading();
		void setIsFrameLoc(bool v);
		void setTActive(bool v);
		bool getTActive();
		bool getTimeTActive();

		void setTexture(SDL_Texture* TextureActive, SDL_Texture* TexturePassive, SDL_Texture* TextureAlt);
		void querryTexture(int numAframesX, int numAframesY, int numPframesX, int numPframesY);
		
		void SDcreate(int number);
		void SDsetId(int index);
		void SDShowHideObjects();
		int SDSearchId(int id);
		int SDSearchisTimerRun();
		void SDSetHide(bool v, int id);
		bool SDisHide(int id);
		bool SDisTimerRun(int id);
		void SDendTimer(bool v, int id);
		void SDsortIds(int l, int p);
	};
	class ObjectTypeRenderNA
	{
		SDL_Texture* texture;
		struct data
		{
			int var = -1;
			int id = -1;
			bool isactive = 0;
			bool alternative = 0;
			SDL_Rect setting = {};
			int opacity = 0;
			int* particlePoz;
			int c_sound = -1;
			int soundTimer = 0;
		};
		data* storage = new data[1];
		int i_storage;
		SDL_Rect drawSet = {};
		friend class Objects;

	public:
		ObjectTypeRenderNA();
		~ObjectTypeRenderNA();
		void loadTextures(int type, int id, SDL_Renderer* renderer);
		void render(int x, int y, int w, int h, int id, SDL_Renderer* renderer, SDL_Rect* cam);
		SDL_Rect getSetting(int id);
		bool isActive(int id);
		bool isAlternative(int id);
		void setIsActive(int id);
		void setIsAlternative(int id);
		int searchId(int i);
		void createDataBase(int idNumber);
		int setIdToStorage(int i);
		void sortIds(int l, int p);
	};

	ObjectTypeRender r_bush;
	ObjectTypeRender r_flame;
	ObjectTypeRender r_longGrass;
	ObjectTypeRender r_runeStone;
	ObjectTypeRender r_bird;
	ObjectTypeRender r_thunderwall;
	ObjectTypeRender r_cyclon;

	ObjectTypeRenderNA r_levitMplatform;
	ObjectTypeRenderNA r_levitSplatform;
	ObjectTypeRenderNA r_fadedArea;
	ObjectTypeRenderNA r_magneticStone;

	bool* isDetectionSystemOff; // dzieki temu mozna ustawic czy detekcja bedzie sie zwiekszac i oddzialywac na ognika 
	bool* isDetectionSetToMax; // dzieki temu ustawiamy poscig
	bool* ghostAchivement; // osiagniecie cien, sprawdzane po wkroczeniu w end area
	bool ambientarea; // wartosc do sprawdzania czy gracz znajduje siê w ambient area
	int mwin_w = 0, mwin_h = 0;
	long* object = new long[4];
	long* objectWD = new long[4];
	bool levelends = 0;
	bool isObjActive;
	int i_obj = 0;
	int i_objWD = 0;
	int i_remObj = 1;
	int rendspeed = 0;
	int delayArea; // przetrzymuje czas (s³u¿y do odstêpów czasowych pomiêdzy aktywacj¹ area region)
	SDL_Renderer* renderer;
	SDL_Rect setting;
	SDL_Rect scrain_poz;
	Point* irlicht_poz;
	long prevScrnPozX = 0;
	long prevScrnPosY = 0;
	Scrain* scrain;
	Events* ev;
	PhysicEngine* phEngine;
	ParticleSpawn particleSpawnMenager;
	ParticleSpawn particleSpawnMenagerFront;
	SoundMenager sm;
	SDL_Rect* camRect;
public:
	int n_maxLevelFlames = 0;
	int n_getflames = 0;

	Objects(SDL_Renderer* render);
	~Objects();

	void renderObjects(SDL_Rect cam); 
	void renderBackObject(SDL_Rect cam);
	void renderBush(SDL_Rect cam, ObjectTypeRender* rbush);
	void renderLevPlatform(SDL_Rect cam, ObjectTypeRenderNA* r_levitMplatform, int i);

	// efekty:
	void BushEffect();
	void FlameEffect();
	void RuneStoneEffect(int x, int y);
	void StartObjectsSetting();

	void loadTextures();
	void createObjects();
	void loadPointers(Scrain* scrain, PhysicEngine* phEngine, Events* ev, SDL_Window* main_window, SDL_Rect* camRect, bool* isDetectionSystemOff, bool* ghostAchivement, bool*isDetectionGetMax, Point* irlicht);
	void showHidedObjects();
	void resetScrain();
	void LoadData(long data, int i);
	void LoadDataWD(long data, int i);
};

#endif