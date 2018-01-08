#ifndef INTERFACE_H
#define INTERFACE_H
#include"Include.h"
#include "FPSControl.h"
#include "Settings.h"
#include "SoundSystem.h"

#define MainMenu 0
#define GameMenu 1
#define SettingMenu 2
#define LevelMap 3
#define AchievementMenu 4
#define GameModes 5
#define ArenaMenu 6
#define GameStats 7
#define Credits 8
#define Intro 9

/*
TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24); //this opens a font style and sets a size

SDL_Color White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "put your text here", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

SDL_Rect Message_rect; //create a rect
Message_rect.x = 0;  //controls the rect's x coordinate
Message_rect.y = 0; // controls the rect's y coordinte
Message_rect.w = 100; // controls the width of the rect
Message_rect.h = 100; // controls the height of the rect

//Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understance

//Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

//Don't forget too free your surface and texture
*/

#define MAXTEXT 21
#define MAXBUTTONS 9
#define MAXCHECKBOXES 7
#define MAXSLIDEBARS 2
#define MAXWINODWS 2
#define MAXIMAGES 8
#define MAXFLAMEICONS 8

class Interface
{
	
	class Text
	{
		SDL_Rect r_message;
		SDL_Texture* t_message;
		int size;
		bool iscreated;
		Interface* intrfc;
	public:
		Text();
		~Text();
		bool isCreated();
		void render(SDL_Renderer* rend);
		void add(int x, int y, int w, int h, int s, const char* text, SDL_Renderer* rend);
		void update(const char* text, int w, int h, SDL_Renderer* rend);
		void remove();
		/* 0 up 1 down 2 right 3 left*/
		void move(short side, int speed);
		void GetPointerToInterface(Interface* intrfc);
	}text[MAXTEXT];

	class Button
	{
		int type; // 0 = Main Button 1 = Normal Button 2 = Back Button 3 = right button 4 = left button 5 = mainTitle
		bool keylock; // odblokowuje sie gdy pusci przycisk
		SDL_Rect temp_pos;
		SDL_Rect p_range;
		bool isPush;
		bool isSelected;
		SDL_Texture* t_active;
		SDL_Texture* t_passive;
		Interface* intrfc;
		SoundMenager sm;
	public:
		Button();
		~Button();
		void render(SDL_Renderer* rend);
		void events();
		bool create(int x, int y, int t, SDL_Renderer* rend);
		bool remove(SDL_Renderer* rend);
		bool isPushed();
		void resetPushed();
		void GetPointerToInterface(Interface* intrfc);
	}button[MAXBUTTONS];

	class CheckBox
	{
		SDL_Rect p_range;
		bool keylock; // odblokowuje sie gdy pusci przycisk
		bool iscreated;
		bool ispushed;
		bool isactive;
		SDL_Texture* t_active;
		SDL_Texture* t_passive;
		Interface* intrfc;
		SoundMenager sm;
	public:
		CheckBox();
		~CheckBox();
		void render(SDL_Renderer* rend);
		void events();
		bool create(int x, int y, int type, SDL_Renderer* rend);
		void remove();
		bool isActive();
		bool isPushed();
		bool isCreated();
		void setActive(bool var);
		void GetPointerToInterface(Interface* intrfc);
	}checkbox[MAXCHECKBOXES];

	class SlideBar
	{
		SDL_Rect p_button; // dla grafiki przycisku
		SDL_Rect p_range; //  dla pola po ktorym porusza sie przycisk
		SDL_Rect p_slide; // dla grafiki slida
		double procent;
		SDL_Texture* t_button;
		SDL_Texture* t_bar;
		Interface* intrfc;
	public:
		SlideBar();
		~SlideBar();
		void render(SDL_Renderer* rend);
		void events();
		bool create(int x, int y, int t, SDL_Renderer* rend);
		void remove();
		double getProcent();
		void setProcent(int procent);
		void GetPointerToInterface(Interface* intrfc);
	}slideBar[MAXSLIDEBARS];

	class Window
	{
		int type; // 0 = normal 1 = levelmap 2 Achievements
		SDL_Rect Position;
		SDL_Rect temp_pos;
		SDL_Texture* t_window;
		Interface* intrfc;
		double size;
	public:
		Window();
		~Window();
		void render(SDL_Renderer* rend);
		bool create(int x, int y, int t, SDL_Renderer* rend);
		bool remove(SDL_Renderer* rend);
		void GetPointerToInterface(Interface* intrfc);
	}window[MAXWINODWS];
	
	class Image
	{
		SDL_Rect p_pos;
		SDL_Rect temp_pos;
		SDL_Texture* t_image;
		Interface* intrfc;
		Uint8 alpha;
		int animSpeed;
		double size;
	public:
		Image();
		~Image();
		bool isCreated();
		void render(SDL_Renderer* rend);
		void move(int x, int y);
		bool create(int x, int y, int t, int rendtype, int speed, SDL_Renderer* rend);
		bool remove(SDL_Renderer* rend, bool anim, int rendType);
		void GetPointerToInterface(Interface* intrfc);
	}image[MAXIMAGES], flameicon[MAXFLAMEICONS];

	class ProgressBar
	{
		SDL_Rect cov_pos;
		SDL_Rect prog_pos;
		int range;
		SDL_Texture* t_Cover;
		SDL_Texture* t_Progress;
		Interface* intrfc;
	public:
		ProgressBar();
		~ProgressBar();
		void render(SDL_Renderer* rend);
		void create(int x, int y, int t, SDL_Renderer* rend);
		void SetProcent(double procent);
		void remove();
		void GetPointerToInterface(Interface* intrfc);
	}progressBar;

	int actualMenuId = 0; // 0 = Main 1 = GameMenu 2 = setting 3 = levelMap 4 = Achievements aktualnie pokazuj¹ce sie menu
	bool actualMenuType = 0; // 0 = dzieje sie na mainmenu 1 = dzieje sie na spauzowanej grze
	bool isMousePress;
	bool isESCPress;
	int alpha;
	bool blankWindow;
	SDL_Rect bg_rect;
	SDL_Point m_position;
	SizeRect clientArea;
	SDL_Texture* frame;
	SDL_Texture* background;
	SDL_Texture* frameMask;
	SDL_Renderer* renderer;
	SDL_Event* events;
	SDL_Window* main_window;
	FPSControl fps;
	

	SDL_Rect bg_poz;

	void RenderBackground(SDL_Renderer* renderer);

	

	void LoadPauseSettings();
	void PauseSettingsEvents();
	void DestroyPauseSettings();

	void LoadLevelMap();
	bool LevelMapEvents();
	void DestroyLevelMap();

	void LoadMainMenu();
	bool MainMenuEvents();
	void DestroyMainMenu();

	void LoadArenaMenu();
	void ArenaMenuEvetsTextUpdate();
	bool ArenaMenuEvents();
	void DestroyArenaMenu();

	void LoadGameModes();
	bool GameModesEvents();
	void DestroyGameModes();

	void LoadGameMenu();
	short GameMenuEvents();
	void DestroyGameMenu();

	void LoadGameStats();
	short GameStatsEvents();
	void DestroyGameStats();

	void LoadSettingMenu();
	bool SettingMenuEvents();
	void DestroySettingMenu();

	void LoadAchievmentsMenu();
	void AchievmentsMenuEvents();
	void DestroyAchievmentsMenu();

	void LoadCredits();
	void CreditsEvents();
	void DestroyCredits();

	void LoadIntro();
	void IntroEvents();
	void DestroyIntro();

	SDL_Point getMousePos();
	int *flameNumber;
	int *maxFlameNumer;
	int n_rendFlames;
	int GetProcentPoz(double procent, bool t); // t = 0 x t = 1 y

public:
	
	Interface(int *flamenumber, int *flameMaxnumber);
	~Interface();

	short loadMenuUi(short type); // menusy
	void renderGameUi(int detectlvl); // interface podczas rozgrywki
	void createGameUi();

	void SetInterface();
	void getPointers(SDL_Renderer* renderer, SDL_Event* events, SDL_Window* main_window);

	void MakeWindowBlack(int speed);
	void UnmakeWindowBlack(int speed);
	void ResetBlackPlankTo(Uint8 alpha);
	short isPlankVisible(bool vis);
};

#endif