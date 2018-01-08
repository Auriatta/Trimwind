#include"Events.h"

Events::Events(SDL_Event* eventt)
{
	Event = eventt;
	e_except = -1;
	l_count = 0;
	runSound = -1;
}

Events::~Events()
{
	effects = nullptr;
	delete effects;
	Event = nullptr;
	delete Event;
	ishplane = nullptr;
	delete ishplane;
	e_except = -1;
	l_count = 0;
}

void Events::scrain(Scrain* scrain)
{
	if (Event->type == SDL_KEYUP && Event->key.keysym.sym == SDLK_a ||
		Event->type == SDL_KEYUP && Event->key.keysym.sym == SDLK_d)
	{
		if (sideKeyStat != 0)
		sideKeyStat = 0;
	}
	
	if (SDL_GetTicks() - t_jDalay >300)
	{
		t_jDalay = 0;
		jumpDelay = 0;
	}

	if (runSound == -1 && scrain->anim_type != 11 &&
		scrain->anim_type != 0)
	{
		sm.addSound("Run", RUNCHANNEL, -1);
		runSound = 1;
	}

	if (Event->type == SDL_KEYDOWN)				
	{
		switch (Event->key.keysym.sym)
		{
		case SDLK_LSHIFT:
		{
			if (scrain->anim_type == 33 || scrain->anim_type == 44)
			{
				if (scrain->GetAnimType() == 33)
				{
					scrain->ResetScrain(0);
				}
				else
					if (scrain->GetAnimType() == 44)
					{
						scrain->ResetScrain(1);
					}
				scrain->resetAnimCycle(0);
				scrain->SetSpeed(0);
				sm.stopSound(RUNCHANNEL);
				runSound = -1;
			}
			break;
		}
		case SDLK_a:
		{
			
			if (scrain->anim_type != 301 && scrain->anim_type != 302 && e_except != 1 && e_except != 3) // && e_except != 1 jeœli pojawia sie bledy w sterowaniu
			{
				
				if (runSound == -1)
				{
					sm.addSound("Run", RUNCHANNEL, -1);
					runSound = 1;
				}
				
				if (runSound == 0)
				{
					sm.resumeSound(RUNCHANNEL);
					runSound = 1;
				}
				sideKeyStat = 1;
				if (scrain->anim_type != 1 && scrain->anim_cycle == 0 || scrain->anim_type == 2 || scrain->anim_type == 11 || scrain->anim_type == 44)
				{
					if (scrain->anim_type == 11)
					{
						sm.stopSound(RUNCHANNEL);
						runSound = -1;
					}
							
					if (scrain->lvl_speed != 0)
						scrain->lvl_speed = 0;
					if (scrain->m_speed != 8)
						scrain->m_speed = 8;
					if (st.getDifficulty() != 3)
					{
						if (e_except == 2)// moze wywolywac bugi
							e_except = -1;
					}
					scrain->anim_type = 1;// lewo
					scrain->anim_cycle = 15;
					scrain->LoadScrain(2, 16);
				}
				else
				{
					SetException(-1);
					if (scrain->anim_cycle == 0);
					scrain->reductJumpKey();
					if (scrain->anim_cycle == 0)
						scrain->anim_cycle = 15;
					scrain->lvl_speed += 1;
					if (scrain->isRessurected() == 0)
						scrain->setUntouch(0);
					if (effects->isRun(1) == true && effects->isTimerRun(1) == false)
					{
						effects->removeEffect(1, true);
					}
				}
			}
			else
				scrain->anim_cycle = 0;

			break;
		}
		case SDLK_d:
		{
			if (scrain->anim_type != 302 && scrain->anim_type != 301 && e_except != 2 && e_except != 3) // && e_except != 2 Jeœli pojawia sie bledyt w sterowaniu
			{
				sideKeyStat = 2;
				
				if (runSound == -1)
				{
					sm.addSound("Run", RUNCHANNEL, -1);
					runSound = 1;
				}
				
				if (runSound == 0)
				{
					sm.resumeSound(RUNCHANNEL);
					runSound = 1;
				}
				if (scrain->anim_type != 2 && scrain->anim_cycle == 0 || scrain->anim_type == 1 || scrain->anim_type == 0 || scrain->anim_type == 33)
				{
						
					if (scrain->anim_type == 0)
					{
						sm.stopSound(RUNCHANNEL);
						runSound = -1;
					}
					if (scrain->lvl_speed != 0)
						scrain->lvl_speed = 0;
					if (scrain->m_speed != 8)
						scrain->m_speed = 8;
					if (st.getDifficulty() != 3)
					{
						if (e_except == 1)// moze wywolywac bugi
							e_except = -1;
					}
					scrain->anim_type = 2; // prawo
					scrain->anim_cycle = 15;
					scrain->LoadScrain(2, 16);
				}
				else
				{
					SetException(-1);
					
					if (scrain->anim_cycle == 0);
					scrain->reductJumpKey();
					if (scrain->anim_cycle == 0)
						scrain->anim_cycle = 15;
					scrain->lvl_speed += 1;
					if (scrain->isRessurected() == 0)
						scrain->setUntouch(0);
					if (effects->isRun(1) == true && effects->isTimerRun(1) == false)
					{
						effects->removeEffect(1, true);
					}
				}
			}
			else
				scrain->anim_cycle = 0;

			break;
		}
		case SDLK_SPACE:
		{
			if (e_except != 3 && e_except != 4 && scrain->GetReductJumpKey_Status() != true && jumpDelay == false)
			{
				if (scrain->anim_type < 300)
				{
					if (runSound > 0)
					{
						sm.pauseSound(RUNCHANNEL);
						runSound = 0;
					}
					sm.addSound("Jump", JUMPCHANNEL, 0);
					if (scrain->anim_type == 0 || scrain->anim_type == 33)
					{
						scrain->SetSize(SCRAINW, SCRAINJH);
						scrain->anim_type = 301;
						sideKeyStat = 0;
					}
					else if (scrain->anim_type == 11 || scrain->anim_type == 44)
					{
						scrain->SetSize(SCRAINW, SCRAINJH);
						scrain->anim_type = 302;
						sideKeyStat = 0;
					}
					else if (scrain->anim_type == 1)
					{
						scrain->anim_type = 311;
						if (GetException() == 2)
							scrain->setUntouch(1);
					}
					else
						if (scrain->anim_type == 2)
						{
							scrain->anim_type = 312;
							if (GetException() == 1)
								scrain->setUntouch(1);

						}
					e_except = 5;
					scrain->SetJumpSeq(1);
					scrain->LoadScrain(2, 7);
					
				}
			}
			break;
		}
		default:
			break;
		}
	}
	else
	{
		if (sideKeyStat == 1 && scrain->anim_cycle == 0 && *ishplane != 1 && scrain->anim_type != 311 && scrain->anim_type != 312 && e_except != 5)  // wspomaganie wcisnietego klawisza, usuwa mn. losowe zaciecia
		{
			PushButton(97);
		}
		else
			if (sideKeyStat == 2 && scrain->anim_cycle == 0 && *ishplane != 1 && scrain->anim_type != 311 && scrain->anim_type != 312 && e_except != 5)
			{
				PushButton(100);
			}
	}
	

	// idle
	if (Event->type == SDL_KEYUP && Event->key.keysym.sym == SDLK_d && scrain->jumpKey_reduct == 1 ||
		Event->type == SDL_KEYUP && Event->key.keysym.sym == SDLK_a && scrain->jumpKey_reduct == 1 || 
		Event->type == SDL_KEYUP && Event->key.keysym.sym == SDLK_SPACE && scrain->jumpKey_reduct == 1)
	{
		scrain->jumpKey_reduct = 0;
	}

	if (scrain->isFallen() == true && runSound>0)
	{
			sm.pauseSound(RUNCHANNEL);
			runSound = 0;
	}

	if (runSound == 1 && scrain->anim_type == 11 ||
		runSound == 1 && scrain->anim_type == 0)
	{
		sm.pauseSound(RUNCHANNEL);
		runSound = 0;
	}
	
	if (Event->type == SDL_KEYUP && scrain->anim_type < 213 || scrain->anim_type == 33 || scrain->anim_type == 44)
	{
		
		if (scrain->isFallen() == false)
		{
			if (scrain->anim_type != 0 && scrain->anim_type != 11 && scrain->anim_type != 3)
			{
				if (scrain->anim_type == 1)
					scrain->anim_type = 33;
				else
				{
					if (scrain->anim_type == 2)
						scrain->anim_type = 44;
				}
				if (scrain->anim_cycle == 0)
				{
					if (scrain->anim_type == 33)
						scrain->anim_type = 0; // idle
					else
						scrain->anim_type = 11;
					scrain->anim_cycle = 0;
					scrain->LoadScrain(2, 16);
					if (runSound == 1)
					{
						sm.pauseSound(RUNCHANNEL);
						runSound = 0;
					}
				}
			}
		}
	}
}

void Events::PushButton(int key)
{
	Event->type = SDL_KEYDOWN;
	Event->key.keysym.sym = key;
	SDL_PushEvent(*&Event);
}
short Events::GetSideKeyStatus()
{
	return sideKeyStat;
}
void Events::ResetSideEffect()
{
	effects->removeEffect(1, true);
}
void Events::ResetSideKeyStatus()
{
	sideKeyStat = 0;
}
SDL_Event* Events::GetEventClass()
{
	return Event;
}
void Events::SetException(short event_code)
{
	e_except = event_code;
}
int Events::GetException()
{
	return e_except;
}
void Events::loadPointers(Effects* effects, bool* ishplane)
{
	this->effects = effects;
	this->ishplane = ishplane;
}
