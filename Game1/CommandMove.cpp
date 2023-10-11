#include "stdafx.h"

Command_State CommandMove::Excute()
{
	return Move();
}


void CommandMove::Update(shared_ptr<BlackBoard> board)
{
	if (board)
	{
		isAttack = board->isAttack;
		moverange = board->GetMoveRange();
		targetPos = board->GetObj()->GetWorldPos();
		dir = board->GetDir();
		spd = board->GetSpeed();
		rength = board->GetRange();
		toTargetDis = board->GetDistance();
		aniNum = board->GetAnimnum();
	}
	else
		return;
}

Command_State CommandMove::Move()
{

	toTargetDis = (targetPos - actor->GetWorldPos()).Length();
	dir = targetPos - actor->GetWorldPos();

	if (actor->actortype == Actor::ActorType::BOSS)
	{
		if (toTargetDis < moverange && !isAttack)
		{

			if (!ismoving || (actor->anim->GetPlayNum() < 2 && actor->anim->GetPlayNum() > 2))
			{
				randMove = RANDOM->Int(2, 3);
				actor->anim->ChangeAnimation(AnimationState::LOOP, randMove);
			}


			if (ismoving)
			{
				moveanim = true;
				dir.Normalize();
				if (randMove == 2)
					actor->MoveWorldPos(dir * spd * DELTA);
				if (randMove == 3)
					actor->MoveWorldPos(dir * (spd * 1.5f) * DELTA);
			}

			float rotY = -atan2f(dir.z, dir.x) + 90 * TORADIAN;
			float dis = 1.0f;

			if (g < 1.0f)
			{
				actor->rotation.y = Util::LerpRotation(actor->rotation.y, rotY, g, dis);
				g += DELTA;
			}
			else if (g > 1.0f)
			{
				g = 0.0f;
			}

			ismoving = true;

			return Command_State::Success;
		}

		else if (toTargetDis > moverange)
		{
			if (actor->anim->GetPlayTime() >= 1.0f)
			{
				randMove = -1;
				if (moveanim)
				{
					actor->anim->ChangeAnimation(AnimationState::LOOP, 2);
				}
				if (!moveanim)
				{
					actor->anim->ChangeAnimation(AnimationState::LOOP, 1);
				}
				ismoving = false;
				moveanim = false;
				return Command_State::Failure;
			}
		}
	}
	
	// 몬스터 타입이 1번일경우
	if (actor->actortype == Actor::ActorType::MONSTER)
	{
		if (toTargetDis < moverange && !isAttack)
		{
			if (!ismoving)
			{
				actor->anim->ChangeAnimation(AnimationState::LOOP, 2, 0.1f);
			}

			dir.Normalize();
			actor->MoveWorldPos(dir * spd * DELTA);

			float rotY = -atan2f(dir.z, dir.x) + 90 * TORADIAN;
			float dis = 1.0f;

			if (g < 1.0f)
			{
				actor->rotation.y = Util::LerpRotation(actor->rotation.y, rotY, g, dis);
				g += DELTA;
			}
			else if (g > 1.0f)
			{
				g = 0.0f;
			}

			ismoving = true;
			return Command_State::Success;
		}

		else if (toTargetDis > moverange && ismoving)
		{

			int temp = RANDOM->Int(0, 1);
			actor->anim->ChangeAnimation(AnimationState::LOOP, temp, 0.5f);
			ismoving = false;
			return Command_State::Failure;

			//if (actor->anim->GetPlayTime() >= 1.0f)
			//{
			//	int temp = RANDOM->Int(0,1);
			//	actor->anim->ChangeAnimation(AnimationState::LOOP, temp, 0.1f);
			//	ismoving = false;
			//	return Command_State::Failure;
			//}
		}
	}

	// 몬스터 타입이 2번일경우
	if (actor->actortype == Actor::ActorType::MONSTER2)
	{
		if (toTargetDis < moverange && !isAttack)
		{
			if (!ismoving)
			{
				actor->anim->ChangeAnimation(AnimationState::LOOP, 1, 0.1f);
			}

			dir.Normalize();
			actor->MoveWorldPos(dir * spd * DELTA);

			float rotY = -atan2f(dir.z, dir.x) + 90 * TORADIAN;
			float dis = 1.0f;

			if (g < 1.0f)
			{
				actor->rotation.y = Util::LerpRotation(actor->rotation.y, rotY, g, dis);
				g += DELTA;
			}
			else if (g > 1.0f)
			{
				g = 0.0f;
			}

			ismoving = true;
			return Command_State::Success;
		}

		else if (toTargetDis > moverange && ismoving)
		{

			actor->anim->ChangeAnimation(AnimationState::LOOP, 0, 0.5f);
			ismoving = false;
			return Command_State::Failure;
			//if (actor->anim->GetPlayTime() >= 1.0f)
			//{
			//	actor->anim->ChangeAnimation(AnimationState::LOOP, 0, 0.1f);
			//	ismoving = false;
			//	return Command_State::Failure;
			//}
		}
	}

	//if (dir != Vector3(0.0f, 0.0f, 0.0f))
	//{
	//	int rnadmove = RANDOM->Int(2,3); // 걷거나 뛰거나

	//	if (rnadmove == 2 && actor->anim->currentAnim != 1)
	//	{
	//		actor->anim->ChangeAnimation(AnimationState::LOOP, 2, 0.05f);

	//	}
	//	if(rnadmove == 3 && actor->anim->currentAnim != 1)
	//	{
	//		actor->anim->ChangeAnimation(AnimationState::LOOP, 3, 0.05f);
	//	}

	//	dir.Normalize();
	//	actor->MoveWorldPos(dir * spd * DELTA);

	//	//if (actor->anim->currentAnim != 1)
	//	//	actor->anim->ChangeAnimation(AnimationState::LOOP, 2, 0.05f);

	//	//actor->MoveWorldPos(dir * spd * DELTA);

	//	float rotY = -atan2f(dir.z, dir.x);
	//	float dis = 1.0f;

	//	if (g < 1.0f)
	//	{
	//		actor->rotation.y = Util::LerpRotation(actor->rotation.y, rotY, g, dis);
	//		g += DELTA;
	//	}
	//	else if (g > 1.0f)
	//	{
	//		g = 0.0f;
	//	}

	//	cout << "[Move]dir != Vector3(0.0f, 0.0f, 0.0f) 움직임모션실행. 각도 보간 Command_State::Success 반환" << endl;

	//	return Command_State::Success;
	//}
	//else
	//{
	//	// idle 상태로 전환시키기
	//	//actor->anim->ChangeAnimation(AnimationState::LOOP, 1, 0.05f);

	//	if (actor->anim->currentAnim != 0)
	//		actor->anim->ChangeAnimation(AnimationState::LOOP, 1, 0.05f);

	//	cout << "[Move]타겟위치까지 도착했음. 일반 모션으로 전환. Command_State::Failure 반환" << endl;
	//	return Command_State::Failure;
	//}
}

CommandMove::~CommandMove()
{
}

void CommandMove::LateUpdate(shared_ptr<BlackBoard> board)
{

}
void CommandMove::CheckState()
{
	if (actor)
		isActive = true;
}