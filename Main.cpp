#include "Include.h"
#include "GameLoop.h"
#include <Windows.h>
#include "Interface.h"
#include <conio.h>
#include "SDL_image.h"
#include <thread>

void ambientThread()
{
	SoundSystem soundsys;
	soundsys.menage();
}

int main(int argc, char *argv[])
{
	SDL_Window* window = 0;
	
	if (0 != SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS))
	{
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);


	AllocConsole();
	SetConsoleTitle(L"Game Console");
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

	SDL_DisplayMode mode;
	SDL_GetDisplayMode(0, 0, &mode);
	int width = mode.w;
	int height = mode.h;

	SDL_Log("Width = %d, Heigh = %d. \n", width, height);

	//window = SDL_CreateWindow(NULL, 0, 0, width, height, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE);
	//900/400
	SettingsControl st;
	st.nullSV_Data();
	st.LoadFromFile();

	short result = 5;
	short gameResult = 0;

	if (st.getIsFullScreen() == true)
		window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, st.getResolution(1).w, st.getResolution(1).h, SDL_WINDOW_FULLSCREEN);
	else
		window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, st.getResolution(1).w, st.getResolution(1).h, NULL);

	if (window == 0)
	{
		SDL_Log("Failed to create window.");
		SDL_Quit();
		return 1;
	}
	
	thread ambientThread(ambientThread);
	SoundMenager sm;
	sm.erase();

	while (result > 0)
	{
		if (gameResult == 0 || gameResult == 4) // uruchamiaj tylko jesli wyjdzie sie z gry
		{

			if (result != 2)
			{
				SDL_SetWindowSize(window, st.getResolution(1).w, st.getResolution(1).h);
				if (st.getIsFullScreen() == true)
					SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
				SDL_DisplayMode DM;
				SDL_GetCurrentDisplayMode(0, &DM);
				SDL_SetWindowPosition(window, (DM.w / 2) - (st.getResolution(1).w / 2), (DM.h / 2) - (st.getResolution(1).h / 2));
			}
			else
			{
				if (st.getIsFullScreen() == true)
					window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, st.getResolution(1).w, st.getResolution(1).h, SDL_WINDOW_FULLSCREEN);
				else
					window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, st.getResolution(1).w, st.getResolution(1).h, NULL);
			}

			SDL_Event event;
			SDL_Renderer* RenderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

			SDL_SetRenderDrawBlendMode(RenderTarget, SDL_BLENDMODE_BLEND);

			if (RenderTarget == NULL)
				result = 0;
			else
			{
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
				Interface g_interface(nullptr, nullptr);
				g_interface.getPointers(RenderTarget, &event, window);
				g_interface.SetInterface();
				if (gameResult == 4)
					result = g_interface.loadMenuUi(3);
				else
				{
					if (result == 5)
						result = g_interface.loadMenuUi(4);
					else
						result = g_interface.loadMenuUi(0);
				}
			}

			SDL_DestroyRenderer(RenderTarget);
			
		}

		if (result == 1)
		{
			gameResult = -1;
			while (gameResult == -1)
			{
				SDL_DisplayMode DM;
				SDL_GetCurrentDisplayMode(0, &DM);
				SDL_SetWindowSize(window, 900, 400);
				SDL_SetWindowFullscreen(window, 0);
				SDL_SetWindowPosition(window, (DM.w / 2) - (st.getResolution(0).w / 2), (DM.h / 2) - (st.getResolution(0).h / 2));
				
				Game Game;
				gameResult = Game.startLoop(window);

			}
			
		}
	}
	SDL_DestroyWindow(window);
	FreeConsole();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
	exit(1);
}