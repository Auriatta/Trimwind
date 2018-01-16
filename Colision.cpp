#include "Colision.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include"Settings.h"

Colision::Colision()
{
	walldetect = 0;
}
Colision::~Colision()
{
	delete[] colisionPionData;
	colisionPionData = nullptr;
	delete[] colisionPozData;
	colisionPozData = nullptr;
	delete[] colisionDeathLineData;
	colisionDeathLineData = nullptr;
	delete[] colisionHideLineData;
	colisionHideLineData = nullptr;
	cam = nullptr;
	effects = nullptr;
	Aline = NULL;
	Bline = NULL;
	Cline = NULL;
	Dline = NULL;
	walldetect = 0;
}

bool Colision::isIntersection(Point p1, Point p2, Point p3, Point p4) 
{
	float x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
	float y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;

	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	if (d == 0) return false;
	
	
	float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
	float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
	float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;


	if (x < min(x1, x2) || x > max(x1, x2) ||
		x < min(x3, x4) || x > max(x3, x4)) return false;
	if (y < min(y1, y2) || y > max(y1, y2) ||
		y < min(y3, y4) || y > max(y3, y4)) return false;

	return true;
}

void Colision::isWallColide(Scrain* scrain, Events* ev)
{
	SDL_Rect r;
	SettingsControl st;
	r = scrain->GetPosition();
	r.w = SCRAINW;
	r.h = SCRAINH;
	int correction = NULL;
	if (isColidePlane(r, &correction, 1, NULL, NULL) == 1)
	{
		if (scrain->GetAnimType() == 2 || scrain->GetAnimType() == 11 || scrain->GetAnimType() == 04 || scrain->GetAnimType() == 44 ||
			scrain->GetAnimType() == 312)
		{
			walldetect = 2;
			r.x -= correction;
			if (scrain->GetAnimType() != 302 && scrain->GetAnimType() != 312)
				scrain->SetAnimType(44);
			if (scrain->GetAnimType() != 312)
				ev->SetException(2);
			
			scrain->SetPosition(r.x, r.y);
			if (scrain->GetAnimType() == 312)
			{
				scrain->ResetScrain(1);
			}
			scrain->SetSpeed(0);
			scrain->resetAnimCycle(0);
		}
		else
			walldetect = 0;
	}
	else
	if (isColidePlane(r, &correction, 1, NULL, NULL) == 7)
	{
		if (scrain->GetAnimType() == 1 || scrain->GetAnimType() == 0 || scrain->GetAnimType() == 03 || scrain->GetAnimType() == 33||
			scrain->GetAnimType() == 311)
		{
			walldetect = 1;
			r.x += correction;
			if (scrain->GetAnimType() != 301 && scrain->GetAnimType() != 311)
				scrain->SetAnimType(33);
			if (scrain->GetAnimType() != 311)
				ev->SetException(1);
			
			scrain->SetPosition(r.x, r.y);
			if (scrain->GetAnimType() == 311)
			{
				scrain->ResetScrain(0);
			}
			scrain->SetSpeed(0);
			scrain->resetAnimCycle(0);
		}
		else
			walldetect = 0;
	}
	else
		walldetect = 0;


	if (walldetect > 0)
	{
		if (st.getDifficulty() == 3)
		{
			if (scrain->GetDetect() < 32)
				scrain->SetDetect(33);
		}
		else
			if (st.getDifficulty() == 2)
			{
				if (scrain->GetDetect() < 20)
					scrain->SetDetect(20);
			}
			else
				if (st.getDifficulty() == 1)
				{
					if (scrain->GetDetect() < 10)
						scrain->SetDetect(10);
				}
	}
}

void Colision::isDeathLineColide(Scrain* scrain, Events* ev)
{
	SDL_Rect data;
	data = scrain->GetPosition();
	SDL_Rect camCoord = cam->GetCameraCoord();
	
	data.x += (data.w / 2);
	data.y += (data.h / 2);
	Dline = data.x - data.w / 2;
	Bline = data.y + data.h / 2;
	Cline = data.x + data.w / 2;
	Aline = data.y - data.h / 2;

	Point p1, p2, p3, p4;
	p1.x = data.x;
	p1.y = data.y;

	for (int i = 0;; i += 4)
	{
		if (colisionDeathLineData[i] != 0)
		{
			p3.x = colisionDeathLineData[i];
			p3.y = colisionDeathLineData[i + 1];
			p4.x = colisionDeathLineData[i + 2];
			p4.y = colisionDeathLineData[i + 3];
			p2.x = Cline;
			p2.y = data.y;
			if (isIntersection(p1, p2, p3, p4) == true)
			{
				ev->SetException(-1); // by usunac efekt blokady klawiszy po dotknieciu sciany przed smiercia
				scrain->SetHP(0);
				StatisticsControl sc;
				sc.setStat(3, sc.getStat(3) + 1);
				break;
			}
			p2.x = Dline;
			p2.y = data.y;
			if (isIntersection(p1, p2, p3, p4) == true)
			{
				ev->SetException(-1);
				scrain->SetHP(0);
				StatisticsControl sc;
				sc.setStat(3, sc.getStat(3) + 1);
				break;
			}
			p2.y = Aline;
			p2.x = data.x;
			if (isIntersection(p1, p2, p3, p4) == true)
			{
				ev->SetException(-1);
				scrain->SetHP(0);
				StatisticsControl sc;
				sc.setStat(3, sc.getStat(3) + 1);
				break;
			}
			p2.y = Bline;
			p2.x = data.x;
			if (isIntersection(p1, p2, p3, p4) == true)
			{
				ev->SetException(-1);
				scrain->SetHP(0);
				StatisticsControl sc;
				sc.setStat(3, sc.getStat(3) + 1);
				break;
			}
		}
		else
			break;
	}
}

bool Colision::isHideLineColide(int x1, int y1, SDL_Rect target)
{
	Point p1, p2, p3, p4;
	p1.x = x1;
	p1.y = y1;
	p2.x = target.x + (target.w/2);
	p2.y = target.y + (target.h/2);
	for (int i = 0;; i += 4)
	{
		if (colisionHideLineData[i] != 0)
		{
			p3.x = colisionHideLineData[i];
			p3.y = colisionHideLineData[i + 1];
			p4.x = colisionHideLineData[i + 2];
			p4.y = colisionHideLineData[i + 3];
			//cout << p1.x << " " << p1.y << " | " << p2.x << " " << p2.y << " | " << p3.x << " " << p3.y << " | " << p4.x << " " << p4.y << endl;
			if (isIntersection(p1, p2, p3, p4) == true)
			{
				return true;
			}
		}
		else
			return false;
	}
	return false;
}

void Colision::checkCol(Scrain* scrain, Events* ev)
{
	// sprawdza czy postac nie natrafila na sciane
	isWallColide(scrain, ev);
	isDeathLineColide(scrain, ev);

}

void Colision::loadColision(string LevelName)
{

	int non = 0;
	//SDL_RWops *savefile = SDL_RWFromFile("testmap.txt", "r");

	fstream savefile;
	savefile.open(LevelName + ".txt", ios::in);

	

	if (savefile.is_open())
	{
		
		delete[] colisionPozData;
		delete[] colisionPionData;
		delete[] colisionDeathLineData;
		delete[] colisionHideLineData;


		int c = 0;
		savefile >> c;
		savefile >> c;
		c += 1;
		colisionPozData = new long[c];
		memset(colisionPozData, 0, c*sizeof(long));
		savefile >> c;
		c += 1;
		colisionPionData = new long[c];
		memset(colisionPionData, 0, c*sizeof(long));
		savefile >> c;
		c += 1;
		colisionDeathLineData = new long[c];
		memset(colisionDeathLineData, 0, c*sizeof(long));
		savefile >> c;
		savefile >> c;
		savefile >> c;
		savefile >> c;
		c += 1;
		colisionHideLineData = new long[c];
		memset(colisionHideLineData, 0, c*sizeof(long));
		savefile >> c;
		c = 0;

		int count = 0;
		for (int i = 0;count < 8; i++)
		{
			switch (count)
			{
			case 0:
			{
				savefile >> colisionPozData[i];
				if (colisionPozData[i] == 0)
				{
					count = 1;
					i = -1;
				}
				break;
			}
			case 1:
			{
				savefile >> colisionPionData[i];
				if (colisionPionData[i] == 0)
				{
					count = 2;
					i = -1;
				}
				break;
			}
			case 2:
			{
				savefile >> colisionDeathLineData[i];
				if (colisionDeathLineData[i] == 0)
				{
					count = 3;
					i = -1;
				}
				break;
			}
			case 3:
			{
				savefile >> non;
				if (non == 0)
				{
					count = 4;
					i = -1;
				}
				break;
			}
			case 4:
			{
				savefile >> non;
				if (non == 0)
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
				savefile >> colisionHideLineData[i];
				if (colisionHideLineData[i] == 0)
				{
					count = 7;
					i = -1;
				}
				break;
			}
			case 7:
			{
				count = 8;
				break;
			}
			};

		}
		
		savefile.close();

	}
	
}

short Colision::isColidePlane(SDL_Rect data, int* correction, bool type, int x1, int y1)
{
	side = 0;
	Point p1, p2, p3, p4;
	if (x1 == 0 && y1 == 0)
	{
		if (data.h == SCRAINH)
			data.h = 64;
		data.x += (data.w / 2);
		data.y += (data.h / 2);
		Dline = data.x - data.w / 2;
		Bline = data.y + data.h / 2;
		Cline = data.x + data.w / 2;
		Aline = data.y - data.h / 2;

		p1.x = data.x;
		p1.y = data.y;
	}
	else
	{
		if (x1 == -1 && y1 == -1)
		{
			Cline = Dline = data.x + data.w;
			Aline = Bline = data.y + data.h;
			p1.x = data.x;
			p1.y = data.y;
		}
		else
		{
			Cline = Dline = data.x + (data.w / 2);
			Aline = Bline = data.y + (data.h / 2);
			p1.x = x1;
			p1.y = y1;
		}
	}


	short i_conter = 0;
	if (type == 0)
		i_conter = 1;
	for (int i = 0;; i += 4)
	{
		if (i_conter == 0)
		{
			if (colisionPionData[i] != 0)
			{
				p3.x = colisionPionData[i];
				p3.y = colisionPionData[i + 1];
				p4.x = colisionPionData[i + 2];
				p4.y = colisionPionData[i + 3];
				p2.x = Cline;
				p2.y = data.y;
				if (isIntersection(p1, p2, p3, p4) == true)
				{
					side = 1;
					if (x1 == 0 && y1 == 0 && correction != NULL)
					*correction = p2.x - p3.x;
					return side;
				}
				p2.x = Dline;
				p2.y = data.y;
				if (isIntersection(p1, p2, p3, p4) == true)
				{

					side = 7;
					if (x1 == 0 && y1 == 0 && correction != NULL)
					*correction = p3.x - p2.x;
					return side;
				}
			}
			else
			{
				break;
			}
		}
		else if (i_conter == 1)
		{
			if (colisionPozData[i] != 0)
			{
				p3.x = colisionPozData[i];
				p3.y = colisionPozData[i + 1];
				p4.x = colisionPozData[i + 2];
				p4.y = colisionPozData[i + 3];
				p2.x = data.x;
				p2.y = Bline;
				if (isIntersection(p1, p2, p3, p4) == true)
				{
					if (x1 == 0 && y1 == 0 || x1 == -1 && y1 == -1)
					{
						if (correction != NULL)
						{
							if (p3.y < p4.y)
								*correction = p2.y - p3.y;
							else
								*correction = p2.y - p4.y;
							if (x1 == -1 && y1 == -1)
								*correction = p2.y - p3.y;
						}
					}
					//cout << p4.x << " " << p3.x << endl;
					side = 5;
					return side;
				}
			}
			else
			{
				break;
			}
		}
	}

	return side;
}

void Colision::getPlaneColideCoord(SDL_Rect data, bool type, int x1, int y1, int* side1, int* side2, int* side3)
{
	Point p1, p2, p3, p4;
	Cline = Dline = data.x + (data.w / 2);
	Aline = Bline = data.y + (data.h / 2);
	p1.x = x1;
	p1.y = y1;

	for (int i = 0;; i += 4)
	{
		if (type == 1)
		{
			if (colisionPionData[i] != 0)
			{
				p3.x = colisionPionData[i];
				p3.y = colisionPionData[i + 1];
				p4.x = colisionPionData[i + 2];
				p4.y = colisionPionData[i + 3];
				p2.x = Cline;
				p2.y = data.y;
				if (isIntersection(p1, p2, p3, p4) == true)
				{
					if (side1 != 0 && side2 != 0)
					{
						*side1 = p3.y;
						*side2 = p4.y;
					}
					if (side3 != 0)
						*side3 = p4.x;
					break;
				}
				p2.x = Dline;
				p2.y = data.y;
				if (isIntersection(p1, p2, p3, p4) == true)
				{
					if (side1 != 0 && side2 != 0)
					{
						*side1 = p3.y;
						*side2 = p4.y;
					}
					if (side3 != 0)
						*side3 = p4.x;
					break;
				}
			}
			else
			{
				break;
			}
		}
		else if (type == 0)
		{
			if (colisionPozData[i] != 0)
			{
				p3.x = colisionPozData[i];
				p3.y = colisionPozData[i + 1];
				p4.x = colisionPozData[i + 2];
				p4.y = colisionPozData[i + 3];
				p2.x = data.x;
				p2.y = Bline;
				if (isIntersection(p1, p2, p3, p4) == true)
				{
					if (side1 != 0 && side2 != 0)
					{
						*side1 = p4.x;
						*side2 = p3.x;
					}
					if (side3 != 0)
						*side3 = p4.y;
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
}


#pragma region func

short Colision::GetWallDetStatus()
{
	return walldetect;
}
void Colision::loadPointers(Effects* effects)
{
	this->effects = effects;
}
void Colision::AtCamera(Camera* camera)
{
	cam = camera;
}
#pragma endregion