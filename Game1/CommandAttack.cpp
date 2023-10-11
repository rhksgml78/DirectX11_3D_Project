#include "stdafx.h"

CommandAttack::~CommandAttack()
{
}

Command_State CommandAttack::Excute()
{
	return Attack();
}

void CommandAttack::Update(shared_ptr<BlackBoard> board)
{
	if (board)
	{
		targetPos = board->GetObj()->GetWorldPos();
		dir = board->GetDir();
		// 공격 애니메이션의 모션은 board에서 가져와서 갱신
		if (actor->actortype == Actor::ActorType::BOSS)
		{
			if (!isAttack and animNum == -1)
			{
				//cout << "공격 선택 업데이트" << endl;
				animNum = RANDOM->Int(7, 11);
				currentAnim = animNum;
				board->SetAnimNum(animNum);
			}
			
			//else if (isAttack and animNum != -1)
			//{
			//	board->SetAnimNum(-1);
			//}
		}
		if (actor->actortype == Actor::ActorType::MONSTER)
		{
			if (!isAttack and animNum == -1)
			{
				//animNum = RANDOM->Int(5, 7);
				//if (currentNum == animNum)
				//	while (animNum != currentNum)
				//		animNum = RANDOM->Int(5, 7);
				//
				//currentNum = animNum;
				//currentAnim = animNum;
				//board->SetAnimNum(animNum);

				animNum = RANDOM->Int(5, 7);
				currentAnim = animNum;
				board->SetAnimNum(animNum);
			}
		}
		if (actor->actortype == Actor::ActorType::MONSTER2)
		{
			if (!isAttack and animNum == -1)
			{
				animNum = RANDOM->Int(5, 7);
				currentAnim = animNum;
				board->SetAnimNum(animNum);
			}
		}

	}
	else
		return;
}

void CommandAttack::LateUpdate(shared_ptr<BlackBoard> board)
{
	if (board)
	{
		board->isAttack = isAttack;
	}
	else
		return;

}

Command_State CommandAttack::Attack()
{
	if (!isAttack)
	{
		range = (targetPos - actor->GetWorldPos()).Length();

		if (actor->actortype == Actor::ActorType::BOSS)
			atkRange = 30.0f;
		if (actor->actortype == Actor::ActorType::MONSTER)
			atkRange = 10.0f;
		if (actor->actortype == Actor::ActorType::MONSTER2)
			atkRange = 50.0f;

		if (range < atkRange)
		{
			if (currentAnim != -1)
			{
				//cout << " 공격범위 안쪽으로 들어왔음. 애니매이션 ONCE_LAST 실행 " << endl;

				actor->anim->ChangeAnimation(AnimationState::ONCE_LAST, currentAnim, 0.1f);
				// 콤보를 만들고싶다면 이쪽부분에서
				// 조건문을 추가적으로 작성해야될듯함.
				// 모든패턴이 완료되고 currentAnim 을 = -1로 바꿔주면서
				// isAttack 을 true 로 변환시킨다면 마무리 조건문으로 들어갈것
				currentAnim = -1;
				isAttack = true;
			}
		}
	}

	if (actor->anim->GetPlayTime() >= 1.0f and isAttack)
	{
		//cout << " 애니매이션 1회 플레이 완료 " << endl;

		isAttack = false;
		animNum = -1;

		if (actor->actortype == Actor::ActorType::BOSS)
		{
			actor->anim->ChangeAnimation(AnimationState::LOOP, 2, 0.1f);
			rotY = -atan2f(dir.z, dir.x) - 90 * TORADIAN;
		}
		if (actor->actortype == Actor::ActorType::MONSTER)
		{
			actor->anim->ChangeAnimation(AnimationState::LOOP, 2, 0.1f);
			rotY = -atan2f(dir.z, dir.x) + 90 * TORADIAN;
		}
		if (actor->actortype == Actor::ActorType::MONSTER2)
		{
			actor->anim->ChangeAnimation(AnimationState::LOOP, 1, 0.1f);
			rotY = -atan2f(dir.z, dir.x) + 90 * TORADIAN;
		}

		// 공격이 끝났을때 플레이어 방향으로 바라보도록
		actor->rotation.y = rotY;
		//float tempcount = 1.0f;
		//if (g < 1.0f)
		//{
		//	// 보간으로 진행
		//	actor->rotation.y = Util::LerpRotation(actor->rotation.y, rotY, g, tempcount);
		//	g += DELTA;
		//}
		//else if (g > 1.0f)
		//{
		//	g = 0.0f;
		//}

		if (range < atkRange)
		{
			//cout << " 공격범위내에 있음 Success 반환 " << endl;

			//cout << "[Attack]아직 공격범위 내에 있음 ! Success 반환" << endl;
			return Command_State::Success;
		}
		else if (range >= atkRange)
		{
			//cout << " 공격범위에서 벗어났기때문에 Failure 반환 " << endl;

			//cout << "[Attack]공격범위 벗어났음! Failure 반환" << endl;
			return Command_State::Failure;
		}

		//cout << " 공격조건에 충족하지않았으므로 Running 반환 " << endl;
		return Command_State::Running;

	}

	//if (dir != Vector3())
	//{
	//	cout << "[Attack]dir != Vector3(). Command_State::Failure 반환" << endl;
	//	return Command_State::Failure;
	//}


	//if (currentAnim != -1 and !isAttack)
	//{
	//	actor->anim->ChangeAnimation(AnimationState::ONCE_LAST, currentAnim, 0.1f);
	//	currentAnim = -1;
	//	isAttack = true;
	//}

	//if (actor->anim->GetPlayTime() >= 1.0f and isAttack)
	//{
	//	isAttack = false;
	//	animNum = -1;

	//	cout << "[Attack]애니메이션재생이끝나고 isAttack 조건일때. Command_State::Success 반환" << endl;
	//	return Command_State::Success;
	//}

	//cout << "[Attack]조건문들에 모두 들어가지않았을때. Command_State::Running 반환" << endl;
	//return Command_State::Running;
}
