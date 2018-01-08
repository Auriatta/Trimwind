#include"PhysicEngine.h"

PhysicEngine::PhysicEngine()
{
	forceX.script = 0;
	forceY.script = 0;
	forceX.power = 0;
	forceY.power = 0;
	hidePlaneDetect = 0;
}

PhysicEngine::~PhysicEngine()
{
	scrain = nullptr;
	delete scrain;
	ev = nullptr;
	delete ev;
	forceX.script = 0;
	forceY.script = 0;
	forceX.power = 0;
	forceY.power = 0;
	hidePlaneDetect = 0;
}

void PhysicEngine::rotate(Scrain* scrain, Force* forceY)
{
	if (scrain->GetAnimType() == 311 || scrain->GetAnimType() == 312)// jump in direction
	{
		if (scrain->GetAnimType() == 311 && forceY->power > 0)
			scrain->SetRotation(forceY->power * 2);
		if (scrain->GetAnimType() == 312 && forceY->power > 0)
			scrain->SetRotation(-forceY->power * 2);
		if (scrain->GetAnimType() == 312 && forceY->power < 0)
			scrain->SetRotation(forceY->power * -3);
		if (scrain->GetAnimType() == 311 && forceY->power < 0)
			scrain->SetRotation(-forceY->power * -3);
	}
	else
		if (scrain->GetAnimType() != 301 || scrain->GetAnimType() != 302)// normal
		{
			if (scrain->GetAnimType() == 1 && forceY->power > 0 ||
				scrain->GetAnimType() == 2 && forceY->power < 0 ||
				scrain->GetAnimType() == 44 && forceY->power < 0 ||
				scrain->GetAnimType() == 33 && forceY->power > 0 ||
				scrain->GetAnimType() == 11 && forceY->power > 0 ||
				scrain->GetAnimType() == 0 && forceY->power < 0)
			{
				scrain->SetRotation(-forceY->power * 2);
			}
			else if (scrain->GetAnimType() == 1 && forceY->power < 0 ||
				scrain->GetAnimType() == 2 && forceY->power > 0 ||
				scrain->GetAnimType() == 44 && forceY->power > 0 ||
				scrain->GetAnimType() == 33 && forceY->power < 0 ||
				scrain->GetAnimType() == 11 && forceY->power > 0 ||
				scrain->GetAnimType() == 0 && forceY->power < 0)
			{
				scrain->SetRotation(-forceY->power*-2);
			}
			else
			{
				if (rotatedMove != true)
				scrain->SetRotation(0);
			}
		}
		else
		{
			if (rotatedMove != true)
			scrain->SetRotation(0);
		}
}

void PhysicEngine::scriptJump(int xPw, int yPw, bool side)
{
	if (side == 0)
	{
		forceX.side = 0;
		scrain->anim_type = 311;
	}
	else
	{
		forceX.side = 1;
		scrain->anim_type = 312;
	}

	forceX.script = 1;
	forceY.script = 1;
	forceX.power = xPw;
	forceY.power = yPw;

	
	scrain->SetJumpSeq(1);
	scrain->LoadScrain(2, 7);
	ev->SetException(3);
}
 
void PhysicEngine::grav()
{
	SDL_Rect r;
	r = scrain->GetPosition();
	

	if (ev->GetException() != 3 && ev->GetException() != 1 && ev->GetException() != 2)
		ev->SetException(4);

	if (forceX.power != 0)
	{
		if (scrain->GetSpeed() == 10 && forceX.script == 0)
		{
			if (forceX.side == 0)
				r.x -= 10;
			else
				r.x += 10;
		}
		else
		{
			if (forceX.side == 0)
				r.x -= 7;
			else
				r.x += 7;
		}
		
		forceX.power--;
		
	}
	else
		if (forceX.power == 0)
	{
		if (forceX.script != 0)
		{
			forceX.script = 0;
			scrain->SetJumpSeq(5);
		}
	}
	r.y += -(forceY.power);
	
	scrain->SetPosition(r.x, r.y);
	if (r.y > 2200)                                 /// tutaj jest border do postaci!!!!!!!!!!
		scrain->SetHP(0);

	rotate(scrain, &forceY);
	
	
}

void ifJumpForce(Scrain* scrain, Force* forceY, Force* forceX, Events* ev)
{
	if (scrain->GetAnimType() > 299 && scrain->GetAnimType() < 313 && ev->GetException() != 3 && scrain->GetJumpSeq() == 2)
	{
		ev->SetException(3);

		if (forceY->power != 12)
			forceY->power = 12;

		if (scrain->GetAnimType() == 311)
		{
			forceX->side = 0;
			if (scrain->GetSpeed() == 10)
			{
				if (forceY->power != 12)
					forceY->power = 12;
				forceX->power = 35;
			}
			else
			{
				if (forceY->script == 0)
				{
					if (forceY->power != 10)
						forceY->power = 10;
					forceX->power = 27;
				}
				else
					forceY->script = 0;
			}
		}
		else if (scrain->GetAnimType() == 312)
		{
			forceX->side = 1;
			if (scrain->GetSpeed() == 10)
			{
				if (forceY->power != 12)
					forceY->power = 12;
				forceX->power = 35;
			}
			else
			{
				if (forceY->script == 0)
				{
					if (forceY->power != 10)
						forceY->power = 10;
					forceX->power = 27;
				}
				else
					forceY->script = 0;
			}
		}
	}
}

void ifGroundFJump(SDL_Rect* r, Scrain* scrain, Colision* colision, Force* forceX, Events* ev, PhysicEngine* ph)
{
	if (ev->GetException() == 3)
	{
		if (colision->GetWallDetStatus() > 0)
		{
			if (scrain->GetAnimType() == 311 || scrain->GetAnimType() == 312)
			{
				forceX->power = 0;
				scrain->resetAnimCycle(0);
			}
		}
		else
		{
			if (scrain->GetAnimType() == 311)
			{
				if (ev->GetSideKeyStatus() == 1 && ph->hidePlaneDetect != 1)
				{
					scrain->resetAnimCycle(15);
					scrain->SetAnimType(1);
					ev->PushButton(97);
					ev->jumpDelay = 1;
					ev->t_jDalay = SDL_GetTicks();
				}
				else
				{
					scrain->SetAnimType(0);
					scrain->reductJumpKey();
					scrain->resetAnimCycle(0);
				}
				ev->ResetSideEffect();
			}
			else if (scrain->GetAnimType() == 312)
			{
				if (ev->GetSideKeyStatus() == 2 && ph->hidePlaneDetect != 1)
				{
					scrain->resetAnimCycle(15);
					scrain->SetAnimType(2);
					ev->PushButton(100);
					ev->jumpDelay = 1;
					ev->t_jDalay = SDL_GetTicks();
				}
				else
				{
					scrain->SetAnimType(11);
					scrain->reductJumpKey();
					scrain->resetAnimCycle(0);
				}
				ev->ResetSideEffect();
			}

		}
		
		ev->SetException(-1);
		scrain->SetJumpSeq(5);
	}
}

void PhysicEngine::check( Colision* colision)
{
	SDL_Rect r;
	r = scrain->GetPosition();
	r.y -= 64 - SCRAINH -1;
	r.h = SCRAINH;
	r.w = SCRAINW;
	correct = 0;
	switch (colision->isColidePlane(r, &correct, 0, 0, 0))
	{
	case 2:
	{
		if (forceY.power>0)
			hidePlaneDetect = 1;
		break;
	}
	case 5:
	{
		if (scrain->isfallen == true)
			scrain->isfallen = false;
		
		if (ev->GetException() == 4)
			ev->SetException(-1);

		ifGroundFJump(&r, scrain, colision, &forceX, ev, this); // when scrain is on the ground from jump

		if (hidePlaneDetect != 1)
		scrain->setting.y -= correct;
		if (hidePlaneDetect != 1)
		{
			forceX.power = 0;
			forceY.power = 0;
		}
		else
			hidePlaneDetect = 0;
		
		break;
	}
	case 0:
	{
		if (scrain->GetJumpSeq() != 4 && scrain->GetJumpSeq() != 0 && forceY.power == 0 || scrain->GetJumpSeq() != 4 && scrain->GetJumpSeq() != 0 && forceY.power == -8)
			scrain->SetJumpSeq(3);

		if (forceX.power == 1)
		{
			forceX.power++;
		}
		else
			if (forceX.power == -1)
			{
				forceX.power--;
			}


		if (forceY.power == -10)
			forceY.power = -10;
		else
			forceY.power--;

		if (scrain->isfallen != true)
		scrain->isfallen = true;

		if (forceX.power == 0) // moze powodowac bugi w ruchu postaci
		{
			if (ev->GetException() == 1 || ev->GetException() == 2)
				ev->SetException(-1);
		}
		break;
	}
	default:
		break;
	}


	ifJumpForce(scrain, &forceY, &forceX, ev);

	if (colision->isColidePlane(r, &correct, 1, 0 ,0) == 7 || colision->isColidePlane(r, &correct, 1,0,0) == 1)
		forceX.power = 0;

}


#pragma region func
bool PhysicEngine::isHidePlaneDetect()
{
	return hidePlaneDetect;
}
void PhysicEngine::loadPointers(Scrain* scr, Events* events)
{
	scrain = scr;
	ev = events;
};
#pragma endregion