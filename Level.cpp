#include "Level.h"
#include<fstream>
#include"SDLStuff.h"
#include <sstream>
#include <conio.h>
#include"Settings.h"

level::level()
{
	bg_level = NULL;
	lay_level = NULL;
	
	for (int i = 0; i < 6; i++)
	{
		base_level[i] = NULL;
	}
	blay_level = NULL;
	bg_worldPos[0] = { 0 };
	bg_worldPos[1] = { 0 };
	bg_drawPos[1] = { 0 };
	bg_drawPos[0] = { 0 };
	bgOriginW[0] = bgOriginW[1] = 0;
	prevcam[0] = prevcam[1] = prevcam[2] = {};
}

level::~level()
{
	for (int i = 0; i < n_baseParts; i++)
	{
		if (base_level[i] != NULL)
			SDL_DestroyTexture(base_level[i]);
		base_level[i] = NULL;
	}
	SDL_DestroyTexture(bg_level);
	SDL_DestroyTexture(lay_level);
	SDL_DestroyTexture(blay_level);
	delete[] spawnPos;
	spawnPos = nullptr;
	delete[] spawnPosEnemyX;
	spawnPosEnemyX = nullptr;
	delete[] spawnPosEnemyY;
	spawnPosEnemyY = nullptr;
}

void level::RenderBGEffects()
{
	particleSpawnMenager.update();
}

void level::LoadBGEffects(SDL_Renderer* renderer, SDL_Rect *camera, int type)
{
	switch (type)
	{
	case 3:
	{
		particleSpawnMenager.spawn(camera->x, camera->y, 0, 0, -1, camera->w, 20, type, renderer, camera);
		break;
	}
	case 8:
	{
		particleSpawnMenager.spawn(camera->x, camera->y, 0, 0, -1, camera->w, 5, type, renderer, camera);
		break;
	}
	default:
		break;
	}
	SDL_QueryTexture(base_level[0], NULL, NULL, &baseSize.w, &baseSize.h);
}

int level::LoadNewLevel(SDL_Window* window, SDL_Renderer* renderer, Objects* objects)
{
	int haveEffect = 0;
	SettingsControl st;
	SoundMenager sm;
	string LevelName = "levels/" + st.getLevelName() + "blay.png";
	const char* fileName = LevelName.c_str();

	blay_level = LoadTextureRW(SDL_RWFromFile(fileName, "rb"), renderer);

	LevelName = "levels/" + st.getLevelName() + "lay.png";
	fileName = LevelName.c_str();
	lay_level = LoadTextureRW(SDL_RWFromFile(fileName, "rb"), renderer);

	LevelName = st.getLevelName() + "bg.png";
	for (int i = 0; i < 6; i++)
	{
		if (LevelName[i] == 't' && LevelName[i+1] == 'u' && LevelName[i+2] == 'n'
			&& LevelName[i+3] == 'd' && LevelName[i+4] == 'r' && LevelName[i+5] == 'a')
		{
			haveEffect = 3;
			LevelName = "levels/tundrabg.png";
			sm.loadSound("Wind", 1);
			
		}
		if (LevelName[i] == 's' && LevelName[i + 1] == 't' && LevelName[i + 2] == 'e'
			&& LevelName[i + 3] == 'p' && LevelName[i + 4] == 'p' && LevelName[i + 5] == 'e'&& LevelName[i + 6] == 's')
		{
			LevelName = "levels/steppesbg.png";
			sm.loadSound("Wind", 1);
		}
		if (LevelName[i] == 'w' && LevelName[i + 1] == 'e' && LevelName[i + 2] == 't'
			&& LevelName[i + 3] == 'l' && LevelName[i + 4] == 'a' && LevelName[i + 5] == 'n'&& LevelName[i + 6] == 'd')
		{
			LevelName = "levels/wetlandsbg.png";
			sm.loadSound("Swamp", 1);
			haveEffect = 8;
		}
	}
	fileName = LevelName.c_str();
	bg_level = LoadTextureRW(SDL_RWFromFile(fileName, "rb"), renderer);
	SDL_QueryTexture(bg_level, NULL, NULL, &bgOriginW[0], &bgOriginH);
	SDL_QueryTexture(blay_level, NULL, NULL, &bgOriginW[1], NULL);
	LevelName = st.getLevelName();
	int non = 0;
	fstream savefile;
	savefile.open(LevelName + ".txt", ios::in);
	if (savefile.is_open())
	{
		// pobierz ile jest colision data, spawnPos oraz Object
		delete[] spawnPos;
		spawnPos = nullptr;

		long lnon = 0;
		int c = 0;
		int c1 = 0;
		savefile >> n_baseParts;
		savefile >> c;
		savefile >> c;
		savefile >> c;
		savefile >> c;
		savefile >> c1;
		spawnPos = new long[c1+1];
		memset(spawnPos, 0, c1*sizeof(int));
		savefile >> c;
		savefile >> c;
		savefile >> c;

		int count = 0;
		for (int i = 0; count < 8; i++)
		{
			switch (count)
			{
			case 0:
			{
				savefile >> non;
				if (non == 0)
				{
					count = 1;
					i = -1;
				}
				break;
			}
			case 1:
			{
				savefile >> non;
				if (non == 0)
				{
					count = 2;
					i = -1;
				}
				break;
			}
			case 2:
			{
				savefile >> non;
				if (non == 0)
				{
					count = 3;
					i = -1;
				}
				break;
			}
			case 3:
			{
				savefile >> lnon;
				if (lnon == 0)
				{
					count = 4;
					i = -1;
				}
				else
					objects->LoadData(lnon, i);
				break;
			}
			case 4:
			{
				savefile >> spawnPos[i];
				if (spawnPos[i] == 0)
				{
					count = 5;
					i = -1;
				}
				break;
			}
			case 5:
			{
				savefile >> non;
				if (non == -1)
				{
					count = 6;
					i = -1;
				}
				break;
			}
			case 6:
			{
				savefile >> non;
				if (non == 0)
				{
					count = 7;
					i = -1;
				}
				break;
			}
			case 7:
			{
				savefile >> lnon;
				
				if (lnon == 0)
				{
					count = 8;
				}
				else
					objects->LoadDataWD(lnon, i);
				break;
			}
			};
			
		}
		savefile.close();

		for (int i = 0; i < c1; i++)
		{
			
			if (spawnPos[i] == 1)
			{
				spawnPosPlayer.x = spawnPos[i + 1];
				spawnPosPlayer.y = spawnPos[i + 2];
				i += 2;
			}
			else
				if (spawnPos[i] == 2)
				{
					n_enemySpawn += 1;
					if (n_enemySpawn == 1)
					{
						spawnPosEnemyX[0] = spawnPos[i + 1];
						spawnPosEnemyY[0] = spawnPos[i + 2];
					}
					else
					{
						DAMLongChanger(&spawnPosEnemyX, n_enemySpawn, 1, 1);
						DAMLongChanger(&spawnPosEnemyY, n_enemySpawn, 1, 1);
						spawnPosEnemyX[n_enemySpawn - 1] = spawnPos[i + 1];
						spawnPosEnemyY[n_enemySpawn - 1] = spawnPos[i + 2];
					}
					i += 2;

				}

		}
		delete[] spawnPos;
		spawnPos = nullptr;
	}

	
	char conv;
	for (int i = 0; i < n_baseParts; i++) // ladowanie czesci planszy do tekstur
	{
		conv = ((i % 10) + 48);
		LevelName = "levels/" + st.getLevelName() + "base" + conv + ".png";
		fileName = LevelName.c_str();
		base_level[i] = LoadTextureRW(SDL_RWFromFile(fileName, "rb"), renderer);
	}
	SDL_QueryTexture(base_level[0], NULL, NULL, &baseSize.w, &baseSize.h);
	baseSize.w *= n_baseParts;
	fileName = nullptr;
	delete fileName;
	return haveEffect;
}

void level::RenderBGLevel(SDL_Renderer* renderer, SDL_Rect* camera)
{
	if (camera != NULL)
	{
		for (int i = 0; i < 2; i++)
		{
			if (blay_level == NULL && i == 1)
				break;
			if (bg_drawPos[i].w == 0 || bg_worldPos[i].w == 0)
			{
				bg_drawPos[i].w = bg_worldPos[i].w = camera->w;
				bg_drawPos[i].h = bg_worldPos[i].h = camera->h;
				bg_drawPos[i].y = camera->y;
				bg_drawPos[i].y -= abs(bgOriginH - baseSize.h);
			}

			if (prevcam[i].w != 0)
			{
				if (i == 1)
					bg_drawPos[i].x += ((camera->x + camera->w) / 2 - (prevcam[i].x + prevcam[i].w) / 2);
				else
					bg_drawPos[i].x += ((camera->x + camera->w) / 3 - (prevcam[i].x + prevcam[i].w) / 3);
				if (i == 1)
					bg_drawPos[i].y += ((camera->y + camera->h) / 3 - (prevcam[i].y + prevcam[i].h) / 3);
				else
				{
					bg_drawPos[i].y += ((camera->y + camera->h) / 4 - (prevcam[i].y + prevcam[i].h) / 4);
				}
			}
			prevcam[i] = *camera;

			if (bg_drawPos[i].x < 0)
				bg_drawPos[i].x = bgOriginW[i] - camera->w;
			else
				if (bg_drawPos[i].x + camera->w >bgOriginW[i])
					bg_drawPos[i].x = 0;
			
			
			if (i == 0)
			{
				if (bg_level != NULL)
				SDL_RenderCopy(renderer, bg_level, &bg_drawPos[i], &bg_worldPos[i]);
			}
			else
			{
				if (blay_level != NULL)
				SDL_RenderCopy(renderer, blay_level, &bg_drawPos[i], &bg_worldPos[i]);
			}
		}
	}
	else
	{
		if (bg_level != NULL)
			SDL_RenderCopy(renderer, bg_level, NULL, NULL);
		if (blay_level != NULL)
		SDL_RenderCopy(renderer, blay_level, NULL, NULL);
	}
}

void level::RenderBaseLevel(SDL_Renderer* renderer, SDL_Rect* camera)
{
	for (int i = 0; i < n_baseParts; i++)
	{
		if (base_level[i] != NULL)
		{
			if (prevcam[2].w != 0)
			{
				if (i == 0) // pierwszy sie oblicza, reszta dopasowuje sie do poprzedniego wzgledem siebie
					base_worldPos[i].x += -((camera->x + camera->w) - (prevcam[2].x + prevcam[2].w));
				else
					base_worldPos[i].x = base_worldPos[i - 1].x + baseSize.w;
			}
			base_worldPos[i].y = -(camera->y);
			/******************************************************
			Samo y tez trzeba obliczac osobno jesli nadejdzie taka potrzeba
			¯eby to odznaczyæ w razie potrzeby napisz cos w stylu if(actuallevel == x)
			w tedy oblicz y w innym razie niech sobie jest na podstawie kamery. 
			Mo¿na tak zrobic np z poziomami powyzej konkretnej wartosci x,
			jesli przyjdzie mapa z tilingiem vert i horiz w tedy bêdzie trzeba ukladac mapê w szereg,
			np napisac ze co 6 czêœci, x sie resetuje do tego co na poczatku a y wzrasta o h
			powodzenia
			*******************************************************/


			if (base_worldPos[i].w != baseSize.w)
			{
				base_worldPos[i].w = baseSize.w;
				base_worldPos[i].h = baseSize.h;
			}
			prevcam[2] = *camera;

			if (camera->w >= base_worldPos[i].x && camera->w <= base_worldPos[i+1].x+baseSize.w)
				SDL_RenderCopy(renderer, base_level[i], NULL, &base_worldPos[i]);
		}
	}

}