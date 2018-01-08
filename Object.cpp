#include "Object.h"
#include"SDLStuff.h"
#include"Settings.h"

// dlugosci dzwieków w ms
#define STONEHIT_LENGHT 2000
#define FLAMEACTIVE_LENGHT 2000
#define PLATFORM_LENGHT 5000
#define RUNESTONE_LENGHT 4000

#define BUSHCHANNEL 4
#define UNLOCKRUNECHANNEL 0

#define DELAYAREA 1000

Objects::Objects(SDL_Renderer* render)
{
	renderer = render;
	i_obj = 4;
	memset(object, 0, 4 + sizeof(long));
	i_objWD = 4;
	memset(objectWD, 0, 4 + sizeof(long));
}

Objects::~Objects()
{
	delete[] object;
	object = nullptr;
	delete[] objectWD;
	objectWD = nullptr;
	camRect = nullptr;
	delete camRect;
	renderer = nullptr;
	delete renderer;
	phEngine = nullptr;
	delete phEngine;

	isDetectionSystemOff = nullptr;
	delete isDetectionSystemOff;
	ghostAchivement = nullptr;
	delete ghostAchivement;
	irlicht_poz = nullptr;
	delete irlicht_poz;
	isDetectionSetToMax = nullptr;
	delete isDetectionSetToMax;
}

void Objects::StartObjectsSetting()
{
	int chance = 0;
	for (int i = 0; i < r_runeStone.SDSize; i++)
	{
		chance = (rand() % 100) + 0;
		if (chance > 49)
			r_runeStone.data[i].var1 = (rand() % 26) + 5;
		else
			r_runeStone.data[i].var1 = -((rand() % 26) + 5);
		if (r_runeStone.data[i].var1 < 11 && r_runeStone.data[i].var1 > -11)
			r_runeStone.data[i].var1 = 0;
		r_runeStone.data[i].offsetY = abs(r_runeStone.data[i].var1);
	}
	for (int i = 0; i < i_objWD; i+=5)
	{
		if (objectWD[i] == 8)
		{
			r_cyclon.data[r_cyclon.SDSearchId(i)].offsetY = objectWD[i + 1];
			r_cyclon.data[r_cyclon.SDSearchId(i)].var1 = 160;
		}
		if (objectWD[i] == 11)
		{
			r_magneticStone.storage[r_magneticStone.searchId(i)].setting.y = objectWD[i + 2];
			objectWD[i + 4] -= 43;
		}
	}
	r_cyclon.animSpeed = 24;
}
void Objects::loadTextures()
{
	short state = 0;
	int n_types[4] = {0,0,0,0}; // pamietaj by dodac o jeden jesli dodajesz nowy przedmiot i wywolac createSpecificData na koncu tej funkcj
	int n_typeWD[6] = {0,0,0,0,0,0};
	while (true)
	{
		#pragma region NormalObjects
		for (int i = 0; i < i_obj; i += 3)
		{
			switch (object[i])
			{
			case 1:
			{
					SDL_RWops *file = SDL_RWFromFile("graphics/bush2ActiveAnim.png", "rb");
					SDL_RWops *file1 = SDL_RWFromFile("graphics/bush2PassiveAnim.png", "rb");
					r_bush.setTexture(LoadTextureRW(file, renderer), LoadTextureRW(file1, renderer), NULL);
					r_bush.querryTexture(4, 1, 15, 1);
					file = nullptr; file1 = nullptr;
					delete file; delete file1;
					sm.loadSound("Bush2", 0);
					sm.loadSound("Bush1", 0);
					sm.loadSound("Bush3", 0);
				break;
			}
			case 2:
			{
				if (state == 0)
				{
					SDL_RWops *file = SDL_RWFromFile("graphics/flameActiveAnim.png", "rb");
					SDL_RWops *file1 = SDL_RWFromFile("graphics/flamePassiveAnim.png", "rb");
					SDL_RWops *file2 = SDL_RWFromFile("graphics/flamePassiveAnimAlt.png", "rb");
					r_flame.setTexture(LoadTextureRW(file, renderer), LoadTextureRW(file1, renderer), LoadTextureRW(file2, renderer));
					r_flame.querryTexture(5, 1, 9, 1);
					n_types[0] += 1;
					file = nullptr; file1 = nullptr; file2 = nullptr;
					delete file; delete file1; delete file2;
					sm.loadSound("FlameActive", 0);
					sm.loadSound("FlameSound", 0);
				}
				else
				{
					r_flame.SDsetId(i);
					n_maxLevelFlames+=1;
				}
				break;
			}
			case 3:
			{
					SDL_RWops *file = SDL_RWFromFile("graphics/bushActiveAnim.png", "rb");
					SDL_RWops *file1 = SDL_RWFromFile("graphics/bushPassiveAnim.png", "rb");
					r_longGrass.activeDarkShading();
					r_longGrass.setTexture(LoadTextureRW(file, renderer), LoadTextureRW(file1, renderer), NULL);
					r_longGrass.querryTexture(5, 1, 6, 1);
					file = nullptr; file1 = nullptr;
					delete file; delete file1;
					sm.loadSound("Bush1", 0);
					sm.loadSound("Bush2", 0);
					sm.loadSound("Bush3", 0);
				break;
			}
			
			case 4:
			{
				if (state == 0)
				{
					SDL_RWops *file = SDL_RWFromFile("graphics/RuneStoneActiveAnim.png", "rb");
					SDL_RWops *file1 = SDL_RWFromFile("graphics/RuneStonePassive.png", "rb");
					SDL_RWops *file2 = SDL_RWFromFile("graphics/RuneStonePassiveAnimationAlt.png", "rb");
					r_runeStone.activeDarkShading();
					r_runeStone.setTexture(LoadTextureRW(file, renderer), LoadTextureRW(file1, renderer), LoadTextureRW(file2, renderer));
					r_runeStone.querryTexture(24, 1, 13, 1);
					n_types[1] += 1;
					sm.loadSound("RuneStone", 0);
					sm.loadSound("RuneStoneUnlock", 0);
					file = nullptr; file1 = nullptr; file2 = nullptr;
					delete file; delete file1; delete file2;
					
				}
				else
				{
					r_runeStone.SDsetId(i);
				}
				break;
			}
			case 7:
			{
				if (state == 0)
				{
					SDL_RWops *file = SDL_RWFromFile("graphics/BirdAnimActive.png", "rb");
					SDL_RWops *file1 = SDL_RWFromFile("graphics/BirdAnimPassive.png", "rb");
					r_bird.setTexture(LoadTextureRW(file, renderer), LoadTextureRW(file1, renderer), NULL);
					r_bird.querryTexture(10, 1, 10, 1);
					n_types[2] += 1;
					file = nullptr; file1 = nullptr;
					delete file; delete file1;
					sm.loadSound("Bird", 0);
				}
				else
				{
					r_bird.SDsetId(i);
				}
				break;
			}
			
			case 10:
			{
				if (state == 0)
				{
					SDL_RWops *file = SDL_RWFromFile("graphics/thunderwallPassiveAnim.png", "rb");
					r_thunderwall.setTexture(NULL , LoadTextureRW(file, renderer), NULL);
					r_thunderwall.querryTexture(0, 0, 8, 1);
					n_types[3] += 1;
					file = nullptr;
					delete file;
					r_thunderwall.animSpeed = 34;
					sm.loadSound("Barrier", 0);
				}
				else
				{
					r_thunderwall.SDsetId(i);
				}
				break;
			}
			default:
				break;
			}

		}
		#pragma endregion

		#pragma region ObjectExtended
		for (int i = 0; i < i_objWD; i += 5)
		{
			switch (objectWD[i])
			{
			case 5:
			{
				if (state == 0)
				{
					n_typeWD[0] += 1;
				}
				else
				{
					r_levitMplatform.setIdToStorage(i);
					r_levitMplatform.loadTextures(1, r_levitMplatform.searchId(i), renderer);
					sm.loadSound("Platform", 0);
				}
				break;
			}
			case 6:
			{
				if (state == 0)
				{
					n_typeWD[1] += 1;
				}
				else
				{
					r_levitSplatform.setIdToStorage(i);
					r_levitSplatform.loadTextures(2, r_levitSplatform.searchId(i), renderer);
					sm.loadSound("Platform", 0);
				}
				break;
			}
			case 9:
			{
				if (state == 1)
				{
					objectWD[i + 2] = abs(objectWD[i + 2] - objectWD[i + 4]);
				}
				switch (objectWD[i+1])
				{
				case 8:
				{
					sm.loadSound("Cave", 1);
					break;
				}
				case 4:
				{
					sm.loadSound("DirtParticle", 0);
					break;
				}
				case 5:
				{
					sm.loadSound("DirtParticle", 0);
					break;
				}
				case 3:
				{
					if (state == 0)
					{
						n_typeWD[2] += 1;
					}
					else
					{
						r_fadedArea.setIdToStorage(i);
						r_fadedArea.loadTextures(3, r_fadedArea.searchId(i), renderer);
					}
					break;
				}
				}
				break;
			}
			case 8:
			{
				if (state == 0)
				{
					SDL_RWops *file = SDL_RWFromFile("graphics/cyclonPassiveAnim.png", "rb");
					r_cyclon.setTexture(NULL, LoadTextureRW(file, renderer), NULL);
					r_cyclon.querryTexture(0, 0, 7, 1);
					n_typeWD[3] += 1;
					file = nullptr;
					delete file;
					sm.loadSound("Cyclon", 0);
					sm.loadSound("CyclonOpen", 0);
					sm.loadSound("CyclonClose", 0);
				}
				else
				{
					r_cyclon.SDsetId(i);
				}
				break;
			}
			case 11:
			{
				if (state == 0)
				{
					n_typeWD[4] += 1;
				}
				else
				{
					r_magneticStone.setIdToStorage(i);
					r_magneticStone.loadTextures(4, r_magneticStone.searchId(i), renderer);
					sm.loadSound("StoneHit1", 0);
					sm.loadSound("StoneHit2", 0);
				}
				break;
			}
			}
		}
		#pragma endregion

		if (state == 0)
		{
			if (n_typeWD[0] != 0)
				r_levitMplatform.createDataBase(n_typeWD[0]);
			if (n_typeWD[1] != 0)
				r_levitSplatform.createDataBase(n_typeWD[1]);
			if (n_typeWD[2] != 0)
				r_fadedArea.createDataBase(n_typeWD[2]);
			if (n_typeWD[3] != 0)
				r_cyclon.SDcreate(n_typeWD[3]);
			if (n_typeWD[4] != 0)
				r_magneticStone.createDataBase(n_typeWD[4]);
			if (n_types[0] != 0)
				r_flame.SDcreate(n_types[0]);
			if (n_types[1] != 0)
				r_runeStone.SDcreate(n_types[1]);
			if (n_types[2] != 0)
				r_bird.SDcreate(n_types[2]);
			if (n_types[3] != 0)
				r_thunderwall.SDcreate(n_types[3]);
		}
		

		if (state == 1)
		{
			if (n_typeWD[0] != 0 && n_typeWD[0] - 1 != 0)
				r_levitMplatform.sortIds(0, n_typeWD[0]-1);
			if (n_typeWD[1] != 0 && n_typeWD[1] - 1 != 0)
				r_levitSplatform.sortIds(0, n_typeWD[1]-1);
			if (n_typeWD[2] != 0 && n_typeWD[2] - 1 != 0)
				r_fadedArea.sortIds(0, n_typeWD[2] - 1);
			if (n_typeWD[4] != 0 && n_typeWD[4] - 1 != 0)
				r_magneticStone.sortIds(0, n_typeWD[4] - 1);
			if (n_types[0] != 0 && n_types[0] - 1 != 0)
				r_flame.SDsortIds(0, n_types[0]);
			if (n_typeWD[3] != 0 && n_typeWD[3] - 1 != 0)
				r_cyclon.SDsortIds(0, n_typeWD[3]);
			if (n_types[1] != 0 && n_types[1] - 1 != 0)
				r_runeStone.SDsortIds(0, n_types[1]);
			if (n_types[2] != 0 && n_types[2] - 1 != 0)
				r_bird.SDsortIds(0, n_types[2]);
			if (n_types[3] != 0 && n_types[3] - 1 != 0)
				r_thunderwall.SDsortIds(0, n_types[3]);
			StartObjectsSetting();
			break;
		}
		state = 1;
	}

	for (int i = 0; i < i_objWD; i += 5) // sprawdzenie czy konkretny typ obiektu nie znajduje sie pod hidearea
	{
		if (objectWD[i] == 9 && objectWD[i + 1] == 3)
		{
			for (int j = 0; j < i_obj; j += 3)
			{
				if (object[j] == 2)
				{
					
					if (object[j + 1]-15 > objectWD[i + 3] && object[j+1]-15 < objectWD[i+3] + objectWD[i+2] &&
						object[j + 2] - 43 > objectWD[i + 4] - objectWD[i + 2] && object[j + 2] - 43 < objectWD[i + 4])
					{
						r_fadedArea.storage[r_fadedArea.searchId(i)].var = j;
						break;
					}
				}
			}
		}
	}
}

void Objects::renderObjects(SDL_Rect cam)
{
	scrain_poz = scrain->GetPosition();
	r_thunderwall.setIsFrameLoc(false);
	r_bush.setIsFrameLoc(false);
	r_flame.setIsFrameLoc(false);
	r_longGrass.setIsFrameLoc(false);
	r_bush.setTActive(false);
	r_flame.setTActive(false);
	r_longGrass.setTActive(false);


	for (int i = 0; i < i_objWD; i += 5)
	{
		if (objectWD[i + 3] - cam.x - 150 < scrain_poz.x - cam.x + (mwin_w - (mwin_w / 4)) && objectWD[i + 3] - cam.x > scrain_poz.x - cam.x - (mwin_w - (mwin_w / 4))) // renderuj tylko obiekty w ramce okna
		{
			switch (objectWD[i])
			{

			case 9: // event area
			{
				switch (objectWD[i + 1]) // typ eventu
				{
				case 3:
				{
					//fog area
					if ((scrain_poz.x + scrain_poz.w / 2) - cam.x > objectWD[i + 3] - cam.x && (scrain_poz.x + scrain_poz.w / 2) - cam.x  < (objectWD[i + 3] - cam.x) + objectWD[i + 2] &&
						scrain_poz.y - cam.y >(objectWD[i + 4] - cam.y) - objectWD[i + 2] && scrain_poz.y - cam.y < objectWD[i + 4] - cam.y)
					{
						if (r_fadedArea.storage[r_fadedArea.searchId(i)].opacity > 0)
						{
							r_fadedArea.storage[r_fadedArea.searchId(i)].opacity -= 8;
							if (r_fadedArea.storage[r_fadedArea.searchId(i)].opacity < 0)
								r_fadedArea.storage[r_fadedArea.searchId(i)].opacity = 0;
							if (r_fadedArea.storage[r_fadedArea.searchId(i)].opacity < 100 &&
								r_fadedArea.storage[r_fadedArea.searchId(i)].var != -1 &&
								r_fadedArea.storage[r_fadedArea.searchId(i)].opacity > 85)
								r_flame.data[r_flame.SDSearchId(r_fadedArea.storage[r_fadedArea.searchId(i)].var)].var1 = 0;
							SDL_SetTextureAlphaMod(r_fadedArea.texture, r_fadedArea.storage[r_fadedArea.searchId(i)].opacity);
						}
					}
					else
					{
						if (r_fadedArea.storage[r_fadedArea.searchId(i)].opacity != 255)
						{
							if (r_fadedArea.storage[r_fadedArea.searchId(i)].opacity < 255)
							{
								r_fadedArea.storage[r_fadedArea.searchId(i)].opacity += 8;
								if (r_fadedArea.storage[r_fadedArea.searchId(i)].opacity > 255)
									r_fadedArea.storage[r_fadedArea.searchId(i)].opacity = 255;
								if (r_fadedArea.storage[r_fadedArea.searchId(i)].opacity > 100 &&
									r_fadedArea.storage[r_fadedArea.searchId(i)].var != -1 &&
									r_fadedArea.storage[r_fadedArea.searchId(i)].opacity < 115)
									r_flame.data[r_flame.SDSearchId(r_fadedArea.storage[r_fadedArea.searchId(i)].var)].var1 = 1;
								SDL_SetTextureAlphaMod(r_fadedArea.texture, r_fadedArea.storage[r_fadedArea.searchId(i)].opacity);
							}
						}
					}
					if (r_fadedArea.storage[r_fadedArea.searchId(i)].opacity > 0)
						r_fadedArea.render(objectWD[i + 3], objectWD[i + 4] - objectWD[i + 2], objectWD[i + 2], objectWD[i + 2], r_fadedArea.searchId(i), renderer, &cam);
					break;
				}
				default:
					break;
				}
				break;
			}

			case 11: // magnetic stone
			{
				if (r_magneticStone.storage[r_magneticStone.searchId(i)].soundTimer + STONEHIT_LENGHT < SDL_GetTicks() && r_magneticStone.storage[r_magneticStone.searchId(i)].c_sound != -1 ||
					r_magneticStone.storage[r_magneticStone.searchId(i)].soundTimer == 0 && r_magneticStone.storage[r_magneticStone.searchId(i)].c_sound != -1)
				{
					r_magneticStone.storage[r_magneticStone.searchId(i)].c_sound = -1;
					r_magneticStone.storage[r_magneticStone.searchId(i)].soundTimer = 0;
				}

				if (r_magneticStone.storage[r_magneticStone.searchId(i)].setting.y == objectWD[i + 2] || r_magneticStone.storage[r_magneticStone.searchId(i)].var == -(objectWD[i + 1])) // jesli jest na górze
				{
					if (r_magneticStone.storage[r_magneticStone.searchId(i)].var == -(objectWD[i + 1]))
					{
						r_magneticStone.storage[r_magneticStone.searchId(i)].setting.y += 8;
						if (r_magneticStone.storage[r_magneticStone.searchId(i)].setting.y > objectWD[i + 4])
						{
							r_magneticStone.storage[r_magneticStone.searchId(i)].setting.y = objectWD[i + 4];

							if (r_magneticStone.storage[r_magneticStone.searchId(i)].setting.y - cam.y > (scrain_poz.y - cam.y) - 500 && r_magneticStone.storage[r_magneticStone.searchId(i)].setting.y - cam.y < (scrain_poz.y - cam.y) + 500) // renderuj tylko obiekty w ramce okna
							{
								r_magneticStone.storage[r_magneticStone.searchId(i)].c_sound = sm.getFreeChannel();
								sm.addSound("StoneHit1", r_magneticStone.storage[r_magneticStone.searchId(i)].c_sound, 0);
								r_magneticStone.storage[r_magneticStone.searchId(i)].soundTimer = SDL_GetTicks();
							}
						}
					}
					else
					{
						if (r_magneticStone.storage[r_magneticStone.searchId(i)].var <= 0)
						{
							r_magneticStone.storage[r_magneticStone.searchId(i)].var = -(objectWD[i + 1]);
						}
						else
						{
							r_magneticStone.storage[r_magneticStone.searchId(i)].var -= 1;
						}
					}
				}
				if (r_magneticStone.storage[r_magneticStone.searchId(i)].setting.y == objectWD[i + 4] || r_magneticStone.storage[r_magneticStone.searchId(i)].var == objectWD[i + 1]) // jeœli jest na dole
				{

					if (r_magneticStone.storage[r_magneticStone.searchId(i)].var == objectWD[i + 1])
					{
						r_magneticStone.storage[r_magneticStone.searchId(i)].setting.y -= 8;
						if (r_magneticStone.storage[r_magneticStone.searchId(i)].setting.y < objectWD[i + 2])
						{
							r_magneticStone.storage[r_magneticStone.searchId(i)].setting.y = objectWD[i + 2];
							particleSpawnMenager.spawn(objectWD[i + 3], objectWD[i + 2], 0, 0, 100, 160, 3, 1, renderer, camRect);

							if (r_magneticStone.storage[r_magneticStone.searchId(i)].setting.y - cam.y >(scrain_poz.y - cam.y) - 500 && r_magneticStone.storage[r_magneticStone.searchId(i)].setting.y - cam.y < (scrain_poz.y - cam.y) + 500) // renderuj tylko obiekty w ramce okna
							{
								r_magneticStone.storage[r_magneticStone.searchId(i)].c_sound = sm.getFreeChannel();
								sm.addSound("StoneHit2", r_magneticStone.storage[r_magneticStone.searchId(i)].c_sound, 0);
								r_magneticStone.storage[r_magneticStone.searchId(i)].soundTimer = SDL_GetTicks();
							}
						}
					}
					else
					{
						if (r_magneticStone.storage[r_magneticStone.searchId(i)].var >= 0)
						{
							r_magneticStone.storage[r_magneticStone.searchId(i)].var = objectWD[i + 1];
						}
						else
						{
							r_magneticStone.storage[r_magneticStone.searchId(i)].var += 1;
						}
					}
				}
				r_magneticStone.render(objectWD[i + 3], r_magneticStone.storage[r_magneticStone.searchId(i)].setting.y, 160, 43, r_magneticStone.searchId(i), renderer, &cam);
				setting = r_magneticStone.storage[r_magneticStone.searchId(i)].setting;
				setting.x -= cam.x;
				setting.y -= cam.y - 40;
				setting.w -= 30;
				setting.h -= 40;
				if (SDL_HasIntersection(&scrain->GetConvPosition(), &setting) == SDL_TRUE)
				{
					StatisticsControl sc;
					sc.setStat(3, sc.getStat(3) + 1);

					scrain->SetHP(0);
				}

				if (r_magneticStone.storage[r_magneticStone.searchId(i)].c_sound != -1)
					sm.changeSoundPos(r_magneticStone.storage[r_magneticStone.searchId(i)].c_sound, setting.x);

				break;
			}
			default:
				break;
			}
		}
	}


	for (int i = 0; i < i_obj; i += 3)
	{
		switch (object[i])
		{
		case 1:
		{
			if (object[i + 1] - cam.x - 150 < scrain_poz.x - cam.x + (mwin_w - (mwin_w / 4)) && object[i + 1] - cam.x > scrain_poz.x - cam.x - (mwin_w - (mwin_w / 4))) // renderuj tylko obiekty w ramce okna
			{
				setting.w = 122;
				setting.h = 75;
				setting.x = object[i + 1] - cam.x;
				setting.y = object[i + 2] - cam.y;
				setting.x -= setting.w / 2;
				setting.y -= setting.h / 2;
				//scrain_poz.x += SCRAINW/2;

				renderBush(cam, &r_bush);
				//scrain_poz.x += SCRAINW;
			}
			break;
		}

		case 2:
		{
			if (object[i + 1] - cam.x - 150 < scrain_poz.x - cam.x + (mwin_w - (mwin_w / 4)) && object[i + 1] - cam.x > scrain_poz.x - cam.x - (mwin_w - (mwin_w / 4)) &&
				object[i + 2] - cam.y > (scrain_poz.y - cam.y) - 400 && object[i + 2] - cam.y < (scrain_poz.y - cam.y) + 400) // renderuj tylko obiekty w ramce okna
			{
				if (r_flame.data[r_flame.SDSearchId(i)].soundTimer + FLAMEACTIVE_LENGHT < SDL_GetTicks() && r_flame.data[r_flame.SDSearchId(i)].soundTimer != 0)
				{
					r_flame.data[r_flame.SDSearchId(i)].c_sound = -1;
					r_flame.data[r_flame.SDSearchId(i)].soundTimer = 0;
				}
				if (r_flame.data[r_flame.SDSearchId(i)].var1 == 0) // chowanie p³omyka gdy jest pod hide area
				{
					//if (inCover.x<object[i+1] && inCover.x + inCover.w < object[i+1] && inCover.y < object[i+1])
					//{
					setting.w = 25;
					setting.h = 86;
					setting.x = object[i + 1] - cam.x;
					setting.y = object[i + 2] - cam.y;
					setting.x -= setting.w / 2;
					setting.y -= setting.h / 2;

					if (scrain_poz.x - cam.x > (setting.x - (setting.w)) && scrain_poz.x - cam.x < (setting.x + (setting.w)) && scrain_poz.y - cam.y < (setting.y + setting.h) && scrain_poz.y - cam.y >(setting.y - (setting.h / 2))
						&& r_flame.SDisHide(r_flame.SDSearchId(i)) == false)
					{
						if (r_flame.getTimeTActive() == false && r_flame.SDisTimerRun(r_flame.SDSearchId(i)) == false)
						{
							sm.stopSound(r_flame.data[r_flame.SDSearchId(i)].c_sound);
							r_flame.data[r_flame.SDSearchId(i)].c_sound = sm.getFreeChannel();
							sm.addSound("FlameActive", r_flame.data[r_flame.SDSearchId(i)].c_sound, 0);
							r_flame.data[r_flame.SDSearchId(i)].soundTimer = SDL_GetTicks();
							FlameEffect();
							r_flame.setTActive(true);
							r_flame.SDendTimer(true, r_flame.SDSearchId(i));
						}
					}
					else
					{
						if (r_flame.SDisHide(r_flame.SDSearchId(i)) == true && r_flame.SDisTimerRun(r_flame.SDSearchId(i)) == false)
						{
							r_flame.renderAlternative(setting, renderer, -1, -1);
						}
						else
							if (r_flame.SDisTimerRun(r_flame.SDSearchId(i)) == false && r_flame.SDisHide(r_flame.SDSearchId(i)) == false)
							{
								r_flame.renderPassive(setting, renderer, -1, -1);

								if (r_flame.data[r_flame.SDSearchId(i)].c_sound == -1)
								{
									r_flame.data[r_flame.SDSearchId(i)].c_sound = sm.getFreeChannel();
									sm.addSound("FlameSound", r_flame.data[r_flame.SDSearchId(i)].c_sound, -1);
								}
								sm.changeSoundPos(r_flame.data[r_flame.SDSearchId(i)].c_sound, setting.x);
							}

					}

					if (r_flame.SDisTimerRun(r_flame.SDSearchId(i)) == true)
					{
						r_flame.renderActive(setting, renderer, -1, -1);
						sm.changeSoundPos(r_flame.data[r_flame.SDSearchId(i)].c_sound, setting.x);
					}
					r_flame.setIsFrameLoc(true);

					//}
				}
			}
			else
			{
				if (r_flame.data[r_flame.SDSearchId(i)].c_sound != -1)
				{
					sm.stopSound(r_flame.data[r_flame.SDSearchId(i)].c_sound);
					r_flame.data[r_flame.SDSearchId(i)].c_sound = -1;
					r_flame.data[r_flame.SDSearchId(i)].soundTimer = 0;
				}
			}
			break;

		}

		case 3:
		{
			if (object[i + 1] - cam.x - 150 < scrain_poz.x - cam.x + (mwin_w - (mwin_w / 4)) && object[i + 1] - cam.x > scrain_poz.x - cam.x - (mwin_w - (mwin_w / 4))) // renderuj tylko obiekty w ramce okna
			{
				setting.w = 150;
				setting.h = 120;
				setting.x = object[i + 1] - cam.x;
				setting.y = object[i + 2] - cam.y;
				setting.x -= setting.w / 2;
				setting.y -= setting.h / 2;

				renderBush(cam, &r_longGrass);

			}
			break;
		}

		case 10:
		{
			if (object[i + 1] - cam.x - 150 < scrain_poz.x - cam.x + (mwin_w - (mwin_w / 4)) && object[i + 1] - cam.x > scrain_poz.x - cam.x - (mwin_w - (mwin_w / 4)) &&
				object[i + 2] - cam.y > (scrain_poz.y - cam.y) - 400 && object[i + 2] - cam.y < (scrain_poz.y - cam.y) + 400) // renderuj tylko obiekty w ramce okna
			{
				setting.w = 60;
				setting.h = 250;
				setting.x = object[i + 1] - cam.x;
				setting.y = object[i + 2] - cam.y;
				if (scrain_poz.x - cam.x > (setting.x - scrain->GetPosition().w) && scrain_poz.x - cam.x < (setting.x + scrain->GetPosition().w) &&
					scrain_poz.y - cam.y < (setting.y + setting.h) && scrain_poz.y - cam.y >(setting.y))
				{
					if (n_maxLevelFlames > n_getflames)
					{
						scrain->SetHP(0);
					}
				}
				if (n_maxLevelFlames > n_getflames)
				{
					if (r_thunderwall.data[r_thunderwall.SDSearchId(i)].c_sound == -1)
					{
						r_thunderwall.data[r_thunderwall.SDSearchId(i)].c_sound = sm.getFreeChannel();
						sm.addSound("Barrier", r_thunderwall.data[r_thunderwall.SDSearchId(i)].c_sound, -1);
					}
					else
						sm.changeSoundPos(r_thunderwall.data[r_thunderwall.SDSearchId(i)].c_sound, setting.x);
					r_thunderwall.renderPassive(setting, renderer, -1, -1);
				}
				else
				{
					if (r_thunderwall.data[r_thunderwall.SDSearchId(i)].c_sound != -1)
					{
						sm.stopSound(r_thunderwall.data[r_thunderwall.SDSearchId(i)].c_sound);
						r_thunderwall.data[r_thunderwall.SDSearchId(i)].c_sound = -1;
					}
					particleSpawnMenager.spawn(object[i + 1], object[i + 2] + setting.h, 0, 0, -1, 2, 5, 4, renderer, camRect);
				}
				r_thunderwall.setIsFrameLoc(true);
			}
			else
			{
				if (r_thunderwall.data[r_thunderwall.SDSearchId(i)].c_sound != -1)
				{
					sm.stopSound(r_thunderwall.data[r_thunderwall.SDSearchId(i)].c_sound);
					r_thunderwall.data[r_thunderwall.SDSearchId(i)].c_sound = -1;
				}
			}

			break;
		}
		default:
			break;
		}

		//else
		//cout << "no render " << i << endl;
	}


	r_bush.runAnim();
	r_flame.runAnim();
	r_longGrass.runAnim();
	r_thunderwall.runAnim();
	
	particleSpawnMenagerFront.update();
}
void Objects::renderBackObject(SDL_Rect cam)
{
	scrain_poz = scrain->GetPosition();
	scrain_poz.x += SCRAINW / 2;
	//r_cyclon.setIsFrameLoc(false);
	//r_cyclon.setTActive(false);
	r_runeStone.setIsFrameLoc(false);
	r_runeStone.setTActive(false);
	r_bird.setIsFrameLoc(false);
	r_bird.setTActive(false);
	ambientarea = false;

	for (int i = 0; i < i_obj; i += 3)
	{
			//cout << "render " << i << endl;
			switch (object[i])
			{
			case 7: // ptak
			{
				if (object[i + 1] - cam.x - 150 < scrain_poz.x - cam.x + (mwin_w - (mwin_w / 4)) && object[i + 1] - cam.x > (scrain_poz.x - cam.x) - (mwin_w - (mwin_w / 4)) &&
					object[i + 2] - cam.y >(scrain_poz.y - cam.y) - 100 && object[i + 2] - cam.y < (scrain_poz.y - cam.y) + 100) // renderuj tylko obiekty w ramce okna
				{
					if (r_bird.data[r_bird.SDSearchId(i)].var1 != -1)
					{
						setting.w = 39;
						setting.h = 26;
						setting.x = object[i + 1] - cam.x;
						setting.y = object[i + 2] - cam.y;
						
						if (scrain_poz.x - cam.x > (setting.x - 200) && scrain_poz.x - cam.x < (setting.x + 200)
							|| r_bird.data[r_bird.SDSearchId(i)].var1 != NULL)
						{
							if (r_bird.getTimeTActive() == false)
							{
								if (r_bird.data[r_bird.SDSearchId(i)].var1 == NULL)
								{
									r_bird.animSpeed = 8;
									r_bird.data[r_bird.SDSearchId(i)].var1 = object[i + 1];
									r_bird.data[r_bird.SDSearchId(i)].offsetY = object[i + 2];
									sm.addSound("Bird", sm.getFreeChannel(), 0);
								}
								r_bird.setTActive(true);
							}
						}
						else
						{
							r_bird.renderPassive(setting, renderer, -1, -1);
						}

						if (r_bird.data[r_bird.SDSearchId(i)].var1 != NULL)
						{
							r_bird.data[r_bird.SDSearchId(i)].var1 -= 3;
							r_bird.data[r_bird.SDSearchId(i)].offsetY -= 2;
							setting.x = r_bird.data[r_bird.SDSearchId(i)].var1 - cam.x;
							setting.y = r_bird.data[r_bird.SDSearchId(i)].offsetY - cam.y;
							if (setting.x < scrain_poz.x - cam.x + (mwin_w - (mwin_w / 4)) && setting.x >(scrain_poz.x - cam.x) - (mwin_w - (mwin_w / 4))
								&& setting.y + setting.h > -(setting.h))
							{
								r_bird.renderActive(setting, renderer, -1, -1);
							}
							else
							{
								r_bird.animSpeed = 74;
								r_bird.data[r_bird.SDSearchId(i)].offsetY = -1;
								r_bird.data[r_bird.SDSearchId(i)].var1 = -1;
							}
						}
						r_bird.setIsFrameLoc(true);
					}
				}
				break;
			}
			case 4:
			{
				if (object[i + 1] - cam.x - 150 < scrain_poz.x - cam.x + (mwin_w - (mwin_w / 4)) && object[i + 1] - cam.x > (scrain_poz.x - cam.x) - (mwin_w - (mwin_w / 4))) // renderuj tylko obiekty w ramce okna
				{
					if (r_runeStone.data[r_runeStone.SDSearchId(i)].soundTimer + RUNESTONE_LENGHT < SDL_GetTicks() && r_runeStone.data[r_runeStone.SDSearchId(i)].soundTimer != 0)
					{
						r_runeStone.data[r_runeStone.SDSearchId(i)].c_sound = -1;
						r_runeStone.data[r_runeStone.SDSearchId(i)].endTimer = 0;
					}
					if (object[i + 2] - cam.y < scrain_poz.y - cam.y + (mwin_h - (mwin_h / 2)) && object[i + 2] - cam.y >scrain_poz.y - cam.y - (mwin_h - (mwin_h / 2))) // renderuj tylko obiekty w ramce okna
					{
						setting.w = 104;
						setting.h = 140;
						setting.x = object[i + 1] - cam.x;
						setting.y = object[i + 2] - cam.y;
						setting.x -= setting.w / 2;
						setting.y -= setting.h / 2;
						setting.y += r_runeStone.data[r_runeStone.SDSearchId(i)].offsetY;
						
						if (scrain_poz.x - cam.x > (setting.x - 70) && scrain_poz.x - cam.x < (setting.x + 70) &&
							scrain_poz.y - cam.y < (setting.y + 110) && scrain_poz.y - cam.y >(setting.y - (setting.h / 2)))
						{
							if (r_runeStone.SDisHide(r_runeStone.SDSearchId(i)) == false || scrain->getSpawn().x != object[i + 1] || scrain->getSpawn().y != object[i + 2])
							{
								SettingsControl st;
								if (st.getDifficulty() != 3 || scrain->isCheckPointSet() == false)
								{
									if (r_runeStone.getTimeTActive() == false)
									{
										RuneStoneEffect(object[i + 1], object[i + 2]);
									}
									if (r_runeStone.getTimeTActive() == false && r_runeStone.SDisTimerRun(r_runeStone.SDSearchId(i)) == false)
									{
										r_runeStone.setTActive(true);
										r_runeStone.SDendTimer(true, r_runeStone.SDSearchId(i));
										r_runeStone.SDSetHide(true, r_runeStone.SDSearchId(i));
										sm.addSound("RuneStoneUnlock", UNLOCKRUNECHANNEL, 0);
									}
								}
							}
						}

						if (r_runeStone.SDisHide(r_runeStone.SDSearchId(i)) == true && r_runeStone.SDisTimerRun(r_runeStone.SDSearchId(i)) == false && r_runeStone.getTimeTActive() == false)
						{
							if (r_runeStone.animSpeed != 104)
							{
								r_runeStone.rendSpeed = 0;
								r_runeStone.animSpeed = 104;
							}
							if (r_runeStone.passiveAnimLock != false)
								r_runeStone.passiveAnimLock = false;
							r_runeStone.renderAlternative(setting, renderer, r_runeStone.SDSearchId(i), 4);
						}
						else
							if (r_runeStone.SDisTimerRun(r_runeStone.SDSearchId(i)) == false && r_runeStone.SDisHide(r_runeStone.SDSearchId(i)) == false)
							{
								if (r_runeStone.animSpeed != 74)
								{
									r_runeStone.rendSpeed = 0;
									r_runeStone.animSpeed = 74;
								}
								if (r_runeStone.passiveAnimLock != true)
									r_runeStone.passiveAnimLock = true;
								r_runeStone.renderPassive(setting, renderer, r_runeStone.SDSearchId(i), 4);
							}
							else
								if (r_runeStone.getTimeTActive() == true)
						{

							if (r_runeStone.animSpeed != 22)
							{
								r_runeStone.rendSpeed = 0;
								r_runeStone.animSpeed = 22;
							}

							if (1 == r_runeStone.rendSpeed)
							{
								if (r_runeStone.data[r_runeStone.SDSearchId(i)].c_sound == -1 && r_runeStone.data[r_runeStone.SDSearchId(i)].offsetY > 10 ||
									r_runeStone.data[r_runeStone.SDSearchId(i)].c_sound == -1 && r_runeStone.data[r_runeStone.SDSearchId(i)].offsetY < -10)
								{
									r_runeStone.data[r_runeStone.SDSearchId(i)].c_sound = sm.getFreeChannel();
									sm.addSound("RuneStone", r_runeStone.data[r_runeStone.SDSearchId(i)].c_sound, 0);
									r_runeStone.data[r_runeStone.SDSearchId(i)].soundTimer = SDL_GetTicks();
									particleSpawnMenager.spawn(object[i + 1] - setting.w / 4, object[i + 2] + 26, 0, 0, 2000, 60, 5, 1, renderer, camRect);
									particleSpawnMenager.spawn(object[i + 1], object[i + 2]- setting.h/5, 0, 0, 200, 10, 5, 1, renderer, camRect);
								}
								if (r_runeStone.data[r_runeStone.SDSearchId(i)].var1 < 0)
									r_runeStone.data[r_runeStone.SDSearchId(i)].var1 += 1;
								else
									if (r_runeStone.data[r_runeStone.SDSearchId(i)].var1 > 0)
										r_runeStone.data[r_runeStone.SDSearchId(i)].var1 -= 1;
								if (r_runeStone.data[r_runeStone.SDSearchId(i)].offsetY != 0)
									r_runeStone.data[r_runeStone.SDSearchId(i)].offsetY -= 1;
							}


							if (r_runeStone.passiveAnimLock != false)
								r_runeStone.passiveAnimLock = false;

							if (r_runeStone.data[r_runeStone.SDSearchId(i)].c_sound != -1)
							{
								sm.changeSoundPos(r_runeStone.data[r_runeStone.SDSearchId(i)].c_sound, setting.x);
							}
							r_runeStone.renderActive(setting, renderer, r_runeStone.SDSearchId(i), 4);


						}
						
						r_runeStone.setIsFrameLoc(true);
					}
				}
				else
				{
					if (r_runeStone.data[r_runeStone.SDSearchId(i)].c_sound != -1)
					{
						sm.stopSound(r_runeStone.data[r_runeStone.SDSearchId(i)].c_sound);
						r_runeStone.data[r_runeStone.SDSearchId(i)].c_sound = -1;
					}
				}
				break;
			}

			default:
				break;
			}
		
		
	}
	
	for (int i = 0; i < i_objWD; i += 5)
	{
			switch (objectWD[i])
			{
			
			case 5: // medium platform
			{
				if (objectWD[i + 3] - cam.x - 150 < scrain_poz.x - cam.x + (mwin_w - (mwin_w / 4)) && objectWD[i + 3] - cam.x > scrain_poz.x - cam.x - (mwin_w - (mwin_w / 4))) // renderuj tylko obiekty w ramce okna
				{
					renderLevPlatform(cam, &r_levitMplatform, i);
				}
				else
				{
					if (r_levitMplatform.storage[r_levitMplatform.searchId(i)].c_sound != -1)
					{
						sm.stopSound(r_levitMplatform.storage[r_levitMplatform.searchId(i)].c_sound);
						r_levitMplatform.storage[r_levitMplatform.searchId(i)].c_sound = -1;
					}
				}
				break;
			}
			case 6: // small platform
			{
				if (objectWD[i + 3] - cam.x - 150 < scrain_poz.x - cam.x + (mwin_w - (mwin_w / 4)) && objectWD[i + 3] - cam.x > scrain_poz.x - cam.x - (mwin_w - (mwin_w / 4))) // renderuj tylko obiekty w ramce okna
				{
					renderLevPlatform(cam, &r_levitSplatform, i);
				}
				else
				{
					if (r_levitSplatform.storage[r_levitSplatform.searchId(i)].c_sound != -1)
					{
						sm.stopSound(r_levitSplatform.storage[r_levitSplatform.searchId(i)].c_sound);
						r_levitSplatform.storage[r_levitSplatform.searchId(i)].c_sound = -1;
					}
				}
				break;
			}
			case 9: // event area
			{
				if (objectWD[i + 3] - cam.x - 150 < scrain_poz.x - cam.x + (mwin_w - (mwin_w / 4)) && objectWD[i + 3] - cam.x > scrain_poz.x - cam.x - (mwin_w - (mwin_w / 4))) // renderuj tylko obiekty w ramce okna
				{
					if (objectWD[i + 1] == 10) // irlicht kill
					{
						if (irlicht_poz->x > objectWD[i + 3] && irlicht_poz->x < objectWD[i + 3]+ objectWD[i+2])
						{
							if (delayArea == 0)
							{
								sm.addSound("IrlichtDeath", sm.getFreeChannel(), 0);
								delayArea = SDL_GetTicks();
								int x = irlicht_poz->x;
								int y = irlicht_poz->y;
								irlicht_poz->x = 0;
								irlicht_poz->y = 0;
								particleSpawnMenagerFront.spawn(x, y, 0, 0, 400, 5, 50, 5, renderer, camRect);
							}
						}
					}
					if (objectWD[i + 1] == 11) // end barrier
					{
						particleSpawnMenagerFront.spawn(objectWD[i + 3], objectWD[i + 4], 0, 0, -1, 30, 15, 6, renderer, camRect);
					}
					if (objectWD[i + 1] == 12) // water drop
					{
						particleSpawnMenagerFront.spawn(objectWD[i + 3], objectWD[i + 4], 0, 0, -1, 280, 1, 7, renderer, camRect);
					}
					if (scrain_poz.x - cam.x > objectWD[i + 3] - cam.x && scrain_poz.x - cam.x  < (objectWD[i + 3] - cam.x) + objectWD[i + 2] &&
						scrain_poz.y - cam.y >(objectWD[i + 4] - cam.y) - objectWD[i + 2] && scrain_poz.y - cam.y < objectWD[i + 4] - cam.y)
					{
						switch (objectWD[i + 1]) // typ eventu
						{
						case 1:
						{
							if (levelends == false)
							{
								SettingsControl st;
								StatisticsControl sc;
								#pragma region ACHIEVEMENTS
								if (st.getLevelName() == "tundra1"&& st.getAchivment(0) == false)
								{
									st.setIsAchivmentDone(true, 0);
									sc.setStat(5, 1);
								}
								if (st.getDifficulty() != 1 && *ghostAchivement == true && st.getAchivment(2) != true && st.getLevelName() != "tundra1" && st.getLevelName() != "wetlands3")
								{
									st.setIsAchivmentDone(true, 2);
									sc.setStat(5, 1);
								}
								if (st.getNumFlames() > 14 && st.getAchivment(3) != true)
								{
									st.setIsAchivmentDone(true, 3);
									sc.setStat(5, 1);
								}
								if (stats.escape > 9 && st.getAchivment(1) != true)
								{
									st.setIsAchivmentDone(true, 1);
									sc.setStat(5, 1);
								}
								if (st.getLevelName() == "wetlands3" && st.getAchivment(4) == false)
								{
									st.setIsAchivmentDone(true, 4);
									sc.setStat(5, 1);
								}
								if (st.getLevelName() == "wetlands3" && st.getAchivment(5) == false && st.getDifficulty() == 3)
								{
									st.setIsAchivmentDone(true, 5);
									sc.setStat(5, 1);
								}

								#pragma endregion
								if (*ghostAchivement == true && st.getLevelName() != "wetlands3")
								{
									sc.setStat(5, 1);
								}
								st.setActualLevel(st.getActualLevel() + 1);
								if (st.getActualLevel() > st.getReachLevel())
									st.setReachLevel(st.getReachLevel() + 1);
								st.SaveToFile();
								levelends = true;
							}
							break;
						}
						case 2:
						{
							//hide area
							if (scrain->GetDetect() < 12 && scrain->GetDetect() != 0)
								scrain->SetDetect(0);
							if (scrain->GetDetect() < 90)
								scrain->setHide(1);
							break;
						}
						case 4:
						{ // osówaj¹ca sie ziemia prtsystem dó³
							if (scrain_poz.x - cam.x > objectWD[i + 3] - cam.x && scrain_poz.x - cam.x  < (objectWD[i + 3] - cam.x) + 30 &&
								scrain_poz.y - cam.y >(objectWD[i + 4] - cam.y) - objectWD[i + 2] && scrain_poz.y - cam.y < objectWD[i + 4] - cam.y)
							{
								particleSpawnMenager.spawn(objectWD[i + 3], objectWD[i + 4], 0, 0, 300, 10, 3, 1, renderer, camRect);
							}
							break;
						}
						case 5:
						{ // osówaj¹ca sie ziemia prtsystem góra
							if (scrain_poz.x - cam.x > objectWD[i + 3] - cam.x && scrain_poz.x - cam.x  < (objectWD[i + 3] - cam.x) + 30 &&
								scrain_poz.y - cam.y >(objectWD[i + 4] - cam.y) - objectWD[i + 2] && scrain_poz.y - cam.y < objectWD[i + 4] - cam.y)
							{
								particleSpawnMenager.spawn(objectWD[i + 3], objectWD[i + 4] - objectWD[i + 2], 0, 0, 300, 10, 3, 1, renderer, camRect);
							}
							break;
						}
						case 6: // detecja aktywacja
						{
							if (delayArea == 0)
							{
								delayArea = SDL_GetTicks();
								if (*isDetectionSystemOff == true)
									*isDetectionSystemOff = false;
							}
							break;
						}
						case 7: // poœcig
						{
							if (delayArea == 0)
							{
								delayArea = SDL_GetTicks();
								if (*isDetectionSystemOff == true)
									*isDetectionSystemOff = false;
								if (*isDetectionSetToMax == false)
								*isDetectionSetToMax = true;
							}
							break;
						}
						case 8: // cave ambient area
						{
							ambientarea = 1;
							if (sm.getActualAmbient() != 1)
							{
								delayArea = SDL_GetTicks();
								sm.changeAmbient(1);
							}
							break;
						}
						case 9: // zatrzymuje
						{
							if (scrain->isRessurected() == true || *isDetectionSetToMax == true)
							{
								scrain->ResetScrain(1);
								scrain->resetAnimCycle(0);
								scrain->SetSpeed(0);
								sm.stopSound(21);
								sm.stopSound(22);
							}
							break;
						}
						default:
							break;
						}
					}
					else
					{
						if (delayArea + DELAYAREA < SDL_GetTicks())
							delayArea = 0;
					}
				}
				break;
			}
			case 8:
			{
				if (objectWD[i + 3] - cam.x - 150 < scrain_poz.x - cam.x + (mwin_w - (mwin_w / 4)) && objectWD[i + 3] - cam.x > scrain_poz.x - cam.x - (mwin_w - (mwin_w / 4)) &&
					(objectWD[i + 4]+80) - cam.y >(scrain_poz.y - cam.y) - 300 && (objectWD[i + 4]+80) - cam.y < (scrain_poz.y - cam.y) + 300) // renderuj tylko obiekty w ramce okna
				{
					setting.w = setting.h = 30;
					setting.x = (objectWD[i + 3] - cam.x) + 80;
					setting.y = (objectWD[i + 4] - cam.y) + 80;

					if (r_cyclon.data[r_cyclon.SDSearchId(i)].isHide == true)
					{
						if (r_cyclon.data[r_cyclon.SDSearchId(i)].var1 < 30)
						{
							r_cyclon.data[r_cyclon.SDSearchId(i)].var1 += 1;
							setting.x -= 15;
							setting.y -= 15;
							if (r_cyclon.data[r_cyclon.SDSearchId(i)].c_sound != -1)
							{
								r_cyclon.data[r_cyclon.SDSearchId(i)].c_sound = -1;
							}
						}
						else
						{
							r_cyclon.data[r_cyclon.SDSearchId(i)].var1 += 3;
							setting.x -= r_cyclon.data[r_cyclon.SDSearchId(i)].var1 / 2;
							setting.y -= r_cyclon.data[r_cyclon.SDSearchId(i)].var1 / 2;
							setting.w = setting.h = r_cyclon.data[r_cyclon.SDSearchId(i)].var1;
							if (r_cyclon.data[r_cyclon.SDSearchId(i)].c_sound == -1)
							{
								r_cyclon.data[r_cyclon.SDSearchId(i)].c_sound = sm.getFreeChannel();
								sm.addSound("CyclonOpen", r_cyclon.data[r_cyclon.SDSearchId(i)].c_sound, 0);
							}
						}
						if (r_cyclon.data[r_cyclon.SDSearchId(i)].var1 > 159)
						{
							r_cyclon.data[r_cyclon.SDSearchId(i)].var1 += r_cyclon.data[r_cyclon.SDSearchId(i)].offsetY;
							r_cyclon.data[r_cyclon.SDSearchId(i)].isHide = false;
							r_cyclon.data[r_cyclon.SDSearchId(i)].c_sound = sm.getFreeChannel();
							sm.addSound("Cyclon", r_cyclon.data[r_cyclon.SDSearchId(i)].c_sound, -1);
						}
					}
					else
					{
						if (r_cyclon.data[r_cyclon.SDSearchId(i)].var1 < 161)
						{
							if (r_cyclon.data[r_cyclon.SDSearchId(i)].var1 > 159)
							{
								sm.stopSound(r_cyclon.data[r_cyclon.SDSearchId(i)].c_sound);
								r_cyclon.data[r_cyclon.SDSearchId(i)].c_sound = sm.getFreeChannel();
								sm.addSound("CyclonClose", r_cyclon.data[r_cyclon.SDSearchId(i)].c_sound, 0);
							}
							r_cyclon.data[r_cyclon.SDSearchId(i)].var1 -= 3;
							setting.w = setting.h = r_cyclon.data[r_cyclon.SDSearchId(i)].var1;
							setting.x -= r_cyclon.data[r_cyclon.SDSearchId(i)].var1 / 2;
							setting.y -= r_cyclon.data[r_cyclon.SDSearchId(i)].var1 / 2;
							
						}
						else
						{
							setting.w = setting.h = 160;
							r_cyclon.data[r_cyclon.SDSearchId(i)].var1 -= 1;
							setting.x -= 80;
							setting.y -= 80;
						}
						if (r_cyclon.data[r_cyclon.SDSearchId(i)].var1 < 30)
						{
							r_cyclon.data[r_cyclon.SDSearchId(i)].var1 -= r_cyclon.data[r_cyclon.SDSearchId(i)].offsetY;
							r_cyclon.data[r_cyclon.SDSearchId(i)].isHide = true;
						}
					}

					if (SDL_HasIntersection(&scrain->GetConvPosition(), &setting) == SDL_TRUE &&
						r_cyclon.data[r_cyclon.SDSearchId(i)].var1 > 30)
					{
						StatisticsControl sc;
						sc.setStat(3, sc.getStat(3) + 1);
						scrain->SetHP(0);
					}
					r_cyclon.renderPassive(setting, renderer, 0, 0);
					r_cyclon.setIsFrameLoc(true);
				}
				else
				{
					if (r_cyclon.data[r_cyclon.SDSearchId(i)].c_sound != -1)
					{
						sm.stopSound(r_cyclon.data[r_cyclon.SDSearchId(i)].c_sound);
						r_cyclon.data[r_cyclon.SDSearchId(i)].c_sound = -1;
					}
				}
				if (r_cyclon.data[r_cyclon.SDSearchId(i)].c_sound != -1)
					sm.changeSoundPos(r_cyclon.data[r_cyclon.SDSearchId(i)].c_sound, setting.x);
				break;
			}
			default:
				break;
			}
			
	}
	
	if (sm.getActualAmbient() == 1 && ambientarea == 0)
		sm.changeAmbient(0);

	r_runeStone.runAnim();
	r_bird.runAnim();
	r_cyclon.runAnim();
	particleSpawnMenager.update();
}
void Objects::renderBush(SDL_Rect cam, ObjectTypeRender* rbush)
{
	if (scrain_poz.x - cam.x > setting.x - (setting.w / 3) && scrain_poz.x - cam.x < setting.x + (setting.w / 2) &&
		scrain_poz.y - cam.y < setting.y + setting.h && scrain_poz.y - cam.y > setting.y)
	{
		BushEffect();
		if (scrain_poz.x != prevScrnPozX || scrain_poz.y != prevScrnPosY)
		{
			if (rbush->getTActive() == false)
				rbush->setTActive(true);
			prevScrnPozX = scrain_poz.x;
			prevScrnPosY = scrain_poz.y;
		}
		if (rbush->getTimeTActive() == true)
			rbush->renderActive(setting, renderer, -1, -1);
		else
			rbush->renderPassive(setting, renderer, -1, -1);
	}
	else
	{
		rbush->renderPassive(setting, renderer, -1, -1);
	}


	rbush->setIsFrameLoc(true);
}
void Objects::renderLevPlatform(SDL_Rect cam, ObjectTypeRenderNA* r_levitplatform, int i)
{
	if (r_levitplatform->storage[r_levitplatform->searchId(i)].soundTimer + PLATFORM_LENGHT < SDL_GetTicks() && r_levitplatform->storage[r_levitplatform->searchId(i)].soundTimer != 0)
	{
		r_levitplatform->storage[r_levitplatform->searchId(i)].c_sound = -1;
		r_levitplatform->storage[r_levitplatform->searchId(i)].soundTimer = 0;
	}

	if (r_levitplatform->storage[r_levitplatform->searchId(i)].c_sound != -1)
		sm.changeSoundPos(r_levitplatform->storage[r_levitplatform->searchId(i)].c_sound, objectWD[i + 3] - cam.x);

	if (r_levitplatform->isAlternative(r_levitplatform->searchId(i)) == true)
	{
		r_levitplatform->render(objectWD[i + 3], objectWD[i + 2],-1,-1, r_levitplatform->searchId(i), renderer, &cam);
	}
	else
		if (r_levitplatform->isActive(r_levitplatform->searchId(i)) == true)
		{
			
			if (r_levitplatform->getSetting(r_levitplatform->searchId(i)).y - 4 > objectWD[i + 2])
			{
				r_levitplatform->render(objectWD[i + 3], r_levitplatform->getSetting(r_levitplatform->searchId(i)).y - (round((double)objectWD[i + 4] / objectWD[i + 2]) * 3), -1, -1, r_levitplatform->searchId(i), renderer, &cam);
				*r_levitplatform->storage[r_levitplatform->searchId(i)].particlePoz = r_levitplatform->getSetting(r_levitplatform->searchId(i)).y+10;
				// jesli nie pojawi sie to problem z dzielenie ^
			}
			else
			{
				r_levitplatform->render(objectWD[i + 3], objectWD[i + 2], -1, -1, r_levitplatform->searchId(i), renderer, &cam);
				r_levitplatform->setIsAlternative(r_levitplatform->searchId(i));
			}
			
		}
		else
		{
			r_levitplatform->render(objectWD[i + 3], objectWD[i + 4], -1, -1, r_levitplatform->searchId(i), renderer, &cam);
			
			if (scrain_poz.x - cam.x + (scrain_poz.w / 2) > (objectWD[i + 3] - cam.x) - 350 && scrain_poz.x - cam.x + (scrain_poz.w / 2) < (objectWD[i + 3] - cam.x) + 350 &&
				scrain_poz.y - cam.y > (objectWD[i + 2] - cam.y) - 180 && scrain_poz.y - cam.y < (objectWD[i + 4] - cam.y))
			{
				r_levitplatform->setIsActive(r_levitplatform->searchId(i));
				r_levitplatform->storage[r_levitplatform->searchId(i)].particlePoz = particleSpawnMenager.spawn(objectWD[i + 3], r_levitplatform->getSetting(r_levitplatform->searchId(i)).y, objectWD[i + 3] + abs(objectWD[i + 3] - objectWD[i + 2]), objectWD[i + 3], 1000, r_levitplatform->getSetting(r_levitplatform->searchId(i)).w, 7, 1, renderer, camRect);
				
				r_levitplatform->storage[r_levitplatform->searchId(i)].c_sound = sm.getFreeChannel();
				sm.addSound("Platform", r_levitplatform->storage[r_levitplatform->searchId(i)].c_sound, 0);
				r_levitplatform->storage[r_levitplatform->searchId(i)].soundTimer = SDL_GetTicks();
			}
		}
	
}
void Objects::BushEffect()
{
	if (scrain->GetDetect() < 12 && scrain->GetDetect() != 0)
		scrain->SetDetect(0);
	if (scrain->GetDetect() < 90)
		scrain->setHide(1);

	if (scrain->GetAnimType() == 1 || scrain->GetAnimType() == 2)
		sm.addSound("Bush3", BUSHCHANNEL, 0);

	if (ev->GetSideKeyStatus() != 1 && ev->GetSideKeyStatus() != 2)
	{
		if (scrain->GetAnimType() == 33)
		{
			sm.pauseSound(21);
			sm.addSound("Bush1", sm.getFreeChannel(), 0);
			scrain->ResetScrain(0);
		}
		else
			if (scrain->GetAnimType() == 44)
			{
				sm.pauseSound(21);
				sm.addSound("Bush2", sm.getFreeChannel(), 0);
				scrain->ResetScrain(1);
			}
		scrain->resetAnimCycle(0);
		scrain->SetSpeed(0);
		sm.pauseSound(RUNCHANNEL);
	}
	/*
	scrain->GetPosition(&pz);
	if (pz.x > (x - 55) && pz.x < (x + 125) && pz.y < (y + 50))
	{
		if (scrain->GetAnimType() == 311 || scrain->GetAnimType() == 312)
		{
			if (scrain->GetAnimType() == 311)
			{
				scrain->SetAnimType(0);
				ev->SetException(1);
			}
			else
			{
				ev->SetException(2);
				scrain->SetAnimType(11);
			}
			scrain->resetAnimCycle(0);
			scrain->LoadScrain(2, 16);
		}
		else
			if (scrain->GetAnimType() == 0 || scrain->GetAnimType() == 11)
			{
				const Uint8 *state = SDL_GetKeyboardState(NULL);
				if (state[SDL_SCANCODE_A] && scrain->GetReductJumpKey_Status() != 1)
				{
					scrain->SetLvlSpeed(4);
					scrain->SetAnimType(1);
					scrain->resetAnimCycle(15);
					ev->SetException(-1);
					scrain->LoadScrain(2, 16);
				}
				else
				if (state[SDL_SCANCODE_D] && scrain->GetReductJumpKey_Status() != 1)
				{
					scrain->SetLvlSpeed(4);
					scrain->SetAnimType(2);
					scrain->resetAnimCycle(15);
					ev->SetException(-1);
					scrain->LoadScrain(2, 16);
				}
				
			}
		scrain->SetDetect(0);
	}
	else
	{
		if (pz.x > x && pz.x <= (x + 130) && scrain->GetAnimType() == 1 && pz.y < (y + 50) && pz.y >(y + 10)||
			pz.x > x && pz.x <= (x + 130) && scrain->GetAnimType() == 33 && pz.y < (y + 50) && pz.y >(y + 10))
		{
			scrain->SetSpeed(0);
			phEngine->scriptJump(5, 5, 0);
		}
		else
		{
			if (pz.x < x && pz.x >= (x - 60) && scrain->GetAnimType() == 2 && pz.y < (y + 50) && pz.y >(y + 10)||
				pz.x < x && pz.x >= (x - 60) && scrain->GetAnimType() == 44 && pz.y < (y + 50) && pz.y >(y + 10))
			{
				scrain->SetSpeed(0);
				phEngine->scriptJump(5, 5, 1);
			}
		}
	}
	*/
}
void Objects::FlameEffect()
{
	scrain->SetHP(100);
	SettingsControl st;
	if (st.getNumFlames() != 100)
		st.setNumFlames(st.getNumFlames()+1);
	if (st.getAchivment(3) == false && st.getNumFlames() == 30)
		st.setIsAchivmentDone(true,3);
	n_getflames += 1;
	switch (st.getDifficulty())
	{
	case 1:
	{
		scrain->SetDetect(scrain->GetDetect() - 10);
		break;
	}
	case 2:
	{
		scrain->SetDetect(scrain->GetDetect() - 7);
		break;
	}
	case 3:
	{
		scrain->SetDetect(scrain->GetDetect() - 5);
		break;
	}
	default:
		break;
	}
}
void Objects::RuneStoneEffect(int x, int y)
{
	if (scrain->getSpawn().x != x || scrain->getSpawn().y != y)
	{
		scrain->setSpawn(x, y);
	}
}

Objects::ObjectTypeRender::ObjectTypeRender()
{
	frameLoc = 0;
	isTActive = 0, t_TActive = 0;
	rendSpeed = 0;
	t_a_Hnon = t_p_Hnon = 0;
	SDSize = 0;
}
Objects::ObjectTypeRender::~ObjectTypeRender()
{
	delete[] data;
	data = nullptr;
	SDL_DestroyTexture(textureActive);
	SDL_DestroyTexture(textureAlt);
	SDL_DestroyTexture(texturePassive);
}
void Objects::ObjectTypeRender::querryTexture(int numAframesX, int numAframesY, int numPframesX, int numPframesY)
{
	if (textureActive != NULL)
	SDL_QueryTexture(textureActive, NULL, NULL, &textureASize.w, &textureASize.h);
	SDL_QueryTexture(texturePassive, NULL, NULL, &texturePSize.w, &texturePSize.h);
	if (numAframesY < 2)
		t_a_Hnon = true;
	if (numPframesY < 2)
		t_p_Hnon = true;
	if (textureActive != NULL)
	{
		frameASize.w = textureASize.w / numAframesX;
		textureASize.w = frameASize.w * numAframesX;
		frameASize.h = textureASize.h / numAframesY;
		textureASize.h = frameASize.h * numAframesY;
	}
	framePSize.w = texturePSize.w / numPframesX;
	texturePSize.w = framePSize.w * numPframesX;
	framePSize.h = texturePSize.h / numPframesY;
	texturePSize.h = framePSize.h * numPframesY;
	animRect_passive.x = animRect_passive.y = 0;
	if (textureActive != NULL)
	{
		animRect_active.x = animRect_active.y = 0;
		animRect_active.h = frameASize.h;
		animRect_active.w = frameASize.w;
	}
	animRect_passive.h = framePSize.h;
	animRect_passive.w = framePSize.w;
};
void Objects::ObjectTypeRender::renderActive(SDL_Rect setting, SDL_Renderer* renderer, int id, short type)
{
	if (textureActive != NULL)
	{
		if (id == -1 && type == -1)
			SDL_RenderCopyEx(renderer, textureActive, &animRect_active, &setting, NULL, NULL, SDL_FLIP_NONE);
		else
		{
			angle = data[id].var1;
			SDL_RenderCopyEx(renderer, textureActive, &animRect_active, &setting, angle, NULL, SDL_FLIP_NONE);
		}
	}
}
void Objects::ObjectTypeRender::renderPassive(SDL_Rect setting, SDL_Renderer* renderer, int id, short type)
{
	if (texturePassive != NULL)
	{
		if (id == -1 && type == -1)
			SDL_RenderCopyEx(renderer, texturePassive, &animRect_passive, &setting, NULL, NULL, SDL_FLIP_NONE);
		else
		{
			switch (type)
			{
			case 4:
			{
				angle = data[id].var1;
				break;
			}
			default:
				break;
			}
			SDL_RenderCopyEx(renderer, texturePassive, &animRect_passive, &setting, angle, NULL, SDL_FLIP_NONE);
		}
	}
}
void Objects::ObjectTypeRender::renderAlternative(SDL_Rect setting, SDL_Renderer* renderer, int id, short type)
{
	if (textureAlt != NULL)
	SDL_RenderCopyEx(renderer, textureAlt, &animRect_passive, &setting, NULL, NULL, SDL_FLIP_NONE);
}
void Objects::ObjectTypeRender::runAnim()
{
	
	if (frameLoc == true || t_TActive == true) // jesli jest jakis obiekt ktory znajduje sie w ramce okna, to oblicz jego klatke
	{
		rendSpeed += 1;
		if (animSpeed / rendSpeed <= 4)
		{
			rendSpeed = 0;
			
			if (passiveAnimLock == false)
			{
				if (animRect_passive.x + framePSize.w >= texturePSize.w)
				{
					animRect_passive.x = 0;
					if (t_p_Hnon == false)
					{
						if (animRect_passive.y + framePSize.h >= texturePSize.h)
							animRect_passive.y = 0;
						else
							animRect_passive.y += framePSize.h;
					}
				}
				else
					animRect_passive.x += framePSize.w;
			}
			
			if (t_TActive == true)
			{
				if (animRect_active.x + frameASize.w >= textureASize.w)
				{
					animRect_active.x = 0;
					if (t_a_Hnon == false)
					{
						if (animRect_active.y + frameASize.h >= textureASize.h)
						{
							animRect_active.y = 0;
							t_TActive = false;
							data[SDSearchisTimerRun()].isHide = true;
							data[SDSearchisTimerRun()].endTimer = false;
						}
						else
							animRect_active.y += frameASize.h;
					}
					else
					{
						t_TActive = false;
						data[SDSearchisTimerRun()].isHide = true;
						data[SDSearchisTimerRun()].endTimer = false;
					}
				}
				else
				{
					animRect_active.x += frameASize.w;
				}
			}
		}
	}
	
}
void Objects::ObjectTypeRender::setTexture(SDL_Texture* TextureActive, SDL_Texture* TexturePassive, SDL_Texture* TextureAlt)
{
	if (TextureAlt != NULL)
		textureAlt = TextureAlt;
	textureActive = TextureActive;
	texturePassive = TexturePassive;
	if (darkShading == true) // œciemnianie dla poziomów ciemniejszych
	{
		SettingsControl st;
		string LevelName = st.getLevelName();
		if (LevelName[0] == 'w' &&  LevelName[1] == 'e' && LevelName[2] == 't' &&
			LevelName[3] == 'l' && LevelName[4] == 'a' && LevelName[5] == 'n' &&
			LevelName[6] == 'd')
		{
			if (TextureAlt != NULL)
				SDL_SetTextureColorMod(textureAlt, 900, 900, 900);
			if (TextureActive!= NULL)
			SDL_SetTextureColorMod(textureActive, 900, 900, 900);
			SDL_SetTextureColorMod(texturePassive, 900, 900, 900);
		}
	}
}

bool Objects::ObjectTypeRender::SDisTimerRun(int id)
{
	if (id == -1)
		id = SDSearchisTimerRun();
	return data[id].endTimer;
}
void Objects::ObjectTypeRender::SDcreate(int number)
{
	delete[] data;
	data = nullptr;
	data = new specific[number + 1];
	SDSize = number + 1;
	for (int i = 0; i < SDSize; i++)
	{
		data[i].id = 0;
		data[i].isHide = 0;
		data[i].endTimer = 0;
		data[i].var1 = 0;
		data[i].offsetY = 0;
		data[i].c_sound = -1;
		data[i].soundTimer = 0;
	}
}
int Objects::ObjectTypeRender::SDSearchId(int id)
{
	if (SDSize == 1)
		return 0;
	int l = 0, r = SDSize-1;
	while (l <= r)
	{
		int middle = (l + r) / 2;
		if (data[middle].id == id)
			return middle;
		else if (data[middle].id > id)
			r = middle - 1;
		else
			l = middle + 1;
	}

	//for (int i = 0; i < SDSize; i++)
		//if (data[i].id == id)
		//	return i;
	return -1;
}
int Objects::ObjectTypeRender::SDSearchisTimerRun()
{
	for (int i = 0; i < SDSize; i++)
	{
		if (data[i].endTimer == 1)
			return i;
	}
	return -1;

}
void Objects::ObjectTypeRender::SDShowHideObjects()
{
	for (int i = 0; i < SDSize; i++)
		data[i].isHide = false;
}
void Objects::ObjectTypeRender::SDsortIds(int l, int p)
{
	long v = data[(l + p) / 2].id;
	int i, j;
	long temp;
	i = l;
	j = p;
	do
	{
		while
			(data[i].id<v) i++;
		while
			(data[j].id>v) j--;
		if
			(i <= j){
			temp = data[i].id;
			data[i].id = data[j].id;
			data[j].id = temp;
			i++; j--;
		}
	} while
		(i <= j);
	if
		(j>l) SDsortIds(l, j);
	if
		(i<p) SDsortIds(i, p);
}

#pragma region func
void Objects::ObjectTypeRender::activeDarkShading()
{
	darkShading = true;
}
void Objects::ObjectTypeRender::setIsFrameLoc(bool v)
{
	if (v != frameLoc)
		frameLoc = v;
}
void Objects::ObjectTypeRender::setTActive(bool v)
{
	isTActive = v;
	if (t_TActive == false && isTActive == true)
		t_TActive = true;
}
bool Objects::ObjectTypeRender::getTActive()
{
	return isTActive;
}
bool Objects::ObjectTypeRender::getTimeTActive()
{
	return t_TActive;
}
void Objects::ObjectTypeRender::SDsetId(int index)
{
	for (int i = 0; i < SDSize; i++)
	{
		if (data[i].id == 0)
		{
			data[i].id = index;
			break;
		}
	}
}
void Objects::ObjectTypeRender::SDSetHide(bool v, int id)
{
	data[id].isHide = v;
};
bool Objects::ObjectTypeRender::SDisHide(int id)
{
	return data[id].isHide;
}
void Objects::ObjectTypeRender::SDendTimer(bool v, int id)
{
	data[id].endTimer = v;
}
#pragma endregion

Objects::ObjectTypeRenderNA::ObjectTypeRenderNA()
{
	i_storage = NULL;
	texture = NULL;
}
Objects::ObjectTypeRenderNA::~ObjectTypeRenderNA()
{
	SDL_DestroyTexture(texture);
	texture = NULL;
	delete storage->particlePoz;
	delete[] storage;
}
void Objects::ObjectTypeRenderNA::loadTextures(int type, int id, SDL_Renderer* renderer)
{
	if (texture == NULL)
	{
		switch (type)
		{
		case 1:
		{
			texture = LoadTextureRW(SDL_RWFromFile("graphics/PlatformMidle.png", "rb"), renderer);
			break;
		}
		case 2:
		{
			texture = LoadTextureRW(SDL_RWFromFile("graphics/PlatformSmall.png", "rb"), renderer);
			break;
		}
		case 3:
		{
			texture = LoadTextureRW(SDL_RWFromFile("graphics/HidedArea.png", "rb"), renderer);
			break;
		}
		case 4:
		{
			texture = LoadTextureRW(SDL_RWFromFile("graphics/magneticStone.png", "rb"), renderer);
		}
		default:
			break;
		}
	}
	if (type != 3)
	{
		SettingsControl st;
		string LevelName = st.getLevelName();
		if (LevelName[0] == 'w' &&  LevelName[1] == 'e' && LevelName[2] == 't' &&
			LevelName[3] == 'l' && LevelName[4] == 'a' && LevelName[5] == 'n' &&
			LevelName[6] == 'd')
		{
			SDL_SetTextureColorMod(texture, 900, 900, 900);
		}
	}
	SDL_QueryTexture(texture, NULL, NULL, &storage[id].setting.w, &storage[id].setting.h);
}
void Objects::ObjectTypeRenderNA::render(int x, int y, int w, int h, int id, SDL_Renderer* renderer, SDL_Rect*cam)
{
	if (w != -1 && storage[id].setting.w != w)
		storage[id].setting.w = w;
	else
		drawSet.w = storage[id].setting.w;
	if (h != -1 && storage[id].setting.h != h)
		storage[id].setting.h = h;
	else
		drawSet.h = storage[id].setting.h;

	storage[id].setting.x = x;
	storage[id].setting.y = y;
	drawSet.x = x - cam->x;
	drawSet.y = y - cam->y;
	
	if (texture != NULL)
	SDL_RenderCopyEx(renderer, texture, NULL, &drawSet, NULL, NULL, SDL_FLIP_NONE);
}
int Objects::ObjectTypeRenderNA::searchId(int i)
{
	int l = 0, r = i_storage-1;
	while (l <= r)
	{
		int middle = (l + r) / 2;
		if (storage[middle].id == i)
			return middle;
		else if (storage[middle].id > i)
			r = middle - 1;
		else
			l = middle + 1;
	}

	for (int j = 0; j < i_storage; j++)
		if (storage[j].id == i)
			return j;
	
}
void Objects::ObjectTypeRenderNA::createDataBase(int idNumber)
{
	delete[] storage;
	storage = new data[idNumber];
	i_storage = idNumber;
	for (int i = 0; i < i_storage; i++)
	{
		storage[i].var = -1;
		storage[i].id = -1;
		storage[i].isactive = 0;
		storage[i].alternative = 0;
		storage[i].setting = {};
		storage[i].opacity = 0;
		storage[i].c_sound = -1;
		storage[i].soundTimer = 0;
	}
}
SDL_Rect Objects::ObjectTypeRenderNA::getSetting(int id)
{
	return storage[id].setting;
}
bool Objects::ObjectTypeRenderNA::isActive(int id)
{
	return storage[id].isactive;
}
bool Objects::ObjectTypeRenderNA::isAlternative(int id)
{
	return storage[id].alternative;
}
void Objects::ObjectTypeRenderNA::setIsActive(int id)
{
	storage[id].isactive = true;
}
void Objects::ObjectTypeRenderNA::setIsAlternative(int id)
{
	storage[id].isactive = false;
	storage[id].alternative = true;
}
int Objects::ObjectTypeRenderNA::setIdToStorage(int index)
{
	
	for (int i = 0; i < i_storage; i++)
	{
		if (storage[i].id == -1)
		{
			storage[i].id = index;
			return i;
		}
	}
	
}
void Objects::ObjectTypeRenderNA::sortIds(int l, int p)
{
	long v = storage[(l + p) / 2].id;
	int i, j;
	long temp;
	i = l;
	j = p;
	do
	{
		while
			(storage[i].id<v) i++;
		while
			(storage[j].id>v) j--;
		if
			(i <= j){
			temp = storage[i].id;
			storage[i].id = storage[j].id;
			storage[j].id = temp;
			i++; j--;
		}
	} while
		(i <= j);
	if
		(j>l) sortIds(l, j);
	if
		(i<p) sortIds(i, p);
}

#pragma region func
void Objects::loadPointers(Scrain* scrain, PhysicEngine* phEngine, Events* ev, SDL_Window* main_window, SDL_Rect* camRect, bool* isDetectionSystemOff, bool* ghostAchivement, bool*isDetectionGetMax, Point* irlicht)
{
	this->ev = ev;
	this->scrain = scrain;
	this->phEngine = phEngine;
	this->camRect = camRect;
	this->isDetectionSystemOff = isDetectionSystemOff;
	this->ghostAchivement = ghostAchivement;
	this->isDetectionSetToMax = isDetectionGetMax;
	irlicht_poz = irlicht;
	SDL_GetWindowSize(main_window, &mwin_w, &mwin_h);
}
void Objects::showHidedObjects()
{
	r_flame.SDShowHideObjects();
}
void Objects::resetScrain()
{
	scrain->ResetScrain(1);
}
#pragma endregion

void Objects::LoadData(long data, int i)
{
	if (i >= i_obj)
	{
		DAMLongChanger(&object, i + 1, 1, 1);
		i_obj = i;
	}
	object[i] = data;
}
void Objects::LoadDataWD(long data, int i)
{
	if (i >= i_objWD)
	{
		DAMLongChanger(&objectWD, i + 1, 1, 1);
		i_objWD = i;
	}
	objectWD[i] = data;
}