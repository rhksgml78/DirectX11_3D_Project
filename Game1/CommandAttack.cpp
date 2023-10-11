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
		// ���� �ִϸ��̼��� ����� board���� �����ͼ� ����
		if (actor->actortype == Actor::ActorType::BOSS)
		{
			if (!isAttack and animNum == -1)
			{
				//cout << "���� ���� ������Ʈ" << endl;
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
				//cout << " ���ݹ��� �������� ������. �ִϸ��̼� ONCE_LAST ���� " << endl;

				actor->anim->ChangeAnimation(AnimationState::ONCE_LAST, currentAnim, 0.1f);
				// �޺��� �����ʹٸ� ���ʺκп���
				// ���ǹ��� �߰������� �ۼ��ؾߵɵ���.
				// ��������� �Ϸ�ǰ� currentAnim �� = -1�� �ٲ��ָ鼭
				// isAttack �� true �� ��ȯ��Ų�ٸ� ������ ���ǹ����� ����
				currentAnim = -1;
				isAttack = true;
			}
		}
	}

	if (actor->anim->GetPlayTime() >= 1.0f and isAttack)
	{
		//cout << " �ִϸ��̼� 1ȸ �÷��� �Ϸ� " << endl;

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

		// ������ �������� �÷��̾� �������� �ٶ󺸵���
		actor->rotation.y = rotY;
		//float tempcount = 1.0f;
		//if (g < 1.0f)
		//{
		//	// �������� ����
		//	actor->rotation.y = Util::LerpRotation(actor->rotation.y, rotY, g, tempcount);
		//	g += DELTA;
		//}
		//else if (g > 1.0f)
		//{
		//	g = 0.0f;
		//}

		if (range < atkRange)
		{
			//cout << " ���ݹ������� ���� Success ��ȯ " << endl;

			//cout << "[Attack]���� ���ݹ��� ���� ���� ! Success ��ȯ" << endl;
			return Command_State::Success;
		}
		else if (range >= atkRange)
		{
			//cout << " ���ݹ������� ����⶧���� Failure ��ȯ " << endl;

			//cout << "[Attack]���ݹ��� �����! Failure ��ȯ" << endl;
			return Command_State::Failure;
		}

		//cout << " �������ǿ� ���������ʾ����Ƿ� Running ��ȯ " << endl;
		return Command_State::Running;

	}

	//if (dir != Vector3())
	//{
	//	cout << "[Attack]dir != Vector3(). Command_State::Failure ��ȯ" << endl;
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

	//	cout << "[Attack]�ִϸ��̼�����̳����� isAttack �����϶�. Command_State::Success ��ȯ" << endl;
	//	return Command_State::Success;
	//}

	//cout << "[Attack]���ǹ��鿡 ��� �����ʾ�����. Command_State::Running ��ȯ" << endl;
	//return Command_State::Running;
}
