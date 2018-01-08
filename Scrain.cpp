#include "Scrain.h"
#include "SDLStuff.h"
#include "SoundSystem.h"

Scrain::Scrain(SDL_Renderer* rendererr, Events* evs, short* migniecie)
{
	this->migniecie = migniecie;
	l_count = 0;
	gothit = 0;
	anim_cycle = 0;
	jump_seq = 0;
	r_angle = 0;
	hp = 100;
	detect = 10;
	m_speed = 8;
	AnimIl = 4;
	anim_type = -1;
	setting.w = SCRAINW;
	setting.h = SCRAINH;
	renderer = rendererr;
	lvl_speed = 0;
	animSpeed = 9;
	lastSpawn = {};
}
Scrain::~Scrain()
{
	l_count = 0;
	r_angle = NULL;
	frameH = frameW = NULL;
	textureH = textureW = NULL;
	anim_cycle = NULL;
	anim_type = NULL;
	rendSpeed = NULL;
	hp = NULL;
	detect = NULL;
	m_speed = NULL;
	CameraCoord = nullptr;
	delete CameraCoord;
	renderer = nullptr;
	delete renderer;
	phforceY = nullptr;
	delete phforceY;
	migniecie = nullptr;
	delete migniecie;
	DeleteScrain();
}
void Scrain::DeleteScrain()
{
	SDL_DestroyTexture(spriteSheet);
	SDL_DestroyTexture(MoveSheet);
	SDL_DestroyTexture(IdleSheet);
	SDL_DestroyTexture(JumpSheet);
	SDL_DestroyTexture(IdleJumpSheet);
}
void Scrain::d_Level()
{
	if (isHide() == false && isUntouch() == false)
	{
		if (GetAnimType() == 0 || GetAnimType() == 11 || GetAnimType() == 33 || GetAnimType() == 44 || GetAnimType() == 301 || GetAnimType() == 302)
		{
			l_count += 1;
			short t = 0;
			if (GetDetect() > -1 && GetDetect() < 41)
			{
				if (st.getDifficulty() == 1)
				{
					t = 7;
				}
					else
						if (st.getDifficulty() == 2)
						{
							t = 4;
						}
						else
							if (st.getDifficulty() == 3)
							{
								t = 2;
							}
			}
			else
				if (GetDetect() > 40 && GetDetect() < 71)
				{
					if (st.getDifficulty() == 1)
					{
						t = 4;
					}
					else
						if (st.getDifficulty() == 2)
						{
							t = 3;
						}
						else
							if (st.getDifficulty() == 3)
							{
								t = 1;
							}
				}
				else
					if (GetDetect() > 60)
					{
						if (st.getDifficulty() == 1)
						{
							t = 3;
						}
						else
							if (st.getDifficulty() == 2)
							{
								t = 2;
							}
							else
								if (st.getDifficulty() == 3)
								{
									t = 1;
								}
					}
			if (l_count == t)
				SetDetect(GetDetect() + 1);
			else
				if (l_count > t)
					l_count = 0;
		}
		else
		{
			l_count += 1;
			short t = 0;
			if (GetDetect() > -1 && GetDetect() < 31)
			{
				if (st.getDifficulty() == 1)
				t = 6;
				else
				if (st.getDifficulty() == 2)
					t = 8;
				else
					if (st.getDifficulty() == 3)
						t = 16;
			}
			else
				if (GetDetect() > 29 && GetDetect() < 71)
				{
					if (st.getDifficulty() == 1)
					t = 8;
					else
					if (st.getDifficulty() == 2)
						t = 12;
					else
						if (st.getDifficulty() == 3)
							t = 18;
				}
				else
					if (GetDetect() > 70)
					{
						if (st.getDifficulty() == 1)
							t = 10;
						else
							if (st.getDifficulty() == 2)
								t = 15;
							else
								if (st.getDifficulty() == 3)
									t = 22;
					}
			if (l_count == t && GetDetect() > -1)
				SetDetect(GetDetect() - 1);
			else
				if (l_count > t)
					l_count = 0;
		}
		if (GetAnimType() == 2 || GetAnimType() == 1 || GetAnimType() == 311 || GetAnimType() == 312)
		{
			if (GetDetect() < 1)
				SetDetect(0);
		}
	}
	else
		isHiden = false;
	
}

void Scrain::ifMove()
{
	if (move != 0)
	{
		if (lvl_speed < 4)
		{
			AnimIl = 4;
			animSpeed = 9;
		}
		if (anim_cycle != 0)
		{
			anim_cycle -= 1;

			

			if (lvl_speed == 4)
			{
				AnimIl = 3;
				animSpeed = 7;
				if (m_speed != 9)
					m_speed = 9;
			}
			else if (lvl_speed >= 9)
			{
				AnimIl = 3;
				if (m_speed != 10)
					m_speed = 10;
				animSpeed = 6;
			}

			if (anim_cycle > 2)
			{
				if (anim_type == 1 || anim_type == 33)
					setting.x -= m_speed;
				if (anim_type == 2 || anim_type == 44)
					setting.x += m_speed;
			}
			else
			{
				if (anim_type == 1 || anim_type == 33)
					setting.x -= m_speed - 1;
				if (anim_type == 2 || anim_type == 44)
					setting.x += m_speed - 1;
			}
		}
		
	}


	d_Level();

}

void Scrain::RenderScrain()
{
	rendSpeed++;
	if (animSpeed / rendSpeed == AnimIl)
		ressurectionEffect();

		if (jump_seq != 0)
		{
			drawingset.x = setting.x - (CameraCoord->x);
			drawingset.y = setting.y - (CameraCoord->y);
			drawingset.h = setting.h;
			drawingset.w = setting.w;
			if (8 / rendSpeed == 4)//8 speed of animation
			{
				rendSpeed = 0;
				ifJump(); // 3 sequens animation
			}
		}
		else
		{
			if (animSpeed / rendSpeed == AnimIl)//8 speed of animation
			{
				drawingset.x = setting.x - (CameraCoord->x);
				drawingset.y = setting.y - (CameraCoord->y);
				drawingset.h = setting.h;
				drawingset.w = setting.w;
				ifMove();
				// normal animation
				rendSpeed = 0;
				animRect.x += frameW;
				if (animRect.x >= textureW - 1)
				{
					animRect.x = 0;
					animRect.y += frameH;
					if (anim_type == 1 || anim_type == 0 || anim_type == 33 || anim_type == 301 || anim_type == 311)
					{
						if (animRect.y >= textureH / 2)
							animRect.y = 0;
					}
					else
						if (anim_type == 2 || anim_type == 11 || anim_type == 44 || anim_type == 302 || anim_type == 312)
						{
							if (animRect.y >= textureH)
								animRect.y = textureH / 2;
						}
						else
						{
							if (animRect.y >= textureH)
								animRect.y = 0;
						}
				}

				if (animRect.x < 4 * frameW || animRect.x > 9) // Gdy postaæ spada
				{
					if (anim_type != 312 && anim_type != 311 && isfallen == 1)
						animRect.x = 9 * frameW;
				}
			}
		}
	if (spriteSheet != NULL)
		SDL_RenderCopyEx(renderer, spriteSheet, &animRect, &drawingset, r_angle, NULL, SDL_FLIP_NONE);
}

void Scrain::LoadScrain(int yFrames, int xFrames)
{
	// load animation sheets
	if (ressurection == true)
		SDL_SetTextureAlphaMod(spriteSheet, 255);
	if (anim_type == -1)
	{
		//IdleSheet = LoadTexture("IdleAnim.png", renderer);
		//MoveSheet = LoadTexture("MoveAnim.png", renderer);
		//IdleJumpSheet = LoadTexture("IdleJumpAnim.png", renderer);
		//JumpSheet = LoadTexture("JumpAnim.png", renderer);

		
		SDL_RWops *file = SDL_RWFromFile("graphics/IdleAnim.png", "rb");
		IdleSheet = LoadTextureRW(file, renderer);
		file = SDL_RWFromFile("graphics/JumpAnim.png", "rb");
		JumpSheet = LoadTextureRW(file, renderer);
		file = SDL_RWFromFile("graphics/MoveAnim.png", "rb");
		MoveSheet = LoadTextureRW(file, renderer);
		file = SDL_RWFromFile("graphics/IdleJumpAnim.png", "rb");
		IdleJumpSheet = LoadTextureRW(file, renderer);
		
		spriteSheet = IdleSheet;
		anim_type = 11;
	}
	

	switch (anim_type)
	{
	case 1:
		spriteSheet = MoveSheet;
		break;
	case 2:
		spriteSheet = MoveSheet;
		break;
	case 33:
		spriteSheet = MoveSheet;
		break;
	case 44:
		spriteSheet = MoveSheet;
		break;
	case 301:
		spriteSheet = IdleJumpSheet;
		break;
	case 302:
		spriteSheet = IdleJumpSheet;
		break;
	case 311:
		spriteSheet = JumpSheet;
		break;
	case 312:
		spriteSheet = JumpSheet;
		break;
	default:
		spriteSheet = IdleSheet;
		break;
	}
	string LevelName = st.getLevelName();
	if (LevelName[0] == 'w' &&  LevelName[1] == 'e' && LevelName[2] == 't' &&
		LevelName[3] == 'l' && LevelName[4] == 'a' && LevelName[5] == 'n' &&
		LevelName[6] == 'd')
	{
		SDL_SetTextureColorMod(spriteSheet, 900, 900, 900);
	}
	SDL_QueryTexture(spriteSheet, NULL, NULL, &textureW, &textureH);
	frameW = textureW /xFrames;
	frameH = textureH / yFrames;
	animRect.x = animRect.y = 0;
	if (anim_type == 2 || anim_type == 11 || anim_type == 302 || anim_type == 312)
		animRect.y = textureH / 2;
	animRect.w = frameW;
	animRect.h = frameH;
	if (ressurection == true)
		SDL_SetTextureAlphaMod(spriteSheet, alpha);
	
}

void Scrain::ifJump()
{
	
	switch (jump_seq)
	{
	case 1:
	{
		jumpKey_reduct = 1;
		// from the ground
		//if (animRect.x <= textureW / 2)
			animRect.x += frameW;
		//else
			SetJumpSeq(2);
		break;
	}
	case 2:
	{
		// in air
		if (animRect.x <= textureW / 5)// before /6
			animRect.x += frameW;
		break;
	}
	case 3:
	{
		// in air goin to ground

		animRect.x += frameW;
		SetJumpSeq(4);
		break;
	}
	case 4:
	{
		if (*phforceY == 0) // jeœli sie zatnie to zresetuj
		{
			if (GetAnimType() == 311)
				ResetScrain(1);
			else
				if (GetAnimType() == 312)
					ResetScrain(2);
		}
		
		
		//waiting for contact with ground
		break;
	}
	case 5:
	{
		// on the ground
		if (animRect.x <= textureW / 2)
			animRect.x += frameW;
		if (GetAnimType() == 302 || GetAnimType() == 301)
			SetSize(SCRAINW, SCRAINH);
		if (GetAnimType() == 1 || GetAnimType() == 2)
		{
			jump_seq = 0;
			animRect.y = 0;
			animRect.x = 0;
			SetSize(SCRAINW, SCRAINH);
			LoadScrain(2, 16);
		}
		animRect.x += frameW;
		if (animRect.x >= textureW)
		{
			jump_seq = 0;
			animRect.y = 0;
			animRect.x = 0;
			SetSize(SCRAINW, SCRAINH);
			lvl_speed = 0;
			if (GetAnimType() == 301)
			{
				SetAnimType(0);
				resetAnimCycle(0);
			}
			else if (GetAnimType() == 302)
			{
				SetAnimType(11);
				resetAnimCycle(0);
			}

			LoadScrain(2, 16);
			
		}
		break;
	}
	default:
		break;
	}
}

void Scrain::ressurectionEffect()
{
	if (ressurection == true)
	{
		if (alpha == 255)
		{
			alpha = 0;
			ressurection = false;
			untouch = false;
			return;
		}
		SDL_SetTextureAlphaMod(spriteSheet, alpha);
		if (alpha + 3 < 255)
			alpha += 3;
		else
			alpha = 255;
	}
}

void Scrain::gotHit()
{
	if (hp != 0)
	{
		if (gothit == false)
			gothit = true;
		else
			gothit = false;
	}
}
void Scrain::isDeath(SDL_Rect cameraPos, Effects* effects)
{
	if (hp <= 0)
	{
		SoundMenager sm;
		sm.addSound("Ressurection", sm.getFreeChannel(), 0);
		*migniecie = 1;
		if (jump_seq != 0)
		{
			jump_seq = 0;
			animRect.y = 0;
			animRect.x = 0;
			SetSize(SCRAINW, SCRAINH);
			SetAnimType(11);
			LoadScrain(2, 16);		
		}
		SDL_SetTextureAlphaMod(spriteSheet, 0);
		lvl_speed = 0;
		if (st.getDifficulty() != 3)
		{
			if (detect > 80)
				detect = 100;
			else
			{
				if (st.getDifficulty() == 1)
					detect = 25;
				else
					detect = 35;
			}
		}
		if (lastSpawn.x != 0 && lastSpawn.y != 0)
		{
			setting.x = lastSpawn.x;
			setting.y = lastSpawn.y;
		}
		else
		{
			setting.x = spawnPosition.x;
			setting.y = spawnPosition.y;
		}
		SDL_Rect resef = setting;
		resef.y += setting.h;
		effects->loadEffect(2, 0, &resef);
		
		hp = 100;
		alpha = 0;
		untouch = true;
		ressurection = true;
	}
}

void Scrain::ResetScrain(bool side)
{
	SetJumpSeq(0);
	ResetAnimRect();
	SetSize(SCRAINW, SCRAINH);
	if (side == 1)
		SetAnimType(11);
	else
		SetAnimType(0);
	
	LoadScrain(2, 16);
}

#pragma region GET
bool Scrain::isHit()
{
	return gothit;
};
bool Scrain::isFallen()
{
	return isfallen;
};
bool Scrain::isHide()
{
	return isHiden;
};
Point Scrain::getSpawn()
{
	return lastSpawn;
};
bool Scrain::isCheckPointSet()
{
	if (lastSpawn.x != 0 && lastSpawn.y != 0)
		return true;
	else
		return false;

};
bool Scrain::isUntouch()
{
	return untouch;
};
bool Scrain::isRessurected()
{
	return ressurection;
};
int Scrain::GetJumpSeq()
{
	return jump_seq;
};
int Scrain::GetSpeed()
{
	return m_speed;
};
SDL_Rect Scrain::GetConvPosition()
{
	return SDL_Rect{ setting.x - CameraCoord->x, setting.y - CameraCoord->y, SCRAINW, SCRAINH };
};
SDL_Rect Scrain::GetPosition()
{
	return SDL_Rect{ setting.x, setting.y, SCRAINW, SCRAINH };
};
int Scrain::GetHP()
{
	return hp;
};
int Scrain::GetAnimType()
{
	return anim_type;
};
int Scrain::GetDetect()
{
	return detect;
};
bool Scrain::GetReductJumpKey_Status()
{
	return jumpKey_reduct;
};
#pragma endregion

#pragma region SET
void Scrain::setHide(bool var)
{
	if (isHiden != var)
		isHiden = var;
}
void Scrain::setSpawn(int x, int y)
{
	lastSpawn.x = x;
	lastSpawn.y = y;
}
void Scrain::setStartLoc(SDL_Rect cmr, Point spawnPos)
{
	setting.x = spawnPos.x;
	setting.y = spawnPos.y;
	spawnPosition = spawnPos;
}
void Scrain::getPointers(SDL_Rect *camCoord, int* phforceY)
{
	CameraCoord = camCoord;
	this->phforceY = phforceY;
}
void Scrain::setUntouch(bool var)
{
	untouch = var;
}
void Scrain::ResetAnimRect()
{
	animRect.y = 0;
	animRect.x = 0;
}
void Scrain::reductJumpKey()
{
	jumpKey_reduct = 0;
}
void Scrain::stopMove()
{
	move = 0;
}
void Scrain::SetJumpSeq(short seq) // sekwencja skoku
{
	jump_seq = seq;
}
void Scrain::SetSize(int w, int h)
{
	setting.w = w;
	setting.h = h;
}
void Scrain::GetSize(SDL_Rect* d)
{
	d->h = setting.h;
	d->w = setting.w;
}
void Scrain::SetRotation(int angle)
{
	r_angle = angle;
}
void Scrain::SetPosition(int x, int y)
{
	if (x != 0)
		setting.x = x;
	if (y != 0)
		setting.y = y;
}
void Scrain::SetSpeed(int sp)
{
	m_speed = sp;
}
void Scrain::SetLvlSpeed(int lsp)
{
	lvl_speed = lsp;
}
void Scrain::SetAnimType(int var)
{
	anim_type = var;
}
void Scrain::SetDetect(int var)
{
	if (var > 100)
		detect = 100;
	else
		detect = var;
}
void Scrain::SetHP(int var)
{
	hp = var;
}
void Scrain::resetAnimCycle(short var)
{
	anim_cycle = var;
}
void Scrain::reductJumpKey(bool var)
{
	jumpKey_reduct = var;
}
#pragma endregion