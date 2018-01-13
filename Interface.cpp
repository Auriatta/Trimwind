#include "Interface.h"
#include"SDLStuff.h"
#include "SoundSystem.h"
#include <string>

#pragma region Interface
Interface::Interface(int *flamenumber, int *flameMaxnumber)
	: fps(55)
{
	this->flameNumber = flamenumber;
	this->maxFlameNumer = flameMaxnumber;
	bg_poz = {};
	actualMenuType = 0;
}
Interface::~Interface()
{
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(frameMask);
	SDL_DestroyTexture(frame);
	renderer = nullptr;
	flameNumber = nullptr;
	maxFlameNumer = nullptr;
	
}

short Interface::loadMenuUi(short type)
{
	SoundMenager sm;
	sm.loadSound("UIClick", 0);
	sm.loadSound("UIChoice", 0);

	isMousePress = false;
	isESCPress = false;
	if (type == 1 || type == 2)
	{
		SettingsControl st;
		SDL_Surface *sshot;
		bg_rect = {};
		
		if (st.getIsFullScreen() == true)
		{
			int scaleX, scaleY;
			SDL_GetRendererOutputSize(renderer, &scaleX, &scaleY);
			sshot = SDL_CreateRGBSurface(SDL_SWSURFACE, scaleX, scaleY, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
			float resX, resY;
			SDL_RenderGetScale(renderer, &resX, &resY);
			bg_rect.h = scaleY / resY;
			bg_rect.w = scaleX / resX;
		}
		else
			sshot = SDL_CreateRGBSurface(SDL_SWSURFACE, st.getResolution(0).w, st.getResolution(0).h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0);
		SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
		background = SDL_CreateTextureFromSurface(renderer, sshot);
		sshot = nullptr;
		SDL_FreeSurface(sshot);
		if (background == NULL)
			return 1;

	}
	if (type == 0)
	{
		SoundMenager sm;
		sm.loadSound("Theme", 1);
		sm.changeAmbient(-1);
		LoadMainMenu();
	}
	else
		if (type == 1)
			LoadGameMenu();
		else
			if (type == 2)
				LoadGameStats();
			else
				if (type == 3)
					LoadCredits();
				else
					if (type == 4)
						LoadIntro();


	if (type == 0 && actualMenuType != 0 || type == 3)
		actualMenuType = type;
	else
		if (actualMenuType != 1 && actualMenuType != 7 && actualMenuType != 8 && actualMenuType != 9)
			actualMenuType = type;

	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		if (actualMenuType == 0)
		{
			RenderBackground(renderer);
			SDL_RenderCopy(renderer, frameMask, NULL, NULL);
		}

		while (SDL_PollEvent(events) != 0)
		{
			if (events->type == SDL_KEYDOWN && events->key.keysym.sym == SDLK_ESCAPE || events->type == SDL_QUIT)
			{
				isESCPress = true;
			}
			if (events->type == SDL_KEYUP && events->key.keysym.sym == SDLK_ESCAPE)
			{
				isESCPress = false;
			}
			if (events->type == SDL_MOUSEBUTTONDOWN)
			{
				isMousePress = true;
			}
			if (events->type == SDL_MOUSEBUTTONUP)
			{
				isMousePress = false;
			}
		}
		//cout << isMousePress << endl;
		switch (actualMenuId)
		{
		case MainMenu:
		{
			if (MainMenuEvents() == true)
				return 0;
			break;
		}
		case GameMenu:
		{
			switch (GameMenuEvents())
			{
			case 1:
				return 0;
			case 2:
				return 1;
			case 3:
				return 2;
			default:
				break;
			}
			break;
		}
		case SettingMenu:
		{
			if (SettingMenuEvents() == true)
				return 2;
			break;
		}
		case LevelMap:
		{
			if(LevelMapEvents() == true)
				return 1;
			break;
		}
		case AchievementMenu:
		{
			AchievmentsMenuEvents();
			break;
		}
		case GameModes:
		{
			GameModesEvents();
			break;
		}
		case ArenaMenu:
		{
			ArenaMenuEvents();
			break;
		}
		case GameStats:
		{
			if (GameStatsEvents() == 1)
				return 0;
			break;
		}
		case Credits:
		{
			CreditsEvents();
			break;
		}
		case Intro:
		{
			IntroEvents();
			return 3;
			break;
		}
		default:
			break;
		}

		
		if (actualMenuType == 0)
			SDL_RenderCopy(renderer, frame, NULL, NULL);
		SDL_RenderPresent(renderer);
		fps.end();
	}
	return 1;
}
void Interface::renderGameUi(int detectlvl)
{
	progressBar.SetProcent(detectlvl);
	progressBar.render(renderer);
	
	n_rendFlames = 0;
	for (int i = 0; i < *maxFlameNumer; i++)
	{
		if (flameicon[i].isCreated() == true)
			flameicon[i].render(renderer);
		else
			n_rendFlames += 1;
	}
	if (n_rendFlames != *flameNumber)
	{
		flameicon[*flameNumber - 1].remove(renderer, 1, 0);
		for (int i = 0; i < *maxFlameNumer; i++)
		{
			if (flameicon[i].isCreated() == true)
				flameicon[i].move(-(GetProcentPoz(1.5, 0)), -1);
		}
	}
		
}
void Interface::createGameUi()
{
	actualMenuType = 1;
	progressBar.GetPointerToInterface(this);
	progressBar.create(GetProcentPoz(2,0),GetProcentPoz(3,1),0, renderer);
	Point p = {};
	p.x = GetProcentPoz(24, 0);
	p.y = GetProcentPoz(2, 1);

	for (int i = 0; i < *maxFlameNumer; i++)
	{
		flameicon[i].GetPointerToInterface(this);
		flameicon[i].create(p.x, p.y, 2, -1, 0, renderer);
		p.x += GetProcentPoz(1.5, 0);
	}

}
void Interface::ResetBlackPlankTo(Uint8 alpha)
{
	this->alpha = alpha;
}
void Interface::MakeWindowBlack(int speed)
{
	if (alpha + speed < 255)
		alpha += speed;
	else
		alpha = 255;
	if (alpha > 0)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
		SDL_RenderFillRect(renderer, NULL);
	}
}
void Interface::UnmakeWindowBlack(int speed)
{
	if (alpha - speed > 0)
		alpha -= speed;
	else
		alpha = 0;
	if (alpha > 0)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
		SDL_RenderFillRect(renderer, NULL);
	}
}
short Interface::isPlankVisible(bool vis)
{
	if (vis == true)
	{
		if (alpha > 0)
			return true;
		else
			return false;
	}
	else
	{
		if (alpha == 255)
			return true;
		else
			return false;
	}
}

int Interface::GetProcentPoz(double procent, bool t)
{
	//nope
	return result;
}

void Interface::SetInterface()
{
	SDL_RWops *file = SDL_RWFromFile("ui/MainMenuFrame.png", "rb");
	frame = LoadTextureRW(file, renderer);
	file = SDL_RWFromFile("ui/mainMenuBG.png", "rb");
	background = LoadTextureRW(file, renderer);
	SDL_QueryTexture(background, NULL, NULL, &bg_poz.w, &bg_poz.h);
	float rescale = (float)clientArea.h / 1080;
	bg_poz.w = round(bg_poz.w * rescale) + GetProcentPoz(15, 0);
	bg_poz.h = round(bg_poz.h * rescale) + GetProcentPoz(15, 1);
	bg_poz.y = GetProcentPoz(5, 1);
	file = SDL_RWFromFile("ui/MainMenuMask.png", "rb");
	frameMask = LoadTextureRW(file, renderer);

	SDL_FreeRW(file);
}
SDL_Point Interface::getMousePos()
{
	SDL_PumpEvents();

	m_position.x = events->button.x;
	m_position.y = events->button.y;

	return m_position;
}
void Interface::getPointers(SDL_Renderer* renderer, SDL_Event* events, SDL_Window* main_window)
{
	this->renderer = renderer;
	this->events = events;
	this->main_window = main_window;
	SDL_DisplayMode c;
	SDL_GetWindowDisplayMode(main_window, &c);
	clientArea.h = c.h;
	clientArea.w = c.w;
}
void Interface::RenderBackground(SDL_Renderer* renderer)
{
	if (bg_poz.x > -(bg_poz.w/2))
		bg_poz.x -= GetProcentPoz(0.1, 0);
	else
	{
		bg_poz.x = 0;
	}
	SDL_RenderCopy(renderer, background, NULL, &bg_poz);
}

// INTRO
void Interface::LoadIntro()
{
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_RenderClear(renderer);
	image[0].GetPointerToInterface(this);
	image[0].create(-1, -1, 3, -1, 0, renderer);
	actualMenuId = Intro;
	image[1].GetPointerToInterface(this);
	image[2].GetPointerToInterface(this);
	int end = 0;
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		image[0].render(renderer);
		end = image[1].create(-1, -1, 4, 0, 8, renderer);
		if (end == true)
			break;
		SDL_RenderPresent(renderer);
		fps.end();
	}
}
void Interface::IntroEvents()
{
	int time = SDL_GetTicks();
	short logot = 0;
	bool crtd = 0;
	while (true)
	{
		SDL_RenderClear(renderer);
		image[0].render(renderer);
		if (logot == 0)
		{
			image[1].render(renderer);
			if (time + 1300 < SDL_GetTicks())
			{
				logot = 1;
				time = SDL_GetTicks();
			}
		}
		if (logot == 1)
		{
			image[1].remove(renderer, true, 0);
			if (crtd == false)
				crtd = image[2].create(-1, -1, 5, 0, 4, renderer);
			else
				image[2].render(renderer);
			if (time + 1700 < SDL_GetTicks())
			{
				DestroyIntro();
				return;
			}
		}
		SDL_RenderPresent(renderer);
	}
}
void Interface::DestroyIntro()
{
	ResetBlackPlankTo(0);
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		image[0].render(renderer);
		image[2].render(renderer);
		if (isPlankVisible(0) == false)
			MakeWindowBlack(10);
		else
			break;
		SDL_RenderPresent(renderer);
		fps.end();
	}
	image[1].remove(renderer, false, -1);
	image[2].remove(renderer, false, -1);
	image[0].remove(renderer, false, -1);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
}
// CREDITS
void Interface::LoadCredits()
{
	actualMenuId = Credits;
	SettingsControl st;
	for (int i = 0; i < 13; i++)
		text[i].GetPointerToInterface(this);

	#pragma region credits
	if (st.getLanguageId() == 2)
	{
		text[0].add(-1, GetProcentPoz(95, 1), 220, 60, 84, "Gra Stworzona", renderer);
		text[1].add(-1, GetProcentPoz(100, 1), 80, 60, 84, "Przez:", renderer);
		text[2].add(-1, GetProcentPoz(105, 1), 260, 60, 84, "Aleksander Ates", renderer);
		text[3].add(-1, GetProcentPoz(120, 1), 800, 60, 84, "\"Z dedykacja dla Janiny oraz Czeslawa Pieniazek\"", renderer);
		text[4].add(-1, GetProcentPoz(140, 1), 500, 60, 84, "Mapa Trimwind: Weronika Dumala", renderer);
		text[5].add(-1, GetProcentPoz(150, 1), 400, 60, 84, "Audio na licencjach envato", renderer);
		text[6].add(-1, GetProcentPoz(160, 1), 400, 60, 84, "Interface Audio Licencja:", renderer);
		text[7].add(-1, GetProcentPoz(164, 1), 300, 60, 84, "www.freesfx.co.uk", renderer);
		text[8].add(-1, GetProcentPoz(170, 1), 140, 60, 84, "Ambient:", renderer);
		text[9].add(-1, GetProcentPoz(174, 1), 400, 60, 84, "Swamp nagrane przez ecfike", renderer);
		text[10].add(-1, GetProcentPoz(178, 1), 300, 60, 84, "Cave przez Sclolex", renderer);
		text[11].add(-1, GetProcentPoz(190, 1), 1500, 60, 84, "\"Wielkie podziekowania dla Weroniki Dumaly, ktora wspiera mnie w kazdej sprawie.\"", renderer);
		text[12].add(-1, GetProcentPoz(200, 1), 1000, 60, 84, "Copyright © 2017 Aleksander Ates. Wszelkie Prawa Zastrzezone.", renderer);
	}
	else
	{
		text[0].add(-1, GetProcentPoz(95, 1), 220, 60, 84, "Game Created", renderer);
		text[1].add(-1, GetProcentPoz(100, 1), 50, 60, 84, "By:", renderer);
		text[2].add(-1, GetProcentPoz(105, 1), 260, 60, 84, "Aleksander Ates", renderer);
		text[3].add(-1, GetProcentPoz(120, 1), 800, 60, 84, "\"Dedicated to Janina & Czeslaw Pieniazek\"", renderer);
		text[4].add(-1, GetProcentPoz(140, 1), 550, 60, 84, "Trimwind Map: Weronika Dumala", renderer);
		text[5].add(-1, GetProcentPoz(150, 1), 400, 60, 84, "Audio on envato licence", renderer);
		text[6].add(-1, GetProcentPoz(160, 1), 400, 60, 84, "Interface Audio licence:", renderer);
		text[7].add(-1, GetProcentPoz(164, 1), 300, 60, 84, "www.freesfx.co.uk", renderer);
		text[8].add(-1, GetProcentPoz(170, 1), 140, 60, 84, "Ambient:", renderer);
		text[9].add(-1, GetProcentPoz(174, 1), 400, 60, 84, "Swamp recorded by ecfike", renderer);
		text[10].add(-1, GetProcentPoz(178, 1), 240, 60, 84, "Cave by Sclolex", renderer);
		text[11].add(-1, GetProcentPoz(190, 1), 1500, 60, 84, "\"Big thanks to Weronika Dumala, who always supported me with everything.\"", renderer);
		text[12].add(-1, GetProcentPoz(200, 1), 1000, 60, 84, "Copyright © 2017 Aleksander Ates. All Rights Reserved.", renderer);
	}
	#pragma endregion
}
void Interface::CreditsEvents()
{
	SDL_Event event;
	double lastpozy = 0;
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		RenderBackground(renderer);
		SDL_RenderCopy(renderer, frameMask, NULL, NULL);
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_SPACE)
				{
					DestroyCredits();
					actualMenuType = 0;
					actualMenuId = MainMenu;
					SoundMenager sm;
					sm.loadSound("Theme", 1);
					sm.changeAmbient(-1);
					LoadMainMenu();
					return;
				}
			}
		}
		for (int i = 0; i < 13; i++)
		{
			text[i].move(0, GetProcentPoz(0.1, 0));
			text[i].render(renderer);
		}
		lastpozy += 0.1;
		if (lastpozy > 150)
			break;
		SDL_RenderCopy(renderer, frame, NULL, NULL);
		SDL_RenderPresent(renderer);
		fps.end();
	}
	DestroyCredits();
	actualMenuType = 0;
	actualMenuId = MainMenu;
	SoundMenager sm;
	sm.loadSound("Theme", 1);
	sm.changeAmbient(-1);
	LoadMainMenu();
}
void Interface::DestroyCredits()
{
	for (int i = 0; i < 13; i++)
		text[i].remove();
}
// GAME STATS
void Interface::LoadGameStats()
{
	SoundMenager sm;
	sm.restartSoundSystem();
	sm.erase();
	sm.loadSound("Outro", 0);
	sm.loadSound("UIClick", 0);
	sm.loadSound("UIChoice", 0);
	actualMenuId = GameStats;
	button[0].GetPointerToInterface(this);
	window[0].GetPointerToInterface(this);
	text[0].GetPointerToInterface(this);
	text[1].GetPointerToInterface(this);
	text[2].GetPointerToInterface(this);
	text[3].GetPointerToInterface(this);
	text[4].GetPointerToInterface(this);
	text[5].GetPointerToInterface(this);
	text[6].GetPointerToInterface(this);
	text[7].GetPointerToInterface(this);
	//text[8].GetPointerToInterface(this);
	SettingsControl st;
	StatisticsControl sc;
	string output;


	bool endcrt[2];
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		if (bg_rect.h != NULL)
			SDL_RenderCopy(renderer, background, NULL, &bg_rect);
		else
			SDL_RenderCopy(renderer, background, NULL, NULL);
		endcrt[0] = window[0].create(GetProcentPoz(27, 0), GetProcentPoz(20, 1), 4, renderer);
		endcrt[1] = button[0].create(GetProcentPoz(4, 0), GetProcentPoz(10, 1), 6, renderer);
		#pragma region Stats
		if (endcrt[0] == true)
		{
			if (endcrt[1] == true)
			{
				sm.addSound("Outro", sm.getFreeChannel(), 0);
				if (st.getLanguageId() == 2)
					text[0].add(GetProcentPoz(8, 0), GetProcentPoz(12.2, 1), 70, 20, 84, "Kontynuuj", renderer);
				else
					text[0].add(GetProcentPoz(8, 0), GetProcentPoz(12.2, 1), 70, 20, 84, "Continue", renderer);
			}

			if (st.getLanguageId() == 2)
				text[1].add(GetProcentPoz(43, 0), GetProcentPoz(23, 1), 130, 30, 84, "STATYSTYKI", renderer);
			else
				text[1].add(GetProcentPoz(43, 0), GetProcentPoz(23, 1), 120, 30, 84, "STATISTICS", renderer);
			
			if (st.getLanguageId() == 2)
			{
				output = "Udalo ci sie uciec " + to_string(sc.getStat(1)) + " razy.";
				text[2].add(GetProcentPoz(30, 0), GetProcentPoz(30, 1), 200, 25, 84, output.c_str(), renderer);
			}
			else
			{
				output = "You'v escaped " + to_string(sc.getStat(1)) + " times.";
				text[2].add(GetProcentPoz(30, 0), GetProcentPoz(30, 1), 160, 25, 84, output.c_str(), renderer);
			}

			if (st.getLanguageId() == 2)
			{
				output = "Zlapano cie " + to_string(sc.getStat(2)) + " razy.";
				text[3].add(GetProcentPoz(30, 0), GetProcentPoz(36, 1), 140, 25, 84, output.c_str(), renderer);
			}
			else
			{
				output = "You'v been catched " + to_string(sc.getStat(2)) + " times.";
				text[3].add(GetProcentPoz(30, 0), GetProcentPoz(36, 1), 200, 25, 84, output.c_str(), renderer);
			}

			if (st.getLanguageId() == 2)
			{
				output = "Pulapki zabily cie " + to_string(sc.getStat(3)) + " razy";
				text[4].add(GetProcentPoz(30, 0), GetProcentPoz(42, 1), 200, 25, 84, output.c_str(), renderer);
			}
			else
			{
				output = "You'v been killed by traps " + to_string(sc.getStat(3)) + " times";
				text[4].add(GetProcentPoz(30, 0), GetProcentPoz(42, 1), 240, 25, 84, output.c_str(), renderer);
			}

			if (st.getLanguageId() == 2)
			{
				if (sc.getStat(4) == 1)
					output = "Bez wykrycia? Tak";
				else
					output = "Bez wykrycia? Nie";
				text[5].add(GetProcentPoz(30, 0), GetProcentPoz(48, 1), 140, 25, 84, output.c_str(), renderer);
			}
			else
			{
				if (sc.getStat(4) == 1)
					output = "Without detection? Yes";
				else
					output = "Without detection? No";
				text[5].add(GetProcentPoz(30, 0), GetProcentPoz(48, 1), 160, 25, 84, output.c_str(), renderer);
			}

			if (st.getLanguageId() == 2)
			{
				output = "Poziom trundosci: " + (string)st.getDifficultyName();
				text[6].add(GetProcentPoz(30, 0), GetProcentPoz(58, 1), 200, 25, 84, output.c_str(), renderer);
			}
			else
			{
				output = "Difficulty level: " + (string)st.getDifficultyName();
				text[6].add(GetProcentPoz(30, 0), GetProcentPoz(58, 1), 170, 25, 84, output.c_str(), renderer);
			}

			if (sc.getStat(5) == true)
			{
				if (st.getLanguageId() == 2)
				{
					output = "Zdobyto nowe osiagniecie!";
					text[7].add(GetProcentPoz(30, 0), GetProcentPoz(70, 1), 240, 25, 84, output.c_str(), renderer);
				}
				else
				{
					output = "You'v got new achievement!";
					text[7].add(GetProcentPoz(30, 0), GetProcentPoz(70, 1), 240, 25, 84, output.c_str(), renderer);
				}
			}
		}
		#pragma endregion
		
		SDL_RenderPresent(renderer);

		fps.end();
		if (endcrt[0] == true && endcrt[1] == true)
		{
			break;
		}
	}
}
short Interface::GameStatsEvents()
{
	button[0].events();
	if (isMousePress == 1)
	{
		if (button[0].isPushed() == true)
		{
			StatisticsControl sc;
			SoundMenager sm;
			sc.resetStats();
			DestroyGameStats();
			sm.restartSoundSystem();
			sm.erase();
			return 1;
		}
	}
	window[0].render(renderer);
	button[0].render(renderer);
	for (int i = 0; i < 8; i++)
		text[i].render(renderer);
	return 0;
}
void Interface::DestroyGameStats()
{
	bool endrmv[2] = {};
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		endrmv[0] = window[0].remove(renderer);
		endrmv[1] = button[0].remove(renderer);
		for (int i = 0; i < 8; i++)
			text[i].remove();
		SDL_RenderPresent(renderer);
		fps.end();
		if (endrmv[0] == true && endrmv[1] == true)
			break;
	}
}
// MAIN MENU
void Interface::LoadMainMenu()
{
	button[0].GetPointerToInterface(this);
	button[1].GetPointerToInterface(this);
	button[2].GetPointerToInterface(this);
	button[3].GetPointerToInterface(this);
	button[4].GetPointerToInterface(this);
	text[0].GetPointerToInterface(this);
	text[1].GetPointerToInterface(this);
	text[2].GetPointerToInterface(this);
	text[3].GetPointerToInterface(this);
	text[4].GetPointerToInterface(this);
	SettingsControl st;

	short endcrt[4] = {};
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		RenderBackground(renderer);
		SDL_RenderCopy(renderer, frameMask, NULL, NULL);
	
		
		endcrt[0] = button[0].create(-1, GetProcentPoz(2,1), 5, renderer);
		if (endcrt[0] == true)
		{
			endcrt[1] = button[1].create(-1, GetProcentPoz(63,1), 0, renderer);
			if (endcrt[1] == true)
			{
				if (st.getLanguageId() == 2)
					text[0].add(-1, GetProcentPoz(65, 1), 200, 60, 84, "Rozgrywka", renderer);
				else
					text[0].add(-1, GetProcentPoz(65, 1), 220, 60, 84, "Start Game", renderer);
			}

			endcrt[2] = button[2].create(-1, GetProcentPoz(76,1), 0, renderer);
			if (endcrt[2] == true)
			{
				if (st.getLanguageId() == 2)
					text[1].add(-1, GetProcentPoz(78, 1), 100, 60, 84, "Opcje", renderer);
				else
					text[1].add(-1, GetProcentPoz(78, 1), 160, 60, 84, "Options", renderer);
			}

			endcrt[3] = button[3].create(GetProcentPoz(76,0), GetProcentPoz(8,1), 0, renderer);
			if (endcrt[3] == true)
			{
				if (st.getLanguageId() == 2)
					text[2].add(GetProcentPoz(81, 0), GetProcentPoz(10, 1), 150, 60, 84, "Wyjscie", renderer);
				else
					text[2].add(GetProcentPoz(83, 0), GetProcentPoz(10, 1), 90, 60, 84, "Quit", renderer);
			}

		}

		
		SDL_RenderCopy(renderer, frame, NULL, NULL);
		SDL_RenderPresent(renderer);
		if (endcrt[1] + endcrt[0] + endcrt[2] + endcrt[3] == 4)
		{
			text[3].add(GetProcentPoz(85.4, 0), GetProcentPoz(84, 1), 120, 50, 84, st.getLanguageName(), renderer);
			button[4].create(GetProcentPoz(93, 0), GetProcentPoz(85, 1), 3, renderer);
			text[4].add(GetProcentPoz(6, 0), GetProcentPoz(88, 1), 130, 50, 84, "ver. 1.0.1", renderer);
			break;
		}
		fps.end();
	}
	
}
bool Interface::MainMenuEvents()
{
	for (int i = 0; i < 5; i++)
		button[i].events();

	if (isMousePress == 1)
	{
		if (button[0].isPushed() == true)
		{
			DestroyMainMenu();
			actualMenuId = Credits;
			LoadCredits();
			return 0;
		}
		if (button[2].isPushed() == true)
		{
			DestroyMainMenu();
			actualMenuId = SettingMenu;
			LoadSettingMenu();
			return 0;
		}
		if (button[1].isPushed() == true)
		{
			DestroyMainMenu();
			actualMenuId = LevelMap;
			LoadLevelMap();
			return 0;
		}
		if (button[3].isPushed() == true)
		{
			DestroyMainMenu();
			return 1;
		}
		if (button[4].isPushed() == true)
		{
			SettingsControl st;
			st.setLanguage(0, 0);
			st.SaveToFile();
			DestroyMainMenu();
			LoadMainMenu();
			return 0;
		}
	}


	for (int i = 0; i < 5; i++)
		button[i].render(renderer);

	for (int i = 0; i < 5;i++)
		text[i].render(renderer);

	return 0;
}
void Interface::DestroyMainMenu()
{
	
	short endrmv[4] = {};
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		RenderBackground(renderer);
		SDL_RenderCopy(renderer, frameMask, NULL, NULL);

		button[4].remove(renderer);
		for (int i = 0; i < 5; i++)
			text[i].remove();

		for (int i = 0; i < 4; i++)
			endrmv[i] = button[i].remove(renderer);
		
		
		SDL_RenderCopy(renderer, frame, NULL, NULL);
		SDL_RenderPresent(renderer);
		if (endrmv[1] + endrmv[2] + endrmv[3] + endrmv[0] == 4 )
			break;
		fps.end();
	}
	
}
// GAME MODES MENU	
void Interface::LoadGameModes()
{
	button[0].GetPointerToInterface(this);
	button[1].GetPointerToInterface(this);
	button[2].GetPointerToInterface(this);
	text[0].GetPointerToInterface(this);
	text[1].GetPointerToInterface(this);
	text[2].GetPointerToInterface(this);
	SettingsControl st;
	bool result = 0;
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		RenderBackground(renderer);
		SDL_RenderCopy(renderer, frameMask, NULL, NULL);

		result = button[0].create(-1, GetProcentPoz(35, 1), 0, renderer);
		button[1].create(-1, GetProcentPoz(56, 1), 0, renderer);
		button[2].create(GetProcentPoz(76, 0), GetProcentPoz(8, 1), 0, renderer);

		SDL_RenderCopy(renderer, frame, NULL, NULL);
		SDL_RenderPresent(renderer);
		if (result == true)
		{
			if (st.getLanguageId() == 2)
				text[0].add(-1, GetProcentPoz(37, 1), 180, 60, 84, "Opowiesc", renderer);
			else
				text[0].add(-1, GetProcentPoz(37, 1), 100, 60, 84, "Story", renderer);
			if (st.getLanguageId() == 2)
				text[1].add(-1, GetProcentPoz(58, 1), 110, 60, 84, "Próba", renderer);
			else
				text[1].add(-1, GetProcentPoz(58, 1), 200, 60, 84, "Challenge", renderer);
			if (st.getLanguageId() == 2)
				text[2].add(GetProcentPoz(82, 0), GetProcentPoz(10, 1), 110, 60, 84, "Cofnij", renderer);
			else
				text[2].add(GetProcentPoz(83, 0), GetProcentPoz(10, 1), 100, 60, 84, "Back", renderer);
			break;
		}

		fps.end();
	}
}
bool Interface::GameModesEvents()
{

	button[0].events(); // story
	button[1].events(); // challenge
	button[2].events(); // return to main

	if (isMousePress == 1)
	{
		if (button[0].isPushed() == true)
		{
			DestroyGameModes();
			actualMenuId = LevelMap;
			LoadLevelMap();
			return 0;
		}
		
		if (button[1].isPushed() == true)
		{
			DestroyGameModes();
			actualMenuId = ArenaMenu;
			LoadArenaMenu();
			return 0;
		}
		
		if (button[2].isPushed() == true)
		{
			DestroyGameModes();
			actualMenuId = MainMenu;
			LoadMainMenu();
			return 0;
		}
	}

	button[0].render(renderer);
	text[0].render(renderer);
	button[1].render(renderer);
	text[1].render(renderer);
	button[2].render(renderer);
	text[2].render(renderer);
}
void Interface::DestroyGameModes()
{
	short endrmv[2] = {};
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		RenderBackground(renderer);
		SDL_RenderCopy(renderer, frameMask, NULL, NULL);

		text[0].remove();
		text[1].remove();
		text[2].remove();

		button[2].remove(renderer);
		endrmv[0] = button[0].remove(renderer);
		endrmv[1] = button[1].remove(renderer);
		

		SDL_RenderCopy(renderer, frame, NULL, NULL);
		SDL_RenderPresent(renderer);
		if (endrmv[1] + endrmv[0] == 2)
			break;
		fps.end();
	}
}
// LEVEL MENU
void Interface::LoadLevelMap()
{
	blankWindow = false;
	window[0].GetPointerToInterface(this);
	button[0].GetPointerToInterface(this);
	button[1].GetPointerToInterface(this);
	
	text[0].GetPointerToInterface(this);
	text[1].GetPointerToInterface(this);
	text[2].GetPointerToInterface(this);
	text[6].GetPointerToInterface(this);

	text[3].GetPointerToInterface(this);
	checkbox[0].GetPointerToInterface(this);

	SettingsControl st;
	if (st.getReachLevel() > 0)
	{
		text[4].GetPointerToInterface(this);
		checkbox[1].GetPointerToInterface(this);
	}
	if (st.getReachLevel() > 1)
	{
		text[5].GetPointerToInterface(this);
		checkbox[2].GetPointerToInterface(this);
	}
	if (st.getReachLevel() > 2)
	{
		text[7].GetPointerToInterface(this);
		checkbox[3].GetPointerToInterface(this);
	}
	if (st.getReachLevel() > 3)
	{
		text[8].GetPointerToInterface(this);
		checkbox[4].GetPointerToInterface(this);
	}
	if (st.getReachLevel() > 4)
	{
		text[9].GetPointerToInterface(this);
		checkbox[5].GetPointerToInterface(this);
	}
	

	short endcrt[1] = {};
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		RenderBackground(renderer);
		SDL_RenderCopy(renderer, frameMask, NULL, NULL);

		endcrt[0] = window[0].create(-1, -1, 1, renderer);
		if (endcrt[0] == true)
		{
			button[0].create(GetProcentPoz(70, 0), GetProcentPoz(25, 1), 2, renderer);
			button[1].create(-1, GetProcentPoz(70, 1), 1, renderer);
			if (st.getLanguageId() == 2)
			text[0].add(GetProcentPoz(73, 0), GetProcentPoz(25, 1), 100, 50, 84, "Powrót", renderer);
			else
				text[0].add(GetProcentPoz(73, 0), GetProcentPoz(25, 1), 90, 50, 84, "Back", renderer);

			if (st.getLanguageId() == 2)
			text[1].add(-1, GetProcentPoz(70.3, 1), 100, 50, 84, "Wejdz", renderer);
			else
				text[1].add(-1, GetProcentPoz(70.3, 1), 100, 50, 84, "Enter", renderer);

			if (st.getLanguageId() == 2)
				text[6].add(-1, GetProcentPoz(60.3, 1), 300, 50, 84, "Wybierz swój Rozdzial", renderer);
			else
				text[6].add(-1, GetProcentPoz(60.3, 1), 300, 50, 84, "Choose your Chapter", renderer);

			if (st.getLanguageId() == 2)
			text[2].add(GetProcentPoz(20, 0), GetProcentPoz(24, 1), 190, 60, 84, "Opowiesc", renderer);
			else
				text[2].add(GetProcentPoz(20, 0), GetProcentPoz(24, 1), 210, 60, 84, "Story Mode", renderer);
			
			
			text[3].add(GetProcentPoz(34.1, 0), GetProcentPoz(39.1, 1), 20, 60, 84, "I", renderer);
			checkbox[0].create(GetProcentPoz(33, 0), GetProcentPoz(44, 1), 1, renderer);

			if (st.getReachLevel() > 0)
			{
				text[4].add(GetProcentPoz(35.7, 0), GetProcentPoz(57.1, 1), 40, 60, 84, "II", renderer);
				checkbox[1].create(GetProcentPoz(35, 0), GetProcentPoz(62, 1), 1, renderer);
			}
			if (st.getReachLevel() > 1)
			{
				text[5].add(GetProcentPoz(43.1, 0), GetProcentPoz(35.1, 1), 60, 60, 84, "III", renderer);
				checkbox[2].create(GetProcentPoz(43, 0), GetProcentPoz(40, 1), 4, renderer);
			}
			if (st.getReachLevel() > 2)
			{
				text[7].add(GetProcentPoz(51.4, 0), GetProcentPoz(43.1, 1), 50, 60, 84, "IV", renderer);
				checkbox[3].create(GetProcentPoz(51, 0), GetProcentPoz(48, 1), 2, renderer);
			}
			if (st.getReachLevel() > 3)
			{
				text[8].add(GetProcentPoz(58.6, 0), GetProcentPoz(27.1, 1), 40, 60, 84, "V", renderer);
				checkbox[4].create(GetProcentPoz(58, 0), GetProcentPoz(32, 1), 2, renderer);
			}
			if (st.getReachLevel() > 4)
			{
				text[9].add(GetProcentPoz(64.6, 0), GetProcentPoz(45.1, 1), 50, 60, 84, "VI", renderer);
				checkbox[5].create(GetProcentPoz(64, 0), GetProcentPoz(50, 1), 3, renderer);
			}

		}

		
		SDL_RenderCopy(renderer, frame, NULL, NULL);
		SDL_RenderPresent(renderer);
		if (endcrt[0] == true)
			break;
		fps.end();
	}
}
bool Interface::LevelMapEvents()
{
	button[0].events();
	button[1].events();
	checkbox[0].events();
	for (int i = 1; i < 6; i++)
	{
		if (checkbox[i].isCreated())
			checkbox[i].events();
	}

	if (isMousePress == 1)
	{
		if (button[0].isPushed() == true) // back
		{
			DestroyLevelMap();
			actualMenuId = MainMenu;
			LoadMainMenu();
			return 0;
		}
		if (button[1].isPushed() == true) // enter
		{
			SoundMenager sm;
			sm.endAmbient();
			short lvl = -1;
			for (int i = 0; i < 6; i++)
			{
				if (checkbox[i].isActive() == true)
				{
					lvl = i;
					break;
				}
			}
			if (lvl != -1)
			{
				SettingsControl st;
				switch (lvl)
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
					break;
				}
				st.setActualLevel(lvl);
				ResetBlackPlankTo(0);
				blankWindow = true;
				DestroyLevelMap();
				while (true) // musi poczekac az muzyka sie skonczy
				{
					if (sound_data.music_fadeout == 0)
						break;
				}
				return 1;
			}
		}

		#pragma region CheckBoxActive
		if (checkbox[0].isPushed() == true && checkbox[0].isCreated() == true)
		{
			for (int i = 0; i < 6; i++)
			{
				if (i != 0)
				{
					if (checkbox[i].isCreated())
						checkbox[i].setActive(0);
				}
			}
			SettingsControl st;
			if (st.getLanguageId() == 2)
				text[6].update("Rozdzial 1: Poznanie",200,-1, renderer);
			else
				text[6].update("Chapter 1: Cognition", 200, -1, renderer);
		}
		if (checkbox[1].isPushed() == true && checkbox[1].isCreated() == true)
		{
			for (int i = 0; i < 6; i++)
			{
				if (i != 1)
				{
					if (checkbox[i].isCreated())
						checkbox[i].setActive(0);
				}
			}
			SettingsControl st;
			if (st.getLanguageId() == 2)
				text[6].update("Rozdzial 2: Poczatek", 200, -1, renderer);
			else
				text[6].update("Chapter 2: Origin", 172, -1, renderer);
		}
		if (checkbox[2].isPushed() == true && checkbox[2].isCreated() == true)
		{
			for (int i = 0; i < 6; i++)
			{
				if (i != 2)
				{
					if (checkbox[i].isCreated())
						checkbox[i].setActive(0);
				}
			}
			SettingsControl st;
			if (st.getLanguageId() == 2)
				text[6].update("Rozdzial 3: Poswiecenie", 230, -1, renderer);
			else
				text[6].update("Chapter 3: Sacrifice", 188, -1, renderer);
		}
		if (checkbox[3].isPushed() == true && checkbox[3].isCreated() == true)
		{
			for (int i = 0; i < 6; i++)
			{
				if (i != 3)
				{
					if (checkbox[i].isCreated())
						checkbox[i].setActive(0);
				}
			}
			SettingsControl st;
			if (st.getLanguageId() == 2)
				text[6].update("Rozdzial 4: Ciemnosc", 210, -1, renderer);
			else
				text[6].update("Chapter 4: Darkness", 200, -1, renderer);
		}
		if (checkbox[4].isPushed() == true && checkbox[4].isCreated() == true)
		{
			for (int i = 0; i < 6; i++)
			{
				if (i != 4)
				{
					if (checkbox[i].isCreated())
						checkbox[i].setActive(0);
				}
			}
			SettingsControl st;
			if (st.getLanguageId() == 2)
				text[6].update("Rozdzial 5: Czelusci", 195, -1, renderer);
			else
				text[6].update("Chapter 5: Into Deeps", 220, -1, renderer);
		}
		if (checkbox[5].isPushed() == true && checkbox[5].isCreated() == true)
		{
			for (int i = 0; i < 6; i++)
			{
				if (i != 5)
				{
					if (checkbox[i].isCreated())
						checkbox[i].setActive(0);
				}
			}
			SettingsControl st;
			if (st.getLanguageId() == 2)
				text[6].update("Rozdzial 6: Koniec", 187, -1, renderer);
			else
				text[6].update("Chapter 6: Final", 160, -1, renderer);
		}
		#pragma endregion
	}

	window[0].render(renderer);
	button[0].render(renderer);
	button[1].render(renderer);
	for (int i = 0; i < 10;i++)
	text[i].render(renderer);
	for (int i = 0; i < 6; i++)
	checkbox[i].render(renderer);
	
}
void Interface::DestroyLevelMap()
{
	short endcrt[1] = {};
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		RenderBackground(renderer);
		SDL_RenderCopy(renderer, frameMask, NULL, NULL);
		
		for (int i = 0; i < 10; i++)
			text[i].remove();
		button[0].remove(renderer);
		button[1].remove(renderer);
		for (int i = 0; i < 6; i++)
		checkbox[i].remove();

		endcrt[0] = window[0].remove(renderer);

		SDL_RenderCopy(renderer, frame, NULL, NULL);
		if (blankWindow == true)
		MakeWindowBlack(10);
		SDL_RenderPresent(renderer);
		if (endcrt[0] == true)
			break;
		fps.end();
	}
	if (isPlankVisible(1) == false)
		ResetBlackPlankTo(255);

}
// Arena Menu
void Interface::LoadArenaMenu()
{
	blankWindow = false;
	window[0].GetPointerToInterface(this);
	button[0].GetPointerToInterface(this);
	//button[1].GetPointerToInterface(this);
	//button[2].GetPointerToInterface(this);
	//button[3].GetPointerToInterface(this);

	text[0].GetPointerToInterface(this);
	text[1].GetPointerToInterface(this);
	//text[2].GetPointerToInterface(this);
	//text[3].GetPointerToInterface(this);
	//text[4].GetPointerToInterface(this);
	//text[5].GetPointerToInterface(this);

	//checkbox[0].GetPointerToInterface(this);
	//checkbox[1].GetPointerToInterface(this);
	//checkbox[2].GetPointerToInterface(this);
	
	SettingsControl st;
	st.setActualLevel(9);
	short endcrt[1] = {};
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		RenderBackground(renderer);
		SDL_RenderCopy(renderer, frameMask, NULL, NULL);

		endcrt[0] = window[0].create(-1, -1, 3, renderer);
		if (endcrt[0] == true)
		{
			button[0].create(GetProcentPoz(70, 0), GetProcentPoz(23, 1), 2, renderer);
			//button[1].create(-1, GetProcentPoz(70, 1), 1, renderer);
			//button[2].create(GetProcentPoz(75, 0), GetProcentPoz(50, 1), 7, renderer);
			//button[3].create(GetProcentPoz(55.3, 0), GetProcentPoz(50, 1), 8, renderer);
			//checkbox[0].create(GetProcentPoz(58.2, 0), GetProcentPoz(38, 1), 5, renderer);

			if (st.getLanguageId() == 2)
				text[0].add(GetProcentPoz(73, 0), GetProcentPoz(23, 1), 100, 50, 84, "Powrót", renderer);
			else
				text[0].add(GetProcentPoz(73, 0), GetProcentPoz(23, 1), 90, 50, 84, "Back", renderer);

			//if (st.getLanguageId() == 2)
			//	text[1].add(-1, GetProcentPoz(70.3, 1), 100, 50, 84, "Wejdz", renderer);
			//else
			//	text[1].add(-1, GetProcentPoz(70.3, 1), 100, 50, 84, "Enter", renderer);
			if (st.getLanguageId() == 2)
				text[1].add(-1,-1, 700, 80, 84, "Dostepne na Androidzie.", renderer);
			else
				text[1].add(-1, -1, 600, 80, 84, "Only on Android.", renderer);

			//if (st.getLanguageId() == 2)
			//	text[2].add(GetProcentPoz(21, 0), GetProcentPoz(22, 1), 130, 60, 84, "Próba", renderer);
			//else
			//	text[2].add(GetProcentPoz(21, 0), GetProcentPoz(24, 1), 150, 60, 84, "Trial by", renderer);

			//if (st.getLanguageId() == 2)
			//	text[3].add(GetProcentPoz(32.3, 0), GetProcentPoz(35, 1), 200, 50, 84, "Przetrwania", renderer);
			//else
			//	text[3].add(GetProcentPoz(32.2, 0), GetProcentPoz(35, 1), 200, 50, 84, "Surviving", renderer);

			//if (st.getLanguageId() == 2)
			//	text[4].add(GetProcentPoz(22, 0), GetProcentPoz(45.3, 1), 490, 40, 84, "Twój poziom wykrycia jest pelny.", renderer);
			//else
			//	text[4].add(GetProcentPoz(22, 0), GetProcentPoz(45.3, 1), 370, 40, 84, "Your detection level is full.", renderer);

			//if (st.getLanguageId() == 2)
			//	text[5].add(GetProcentPoz(22, 0), GetProcentPoz(48.6, 1), 460, 40, 84, "Uciekaj jak dlugo tylko zdolasz.", renderer);
			//else
			//	text[5].add(GetProcentPoz(22, 0), GetProcentPoz(48.6, 1), 540, 40, 84, "You have to run as long as you can.", renderer);

		}


		SDL_RenderCopy(renderer, frame, NULL, NULL);
		SDL_RenderPresent(renderer);
		if (endcrt[0] == true)
			break;
		fps.end();
	}
}
void Interface::ArenaMenuEvetsTextUpdate()
{
	SettingsControl st;
	switch (st.getActualLevel())
	{
	case 8:
	{
		if (st.getLanguageId() == 2)
			text[3].update("Lotów", 80, -1, renderer);
		else
			text[3].update("Falling", 90, -1, renderer);

		if (st.getLanguageId() == 2)
			text[4].update("Omijaj przeszkody podczas spadania.", 400, -1, renderer);
		else
			text[4].update("While falling from the sky, try to avoid obstacles.", 460, -1, renderer);

		if (st.getLanguageId() == 2)
			text[5].update(" ", 1, -1, renderer);
		else
			text[5].update(" ", 1, -1, renderer);
		break;
	}
	case 9:
	{
		if (st.getLanguageId() == 2)
			text[3].update("Przetrwania", 140, -1, renderer);
		else
			text[3].update("Surviving", 140, -1, renderer);

		if (st.getLanguageId() == 2)
			text[4].update("Twój poziom wykrycia jest pelny.", 340, -1, renderer);
		else
			text[4].update("Your detection level is full.", 270, -1, renderer);

		if (st.getLanguageId() == 2)
			text[5].update("Uciekaj jak dlugo tylko zdolasz.", 330, -1, renderer);
		else
			text[5].update("You have to run as long as you can.", 370, -1, renderer);
		break;
	}
	case 10:
	{
		if (st.getLanguageId() == 2)
			text[3].update("Zbieractwa", 140, -1, renderer);
		else
			text[3].update("Gathering", 130, -1, renderer);

		if (st.getLanguageId() == 2)
			text[4].update("Zbierz wszystkie plomyki porozrzucane,", 380, -1, renderer);
		else
			text[4].update("Gather every single flame from this place.", 400, -1, renderer);

		if (st.getLanguageId() == 2)
			text[5].update("po tym miejscu.", 155, -1, renderer);
		else
			text[5].update(" ", 1, -1, renderer);
		break;
	}
	default:
		break;
	}
}
bool Interface::ArenaMenuEvents()
{
	button[0].events(); // back
	//button[1].events(); // enter
	//button[2].events(); // right
	//button[3].events(); // left

	//checkbox[0].events();
	if (isMousePress == 1)
	{
		if (button[0].isPushed() == true)
		{
			DestroyArenaMenu();
			actualMenuId = MainMenu;
			LoadMainMenu();
			return 0;
		}
		/*
		if (button[1].isPushed() == true)
		{
			//load selected level
		}
		if (button[2].isPushed() == true)
		{
			SettingsControl st;
			if (st.getActualLevel() != 10)
				st.setActualLevel(st.getActualLevel() + 1);
			else
				st.setActualLevel(8);
			ArenaMenuEvetsTextUpdate();
		}
		if (button[3].isPushed() == true)
		{
			SettingsControl st;
			if (st.getActualLevel() != 8)
				st.setActualLevel(st.getActualLevel() - 1);
			else
				st.setActualLevel(10);
			ArenaMenuEvetsTextUpdate();
		}

		if (checkbox[0].isPushed() == true)
		{
			//reszte checkboxow wyzeruj
		}
		*/
	}

	window[0].render(renderer);

	button[0].render(renderer);
	//button[1].render(renderer);
	//button[2].render(renderer);
	//button[3].render(renderer);

	text[0].render(renderer);
	text[1].render(renderer);
	//text[2].render(renderer);
	//text[3].render(renderer);
	//text[4].render(renderer);
	//text[5].render(renderer);

	//checkbox[0].render(renderer);
}
void Interface::DestroyArenaMenu()
{
	short endcrt[1] = {};
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		RenderBackground(renderer);
		SDL_RenderCopy(renderer, frameMask, NULL, NULL);

		for (int i = 0; i < 2; i++)
			text[i].remove();

		//checkbox[0].remove();

		//for (int i = 0; i < 4; i++)
			button[0].remove(renderer);

		endcrt[0] = window[0].remove(renderer);

		SDL_RenderCopy(renderer, frame, NULL, NULL);
		if (blankWindow == true)
			MakeWindowBlack(10);
		SDL_RenderPresent(renderer);
		if (endcrt[0] == true)
			break;
		fps.end();
	}
	if (isPlankVisible(1) == false)
		ResetBlackPlankTo(255);
}
// SETTINGS MENU
void Interface::LoadSettingMenu()
{
	bool endcrt[2] = {};

	for (int i = 0; i < 9;i++)
		button[i].GetPointerToInterface(this);

	for (int i = 0; i < 13; i++)
		text[i].GetPointerToInterface(this);

	checkbox[0].GetPointerToInterface(this);
	checkbox[1].GetPointerToInterface(this);
	slideBar[0].GetPointerToInterface(this);
	window[0].GetPointerToInterface(this);
	SettingsControl st;

	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		RenderBackground(renderer);
		SDL_RenderCopy(renderer, frameMask, NULL, NULL);

		endcrt[0] = window[0].create(-1, -1, 0, renderer);
		endcrt[1] = button[0].create(GetProcentPoz(76, 0), GetProcentPoz(8, 1), 0, renderer);
		

		if (endcrt[0] == true)
		{
			if (endcrt[1] == true)
			{
				if (st.getLanguageId() == 2)
				text[0].add(GetProcentPoz(80, 0), GetProcentPoz(10, 1), 200, 60, 84, "Osiagniecia", renderer);
				else
					text[0].add(GetProcentPoz(80, 0), GetProcentPoz(10, 1), 210, 60, 84, "Achievements", renderer);
			}
			if (st.getLanguageId() == 2)
				text[6].add(GetProcentPoz(35, 0), GetProcentPoz(23, 1), 200, 60, 84, "Ustawienia", renderer);
			else
				text[6].add(GetProcentPoz(35, 0), GetProcentPoz(23, 1), 160, 60, 84, "Settings", renderer);
			if (st.getLanguageId() == 2)
			text[1].add(GetProcentPoz(36, 0), GetProcentPoz(30, 1), 250, 50, 84, "Wielkosc Okna Gry:", renderer);
			else
				text[1].add(GetProcentPoz(36, 0), GetProcentPoz(30, 1), 250, 50, 84, "Game Window Size:", renderer);
			text[2].add(GetProcentPoz(51, 0), GetProcentPoz(31, 1), 120, 45, 84, st.getResolutionName(0), renderer);
			button[5].create(GetProcentPoz(50, 0), GetProcentPoz(31, 1), 4, renderer);
			button[6].create(GetProcentPoz(58, 0), GetProcentPoz(31, 1), 3, renderer);
			if (st.getLanguageId() == 2)
				text[3].add(GetProcentPoz(36, 0), GetProcentPoz(35, 1), 250, 50, 84, "Rozdzielczosc Menu:", renderer);
			else
				text[3].add(GetProcentPoz(36, 0), GetProcentPoz(35, 1), 230, 50, 84, "Menu Resolution:", renderer);
			text[4].add(GetProcentPoz(51, 0), GetProcentPoz(36, 1), 120, 45, 84, st.getResolutionName(1), renderer);
			button[7].create(GetProcentPoz(50, 0), GetProcentPoz(36, 1), 4, renderer);
			button[8].create(GetProcentPoz(58, 0), GetProcentPoz(36, 1), 3, renderer);
			if (st.getLanguageId() == 2)
			text[5].add(GetProcentPoz(40, 0), GetProcentPoz(40.5, 1), 155, 43, 84, "Pelny Ekran:", renderer);
			else
				text[5].add(GetProcentPoz(39, 0), GetProcentPoz(40.5, 1), 160, 43, 84, "Full Screen:", renderer);
			checkbox[1].create(GetProcentPoz(49, 0), GetProcentPoz(40, 1),0, renderer);
			if (st.getLanguageId() == 2)
				text[7].add(GetProcentPoz(36, 0), GetProcentPoz(49, 1), 100, 50, 84, "Dzwiek:", renderer);
			else
				text[7].add(GetProcentPoz(36, 0), GetProcentPoz(49, 1), 100, 50, 84, "Sound:", renderer);
			slideBar[0].create(GetProcentPoz(42, 0), GetProcentPoz(49.3, 1), 0, renderer);
			if (st.getLanguageId() == 2)
				text[8].add(GetProcentPoz(40, 0), GetProcentPoz(54.5, 1), 100, 43, 84, "Wycisz:", renderer);
			else
				text[8].add(GetProcentPoz(40, 0), GetProcentPoz(54.5, 1), 80, 43, 84, "Mute:", renderer);
			checkbox[0].create(GetProcentPoz(45, 0), GetProcentPoz(53.8, 1),0, renderer);
			if (st.getLanguageId() == 2)
				text[9].add(GetProcentPoz(36, 0), GetProcentPoz(63, 1), 235, 50, 84, "Poziom Trudnosci:", renderer);
			else
				text[9].add(GetProcentPoz(36, 0), GetProcentPoz(63, 1), 235, 50, 84, "Difficulty Level:", renderer);

			text[12].add(GetProcentPoz(51.4, 0), GetProcentPoz(63, 1), 120, 50, 84, st.getDifficultyName(), renderer);
			button[3].create(GetProcentPoz(50, 0), GetProcentPoz(64, 1), 4, renderer);
			button[4].create(GetProcentPoz(58, 0), GetProcentPoz(64, 1), 3, renderer);

			button[1].create(GetProcentPoz(55, 0), GetProcentPoz(24, 1), 2, renderer);
			if (st.getLanguageId() == 2)
			text[10].add(GetProcentPoz(58, 0), GetProcentPoz(24, 1), 100, 50, 84, "Powrót", renderer);
			else
				text[10].add(GetProcentPoz(58, 0), GetProcentPoz(24, 1), 70, 50, 84, "Back", renderer);
			button[2].create(GetProcentPoz(56, 0), GetProcentPoz(70, 1), 1, renderer);
			if (st.getLanguageId() == 2)
				text[11].add(GetProcentPoz(57.5, 0), GetProcentPoz(70, 1), 90, 50, 84, "Zapisz", renderer);
			else
				text[11].add(GetProcentPoz(58.3, 0), GetProcentPoz(70, 1), 60, 50, 84, "Save", renderer);
			
			
		}

		SDL_RenderCopy(renderer, frame, NULL, NULL);
		SDL_RenderPresent(renderer);

		if (endcrt[0] == true && endcrt[1] == true)
			break;
		fps.end();
	}

	checkbox[1].setActive(st.getIsFullScreen());
	if (st.getIsFullScreen())
		text[2].update("FullScreen", -1,-1, renderer);
	if (st.getSoundLevel()!= 0)
		slideBar[0].setProcent(st.getSoundLevel());

	
	
}
bool Interface::SettingMenuEvents()
{
	button[0].events();
	button[1].events();
	button[2].events();
	button[3].events(); // poziom trudnosci lewa
	button[4].events(); // poziom trudnosci prawa
	button[5].events(); // rozdzielczosc gry lewa
	button[6].events(); // rozdzielczosc gry prawa
	button[7].events(); // rozdzielczosc menu lewa
	button[8].events(); // rozdzielczosc menu prawa
	checkbox[0].events();
	checkbox[1].events();
	if (checkbox[0].isActive() == false)
	slideBar[0].events();
	
	if (isMousePress == 1)
	{
		if (button[0].isPushed() == true)
		{
			DestroySettingMenu();
			actualMenuId = AchievementMenu;
			LoadAchievmentsMenu();
			return 0;
		}
		if (button[1].isPushed() == true)
		{
			DestroySettingMenu();
			actualMenuId = MainMenu;
			LoadMainMenu();
			return 0;
		}
		if (checkbox[1].isPushed() == true)
		{
			SettingsControl st;
			if (checkbox[1].isActive() == true)
			{
				text[2].update("FullScreen", -1, -1, renderer);
				st.setIsFullScreen(true);
			}
			else
			{
				text[2].update(st.getResolutionName(0), -1, -1, renderer);
				st.setIsFullScreen(false);
			}
		}
		if (checkbox[0].isPushed() == true)
		{
			if (checkbox[0].isActive() == true)
				slideBar[0].setProcent(0);
		}
		if (button[5].isPushed() == true)
		{
			if (checkbox[1].isActive() == false)
			{
				SettingsControl st;
				st.setResolution(0, 0, {});
				text[2].update(st.getResolutionName(0), -1, -1, renderer);
			}
		}
		if (button[6].isPushed() == true)
		{
			if (checkbox[1].isActive() == false)
			{
				SettingsControl st;
				st.setResolution(0, 1, {});
				text[2].update(st.getResolutionName(0), -1, -1, renderer);
			}
		}
		if (button[7].isPushed() == true)
		{
			SettingsControl st;
			st.setResolution(1, 0, {});
			text[4].update(st.getResolutionName(1), -1, -1, renderer);
			
		}
		if (button[8].isPushed() == true)
		{
			SettingsControl st;
			st.setResolution(1, 1, {});
			text[4].update(st.getResolutionName(1), -1, -1, renderer);
		}
		
		if (button[3].isPushed() == true)
		{
			SettingsControl st;
			text[12].update(st.setDifficulty(0, 0), -1, -1, renderer);
			st.setReachLevel(0);
			st.SaveToFile();
		}
		if (button[4].isPushed() == true)
		{
			SettingsControl st;
			text[12].update(st.setDifficulty(0, 1), -1, -1, renderer);
			st.setReachLevel(0);
			st.SaveToFile();
		}
		
		if (button[2].isPushed() == true)
		{
			SoundMenager sm;
			SettingsControl st;
			SDL_DisplayMode DM;
			SDL_GetDesktopDisplayMode(0, &DM);
			st.setSoundLevel(slideBar[0].getProcent());
			sm.restartSoundSystem();
			st.SaveToFile();
			int s = 0;
			SDL_GetWindowSize(main_window, &s, NULL);
			while (st.getResolution(1).w > DM.w && st.getResolution(1).h > DM.h)
			{
				st.setResolution(1, 0, {});
			}
			DestroySettingMenu();
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(main_window);
			return 1;
		}
	}

	window[0].render(renderer);

	for (int i = 0; i < 9;i++)
		button[i].render(renderer);

	for (int i = 0; i < 13; i++)
		text[i].render(renderer);
	
	checkbox[0].render(renderer);
	checkbox[1].render(renderer);
	slideBar[0].render(renderer);

	
}
void Interface::DestroySettingMenu()
{
	
	short endremoving[2] = {};
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		RenderBackground(renderer);
		SDL_RenderCopy(renderer, frameMask, NULL, NULL);

		for (int i = 0; i < 9;i++)
			button[i].remove(renderer);

		checkbox[0].remove();
		checkbox[1].remove();
		slideBar[0].remove();

		for (int i = 0; i < 13;i++)
			text[i].remove();

		endremoving[0] = window[0].remove(renderer);
		endremoving[1] = button[0].remove(renderer);

		SDL_RenderCopy(renderer, frame, NULL, NULL);

		SDL_RenderPresent(renderer);

		if (endremoving[0] == true)
			break;
		fps.end();
	}
	
}
// GAME MENU
void Interface::LoadGameMenu()
{
	blankWindow = false;
	actualMenuId = 1;
	image[0].GetPointerToInterface(this);
	button[0].GetPointerToInterface(this);
	button[1].GetPointerToInterface(this);
	button[2].GetPointerToInterface(this);
	text[0].GetPointerToInterface(this);
	text[1].GetPointerToInterface(this);
	text[2].GetPointerToInterface(this);
	text[3].GetPointerToInterface(this);
	SettingsControl st;
	bool endcrt[2] = {};
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		if (bg_rect.h != NULL)
			SDL_RenderCopy(renderer, background, NULL, &bg_rect);
		else
			SDL_RenderCopy(renderer, background, NULL, NULL);

		endcrt[0] = image[0].create(-1, -1, 1, 0, 10, renderer);

		if (st.getLanguageId() == 2)
			text[2].add(GetProcentPoz(85, 0), GetProcentPoz(6, 1), 50, 30, 84, "Pauza", renderer);
		else
			text[2].add(GetProcentPoz(85, 0), GetProcentPoz(6, 1), 50, 30, 84, "Pause", renderer);

		if (endcrt[0] == true)
		{
			button[0].create(GetProcentPoz(4, 0), GetProcentPoz(45, 1), 6, renderer);//46
			button[2].create(GetProcentPoz(4, 0), GetProcentPoz(60, 1), 6, renderer);
			endcrt[1] = button[1].create(GetProcentPoz(4, 0), GetProcentPoz(75, 1), 6, renderer);//75
			if (endcrt[1] == true)
			{
				if (st.getLanguageId() == 2)
					text[0].add(GetProcentPoz(8, 0), GetProcentPoz(47.5, 1), 70, 20, 84, "Kontynuuj", renderer);
				else
					text[0].add(GetProcentPoz(8.2, 0), GetProcentPoz(47.5, 1), 60, 20, 84, "Continue", renderer);
			
					text[3].add(GetProcentPoz(9.1, 0), GetProcentPoz(62.4, 1), 50, 20, 84, "Restart", renderer);

				if (st.getLanguageId() == 2)
					text[1].add(GetProcentPoz(9.9, 0), GetProcentPoz(77.6, 1), 40, 20, 84, "Wyjdz", renderer);
				else
					text[1].add(GetProcentPoz(9.9, 0), GetProcentPoz(77.6, 1), 30, 20, 84, "Quit", renderer);
			}
		}
		
		text[2].render(renderer);
		SDL_RenderPresent(renderer);

		if (endcrt[0] == true && endcrt[1] == true)
			break;
		fps.end();
	}
}
short Interface::GameMenuEvents()
{
	button[0].events();
	button[1].events();
	button[2].events();
	if (isESCPress == true)
	{
		DestroyGameMenu();
		return 1;
	}
	if (isMousePress == 1)
	{
		
		if(button[0].isPushed())
		{
			DestroyGameMenu();
			return 1;
		}
		if (button[1].isPushed())
		{
			SoundMenager sm;
			blankWindow = true;
			ResetBlackPlankTo(0);
			DestroyGameMenu();
			sm.restartSoundSystem();
			sm.erase();
			return 2;
		}
		if (button[2].isPushed())
		{
			blankWindow = true;
			ResetBlackPlankTo(0);
			DestroyGameMenu();
			return 3;
		}
		
	}

	if (bg_rect.h != NULL)
		SDL_RenderCopy(renderer, background, NULL, &bg_rect);
	else
		SDL_RenderCopy(renderer, background, NULL, NULL);

	image[0].render(renderer);
	button[0].render(renderer);
	button[1].render(renderer);
	button[2].render(renderer);
	text[0].render(renderer);
	text[1].render(renderer);
	text[2].render(renderer);
	text[3].render(renderer);
	return 0;
}
void Interface::DestroyGameMenu()
{
	if (blankWindow == true)
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	bool endcrt[2] = {};


	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		if (bg_rect.h != NULL)
			SDL_RenderCopy(renderer, background, NULL, &bg_rect);
		else
			SDL_RenderCopy(renderer, background, NULL, NULL);
		text[0].remove();
		text[1].remove();
		text[2].remove();
		text[3].remove();
		button[0].remove(renderer);
		button[2].remove(renderer);
		endcrt[0] = button[1].remove(renderer);
		endcrt[1] = image[0].remove(renderer, 1, 0);
		
		if (blankWindow == true)
		{
			MakeWindowBlack(10);
		}
		SDL_RenderPresent(renderer);
		if (endcrt[0] == true && endcrt[1] == true)
		{
			if (blankWindow == true)
				progressBar.remove();

			blankWindow = false;
			break;
		}
		fps.end();
	}

}
// Achievements Menu
void Interface::LoadAchievmentsMenu()
{
	
	window[0].GetPointerToInterface(this);
	button[0].GetPointerToInterface(this);

	for (int i = 0; i < 17;i++)
	text[i].GetPointerToInterface(this);

	for (int i = 0; i < 8; i++)
	image[i].GetPointerToInterface(this);

	SettingsControl st;

	bool endcrt[2] = {};

	for (int i = 0; i < 8; i++)
	{
		if (st.getAchivment(i) == true)
			break;
		if (i == 7)
			endcrt[1] = true;
	}

	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		RenderBackground(renderer);
		SDL_RenderCopy(renderer, frameMask, NULL, NULL);
		
		endcrt[0] = window[0].create(-1, -1, 2, renderer);

		if (endcrt[0] == true)
		{
			if (st.getLanguageId() == 2)
				text[0].add(GetProcentPoz(21, 0), GetProcentPoz(22, 1), 200, 60, 84, "Osiagniecia", renderer);
			else
				text[0].add(GetProcentPoz(21, 0), GetProcentPoz(22, 1), 265, 60, 84, "Achievements", renderer);

			button[0].create(GetProcentPoz(70, 0), GetProcentPoz(23, 1), 2, renderer);
			if (st.getLanguageId() == 2)
				text[1].add(GetProcentPoz(73, 0), GetProcentPoz(22.8, 1), 100, 50, 84, "Powrot", renderer);
			else
				text[1].add(GetProcentPoz(73, 0), GetProcentPoz(23, 1), 80, 50, 84, "Back", renderer);

			/*
			if (st.getLanguageId() == 2)
				text[16].add(GetProcentPoz(36, 0), GetProcentPoz(30, 1), 200, 45, 84, "Zlapane plomyki:", renderer);
			else
				text[16].add(GetProcentPoz(36, 0), GetProcentPoz(30, 1), 200, 45, 84, "Cached flames:", renderer);
			
			text[19].add(GetProcentPoz(53, 0), GetProcentPoz(30, 1), 20, 45, 84, &count, renderer);

			if (st.getLanguageId() == 2)
				text[17].add(GetProcentPoz(36, 0), GetProcentPoz(33, 1), 270, 45, 84, "Poziomy bez wykrycia:", renderer);
			else
				text[17].add(GetProcentPoz(36, 0), GetProcentPoz(33, 1), 300, 45, 84, "Levels without dettection:", renderer);

			if (st.getLanguageId() == 2)
				text[18].add(GetProcentPoz(36, 0), GetProcentPoz(36, 1), 270, 45, 84, "Odblokowane pucharki:", renderer);
			else
				text[18].add(GetProcentPoz(36, 0), GetProcentPoz(36, 1), 300, 45, 84, "Unlocked goblets:", renderer);
				*/

			#pragma region ACHIVEMENTS
			if (st.getLanguageId() == 2)
				text[2].add(GetProcentPoz(25.2, 0), GetProcentPoz(45.4, 1), 250, 45, 84, "Poczatek Przygody", renderer);
			else
				text[2].add(GetProcentPoz(25.2, 0), GetProcentPoz(45.4, 1), 140, 45, 84, "Story Begin", renderer);
			if (st.getLanguageId() == 2)
				text[3].add(GetProcentPoz(25.6, 0), GetProcentPoz(48.6, 1), 410, 40, 84, "Ukoncz pierwszy poziom opowiesci", renderer);
			else
				text[3].add(GetProcentPoz(25.6, 0), GetProcentPoz(48.6, 1), 370, 40, 84, "Finish first level of story mode", renderer);


			if (st.getLanguageId() == 2)
				text[4].add(GetProcentPoz(25.2, 0), GetProcentPoz(53.7, 1), 360, 45, 84, "Szybki Jak Strzala w Kolanie", renderer);
			else
				text[4].add(GetProcentPoz(25.2, 0), GetProcentPoz(53.7, 1), 385, 45, 84, "Fast Like an Arrow in the Knee", renderer);
			if (st.getLanguageId() == 2)
				text[5].add(GetProcentPoz(25.6, 0), GetProcentPoz(57, 1), 280, 40, 84, "Ucieknij przed ognikiem", renderer);
			else
				text[5].add(GetProcentPoz(25.6, 0), GetProcentPoz(57, 1), 310, 40, 84, "Run away from the glimmer", renderer);

			if (st.getLanguageId() == 2)
				text[6].add(GetProcentPoz(25.2, 0), GetProcentPoz(62.3, 1), 140, 45, 84, "Cien ducha", renderer);
			else
				text[6].add(GetProcentPoz(25.2, 0), GetProcentPoz(62.3, 1), 210, 45, 84, "Shadow of ghost", renderer);
			if (st.getLanguageId() == 2)
				text[7].add(GetProcentPoz(25.6, 0), GetProcentPoz(65.3, 1), 410, 40, 84, "Nie wykryto cie przez caly poziom", renderer);
			else
				text[7].add(GetProcentPoz(25.6, 0), GetProcentPoz(65.3, 1), 375, 40, 84, "You was'nt detect by the whole level", renderer);

			if (st.getLanguageId() == 2)
				text[8].add(GetProcentPoz(54, 0), GetProcentPoz(45.4, 1), 200, 45, 84, "Ogniste Serce", renderer);
			else
				text[8].add(GetProcentPoz(54, 0), GetProcentPoz(45.4, 1), 140, 45, 84, "Fiery Heart", renderer);
			if (st.getLanguageId() == 2)
				text[9].add(GetProcentPoz(54.4, 0), GetProcentPoz(48.6, 1), 420, 40, 84, "Znajdz tuzin plomyków w opowiesci", renderer);
			else
				text[9].add(GetProcentPoz(54.4, 0), GetProcentPoz(48.6, 1), 365, 40, 84, "Find dozen flames in story mode", renderer);

			if (st.getLanguageId() == 2)
				text[10].add(GetProcentPoz(54, 0), GetProcentPoz(53.7, 1), 250, 45, 84, "Czy to juz Koniec?", renderer);
			else
				text[10].add(GetProcentPoz(54, 0), GetProcentPoz(53.7, 1), 200, 45, 84, "Is this the End?", renderer);
			if (st.getLanguageId() == 2)
				text[11].add(GetProcentPoz(54.4, 0), GetProcentPoz(57, 1), 260, 40, 84, "Poznaj cala opowiesc", renderer);
			else
				text[11].add(GetProcentPoz(54.4, 0), GetProcentPoz(57, 1), 300, 40, 84, "Get to know the whole story", renderer);

			if (st.getLanguageId() == 2)
				text[12].add(GetProcentPoz(54, 0), GetProcentPoz(62.3, 1), 250, 45, 84, "Zaciekly Kurczak", renderer);
			else
				text[12].add(GetProcentPoz(54, 0), GetProcentPoz(62.3, 1), 200, 45, 84, "Furious Chicken", renderer);
			if (st.getLanguageId() == 2)
				text[13].add(GetProcentPoz(54.4, 0), GetProcentPoz(65.3, 1), 440, 40, 84, "Przejdz gre na najwyzszym poziomie trudnosci", renderer);
			else
				text[13].add(GetProcentPoz(54.4, 0), GetProcentPoz(65.3, 1), 430, 40, 84, "End the game on the highest difficulty level", renderer);

			if (st.getLanguageId() == 2)
				text[14].add(GetProcentPoz(54, 0), GetProcentPoz(37.2, 1), 320, 45, 84, "", renderer);
			else
				text[14].add(GetProcentPoz(54, 0), GetProcentPoz(37.2, 1), 220, 45, 84, "", renderer);
			if (st.getLanguageId() == 2)
				text[15].add(GetProcentPoz(54.4, 0), GetProcentPoz(40.2, 1), 445, 40, 84, "", renderer);
			else
				text[15].add(GetProcentPoz(54.4, 0), GetProcentPoz(40.2, 1), 390, 40, 84, "", renderer);
			if (st.getLanguageId() == 2)
				text[16].add(GetProcentPoz(25.6, 0), GetProcentPoz(37.2, 1), 320, 45, 84, "", renderer);
			else
				text[16].add(GetProcentPoz(25.6, 0), GetProcentPoz(37.2, 1), 220, 45, 84, "", renderer);
			#pragma endregion

			#pragma region Pucharki
			if (endcrt[1] == false)
			{
				if (st.getAchivment(0) == true)
					endcrt[1] = image[0].create(GetProcentPoz(23, 0), GetProcentPoz(47, 1), 0, 0,4, renderer);

				if (st.getAchivment(1) == true)
					endcrt[1] = image[1].create(GetProcentPoz(23, 0), GetProcentPoz(55.3, 1), 0, 0, 4, renderer);

				if (st.getAchivment(2) == true)
					endcrt[1] = image[2].create(GetProcentPoz(23, 0), GetProcentPoz(63.6, 1), 0, 0, 4, renderer);

				if (st.getAchivment(3) == true)
					endcrt[1] = image[3].create(GetProcentPoz(51.6, 0), GetProcentPoz(47, 1), 0, 0, 4, renderer);

				if (st.getAchivment(4) == true)
					endcrt[1] = image[4].create(GetProcentPoz(51.6, 0), GetProcentPoz(55.3, 1), 0, 0, 4, renderer);

				if (st.getAchivment(5) == true)
					endcrt[1] = image[5].create(GetProcentPoz(51.6, 0), GetProcentPoz(63.6, 1), 0, 0, 4, renderer);

				if (st.getAchivment(6) == true)
					endcrt[1] = image[6].create(GetProcentPoz(23, 0), GetProcentPoz(38.6, 1), 0, 0, 4, renderer);

				if (st.getAchivment(7) == true)
					endcrt[1] = image[7].create(GetProcentPoz(51.6, 0), GetProcentPoz(38.6, 1), 0, 0, 4, renderer);
			}
			

			#pragma endregion

			#pragma region renderwhile
			for (int i = 0; i < 17; i++)
				text[i].render(renderer);

			#pragma endregion
		}

		SDL_RenderCopy(renderer, frame, NULL, NULL);
		SDL_RenderPresent(renderer);

		if (endcrt[0] == true && endcrt[1] == true)
		{
			break;
		}
		fps.end();
	}
	
}
void Interface::AchievmentsMenuEvents()
{
	button[0].events();
	if (isMousePress == 1)
	{
		if (button[0].isPushed() == true)
		{
			DestroyAchievmentsMenu();
			actualMenuId = MainMenu;
			LoadMainMenu();
			return;
		}
	}


	window[0].render(renderer);
	button[0].render(renderer);
	for (int i = 0; i < 17; i++)
		text[i].render(renderer);

	for (int i = 0; i < 8; i++)
		image[i].render(renderer);
}
void Interface::DestroyAchievmentsMenu()
{
	
	short endrmv = 0;
	while (true)
	{
		fps.start();
		SDL_RenderClear(renderer);
		RenderBackground(renderer);
		SDL_RenderCopy(renderer, frameMask, NULL, NULL);

		for (int i = 0; i < 17; i++)
		text[i].remove();

		for (int i = 0; i < 8; i++)
		image[i].remove(renderer,0,0);

		button[0].remove(renderer);
		endrmv = window[0].remove(renderer);

		SDL_RenderCopy(renderer, frame, NULL, NULL);
		SDL_RenderPresent(renderer);

		if (endrmv == true)
			break;
		fps.end();
	}
	
}

#pragma endregion

#pragma region Window
Interface::Window::Window()
{
	type = -1;
	Position = {};
	temp_pos = {};
}
Interface::Window::~Window()
{
	SDL_DestroyTexture(t_window);
	intrfc = nullptr;
}
bool Interface::Window::create(int x, int y, int t, SDL_Renderer* rend)
{
	if (t_window == NULL)
	{
		SDL_RWops *file = NULL;
		switch (t)
		{
		case 0:
		{
			file = SDL_RWFromFile("ui/normalWindow.png", "rb");
			break;
		}
		case 1:
		{
			file = SDL_RWFromFile("ui/levelmapWindow.png", "rb");
			break;
		}
		case 2:
		{
			file = SDL_RWFromFile("ui/AchievementsWindow.png", "rb");
			break;
		}
		case 3:
		{
			file = SDL_RWFromFile("ui/ArenaWindow.png", "rb");
			break;
		}
		case 4:
		{
			file = SDL_RWFromFile("ui/StatsWindow.png", "rb");
			break;
		}
		default:
			break;
		}
		t_window = LoadTextureRW(file, rend);
		SDL_QueryTexture(t_window, NULL, NULL, &Position.w, &Position.h);
		if (intrfc->actualMenuType == 0)
		{
			float rescale = (float)intrfc->clientArea.h / 1080;
			Position.w = (int)round(Position.w * rescale);
			Position.h = (int)round(Position.h * rescale);
		}
		if (x == -1)
			x = intrfc->clientArea.w / 2 - Position.w / 2;
		if (y == -1)
			y = intrfc->clientArea.h / 2 - Position.h / 2;

		Position.x = x;
		Position.y = y;
		temp_pos.x = x + (Position.w / 2);
		temp_pos.y = y + (Position.h / 2);
		type = t;
	}
	else
	{

		if (temp_pos.h < Position.h)
		{
			size = (double)5 / 100 * Position.h;
			temp_pos.h += (int)round(size);
			if(temp_pos.h > Position.h)
				temp_pos.h = Position.h;
		}
		else
			temp_pos.h = Position.h;
		if (temp_pos.w < Position.w)
		{
			size = (double)5 / 100 * Position.w;
			temp_pos.w += (int)round(size);
			if(temp_pos.w > Position.w)
				temp_pos.w = Position.w;
		}
		else
			temp_pos.w = Position.w;
		if (temp_pos.x > Position.x)
		{
			size = (double)5 / 100 * (Position.w/2);
			temp_pos.x -= (int)round(size);
			if(temp_pos.x < Position.x)
				temp_pos.x = Position.x;
		}
		else
			temp_pos.x = Position.x;
		if (temp_pos.y > Position.y)
		{
			size = (double)5 / 100 * (Position.h/2);
			temp_pos.y -= (int)round(size);
			if (temp_pos.y < Position.y)
				temp_pos.y = Position.y;
		}
		else
			temp_pos.y = Position.y;

		SDL_RenderCopy(rend, t_window, NULL, &temp_pos);

		if (temp_pos.h == Position.h && temp_pos.w == Position.w)
			return true;
		
	}
	return false;
}
bool Interface::Window::remove(SDL_Renderer* rend)
{
	if (Position.h != 0 && Position.w != 0 && t_window != NULL)
	{
		bool done = 0;

		if (Position.h > 0)
		{
			size = (double)4 / 100 * temp_pos.h;
			Position.h -= (int)round(size);
		}
		else
		{
			Position.h = 0;
		}

		if (Position.w > 0)
		{
			size = (double)4 / 100 * temp_pos.w;
			Position.w -= (int)round(size);
		}
		else
		{
			Position.w = 0;
		}

		if (Position.x > temp_pos.x - (temp_pos.w / 2))
		{
			size = (double)4 / 100 * (temp_pos.w/2);
			Position.x += (int)round(size);
		}
		else
		{
			Position.x = 0;
		}

		if (Position.y > temp_pos.y-(temp_pos.h / 2))
		{
			size = (double)4 / 100 * (temp_pos.h / 2);
			Position.y += (int)round(size);
		}
		else
		{
			Position.y = 0;
		}

		if (Position.h <= 0 && Position.w <= 0)
		{
			done = 1;
		}
		SDL_RenderCopy(rend, t_window, NULL, &Position);
		if (done == 1)
		{
			SDL_DestroyTexture(t_window);
			t_window = NULL;
			Position = {};
			temp_pos = {};
			type = -1;
			return true;
		}
	}
	else
		return true;

	return false;
}
void Interface::Window::render(SDL_Renderer* rend)
{
	if (t_window != NULL)
	SDL_RenderCopy(rend, t_window, NULL, &Position);
}
void Interface::Window::GetPointerToInterface(Interface* intrfc)
{
	this->intrfc = intrfc;
}
#pragma endregion

#pragma region Text
Interface::Text::Text()
{
	r_message = {};
	size = 0;
	iscreated = 0;
}
Interface::Text::~Text()
{
	SDL_DestroyTexture(t_message);
	intrfc = nullptr;
}
bool Interface::Text::isCreated()
{
	return iscreated;
}
void Interface::Text::add(int x, int y, int w, int h, int s, const char* text, SDL_Renderer* rend)
{
	if (r_message.h == NULL && r_message.w == NULL)
	{
		size = s;
		TTF_Font* t_font = TTF_OpenFont("beryliumrg.ttf", size);
		SDL_Color c_font = { 44, 37, 28 };
		SDL_Surface* surfaceMessage = TTF_RenderText_Blended(t_font, text, c_font);
		t_message = SDL_CreateTextureFromSurface(rend, surfaceMessage);
		r_message.w = w;
		r_message.h = h;
		if (intrfc->actualMenuType == 0)
		{
			float rescale = (float)intrfc->clientArea.h / 1080;
			r_message.w = round(r_message.w * rescale);
			r_message.h = round(r_message.h * rescale);
		}
		if (x == -1)
			x = intrfc->clientArea.w / 2 - r_message.w / 2;
		if (y == -1)
			y = intrfc->clientArea.h / 2 - r_message.h / 2;
		r_message.x = x;
		r_message.y = y;
		iscreated = 1;
		SDL_FreeSurface(surfaceMessage);
		TTF_CloseFont(t_font);
	}
}
void Interface::Text::update(const char* text, int w, int h, SDL_Renderer* rend)
{
	TTF_Font* t_font = TTF_OpenFont("beryliumrg.ttf", size);
	SDL_Color c_font = { 44, 37, 28 };
	SDL_Surface* surfaceMessage = TTF_RenderText_Blended(t_font, text, c_font);
	if (w == -1)
		r_message.w = (sizeof(text) / sizeof(char)) * intrfc->GetProcentPoz(1.6,0);
	else
	{
		r_message.w = w;
	}
	if (h != -1)
		r_message.h = h;
	t_message = SDL_CreateTextureFromSurface(rend, surfaceMessage);
	SDL_FreeSurface(surfaceMessage);
	TTF_CloseFont(t_font);
}
void Interface::Text::remove()
{
	if (t_message != NULL)
	{
		SDL_DestroyTexture(t_message);
		r_message = {};
		size = 0;
		iscreated = 0;
	}
}
void Interface::Text::render(SDL_Renderer* rend)
{
	if (t_message != NULL)
		SDL_RenderCopy(rend, t_message, NULL, &r_message);
}
/* 0 up 1 down 2 right 3 left*/
void Interface::Text::move(short side , int speed)
{
	switch (side)
	{
	case 0:
	{
		
		r_message.y -= speed;
		break;
	}
	case 1:
	{
		r_message.y += speed;
		break;
	}
	case 2:
	{
		r_message.x += speed;
		break;
	}
	case 3:
	{
		r_message.x -= speed;
		break;
	}
	default:
		return;
		break;
	}

}
void Interface::Text::GetPointerToInterface(Interface* intrfc)
{
	this->intrfc = intrfc;
}
#pragma endregion

#pragma region Button
Interface::Button::Button()
{
	type = -1;
	p_range = {};
	temp_pos = {};
	isPush = 0;
	
}
Interface::Button::~Button()
{
	SDL_DestroyTexture(t_active);
	SDL_DestroyTexture(t_passive);
	intrfc = nullptr;
}
void Interface::Button::resetPushed()
{
	isPush = NULL;
}
bool Interface::Button::isPushed()
{
	return isPush;
}
bool Interface::Button::create(int x, int y, int t, SDL_Renderer* rend)
{
	if (t_active == NULL && t_passive == NULL)
	{
		SDL_RWops *file = NULL;
		SDL_RWops *file1 = NULL;
			#pragma region type
		switch (t)
		{
		case 0:
		{
			file = SDL_RWFromFile("ui/mainPasButton.png", "rb");
			file1 = SDL_RWFromFile("ui/mainActButton.png", "rb");
			break;
		}
		case 1:
		{
			file = SDL_RWFromFile("ui/normalPasButton.png", "rb");
			file1 = SDL_RWFromFile("ui/normalActButton.png", "rb");
			break;
		}
		case 2:
		{
			file = SDL_RWFromFile("ui/backPasButton.png", "rb");
			file1 = SDL_RWFromFile("ui/backActButton.png", "rb");
			break;
		}
		case 3:
		{
			file = SDL_RWFromFile("ui/rightPasButton.png", "rb");
			file1 = SDL_RWFromFile("ui/rightActButton.png", "rb");
			break;
		}
		case 4:
		{
			file = SDL_RWFromFile("ui/leftPasButton.png", "rb");
			file1 = SDL_RWFromFile("ui/leftActButton.png", "rb");
			break;
		}
		case 5:
		{
			file = SDL_RWFromFile("ui/maintitleButton.png", "rb");
			file1 = SDL_RWFromFile("ui/maintitleButton.png", "rb");
			break;
		}
		case 6:
		{
			file = SDL_RWFromFile("ui/gamePasButton.png", "rb");
			file1 = SDL_RWFromFile("ui/gameActButton.png", "rb");
			break;
		}
		case 7:
		{
			file = SDL_RWFromFile("ui/rightBigPasButton.png", "rb");
			file1 = SDL_RWFromFile("ui/rightBigActButton.png", "rb");
			break;
		}
		case 8:
		{
			file = SDL_RWFromFile("ui/leftBigPasButton.png", "rb");
			file1 = SDL_RWFromFile("ui/leftBigActButton.png", "rb");
			break;
		}
		default:
			break;
		}
		#pragma endregion

		type = t;
		t_passive = LoadTextureRW(file, rend);
		t_active = LoadTextureRW(file1, rend);

		SDL_QueryTexture(t_active, NULL, NULL, &p_range.w, &p_range.h);
		
		float rescale;
		if (intrfc->actualMenuType == 0)
		{
			rescale = (float)intrfc->clientArea.h / 1080;
			p_range.w = round(p_range.w * rescale);
			p_range.h = round(p_range.h * rescale);
		}

		if (x == -1)
			p_range.x = intrfc->clientArea.w / 2 - p_range.w / 2;
		else
			p_range.x = x;

		if (y == -1)
			p_range.y = intrfc->clientArea.h / 2 - p_range.h / 2;
		else
			p_range.y = y;

		temp_pos.y = p_range.y;
		temp_pos.x = p_range.x;
		temp_pos.h = p_range.h;
		temp_pos.w = p_range.w;
		if (type == 0)
		{
			temp_pos.x = intrfc->clientArea.w + 1;
		}
		else
		if (type == 5)
		{
			temp_pos.y = -(p_range.h);
		}
		else
			if (type == 6)
			{
				temp_pos.x = -(p_range.w);
			}

		
	}
	else
	{
		SDL_RenderCopy(rend, t_passive, NULL, &temp_pos);
		switch (type)
		{
		case 0:
		{
			if (temp_pos.x > p_range.x)
			{
				temp_pos.x -= 25;
			}
			else
			{
				temp_pos.x = p_range.x;
				return true;
			}
			break;
		}
		case 5:
		{
			if (temp_pos.y < p_range.y)
			{
				temp_pos.y += 18;
			}
			else
			{
				temp_pos.y = p_range.y;
				return true;
			}
			break;
		}
		case 6:
		{
			if (temp_pos.x < p_range.x)
			{
				temp_pos.x += 25; 
			}
			else
			{
				temp_pos.x = p_range.x;
				return true;
			}
			break;
		}
		default:
		{
				return true;
			break;
		}
		}

		
	}
	return false;
}
bool Interface::Button::remove(SDL_Renderer* rend)
{
	if (p_range.h != 0 && p_range.w != 0 && t_passive != NULL)
	{
		bool done = 0;

		switch (type)
		{
		case 0:
		{
			if (temp_pos.x == p_range.x && temp_pos.x < intrfc->clientArea.w)
			{
				temp_pos.x = intrfc->clientArea.w;
			}
			else
			{
				if (p_range.x <= temp_pos.x)
					p_range.x += 25;
				else
				{
					done = 1;
				}
			}
			break;
		}
		case 5:
		{
			if (temp_pos.y == p_range.y && temp_pos.y + temp_pos.h > 0)
			{
				temp_pos.y = -(intrfc->clientArea.h);
			}
			else
			{
				if (p_range.y >= temp_pos.y)
					p_range.y -= 20;
				else
				{
					done = 1;
				}
			}
			break;
		}
		case 6:
		{
			if (temp_pos.x == p_range.x && temp_pos.x + temp_pos.w > 0)
			{
				temp_pos.x = -(intrfc->clientArea.w);
			}
			else
			{
				if (p_range.x >= temp_pos.x)
					p_range.x -= 25;
				else
				{
					done = 1;
				}
			}
			break;
		}
		default:
		{
			done = 1;
			break;
		}
		}

		if (isPush==1)
			SDL_RenderCopy(rend, t_active, NULL, &p_range);
		else
			SDL_RenderCopy(rend, t_passive, NULL, &p_range);

		if (done == 1)
		{
			SDL_DestroyTexture(t_active);
			SDL_DestroyTexture(t_passive);
			t_active = NULL;
			t_passive = NULL;
			type = -1;
			isPush = NULL;
			temp_pos = {};
			p_range = {};
			return true;
		}
	}
	else
		return true;
	return false;
}
void Interface::Button::events()
{	
	if (keylock == true)
		isPush = false;
	//if (intrfc->getMousePos().x > p_range.x && intrfc->getMousePos().x < p_range.x + p_range.w &&
	//	intrfc->getMousePos().y > p_range.y && intrfc->getMousePos().y < p_range.y + p_range.h)

	if (SDL_EnclosePoints(&intrfc->getMousePos(), 1, &p_range,NULL))
	{
		if (isSelected != true)
		{
			sm.addSound("UIChoice", sm.getFreeChannel(), 0);
			isSelected = true;
		}
		if (intrfc->isMousePress == 1 && keylock == false && isPush == false)
		{
			sm.addSound("UIClick", sm.getFreeChannel(), 0);
			isPush = true;
			keylock = 1;
		}
	}
	else
		if (keylock == false)
			isSelected = false;
	if (intrfc->isMousePress == 0 && keylock != 0)
		keylock = 0;
}
void Interface::Button::render(SDL_Renderer* rend)
{
	if (t_passive != NULL && t_active != NULL)
	{
		if (isSelected == false)
			SDL_RenderCopy(rend, t_passive, NULL, &temp_pos);
		else
			SDL_RenderCopy(rend, t_active, NULL, &temp_pos);
	}
}
void Interface::Button::GetPointerToInterface(Interface* intrfc)
{
	this->intrfc = intrfc;
}
#pragma endregion

#pragma region CheckBox
Interface::CheckBox::CheckBox()
{
	isactive = 0;
	p_range = {};
	iscreated = 0;
	ispushed = 0;
}
Interface::CheckBox::~CheckBox()
{
	SDL_DestroyTexture(t_active);
	SDL_DestroyTexture(t_passive);
	intrfc = nullptr;
}
bool Interface::CheckBox::isActive()
{
	return isactive;
}
bool Interface::CheckBox::isPushed()
{
	return ispushed;
}
bool Interface::CheckBox::isCreated()
{
	return iscreated;
}
void Interface::CheckBox::setActive(bool var)
{
	isactive = var;
}
bool Interface::CheckBox::create(int x, int y, int t, SDL_Renderer* rend)
{
	if (t_active == NULL && t_passive == NULL)
	{
		SDL_RWops *file = NULL;
		SDL_RWops *file1 = NULL;

		switch (t)
		{
		case 0:
		{
			file = SDL_RWFromFile("ui/checkBoxPas.png", "rb");
			file1 = SDL_RWFromFile("ui/checkBoxAct.png", "rb");
			break;
		}
		case 1:
		{
			file = SDL_RWFromFile("ui/LevelTundraIconPas.png", "rb");
			file1 = SDL_RWFromFile("ui/LevelTundraIconAct.png", "rb");
			break;
		}
		case 2:
		{
			file = SDL_RWFromFile("ui/LevelForestIconPas.png", "rb");
			file1 = SDL_RWFromFile("ui/LevelForestIconAct.png", "rb");
			break;
		}
		case 3:
		{
			file = SDL_RWFromFile("ui/LevelWetlandsIconPas.png", "rb");
			file1 = SDL_RWFromFile("ui/LevelWetlandsIconAct.png", "rb");
			break;
		}
		case 4:
		{
			file = SDL_RWFromFile("ui/LevelIslandsIconPas.png", "rb");
			file1 = SDL_RWFromFile("ui/LevelIslandsIconAct.png", "rb");
			break;
		}
		case 5:
		{
			file = SDL_RWFromFile("ui/ArenaButtonPas.png", "rb");
			file1 = SDL_RWFromFile("ui/ArenaButtonAct.png", "rb");
			break;
		}
		default:
			break;
		}
		
		t_passive = LoadTextureRW(file, rend);
		t_active = LoadTextureRW(file1, rend);
		p_range.x = x;
		p_range.y = y;
		SDL_QueryTexture(t_active, NULL, NULL, &p_range.w, &p_range.h);
		if (intrfc->actualMenuType == 0)
		{
			float rescale = (float)intrfc->clientArea.h / 1080;
			p_range.w = round(p_range.w * rescale);
			p_range.h = round(p_range.h * rescale);
		}
		iscreated = 1;
	}
	else
	{
		return true;
	}
	return false;
}
void Interface::CheckBox::remove()
{
	if (t_passive != NULL || t_active != NULL)
	{
		SDL_DestroyTexture(t_active);
		SDL_DestroyTexture(t_passive);
		t_active = NULL;
		t_passive = NULL;
		isactive = 0;
		iscreated = 0;
		p_range = {};
	}
}
void Interface::CheckBox::events()
{
	if (keylock == true)
		ispushed = 0;
	if (intrfc->getMousePos().x > p_range.x && intrfc->getMousePos().x < p_range.x + p_range.w &&
		intrfc->getMousePos().y > p_range.y && intrfc->getMousePos().y < p_range.y + p_range.h &&
		intrfc->isMousePress == 1 && keylock == 0)
	{
		if (isactive == 1)
			isactive = 0;
		else
			isactive = 1;

		sm.addSound("UIChoice", sm.getFreeChannel(), 0);
		ispushed = 1;
		keylock = 1;
	}
	if (intrfc->isMousePress == 0 && keylock != 0)
		keylock = 0;
}
void Interface::CheckBox::render(SDL_Renderer* rend)
{
	if (t_passive != NULL && t_active != NULL)
	{
		if (isActive() == false)
			SDL_RenderCopy(rend, t_passive, NULL, &p_range);
		else
			SDL_RenderCopy(rend, t_active, NULL, &p_range);
	}
}
void Interface::CheckBox::GetPointerToInterface(Interface* intrfc)
{
	this->intrfc = intrfc;
}
#pragma endregion

#pragma region SlideBar
Interface::SlideBar::SlideBar()
{
	p_button = {};
	p_range = {};
	procent = 0;
}
Interface::SlideBar::~SlideBar()
{
	SDL_DestroyTexture(t_button);
	SDL_DestroyTexture(t_bar);
	intrfc = nullptr;
}
double Interface::SlideBar::getProcent()
{
	return procent;
}
void Interface::SlideBar::setProcent(int proc)
{
	procent = round((double)proc / 100 * p_range.w);
	p_button.x = (p_range.x + (int)round(procent)) - (intrfc->GetProcentPoz(0.3, 0) + p_button.w / 2);
	procent = proc;
}
bool Interface::SlideBar::create(int x, int y,int t, SDL_Renderer* rend)
{
	if (p_range.h == 0 && p_range.w == 0 && t_button == NULL && t_bar == NULL)
	{
		SDL_RWops *file = NULL;
		SDL_RWops *file1 = NULL;
		#pragma region type
		switch (t)
		{
		case 0:
		{
			file = SDL_RWFromFile("ui/normalSlideBarButton.png", "rb");
			file1 = SDL_RWFromFile("ui/normalSlideBarSlide.png", "rb");
			break;
		}
		case 1:
		{
			file = SDL_RWFromFile("ui/normalSlideBarButton.png", "rb");
			file1 = SDL_RWFromFile("ui/largeSlideBarSlide.png", "rb");
			break;
		}
		default:
			break;
		}
		#pragma endregion
		t_button = LoadTextureRW(file, rend);
		t_bar = LoadTextureRW(file1, rend);
		p_range.x = x;
		p_range.y = y;
		SDL_QueryTexture(t_bar, NULL, NULL, &p_range.w, &p_range.h);
		SDL_QueryTexture(t_button, NULL, NULL, &p_button.w, &p_button.h);
		if (intrfc->actualMenuType == 0)
		{
			float rescale = (float)intrfc->clientArea.h / 1080;
			p_button.w = round(p_button.w * rescale);
			p_button.h = round(p_button.h * rescale);
			p_range.w = round(p_range.w * rescale);
			p_range.h = round(p_range.h * rescale);
		}
		p_slide = p_range;
		p_range.x += intrfc->GetProcentPoz(0.3, 0);
		p_range.w -= intrfc->GetProcentPoz(1.5, 0);
		p_button.x = x + intrfc->GetProcentPoz(0.3, 0);
		p_button.y = y + intrfc->GetProcentPoz(0.2, 0);
	}
	else
	{
		return true;
	}
	return false;
}
void Interface::SlideBar::remove()
{
	if (p_range.h != 0 && p_range.w != 0 && t_bar != NULL)
	{
		SDL_DestroyTexture(t_bar);
		SDL_DestroyTexture(t_button);
		t_bar = NULL;
		t_button = NULL;
		p_range = {};
		p_button = {};
		p_slide = {};
	}
}
void Interface::SlideBar::events()
{
	if (intrfc->getMousePos().x > p_range.x && intrfc->getMousePos().x < p_range.x + p_range.w &&
		intrfc->getMousePos().y > p_range.y && intrfc->getMousePos().y < p_range.y + p_range.h &&
		intrfc->isMousePress == 1)
	{
		p_button.x = intrfc->getMousePos().x - intrfc->GetProcentPoz(0.1, 0);
		procent = p_button.x - p_range.x;
		procent = round((double)procent / (p_range.w - (p_button.w + intrfc->GetProcentPoz(1, 1))) * 100);
		if (procent > 100)
			procent = 100;
		else
			if (procent < 3)
				procent = 0;
		cout << "PROCENT " << procent << endl;
	}
}
void Interface::SlideBar::render(SDL_Renderer* rend)
{
	if (t_bar != NULL && t_button != NULL)
	{
		SDL_RenderCopy(rend, t_bar, NULL, &p_slide);
		SDL_RenderCopy(rend, t_button, NULL, &p_button);
	}
}
void Interface::SlideBar::GetPointerToInterface(Interface* intrfc)
{
	this->intrfc = intrfc;
}
#pragma endregion

#pragma region Image
Interface::Image::Image()
{
	p_pos = {};
	temp_pos = {};
	alpha = NULL;
}
Interface::Image::~Image()
{
	SDL_DestroyTexture(t_image);
	t_image = NULL;
	intrfc = nullptr;
}
bool Interface::Image::isCreated()
{
	if (t_image != NULL)
		return true;
	else
		return false;
}
void Interface::Image::move(int x, int y)
{
	if (x != -1)
		p_pos.x += x;
	if (y != -1)
		p_pos.y += y;
}
bool Interface::Image::create(int x, int y, int t, int rendType, int speed, SDL_Renderer* rend)
{
	if (t_image == NULL)
	{
		SDL_RWops *file = NULL;
		#pragma region type
		switch (t)
		{
		case 0:
		{
			file = SDL_RWFromFile("ui/pucharekIcon.png", "rb");
			break;
		}
		case 1:
		{
			file = SDL_RWFromFile("ui/GameMenuMask.png", "rb");
			break;
		}
		case 2:
		{
			file = SDL_RWFromFile("ui/flameIcon.png", "rb");
			break;
		}
		case 3:
		{
			file = SDL_RWFromFile("ui/LogoBG.png", "rb");
			break;
		}
		case 4:
		{
			file = SDL_RWFromFile("ui/Logo1.png", "rb");
			break;
		}
		case 5:
		{
			file = SDL_RWFromFile("ui/Logo2.png", "rb");
			break;
		}
		default:
			break;
		}
		#pragma endregion

		t_image = LoadTextureRW(file, rend);

		SDL_QueryTexture(t_image, NULL, NULL, &p_pos.w, &p_pos.h);

		if (x != -1 && y != -1)
		{
			float rescale = (float)intrfc->clientArea.h / 1080;
			p_pos.w = round(p_pos.w * rescale);
			p_pos.h = round(p_pos.h * rescale);
		}
		p_pos.x = x;
		p_pos.y = y;
		animSpeed = speed;
		alpha = 0;
		if (rendType == 0)
			SDL_SetTextureAlphaMod(t_image, alpha);
		else
		{
			return true;
		}
	}
	else
	{
		if (p_pos.x != -1 && p_pos.y != -1)
			SDL_RenderCopy(rend, t_image, NULL, &p_pos);
		else
			SDL_RenderCopy(rend, t_image, NULL, NULL);
		switch (rendType)
		{
		case 0:
		{
			if (alpha == 255)
				return true;
			if (alpha + animSpeed < 255)
				alpha += animSpeed;
			else
				alpha = 255;
			SDL_SetTextureAlphaMod(t_image, alpha);
			break;
		}
		default:
		{
			return true;
			break;
		}
		}
		

	}
	return false;
}
bool Interface::Image::remove(SDL_Renderer* rend, bool anim, int rendType)
{
	if (t_image != NULL)
	{
		if (anim == true)
		{
			if (p_pos.x != -1 && p_pos.y != -1)
				SDL_RenderCopy(rend, t_image, NULL, &p_pos);
			else
				SDL_RenderCopy(rend, t_image, NULL, NULL);
			switch (rendType)
			{
			case 0:
			{
				if (alpha == 0)
				{
					anim = false;
					break;
				}
				if (alpha - animSpeed > 0)
					alpha -= animSpeed;
				else
					alpha = 0;
				SDL_SetTextureAlphaMod(t_image, alpha);
				break;
			}
			default:
			{
				anim = false;
				break;
			}
			}
			if (anim == false)
			{
				SDL_DestroyTexture(t_image);
				t_image = NULL;
				size = NULL;
				alpha = NULL;
				temp_pos = {};
				p_pos = {};
				animSpeed = 0;
				return true;
			}
		}
	}
	else
		return true;
}
void Interface::Image::render(SDL_Renderer* rend)
{
	if (t_image != NULL)
	{
		if (p_pos.x != -1 && p_pos.y != -1)
			SDL_RenderCopy(rend, t_image, NULL, &p_pos);
		else
			SDL_RenderCopy(rend, t_image, NULL, NULL);
	}
}
void Interface::Image::GetPointerToInterface(Interface* intrfc)
{
	this->intrfc = intrfc;
}
#pragma endregion

#pragma region ProgressBar
Interface::ProgressBar::ProgressBar()
{
	cov_pos = {};
	prog_pos = {};
}
Interface::ProgressBar::~ProgressBar()
{
	SDL_DestroyTexture(t_Cover);
	SDL_DestroyTexture(t_Progress);
	intrfc = nullptr;
}
void Interface::ProgressBar::create(int x, int y, int t, SDL_Renderer* rend)
{
	if (t_Cover == NULL)
	{
		SDL_RWops *file = NULL;
		SDL_RWops *file1 = NULL;
		switch (t)
		{
		case 0:
		{
			file = SDL_RWFromFile("ui/DetectionBarCover.png", "rb");
			file1 = SDL_RWFromFile("ui/DetectionBarProgress.png", "rb");
			break;
		}
		default:
			break;
		}
		t_Cover = LoadTextureRW(file, rend);
		t_Progress = LoadTextureRW(file1, rend);
		SDL_QueryTexture(t_Cover, NULL, NULL, &cov_pos.w, &cov_pos.h);
		SDL_QueryTexture(t_Progress, NULL, NULL, &prog_pos.w, &prog_pos.h);
		cov_pos.x = x;
		cov_pos.y = y;
		prog_pos.x = x + intrfc->GetProcentPoz(1.3, 0);
		prog_pos.y = y + intrfc->GetProcentPoz(0.5, 0);
		range = prog_pos.w;
	}
}
void Interface::ProgressBar::remove()
{
	if (t_Cover != NULL && t_Progress != NULL)
	{
		cov_pos = {};
		prog_pos = {};
		SDL_DestroyTexture(t_Cover);
		t_Cover = NULL;
		SDL_DestroyTexture(t_Progress);
		t_Progress = NULL;
	}
}
void Interface::ProgressBar::render(SDL_Renderer* rend)
{
	if (t_Cover != NULL && t_Progress != NULL)
	{
		SDL_RenderCopy(rend, t_Progress, NULL, &prog_pos);
		SDL_RenderCopy(rend, t_Cover, NULL, &cov_pos);
	}
}
void Interface::ProgressBar::SetProcent(double procent)
{
	procent = (double)(procent / 100 * range);
	prog_pos.w = round(procent);
}
void Interface::ProgressBar::GetPointerToInterface(Interface* intrfc)
{
	this->intrfc = intrfc;
}
#pragma endregion


