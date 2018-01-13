#include"AI.h"
#include <fstream>
#include<time.h>
#include<conio.h>
#include"Settings.h"

AI::AI()
{
	spawn = {};
	firstRun = true;
	isOffLevel = true;
	next = 0;
	ghostAchivement = true;
	minDist = -1;
	prevTheta = -1;
	endCicle = 0;
	r = 0;
	deley = 0;
	r_deley = 0;
	c_speed = 90;
	theta = 0;
	actualPPId = -1;
	m_speedX = m_speedY = 3;
	setCorrection();
	n_pathPoints = 0;
	status = 0;
	srand(time(NULL));
	audioHunt = 0;
	detect_regionSize = 2000;
	SoundMenager sm;
	sm.loadSound("Hunt", 0);
	sm.loadSound("HuntEnd", 0);
	sm.loadSound("IrlichtDeath", 0);
}
AI::~AI()
{
	delete pathPoint;
	pathPoint = nullptr;
	delete[] pf_checkedIds;
	pf_checkedIds = nullptr;
	delete[] pf_wayIds;
	pf_wayIds = nullptr;
	scrain = nullptr;
	col = nullptr;
	cam = nullptr;
	renderer = nullptr;
	camr = nullptr;
}

AI::grid::grid()
{
	x = y = 0;
	N_links = 0;
	memset(&linkIndex, -1, 7 * sizeof(int));
}

void AI::CPUControl()
{

	if (scrain->isRessurected() == true || point.x == 0 && point.y == 0) // koniec poscigu
	{
		if (setFullDetection == true)
		{
			moveType = 0;
			point = spawn;
			AIfindNearPoint(0);
			status = 0;
			setFullDetection = false;
			isOffLevel = true;
		}
	}

	if (firstRun == true)
	{
		spawn = point;
		firstRun = false;
	}

	

#pragma region correct from plane col
	if (status != 0)
	{
		if (point.x < (camr->x) + (camr->w) && point.x > camr->x &&
			point.y < (camr->y) + (camr->h) && point.y> camr->y)
		{
			AICheckPlane();
			if (correction != 0 && status != 4)
			{
				if ( status == 5)
				{
					pf_goal_c = goal;
					status = 4;
					return;
				}
				
				point.y += correction;
				moveType = 1;

				correction = 0;
				return;
			}
		}
	}
#pragma endregion

	//cout << "status " << status << "|" << point.x << " " << point.y << "|" << goal.x << " " << goal.y << endl;

	if (scrain->isRessurected() == true && status == 3 && setFullDetection != true)
	{
		SoundMenager sm;
		sm.stopSound(HUNTCHANNEL);
		AIfindNearPoint(0);
		goal = point;
		status = 0;
	}

	if (isOffLevel == true)
		scrain->SetDetect(0);
	else
	{
		#pragma region POSCIG
		if (setFullDetection == true) // poœcig
		{
			if (scrain->GetDetect() < 90)
			{
				AIfindNearPoint(1);
				point = pf_goal_c;
				pf_goal_c = {};
				pf_goal_i = -1;
				status = 3;
				SoundMenager sm;
				if (audioHunt == 0)
					sm.stopSound(ENDHUNTCHANEL);
				sm.addSound("Hunt", HUNTCHANNEL, -1);
				audioHunt = 1;
			}
			if (point.x > (camr->x) + (camr->w) || point.x < camr->x &&
				point.y > (camr->y) + (camr->h) || point.y < camr->y)
			{
				if (m_speedX != 10)
				{
					m_speedX = 10;
					m_speedY = 9;
					setCorrection();
				}
			}
			scrain->SetDetect(100);
		}
		#pragma endregion
	}

	
#pragma region DETECTION
	#pragma region Looking
	if (scrain->GetPosition().x + (scrain->GetPosition().w / 2) < point.x + DETECT_SIZE && scrain->GetPosition().x + (scrain->GetPosition().w / 2) > point.x - DETECT_SIZE &&
		scrain->GetPosition().y + (scrain->GetPosition().h / 2) < point.y + (DETECT_SIZE-200) && scrain->GetPosition().y + (scrain->GetPosition().h / 2) > point.y  - (DETECT_SIZE-200)&&
		status != 3 && status != 4 && scrain->isUntouch() != 1 && detection == true && SDL_GetTicks() - DeleybtwCatch > 700)
	{
		if (col->isColidePlane(scrain->GetPosition(), NULL, 1, point.x, point.y) == 0 &&
			col->isColidePlane(scrain->GetPosition(), NULL, 0, point.x, point.y) == 0 &&
			col->isHideLineColide(point.x, point.y, scrain->GetPosition()) == 0)
		{
			if (scrain->isHide() == false || SDL_GetTicks() - DeleybtwCatch < 1000)
			{
				DeleybtwCatch = NULL;
				if (ghostAchivement != false)
					ghostAchivement = false;
				if (status != 2 && status != 4)
				{
					moveType = 1;
					timeHock = SDL_GetTicks();
					SettingsControl st;
					if(st.getDifficulty() == 3)
						timePassed = (rand() % 300) + 10;
					else
					{
						timePassed = (rand() % 2000) + 200;
						if (timePassed < 400)
						{
							timePassed = timeHock = 0;
							moveType = 0;
						}
					}
					irlichtTurn = 1;
				}
				if (m_speedX != 5 || m_speedY != 4)
				{
					m_speedX = 5;
					m_speedY = 4;
					setCorrection();
				}
				status = 3;
				next = 0;
				minDist = -1;
				prevTheta = -1;
				endCicle = 0;
				r = 0;
				deley = 0;
				r_deley = 0;
				c_speed = 90;
				theta = 0;
				SoundMenager sm;
				if (audioHunt == 0)
					sm.stopSound(ENDHUNTCHANEL);
				sm.addSound("Hunt", HUNTCHANNEL, -1);
				audioHunt = 1;
				return;
			}
		}
	}
	if (col->isHideLineColide(point.x, point.y, scrain->GetPosition()) == true || col->isColidePlane(scrain->GetPosition(), NULL, 1, point.x, point.y) != 0)
	{
		if (status != 0 && status != 1 && status != 5 && status != 6 && setFullDetection != true)
		{
			if (moveType != 0)
				moveType = 0;

			pf_goal_i = -1;
			pf_goal_c = {};
			pf_nextId = -1;
			timeHock = 0;
			timePassed = 0;
			AIfindNearPoint(1);
			AIfindNearPoint(0);
			status = 5;
			return;

		}
	}
	else
	{
		if (point.x < cam->GetCameraCoord().x && status == 3 || point.x > cam->GetCameraCoord().x + cam->GetCameraCoord().w && status == 3 ||
			point.y < cam->GetCameraCoord().y && status == 3 || point.y > cam->GetCameraCoord().y + cam->GetCameraCoord().h && status == 3)
		{
			if (scrain->GetDetect() < 90)
			{
				if (moveType != 0)
					moveType = 0;
				timeHock = SDL_GetTicks();

				SettingsControl st;
				if (st.getDifficulty() == 1)
					timePassed = (rand() % 1000) + 400;
				else
					if (st.getDifficulty() == 2)
						timePassed = (rand() % 1500) + 900;
					else
						if (st.getDifficulty() == 3)
							timePassed = (rand() % 2500) + 1100;
				//SoundMenager sm;
				//sm.stopSound(HUNTCHANNEL);
				status = 2;
				return;
			}
		}

	}
#pragma endregion
	#pragma region Detect Region
	if (isOffLevel == false)
	{
		if (scrain->GetDetect() > 0 && scrain->GetDetect() < 20)
			detect_regionSize = 2300;
		else
			if (scrain->GetDetect() > 19 && scrain->GetDetect() < 40)
				detect_regionSize = 1600;
			else
				if (scrain->GetDetect() > 39 && scrain->GetDetect() < 60)
					detect_regionSize = 1200;
				else
					if (scrain->GetDetect() > 59 && scrain->GetDetect() < 90)
						detect_regionSize = 500;

		if (status != 6 && status != 5 && status != 3 && status != 2 && status != 4 && scrain->GetDetect() > 89
			&& SDL_GetTicks() - detect_deley > 8000)
		{
			goal.x = scrain->GetPosition().x + (scrain->GetPosition().w / 2);
			goal.y = scrain->GetPosition().y + (scrain->GetPosition().h / 2);
			AIfindNearPoint(1);
			if (pf_goal_i != actualPPId)
			{
				//cout << "GetDetection" << endl;
				next = 0;
				minDist = -1;
				prevTheta = -1;
				endCicle = 0;
				r = 0;
				deley = 0;
				r_deley = 0;
				c_speed = 90;
				theta = 0;
				cout << "I will look for :" << pf_goal_i << endl;
				bool result = AIFindPathTo(pf_goal_i, 0, 0, 0);
				detect_deley = SDL_GetTicks();
				goal = point;
				setCorrection();
				if (result == true)
					status = 6;
				else
				{
					cout << "\nCant find path to " << endl;
					status = 0;
				}
			}
			else
			{
				pf_goal_c = {};
				pf_goal_i = 0;
				if (deley != 0)
					deley = SDL_GetTicks();
			}
		}
	}
	else
	{
		if (detect_regionSize != 99999)
			detect_regionSize = 99999;
	}
	#pragma endregion
#pragma endregion

#pragma region STATUS 0
	if (status == 0) // find near point
	{
		if (point.x < goal.x + crrX && point.x > goal.x - crrX && point.y < goal.y + crrY && point.y > goal.y - crrY)
		{
			if (ComparePointsSearch(0) != -1)
			{
				actualPPId = ComparePointsSearch(0);
				if (pathPoint[actualPPId].N_links == 1)
				{
					goal.x = pathPoint[pathPoint[actualPPId].linkIndex[0]].x;
					goal.y = pathPoint[pathPoint[actualPPId].linkIndex[0]].y;
					return;
				}
				else
					status = 1;
			}
			else
			{
				AIfindNearPoint(0);
				moveType = 0;
				timeHock = 0;
				timePassed = 0;
			}
		}
		else
		{
			//cout << "Move to" << endl;
			AImoveToPoint();
			if (prevMoveX == point.x) // IF STUCK
			{
				point.x = goal.x;
				point.y = goal.y;
			}
		}
	}
#pragma endregion

#pragma region STATUS 1
	if (status == 1) // patrol
	{
		if (m_speedX != 3 || m_speedY != 3)
		{
			m_speedX = 3;
			m_speedY = 3;
			setCorrection();
		}
		//cout << "status 1 point " << actualPPId << endl;
		if (point.x < goal.x + crrX && point.x > goal.x - crrX && point.y < goal.y + crrY && point.y > goal.y - crrY ||
			SDL_GetTicks() - deley > r_deley && deley != 0)
		{
			if (goal.x > scrain->GetPosition().x + detect_regionSize || goal.x < scrain->GetPosition().x - detect_regionSize)
			{
				if (scrain->GetDetect() < 90)
				{
					pf_goal_i = GetRandPointFromPull();
					bool result = AIFindPathTo(pf_goal_i, 0, 0, 0);
					goal = point;
					m_speedX = 4;
					m_speedY = 4;
					setCorrection();
					if (result == true)
						status = 6;
					else
						pf_goal_i = -1;
					return;
				}
				else
				{
					if (deley != 0)
						deley = SDL_GetTicks();
				}
			}
			else
			{

				if (pathPoint[actualPPId].N_links == 1)
				{
					SetCircleMoves();
				}
				else
				{
					int rid = 0;
					do
					{
						rid = (rand() % pathPoint[actualPPId].N_links) + 0;
					} while (pathPoint[actualPPId].linkIndex[rid] == prevPPid); // zapobieganie powrotowi

					prevPPid = actualPPId;
					actualPPId = pathPoint[actualPPId].linkIndex[rid];
					
					goal.x = pathPoint[actualPPId].x;
					goal.y = pathPoint[actualPPId].y;

					if (pathPoint[actualPPId].N_links == 1 || pathPoint[actualPPId].N_links == 0)
					{
						originGoal = goal;
						goal.x -= 35;
					}
				}

			}
		}
		else
		{
			if (r != 0)
				AIcicleMove();
			else
				AImoveToPoint();
		}
			
	}
#pragma endregion

#pragma region STATUS 2
	if (status == 2) // memory looking for player
	{
		rgoal.x = goal.x;
		rgoal.y = goal.y;
		if (col->isColidePlane(rgoal, NULL, 0, point.x, point.y) != 0)
		{
			if (moveType != 1)
				moveType = 1;
			m_speedX = 4;
			setCorrection();
			status = 4;
			return;
		}
		else
			moveType = 0;

		if (scrain->isUntouch() == 1)
		{
			status = 0;
			return;
		}
		if (scrain->GetPosition().x + (scrain->GetPosition().w / 2) < point.x + DETECT_SIZE / 3 && scrain->GetPosition().x + (scrain->GetPosition().w/2) > point.x - DETECT_SIZE / 3 &&
			scrain->GetPosition().y + (scrain->GetPosition().h/2) < point.y + DETECT_SIZE / 3 && scrain->GetPosition().y + (scrain->GetPosition().h / 2) > point.y - DETECT_SIZE / 3 &&
			col->isHideLineColide(point.x, point.y, scrain->GetPosition()) == 0 &&
			col->isColidePlane(scrain->GetPosition(), NULL, 1, point.x, point.y) == 0 &&
			col->isColidePlane(scrain->GetPosition(), NULL, 0, point.x, point.y) == 0)
		{
			status = 3;
		}
		else
		{
			if (point.x < goal.x + crrX && point.x > goal.x - crrX && point.y < goal.y + crrY && point.y > goal.y - crrY)
			{
				m_speedX = m_speedY = 3;
				setCorrection();
				//cout << "cant find anything, back to patrol" << endl;
				timeHock = timePassed = 0;
				SettingsControl st;
				if (st.getDifficulty() != 1 && st.getAchivment(1) == false)
					st.setIsAchivmentDone(true, 1);
				StatisticsControl sc;
				sc.setStat(1, sc.getStat(1) + 1);
				status = 0;
			}
			else
			{
				if (timeHock != 0 && timePassed != 0 && timeHock + timePassed > SDL_GetTicks()
					&& scrain->GetDetect() != 0 || scrain->GetDetect() > 89)
				{
					if (scrain->GetPosition().x + (scrain->GetPosition().w / 2) != goal.x ||
						scrain->GetPosition().y + (scrain->GetPosition().h / 2) != goal.x)
					{
						goal.x = scrain->GetPosition().x + (scrain->GetPosition().w / 2);
						goal.y = scrain->GetPosition().y + (scrain->GetPosition().h / 2);
					}
				}
				else
				{
					if (scrain->GetDetect() == 0)
					{
						timeHock == 0;
						timePassed == 0;
					}
				}
				//cout << "im looking" << endl;
				AImoveToPoint();
			}
		}
	}
#pragma endregion

#pragma region STATUS 3

	if (status == 3) // agressive
	{
		if (scrain->isUntouch() == 1)
		{
			status = 0;
			return;
		}

		if (moveType != 0 && timeHock != 0) // skoncz udaremniac atak jesli grasz skoczy
		{
			if (scrain->GetAnimType() == 312 || scrain->GetAnimType() == 311)
			{
				moveType = 0;
				timeHock = timePassed = 0;
			}
		}

		if (col->isColidePlane(scrain->GetPosition(), NULL, 0, point.x, point.y) != 0 && timeHock == 0 && timePassed == 0)
		{
			if (moveType != 1)
				moveType = 1;
			m_speedX = 5;
			setCorrection();
			if (prevMoveX == point.x)
			{
				timeHock = timePassed = 0;
				status = 4;
				return;
			}
		}

		if (col->isColidePlane(scrain->GetPosition(), NULL, 1, point.x, point.y) == 0 &&
			col->isHideLineColide(point.x, point.y, scrain->GetPosition()) == 0)
		{
			switch (st.getDifficulty())
			{
			case 1:
			{
				if (m_speedX != 4 && m_speedY != 4)
					m_speedX = m_speedY = 4;
				setCorrection();
				break;
			}
			case 3:
			{
				if (m_speedX != 5 && m_speedY != 5)
					m_speedX = m_speedY = 5;
				setCorrection();
				break;
			}
			default:
			{
				if (m_speedX != 5 && m_speedY != 4)
				{
					m_speedX = 5;
					m_speedY = 4;
				}
				setCorrection();
				break;
			}
			}
				

			if (scrain->GetPosition().x + (scrain->GetPosition().w / 2) != goal.x ||
				scrain->GetPosition().y + (scrain->GetPosition().h / 2) != goal.y)
			{
				goal.x = scrain->GetPosition().x + (scrain->GetPosition().w / 2);
				goal.y = scrain->GetPosition().y + (scrain->GetPosition().h / 2);
			}


			if (prevMoveX == point.x && point.x > goal.x - crrX && point.x < goal.x + crrX)
			{
				if (scrain->GetAnimType() == 0 || scrain->GetAnimType() == 11) // jesli screin sie zatrzyma, a ja udaje ze nie szarzuje i jestem nad nim, zaczne szarzowac
				{
					timeHock = timePassed = 0;
					moveType = 0;
				}
			}

			//cout << "I follow " << timeHock << endl;

			if (point.x < scrain->GetPosition().x + scrain->GetPosition().w && point.x > scrain->GetPosition().x &&
				point.y < scrain->GetPosition().y + scrain->GetPosition().h&& point.y > scrain->GetPosition().y)
			{

				if ( scrain->isFallen() == 0)
				{
					SoundMenager sm;
					sm.addSound("IrlichtCatch", sm.getFreeChannel(), 0);
					sm.stopSound(HUNTCHANNEL);
					m_speedX = m_speedY = 3;
					setCorrection();
					StatisticsControl sc;
					sc.setStat(2, sc.getStat(2)+1);
					if (st.getDifficulty() == 1)
					{
						scrain->SetHP(scrain->GetHP() - 50);
						scrain->gotHit();
						scrain->setUntouch(1);
					}
					else
						scrain->SetHP(0);
					DeleybtwCatch = SDL_GetTicks();
					goal.x = prevMoveX + ((prevMoveX - point.x)*3);
					goal.y = scrain->GetPosition().y + scrain->GetPosition().h;
					status = 0;
					return;
				}
			}
			else
			{
				#pragma region udaremnianie poscigu
				if (prevMoveX != 0 && moveType == 1 && prevMoveX < point.x && irlichtLookSide == 1 && timeHock != 0 && timePassed != 0 ||
					prevMoveX != 0 && moveType == 1 && prevMoveX > point.x && irlichtLookSide == 2 && timeHock != 0 && timePassed != 0) // jesli zawrocil
					isSCTurnBack = 1;

				if (isSCTurnBack == 1 && irlichtTurn == 1)
				{
					prevMoveX = point.x;
					isSCTurnBack = 0;
					irlichtTurn = 0;
				}

				if (col->isColidePlane(scrain->GetPosition(), NULL, 0, point.x, point.y) == 0 &&
					timeHock != 0 && timePassed != 0 && timeHock + timePassed < SDL_GetTicks() ||
					col->isColidePlane(scrain->GetPosition(), NULL, 0, point.x, point.y) == 0 &&
					timeHock == 0 && timePassed == 0 ||
					isSCTurnBack == 1)
				{
					isSCTurnBack = 0;
					irlichtLookSide = 0;
					prevMoveX = 0;
					timeHock = timePassed = 0;
					moveType = 0;
				}
				#pragma endregion

				AImoveToPoint();
			}
		}
		else
		{
			SoundMenager sm;
			sm.stopSound(HUNTCHANNEL);
			sm.addSound("HuntEnd", HUNTCHANNEL, 0);
			m_speedX = m_speedY = 3;
			setCorrection();
			goal.x = point.x;
			goal.y = point.y;
			status = 0;
		}
	}
#pragma endregion

#pragma region STATUS 4
	if (status == 4) // avoid obstacles
	{
		//cout << "status 4" << endl;
		if (scanForAv == 0)
		{
			moveType = 1;
			setCorrection();
			AiAvoidObstacles();
		}
		else
		{
			if (m_speedX != 6 && m_speedY != 6 && setFullDetection != true)
			{
				m_speedX = 6;
				m_speedY = 6;
				setCorrection();
			}
			//cout << "Go To " << goal.x << " " << goal.y << " From " << WorldPoint.x << " " << WorldPoint.y << endl;
			AImoveToPoint();
			if (point.y < goal.y + crrY && point.y > goal.y - crrY)
			{
				moveType = 0;
				scanForAv = 0;
				m_speedX = 4;
				m_speedY = 4;
				setCorrection();
				if (pf_goal_i != -1)
				{
					goal = pf_goal_c;
					pf_goal_c.x = pathPoint[pf_goal_i].x;
					pf_goal_c.y = pathPoint[pf_goal_i].y;
					status = 6;
					return;
				}
				status = 3;
				return;
			}
			else
				if (point.x < goal.x + crrX && point.x > goal.x - crrX)
					moveType = 2;
		}

	}
#pragma endregion

#pragma region STATUS 5
if (status == 5) // quick pathfind looking for player
{
	if (m_speedX != 5 || m_speedY != 5)
	{
		m_speedX = m_speedY = 5;
		setCorrection();
	}
	//cout << "status 5 " << pf_goal_i << " " << actualPPId << endl;
	if (point.x < goal.x + crrX && point.x > goal.x - crrX && point.y < goal.y + crrY && point.y > goal.y - crrY)
	{
		if (goal.x == pathPoint[pf_goal_i].x && goal.y == pathPoint[pf_goal_i].y)
		{
			if (pathPoint[actualPPId].N_links == 1)
			{
				originGoal = goal;
				goal.x -= 35;
			}
			r = 0;
			pf_goal_i = -1;
			pf_goal_c = {};
			if (pf_nextId != -1)
			{
				prevPPid = actualPPId;
				actualPPId = pf_nextId;
			}
			m_speedX = m_speedY = 3;
			setCorrection();
			pf_nextId = -1;
			SoundMenager sm;
			sm.stopSound(HUNTCHANNEL);
			status = 1;
			return;
		}
		else
		{
			AIQuickFindPathTo();
		}
	}
	else
	{
		setCorrection();
		AImoveToPoint();
	}
}
#pragma endregion

#pragma region STATUS 6
if (status == 6) // pathfinding to pf_goal
{
	if (point.x < pf_goal_c.x + crrX && point.x > pf_goal_c.x - crrX && point.y < pf_goal_c.y + crrY && point.y > pf_goal_c.y - crrY)
	{
		if (pathPoint[actualPPId].N_links == 1)
		{
			originGoal = goal;
			goal.x -= 35;
		}
		r = 0;
		pf_goal_c = {};
		pf_goal_i = -1;
		m_speedX = m_speedY = 3;
		setCorrection();
		status = 1;

	}
	else
	{
		if (point.x < (camr->x) + (camr->w) && point.x > camr->x &&
			point.y < (camr->y) + (camr->h) && point.y> camr->y)
		{
			if ( m_speedX != 4)
			{
				m_speedX = 4;
				m_speedY = 4;
				setCorrection();
			}
		}
		else
		{
			if (m_speedX != 9)
			{
				m_speedX = 9;
				m_speedY = 9;
				setCorrection();
			}
		}

		if (point.x < goal.x + crrX && point.x > goal.x - crrX && point.y < goal.y + crrY && point.y > goal.y - crrY && pf_goal_i != -1)
		{
			//cout << "status 6 ";
			
				pf_actualWayPoint += 1;
				if (pf_actualWayPoint < pf_n_wayIds && pf_actualWayPoint > -1)
			{
				prevPPid = actualPPId;
				actualPPId = pf_wayIds[pf_actualWayPoint];
				goal.x = pathPoint[pf_wayIds[pf_actualWayPoint]].x;
				goal.y = pathPoint[pf_wayIds[pf_actualWayPoint]].y;
				//cout << pf_actualWayPoint << "/" << pf_n_wayIds-1 << " " << pf_goal_i << " " << pf_goal_c.x << " " << pf_goal_c.y << " ActualI " << actualPPId << endl;
			}
		}
		else
			AImoveToPoint();
	}
}
#pragma endregion


if (prevMoveX == point.x ) // if stuck
{
	if (waittime == 0)
	{
		waittime = SDL_GetTicks();
	}
	else
		if (abs((int)SDL_GetTicks() - waittime) > 2000)
		{
			moveType = 0;
			if (status == 3)
			{
				timeHock = timePassed = 0;
				scanForAv = 0;
				AIfindNearPoint(0);
				point = goal;
				AIfindNearPoint(1);
				bool result = AIFindPathTo(pf_goal_i, 0, 0, 0);
				goal = point;
				if (result == true)
					status = 6;
				else
				{
					SoundMenager sm;
					sm.stopSound(HUNTCHANNEL);
					status = 0;
				}
				StatisticsControl sc;
				sc.setStat(1, sc.getStat(1) + 1);
			}
			else
			{
				AIfindNearPoint(0);
				point = goal;
				status = 0;
			}
			setCorrection();
			waittime = 0;
		}
}
else
{
	if (waittime != 0)
		waittime = 0;
}

if (audioHunt == 1 && status != 3 && status != 5 && status != 4)
{
	SoundMenager sm;
	sm.stopSound(HUNTCHANNEL);
	sm.addSound("HuntEnd", ENDHUNTCHANEL, 0);
	audioHunt = 0;
}

if (prevMoveX != point.x)
	prevMoveX = point.x;

}
void AI::SetCircleMoves()
{
	if (r == 0)
	{
		if (originGoal.y > point.y)
			r = 35;
		else
			if (originGoal.y <= point.y)
				r = -35;
		r_deley = (rand() % 11000) + 1000;
		deley = SDL_GetTicks();
		goal = originGoal;
	}
	else
	{
		if (SDL_GetTicks() - deley > r_deley)
		{
			if (endCicle == 1)
			{
				next = 0;
				minDist = -1;
				prevTheta = -1;
				endCicle = 0;
				r = 0;
				deley = 0;
				r_deley = 0;
				c_speed = 90;
				theta = 0;
				prevPPid = actualPPId;
				goal.x = pathPoint[pathPoint[actualPPId].linkIndex[0]].x;
				goal.y = pathPoint[pathPoint[actualPPId].linkIndex[0]].y;
				actualPPId = pathPoint[actualPPId].linkIndex[0];
				setCorrection();
			}
			else
			{
				float fx = pathPoint[pathPoint[actualPPId].linkIndex[0]].x - point.x;
				float fy = pathPoint[pathPoint[actualPPId].linkIndex[0]].y - point.y;
				double diste = sqrtf(fx*fx + fy*fy);
				if (next == false && minDist < diste + 2 && minDist > diste - 2)
				{
					endCicle = 1;
				}
				else
				{

					if (minDist > diste || minDist == -1)
					{
						prevTheta = theta;
						minDist = diste;
						next = 1;
					}

					AIcicleMove();
					if (next == 1 && minDist != diste)
						next = 0;
				}
			}
		}
	}
}
int AI::GetRandPointFromPull()
{

	int * PointsIdsPull = new int[1];
	PointsIdsPull[0] = 0;
	int count = 0, id = -1;
	int l=-1, p=-1;
	p = PullSearch(scrain->GetPosition().x + detect_regionSize, 0);
	l = PullSearch(scrain->GetPosition().x - detect_regionSize, 1);
	//cout << "PullSearch complete " << pathPoint[l].x << "><" << pathPoint[p].x << " " << scrain->GetPosition().x - detect_regionSize << "><" << scrain->GetPosition().x + detect_regionSize << endl;

	if (l != -1 && p != -1)
	{
		if (l == 0 && p == 0 || l>p)
		{
			AIfindNearPoint(1);
			id = pf_goal_i;
		}
		else
		{
			for (int i = l; i < p; i++)
			{
				PointsIdsPull[count] = i;
				count += 1;
				DAMIntChanger(&PointsIdsPull, count + 1, 1, 1);
			}
			if (count != 0)
				id = (rand() % count) + 0;
			else
				id = count;
			id = PointsIdsPull[id];
		}
	}
	//cout << "RandGoal: " << id << endl;
	delete[] PointsIdsPull;
	PointsIdsPull = nullptr;
	return id;
}
int AI::PullSearch(int var, bool type) // 1 = wiêcej niz ">"  |  0 = mniej niz "<" 
{
	int l = 0; r = n_pathPoints - 1;
	bool s = 0;
	int middle = (l + r) / 2;
	while (true)
	{
		if (type == 0)
		{
			if (var > pathPoint[middle].x)
			{
				if (var <= pathPoint[middle + 1].x)
					return middle;
				else
				{
					middle += 1;
					if (middle >= r)
						return r;
				}
			}
			else
			{
				middle -= 1;
				if (middle <= l)
					return l;
			}
		}
		else
		{
			if (var < pathPoint[middle].x)
			{
				if (var >= pathPoint[middle - 1].x)
					return middle;
				else
				{
					middle -= 1;
					if (middle <= l)
						return l;
				}
			}
			else
			{
				middle += 1;
				if (middle >= r)
					return r;
			}
		}

		if (middle > r)
		{
			return r;
		}
		else
			if (middle < l)
				return l;

	}
	if (type == 0)
		return l;
	else
		return r;
}

void AI::AIcicleMove()
{
	if (theta >= 2 * 3.14)
		theta = 0;
	
	point.x = goal.x - 35 * cos(theta);
	point.y = goal.y + r * sin(theta);
	theta += 2 * 3.14 / c_speed;
	if (c_speed != 50)
	c_speed -= 2;
}
void AI::AImoveToPoint()
{
	if (moveType == 0 && goal.x != point.x )
	{
		hypx = goal.x - point.x;
		hypy = goal.y - point.y;
		dist = sqrtf(hypx*hypx + hypy*hypy);
		hypx = hypx / dist * m_speedX;
		hypy = hypy / dist * m_speedY;
		hypx = round(hypx);
		hypy = round(hypy);
		point.x += hypx;
		if (correction == 0)
		point.y += hypy;
	}
	else
		if (moveType == 1 && goal.x != point.x)
		{
			if (point.x > goal.x + m_speedX)
				point.x -= m_speedX;
			else
				if (point.x < goal.x - m_speedX)
					point.x += m_speedX;
		}
		else
			if (moveType == 2 && goal.y != point.y)
			{
				if (point.y > goal.y + m_speedY)
					point.y -= m_speedY;
				else
					if (point.y < goal.y - m_speedY)
						point.y += m_speedY;
			}

}
void AI::AIfindNearPoint(bool target)
{
	int n = 0, index = -1;
	if (target == 0)
	{
		for (int i = 0; i < n_pathPoints; i++)
		{
			if (abs(point.x - pathPoint[i].x) + abs(point.y - pathPoint[i].y) < n || i == 0)
			{
				n = abs(point.x - pathPoint[i].x) + abs(point.y - pathPoint[i].y);
				index = i;
			}
		}
		if (index != -1)
		{
			goal.x = pathPoint[index].x;
			goal.y = pathPoint[index].y;
			actualPPId = index;
			//cout << "found point near me" << endl;
		}
	}
	else
	{
		for (int i = 0; i < n_pathPoints; i++)
		{
			if (abs(scrain->GetPosition().x - pathPoint[i].x) + abs(scrain->GetPosition().y - pathPoint[i].y) < n || i == 0)
			{
				n = abs(scrain->GetPosition().x - pathPoint[i].x) + abs(scrain->GetPosition().y - pathPoint[i].y);
				index = i;
			}
		}
		if (pathPoint[index].x != 0)
		{
			pf_goal_c.x = pathPoint[index].x;
			pf_goal_c.y = pathPoint[index].y;
			pf_goal_i = index;
			//cout << "found point near scrain " << index << endl;
		}
	}
	
	
}
void AI::AiAvoidObstacles()
{
	int side[2] = {};
	short result = 0;
	SDL_Rect Rscrain = scrain->GetPosition();
	if (col->isColidePlane(Rscrain, NULL, 0, point.x, point.y) != 0)
	{
		col->getPlaneColideCoord(Rscrain, 0, point.x, point.y, &side[0], &side[1], 0);
		if (abs(point.x - side[0]) < abs(point.x - side[1]))
		{
			goal.x = side[0] - 15;
		}
		else
		{
			goal.x = side[1] + 15;
		}
		goal.y = Rscrain.y + (Rscrain.h / 2);
		scanForAv = 1;
		return;
	}

	/*
	SDL_Rect pointRect;
	if (WorldPoint.y < Rscrain.y)
	{
		sidec = 1;
		pointRect.x = point.x;
		pointRect.y = point.y;
		pointRect.w = pointRect.h = 0;
	}
	for (int i = 0; i < 20; i++)
	{
		detectionLine.h = Rscrain.h;
		detectionLine.w = Rscrain.w;
		detectionLine.x = Rscrain.x + adddest;
		detectionLine.y = Rscrain.y;
		for (int j = 0; j < 2; j++)
		{
			if (sidec == 0)
			{
				result = col->isColidePlane(detectionLine, NULL, 0, point.x, point.y);
			}
			else
				result = col->isColidePlane(pointRect, NULL, 0, detectionLine.x + (detectionLine.w / 2), detectionLine.y + (detectionLine.h / 2));
			if (result != 0)
			{
				adddest += 13;
			}
			else
			{
				if (j == 1)
					goal.x = detectionLine.x + (Rscrain.w / 2);
				else
					goal.x = detectionLine.x + (Rscrain.w / 2);
				if (WorldPoint.y > Rscrain.y)
					goal.y = detectionLine.y + (Rscrain.h / 2);
				else
					goal.y = detectionLine.y;
				scanForAv = 1;
				detectionLine = {};
				return;
			}
			detectionLine.x = Rscrain.x - adddest;
		}

	}
	*/
	status = 0;
}
void AI::AICheckPlane()
{
	if (correction == 0)
	{
		detectionLine.x = point.x + 5;
		detectionLine.y = point.y + 10;
		detectionLine.w = 0;
		detectionLine.h = -20;
		col->isColidePlane(detectionLine, &correction, 0, -1, -1);
		detectionLine.x = point.x - 5;
		col->isColidePlane(detectionLine, &correction, 0, -1, -1);
		detectionLine.x;
		col->isColidePlane(detectionLine, &correction, 0, -1, -1);
	}
}
bool AI::AIFindPathTo(int index, int x, int y, bool mode) // 0 mode = index | 1 mode = x,y
{
	if (index != -1)
	{
		//cout << "Finding path" << endl;
		pf_actualWayPoint = 0;
		delete[] pf_checkedIds;
		pf_checkedIds = nullptr;
		delete[] pf_wayIds;
		pf_wayIds = nullptr;
		pf_n_checkedIds = 0;
		pf_n_wayIds = 0;
		pf_checkedIds = new int[1];
		pf_wayIds = new int[1];
		pf_wayIds[0] = 0;
		pf_goal_c = {};
		pf_goal_i = -1;

		if (mode == 0)
		{
			pf_goal_i = index;
			pf_goal_c.x = pathPoint[index].x;
			pf_goal_c.y = pathPoint[index].y;
		}
		else
			if (mode == 1)
			{
				pf_goal_c.x = x;
				pf_goal_c.y = y;
				pf_goal_i = ComparePointsSearch(1);
			}


		float fx;
		float fy;
		double distg; // dystans od aktualnego miejsca do po³¹czonego punktu
		double disth; // dystans od po³¹czonego punktu do celu
		double minDist = 0;
		int prevPid = -1;
		int actualPId = actualPPId;
		// actual id w ktorym sie znajduje algorytm, nextid do którego przejdzie algorytm jesli sprawdzi pozytywnie
		int nextId = -1;
		bool clear = 0; // w razie nie powodzenia wyczysci zmienne
		pf_wayIds[pf_n_wayIds] = actualPId; 
		pf_n_wayIds += 1;
		while (true) // g³ówna pêtla
		{
			minDist = 0;
			for (int i = 0; i < pathPoint[actualPId].N_links + 1; i++) // pêtla szukaj¹ca najmniejszej odleglosci
			{
				fx = pathPoint[actualPId].x - pathPoint[pathPoint[actualPId].linkIndex[i]].x;
				fy = pathPoint[actualPId].y - pathPoint[pathPoint[actualPId].linkIndex[i]].y;
				distg = sqrtf(fx*fx + fy*fy);
				fx = pathPoint[pf_goal_i].x - pathPoint[pathPoint[actualPId].linkIndex[i]].x;
				fy = pathPoint[pf_goal_i].y - pathPoint[pathPoint[actualPId].linkIndex[i]].y;
				disth = sqrtf(fx*fx + fy*fy);
				if (disth + distg < minDist || minDist == 0 && binarySearch(pf_checkedIds, pathPoint[actualPId].linkIndex[i], 0, pf_n_checkedIds - 1) == -1 || pathPoint[actualPId].linkIndex[i] == pf_goal_i)
				{
					//cout << "First pool id" << pathPoint[actualPId].linkIndex[i] << endl;
					if (binarySearch(pf_checkedIds, pathPoint[actualPId].linkIndex[i], 0, pf_n_checkedIds - 1) == -1 &&
						binarySearch(pf_wayIds, pathPoint[actualPId].linkIndex[i], 0, pf_n_wayIds - 1) == -1 &&
						prevPid != pathPoint[actualPId].linkIndex[i] || pathPoint[actualPId].linkIndex[i] == pf_goal_i)
					{
						minDist = disth + distg;
						nextId = pathPoint[actualPId].linkIndex[i];
						//cout << "Second pool id " << nextId << endl;
					}
				}
			}

			if (nextId != -1)
			{
				if (nextId == pf_goal_i)
				{
					for (int l = 0; l < 2; l++)
					{
						//cout << "wynik " << nextId << pf_n_wayIds << endl;

						DAMIntChanger(&pf_wayIds, pf_n_wayIds + 1, 1, 1);
						pf_wayIds[pf_n_wayIds] = actualPId;
						pf_n_wayIds += 1;
						prevPid = actualPId;
						actualPId = nextId;
					}					// uwaga czyszczenie poprzedzone powodzeniem nie czysci wayids!

					delete[] pf_checkedIds;
					pf_checkedIds = nullptr;
					pf_n_checkedIds = 0;
					return true;
				}
				else
				{
					if (pathPoint[nextId].N_links == 1) // jesli punkt jest slepym zaulkiem dodaj do listy sprawdzonych
					{
						//cout << "Slepy zaulek " << nextId << endl;
						if (pf_n_checkedIds != 0)
							DAMIntChanger(&pf_checkedIds, pf_n_checkedIds + 1, 1, 1);
						pf_checkedIds[pf_n_checkedIds] = nextId;
						pf_n_checkedIds += 1;
						quickSort(pf_checkedIds, 0, pf_n_checkedIds - 1);
					}
					else
					{
						//cout << "Droga " << nextId << " " << pf_n_wayIds << " " << pathPoint[nextId].N_links << " " << actualPId << endl;
						if (pf_n_wayIds != 0)
							DAMIntChanger(&pf_wayIds, pf_n_wayIds + 1, 1, 1); // wywala blad
						prevPid = actualPId;
						actualPId = nextId;
						pf_wayIds[pf_n_wayIds] = actualPId;
						pf_n_wayIds += 1;
						
					}
					nextId = -1;
				}
			}
			else
			{
				/* jeœli nie ma juz nowych punktow, ktore nie znajduj¹ sie na liscie sprawdzonych,
				dodaj aktualny punkt do sprawdzonych po czym cofnij sie do poprzedniego punktu*/
				if (pf_n_checkedIds != 0)
					DAMIntChanger(&pf_checkedIds, pf_n_checkedIds + 1, 1, 1);
				pf_checkedIds[pf_n_checkedIds] = actualPId;
				pf_n_checkedIds += 1;
				if (pf_n_wayIds - 1 < 1) // jesli nie ma juz punktow by sie cofnac zakoncz funkcje
				{
					clear = true;
				}
				else
				{
					//cout << "WayBack " << pf_n_wayIds << endl;
					quickSort(pf_checkedIds, 0, pf_n_checkedIds - 1);
					prevPid = actualPId;
					actualPId = pf_wayIds[pf_n_wayIds - 1];
					pf_wayIds[pf_n_wayIds - 1] = 0;
					//cout << " actualId " << actualPId;
					DAMIntChanger(&pf_wayIds, pf_n_wayIds - 1, 1, 0);
					pf_n_wayIds -= 1;
				}
			}
			if (clear == true) // czyszczenie w przypadku niepowodzenia
			{
				delete[] pf_checkedIds;
				pf_checkedIds = nullptr;
				delete[] pf_wayIds;
				pf_wayIds = nullptr;
				pf_n_checkedIds = 0;
				pf_n_wayIds = 0;
				pf_goal_c = {};
				pf_goal_i = {};
				return false;
			}
		}
	}
}
void AI::AIQuickFindPathTo()
{ // przed uzyciem zapisz index celu w pf_goal_i
	if (pf_nextId != -1)
	{
		prevPPid = actualPPId;
		actualPPId = pf_nextId;
	}
	float fx;
	float fy;
	double distg; // dystans od aktualnego miejsca do po³¹czonego punktu
	double disth; // dystans od po³¹czonego punktu do celu
	double minDist = 0;
	for (int i = 0; i < pathPoint[actualPPId].N_links+1; i++) // pêtla szukaj¹ca najmniejszej odleglosci
	{
		fx = pathPoint[actualPPId].x - pathPoint[pathPoint[actualPPId].linkIndex[i]].x;
		fy = pathPoint[actualPPId].y - pathPoint[pathPoint[actualPPId].linkIndex[i]].y;
		distg = sqrtf(fx*fx + fy*fy);
		fx = pathPoint[pf_goal_i].x - pathPoint[pathPoint[actualPPId].linkIndex[i]].x;
		fy = pathPoint[pf_goal_i].y - pathPoint[pathPoint[actualPPId].linkIndex[i]].y;
		disth = sqrtf(fx*fx + fy*fy);
		if (disth + distg < minDist &&  pathPoint[actualPPId].linkIndex[i] != prevPPid || i == 0)
		{
			minDist = disth + distg;
			pf_nextId = pathPoint[actualPPId].linkIndex[i];
		}
	}
	//cout << "Next id " << pf_nextId << endl;
	goal.x = pathPoint[pf_nextId].x;
	goal.y = pathPoint[pf_nextId].y;
	
	if (pathPoint[pf_nextId].N_links == 1)
	{
		status = 1;
		return;
	}
	
}

int AI::ComparePointsSearch(bool type) // type 0 = goal 1 = pf_goal 2 = input
{
	short state = 0;
	Point var = {};
	if (type == 0)
		var = goal;
	else
	if (type == 1)
		var = pf_goal_c;

	while (state != 2)
	{
		int l = 0, r = n_pathPoints-1;
		while (l <= r)
		{
			int middle = (l + r) / 2;
			if (var.x > pathPoint[middle].x - 15 && var.x < pathPoint[middle].x + 15 &&
				var.y > pathPoint[middle].y - 15 && var.y < pathPoint[middle].y + 15)
				return middle;
			else
			{
				if (state == 0)
				{
					if (pathPoint[middle].x > var.x)
						r = middle - 1;
					else
						l = middle + 1;
				}
				else
					if (state == 1)
					{
						if (pathPoint[middle].y > var.y)
							r = middle - 1;
						else
							l = middle + 1;
					}

			}
		}
		switch (state)
		{
		case 0:
			state = 1;
			break;
		case 1:
			state = 2;
			break;
		default:
			break;
		}
	}

	return -1;

}

void AI::loadGrid()
{
	SettingsControl st;
	string LevelName = st.getLevelName() + ".txt";
	int null = 0;
	fstream savefile;
	savefile.open(LevelName, ios::in);
	if (savefile.is_open())
	{
		savefile >> null;
		savefile >> null;
		savefile >> null;
		savefile >> null;
		savefile >> null;
		savefile >> null;
		savefile >> null;
		delete[] pathPoint;
		pathPoint = nullptr;
		null += 1;
		pathPoint = new grid[null];
		n_pathPoints = null;
		savefile >> null;
		savefile >> null;

		int count = 0;
		for (int i = 0;count < 6; i++)
		{

			switch (count)
			{
			case 0:
			{
				savefile >> null;
				if (null == 0)
				{
					count = 1;
					i = -1;
				}
				break;
			}
			case 1:
			{
				savefile >> null;
				if (null == 0)
				{
					count = 2;
					i = -1;
				}
				break;
			}
			case 2:
			{
				savefile >> null;
				if (null == 0)
				{
					count = 3;
					i = -1;
				}
				break;
			}
			case 3:
			{
				savefile >> null;
				if (null == 0)
				{
					count = 4;
					i = -1;
				}
				break;
			}
			case 4:
			{
				savefile >> null;
				
				if (null == 0)
				{
					count = 5;
					i = -1;
				}
				break;
			}
			case 5:
			{

				savefile >> null;
				if (null != -1)
				{
					pathPoint[i].x = null;
					savefile >> pathPoint[i].y;
					
					savefile >> pathPoint[i].N_links;
					if (pathPoint[i].N_links != 0)
					{
						for (int k = 0; k < pathPoint[i].N_links; k++)
						{
							savefile >> pathPoint[i].linkIndex[k];
						}
					}

				}
				else
				{
					count = 6;
				}
				break;
			}
			};

		}
		savefile.close();

	}


}

#pragma region func
void AI::setCorrection()
{
	crrX = crrY = m_speedX + 1;
}
void AI::MakeWorldPoint()
{
	WorldPoint.x = point.x - cam->GetCameraCoord().x;
	WorldPoint.y = point.y - cam->GetCameraCoord().y;
}
Point* AI::GetPoint()
{
	return &point;
}
void AI::setPoint(int x, int y)
{
	point.x = x;
	point.y = y;
	goal = point;
}

void AI::getPointers(Scrain* scrain, Colision* col, Camera* cam, SDL_Renderer* renderer)
{
	this->scrain = scrain;
	this->col = col;
	this->cam = cam;
	this->renderer = renderer;
	camr = cam->GetNewCoord();
}
#pragma endregion

