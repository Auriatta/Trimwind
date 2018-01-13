#include"GameLoop.h"
#include"Object.h"
#include "SDLStuff.h"

Game::Game()
{
	done = 0;
	n_fps = 0;
	migniecie = 0;
}

Game::~Game()
{
	delete irlicht;
	delete objects;
	delete cam;
	delete scrain;
	delete effects;
	delete g_interface;

	SDL_DestroyTexture(target_texture);
	SDL_DestroyRenderer(RenderTarget);
}

void Game::render()
{
	SDL_RenderClear(RenderTarget);

	SDL_SetRenderTarget(RenderTarget, target_texture);
	if (lvl.bg_level != NULL)
	{
		lvl.RenderBGLevel(RenderTarget, cam->GetNewCoord());
	}
	else
		done = 1;
	lvl.RenderBGEffects();

	objects->renderBackObject(*cam->GetNewCoord());
	lvl.RenderBaseLevel(RenderTarget, cam->GetNewCoord());
	irlicht->render(*cam->GetHeuraCoord());
	scrain->RenderScrain();
	// render things
	effects->renderEffects();

	objects->renderObjects(*cam->GetNewCoord());

	if (cam->GetNewCoord() != NULL)
	{
		SDL_RenderCopy(RenderTarget, lvl.lay_level, cam->GetNewCoord(), NULL);
	}
	else
		SDL_RenderCopy(RenderTarget, lvl.lay_level, NULL, NULL);
	g_interface->renderGameUi(scrain->GetDetect());

	SDL_SetRenderTarget(RenderTarget, nullptr);
#pragma region fadedscreen
	if (firstRun == true || migniecie == 2)
	{
		SDL_SetTextureBlendMode(target_texture, SDL_BLENDMODE_BLEND);

		if (deley == 0 || migniecie == 2)
		{
			if (migniecie == 2)
			{
				if (alpha + 27 < 255)
					alpha += 27;
				else
					alpha = 255;
			}
			else
			{
				if (alpha + 6 < 255)
					alpha += 6;
				else
					alpha = 255;
			}
			SDL_SetTextureAlphaMod(target_texture, alpha);
			if (alpha == 255)
			{
				firstRun = false;
				migniecie = 0;
			}

		}
		else
		{
			if (firstRun == true && alpha == 0 && deley == 10)
			{
				sm.changeAmbient(-1);
				sm.addSound("LevelIntro", sm.getFreeChannel(), 0);
			}
			deley -= 1;
		}
	}
	if (endLevel == true || migniecie == 1)
	{
		SDL_SetTextureBlendMode(target_texture, SDL_BLENDMODE_BLEND);

		if (migniecie == 0)
		{
			if (alpha - 6 > 0)
				alpha -= 6;
			else
				alpha = 0;
		}
		else
		{
			if (alpha - 50 > 70)
				alpha -= 50;
			else
				alpha = 70;
		}
		SDL_SetTextureAlphaMod(target_texture, alpha);
		if (alpha == 0 || alpha == 70 && migniecie == 1)
		{
			endLevel = false;
			if (migniecie == 1)
				migniecie = 2;
		}
	}
#pragma endregion

	SDL_RenderCopy(RenderTarget, target_texture, NULL, NULL);
	SDL_RenderPresent(RenderTarget);

	if (endLevel == true || firstRun == true || migniecie > 0)
		SDL_SetTextureBlendMode(target_texture, SDL_BLENDMODE_NONE);
}

int Game::startLoop(SDL_Window* window)
{
	sm.restartSoundSystem();
	sm.erase();
	SettingsControl st;
	FPSControl c_FPS(59);//59
	Colision col;
	PhysicEngine phisEngine;
	Events gameEvents(&event);
	SDL_Rect rScrain;
	SizeRect ws;
	AI ai;
	lvlid = st.getActualLevel();
	alpha = 0;
	SDL_GetWindowSize(window, &ws.w, &ws.h);
	RenderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE); // | SDL_RENDERER_PRESENTVSYNC
	if (RenderTarget == NULL)
		return 0;
	

	{
		WindowResolution res(RenderTarget, window);
		if (st.getResolution(0).w != 900 && st.getIsFullScreen() == false);
		res.ToSpecificSize(st.getResolution(0).w, st.getResolution(0).h);//1201 533     150 67
		if (st.getIsFullScreen() == true)
			res.ToFullScreen();
	};
	target_texture = SDL_CreateTexture(RenderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 900, 400);
	SDL_SetTextureBlendMode(target_texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(target_texture, alpha);
	irlicht = new Irlicht(RenderTarget);
	objects = new Objects(RenderTarget);
	int bg_effect = lvl.LoadNewLevel(window, RenderTarget, objects);
	objects->loadTextures();
	col.loadColision(st.getLevelName());
	ai.loadGrid();
	cam = new Camera(lvl.baseSize.w, lvl.baseSize.h, ws.w, ws.h, &rScrain);
	scrain = new Scrain(RenderTarget, &gameEvents, &migniecie);
	scrain->LoadScrain(2, 16);
	col.AtCamera(cam);

	phisEngine.loadPointers(scrain, &gameEvents);
	scrain->setStartLoc(cam->GetCameraCoord(), lvl.spawnPosPlayer);
	irlicht->GetPointers(cam);
	irlicht->loadSpawnsPoints(lvl.spawnPosEnemyX, lvl.spawnPosEnemyY, lvl.n_enemySpawn);
	irlicht->load();
	irlicht->spawn(-1);
	ai.setPoint(irlicht->GetPositionPointer()->x, irlicht->GetPositionPointer()->y);
	rScrain = scrain->GetPosition();
	scrain->GetSize(&rScrain);
	cam->Set();
	ai.getPointers(scrain, &col, cam, RenderTarget);
	lvl.LoadBGEffects(RenderTarget, cam->GetNewCoord(), bg_effect);
	objects->loadPointers(scrain, &phisEngine, &gameEvents, window, cam->GetNewCoord(), &ai.isOffLevel, &ai.ghostAchivement, &ai.setFullDetection, &ai.point);
	scrain->getPointers(cam->GetNewCoord(), &phisEngine.forceY.power);
	effects = new Effects(RenderTarget, &rScrain, cam->GetNewCoord());
	col.loadPointers(effects);
	gameEvents.loadPointers(effects, &phisEngine.hidePlaneDetect);
	irlicht->SetPointerToPoint(ai.GetPoint());
	g_interface = new Interface(&objects->n_getflames, &objects->n_maxLevelFlames);
	g_interface->getPointers(RenderTarget, &event, window);
	g_interface->createGameUi();
	deley = 20;
	sm.loadSound("LevelIntro", 0);
	sm.loadSound("Ressurection", 0);
	sm.loadSound("Run", 0);
	sm.loadSound("Jump", 0);
	while (done == 0) // main loop
	{
		c_FPS.start();

		if (endLevel == false && firstRun == false)
		{

			while (SDL_PollEvent(&event) != 0)
			{
				gameEvents.scrain(scrain);

				if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE && firstRun != true)
				{
					done = g_interface->loadMenuUi(1);
					break;
				}

				if (event.type == SDL_QUIT)
				{
					done = 1;
					break;
				}
				//if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q)
				//	pause = 1;
				if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_e)
				{
					if (ai.detection == false)
						ai.detection = true;
					else
						ai.detection = false;
				}
			}
		}
		
		if (done > 0)
			break;
		


		rScrain = scrain->GetPosition();
		scrain->GetSize(&rScrain);
		scrain->isDeath(cam->GetCameraCoord(), effects);
		gameEvents.scrain(scrain);
		phisEngine.grav();
		phisEngine.check(&col);
		irlicht->runAnim();
		ai.CPUControl();
		irlicht->followPoint();

		col.checkCol(scrain, &gameEvents);
		if (scrain->isRessurected() == true)
			effects->removeEffect(1, 0);
		if (col.GetWallDetStatus() > 0 && effects->isRun(1) == false ||
			scrain->isHit() == true && effects->isRun(1) == false)
		{
			scrain->gotHit();
			effects->loadEffect(1, 1, NULL);
		}

		cam->Set();
		render();

		if (alpha == 0 && endLevel == false && firstRun == false && migniecie == 0)
		{
			g_interface->loadMenuUi(2);
			//firstRun = true;
			if (st.getLevelName() == "none")
			{
				if (st.getAchivment(4) == false)
				{
					st.setIsAchivmentDone(4, true);
					st.SaveToFile();
					return 4;
				}
				else
				{
					return 0;
				}
			}
			return 1;
		}

		//if (pause == 1)
		//{
		///	for (;;)
		//	{
		//		SDL_PollEvent(&event);
		//		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q)
		//		{
		//			pause = 0;
		//			break;
		//		}
		///	}
		//}

		if (checkLevelState(st) == true)
		{
			endLevel = true;
			SDL_SetTextureBlendMode(target_texture, SDL_BLENDMODE_BLEND);
		}

		c_FPS.end();
	}
	
	if (done == 1)
		return 0;
	else
		return -1;
}

bool Game::checkLevelState(SettingsControl st)
{

	if (lvlid != st.getActualLevel())
	{
		switch (st.getActualLevel())
		{
		case 0:
		{
			st.setLevelName("tundra1");
			break;
		}
		case 1:
		{
			st.setLevelName("tundra2");
			break;
		}
		case 2:
		{
			st.setLevelName("steppes");
			break;
		}
		case 3:
		{
			st.setLevelName("wetlands1");
			break;
		}
		case 4:
		{
			st.setLevelName("wetlands2");
			break;
		}
		case 5:
		{
			st.setLevelName("wetlands3");
			break;
		}
		default:
			st.setLevelName("none");
			break;
		}
		return true;
	}
	else
		return false;
}