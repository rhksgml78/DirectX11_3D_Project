#include "stdafx.h"

void APlayer::Init()
{
	root = Actor::Create();
	root->LoadFile("Player2.xml");
	player = (Actor*)root->Find("RootNode");
	//float colortemp = 0.1f;
	//root->Find("vanguard_Mesh")->material->diffuse = Color(colortemp, colortemp, colortemp, 1.0f);
	//root->Find("vanguard_Mesh")->material->specular = Color(colortemp, colortemp, colortemp, 1.0f);
	//root->Find("vanguard_Mesh")->material->emissive = Color(colortemp, colortemp, colortemp, 1.0f);

	//칼목록
	// sword.xml
	// axe.xml
	// BastardSword.xml

	sword = Actor::Create();
	sword->LoadFile("BastardSword.xml");
	sword->LoadFile("sword.xml");
	sword->name = "root2";
	sword->parent = root->Find("Sword");

	axe = Actor::Create();
	axe->LoadFile("axe.xml");
	axe->name = "axe";
	axe->parent = root->Find("Sword");
	axe->visible = false;

	Lefthand = Actor::Create();
	Lefthand->LoadFile("Largeshield.xml");
	Lefthand->parent = root->Find("Handsocket_L");

	armor = Actor::Create("armor");
	armor->LoadFile("SpartanArmor.xml");
	armor->LoadFile("armor1.xml");
	armor->parent = root->Find("Armorsocket");

	lock = Actor::Create();
	//lock->LoadFile("Monster1.xml");

	//Cam = Camera::Create("Player_Cam");
	//Camera::main = Cam;
	//Cam->rotation.x = 19 * TORADIAN;
	//Cam->rotation.y = -180 * TORADIAN;

	trail = new SlashTrail();
	trail->Bottom = root->Find("BottomTrail");
	trail->Top = root->Find("TopTrail");
	trail->interval = 0;
	trail->maxTrail = 60;
	trail->material->diffuseMap = RESOURCE->textures.Load("../Texture/slashtrail.png");
	trail->material->diffuse = Color(0.5f, 0.5f, 0.5f, 1.0);
	trailEffect = true;

	state = AnimState::IDLE;
	prevState = state;
	root->anim->ChangeAnimation(AnimationState::LOOP, (int)state);

	weapon = WeaponState::Sword;

	isLanding = true;
	isRunning = false;
	camLock = false;
	secondComboAttack = false;
	thirdComboAttack = false;
	roolInput = false;
	lerpAfterRoll = false;

	speed = 10.0f;
	gravity = 0.0f;
	roolInterpolationTimer = 0.0f;
	secondComboTimer = 0.0f;
	thirdComboTimer = 0.0f;
	lerpAmount = 0.0f;

	// 콜라이더 전달 변수
	AtkCol1 = root->Find("AttackCol")->collider; //기본사이즈 <Scale X="40" Y="5" Z="5"/>
	HitCol = root->Find("mixamorig:Hips")->collider;

	// 무기에있는 콜라이더 체크용 visible
	root->Find("AttackCol")->collider->visible = false;
}

void APlayer::Update()
{
	//플레이어의 현재재생중인 애니매이션 번호 출력
	//cout << player->anim->GetPlayNum() << endl;

	if (root->Find("Offset")->GetWorldPos().y == 0)
	{
		root->Find("Offset")->SetWorldPosY(0);
	}

	CollOn(); // 콜라이더가 꺼져있다면 켜주는 함수
	SetPower(); // 공격모션에 따라 플레이어의 데미지 세팅
	SetAtkCol(); // 공격용 콜라이더 세팅

	if (isDameged) // 데미지 받았을때 보스 색변하도록
	{
		float temp = RANDOM->Float(0.5f, 1.0f);
		root->Find("vanguard_Mesh")->material->ambient = Color(temp, 0.5f, 0.5f, 1.0f);
		root->Find("vanguard_visor")->material->ambient = Color(temp, 0.5f, 0.5f, 1.0f);
		if (TIMER->GetTick(Damegedzerotime, 0.5f))
		{
			isDameged = false;
			root->Find("vanguard_Mesh")->material->ambient = Color(0.5f, 0.5f, 0.5f, 1.0f);
			root->Find("vanguard_visor")->material->ambient = Color(0.5f, 0.5f, 0.5f, 1.0f);
		}
	}



	if (state == AnimState::IDLE)
	{
		if (INPUT->KeyDown('S')) // S키누르면 뒤로돌기
			root->Find("Offset")->rotation.y -= 180 * TORADIAN;
	}


	if (INPUT->KeyDown('C')) hp -= 22;

	lastPos = root->GetWorldPos();

	//if (INPUT->KeyDown('Q'))
	//{
	//	camLock = !camLock;
	//	root->Find("Offset")->rotation.y = root->Find("Campoint")->rotation.y;
	//	//root->Find("Offset")->rotation.y = root->Find("Socket")->rotation.y;
	//}

	if (!isLanding)
	{
		gravity -= GRAVITY_ACCELERATION * DELTA;
		root->MoveWorldPos(Vector3{0 , gravity , 0} *20 * DELTA);
	}

	if (root->GetWorldPos().y < 0)
	{
		root->SetWorldPosY(0);
		isLanding = true;
	}

	if (INPUT->KeyDown(VK_LBUTTON) && INPUT->KeyPress(VK_SHIFT))
	{
		state = AnimState::STRONG_ATTACK;
	}


	if (state == AnimState::IDLE or
		state == AnimState::IDLE1 or
		state == AnimState::IDLE2 or 
		state == AnimState::BLOCK or
		state == AnimState::WALK or
		state == AnimState::LEFT_WALK or
		state == AnimState::RIGHT_WALK or
		state == AnimState::RUN_FRONT or
		state == AnimState::SIDE_WALK_LEFT or
		state == AnimState::SIDE_WALK_RIGHT or
		state == AnimState::LEFT_WALK or
		state == AnimState::RIGHT_WALK or
		state == AnimState::WALK_BACK)
	{


		if (INPUT->KeyDown(VK_RIGHT))
		{
			state = AnimState::EQUIP;
			isWeaponCange = false;
		}

		//if (INPUT->KeyDown(VK_RIGHT) && weapon == WeaponState::Sword)
		//{
		//	//weapon = WeaponState::Axe;
		//	state = AnimState::EQUIP;
		//	isWeaponCange = false;
		//}

		//if (INPUT->KeyDown(VK_RIGHT) && weapon == WeaponState::Axe)
		//{
		//	//weapon = WeaponState::Sword;
		//	state = AnimState::EQUIP;
		//	isWeaponCange = false;
		//}

		if (INPUT->KeyDown('F') and isLanding)
		{
			gravity = 4.0f;
			isLanding = false;
		}

		if (INPUT->KeyDown(VK_LBUTTON) && INPUT->KeyPress(VK_SHIFT))
		{
			state = AnimState::STRONG_ATTACK;
		}

		if (weapon == WeaponState::Sword and INPUT->KeyDown(VK_LBUTTON))
		{
			state = AnimState::NORMAL_ATTACK;
		}

		else if (weapon == WeaponState::Axe and INPUT->KeyDown(VK_LBUTTON))
		{
			state = AnimState::SECOND_WEAPON_FIRST_ATTACK;
		}


		if (INPUT->KeyDown('R'))
		{
			state = AnimState::DRINKING;
		}

		if (weapon == WeaponState::Sword)
		{
			if (secondComboAttack)
			{
				secondComboTimer += DELTA;

				if (secondComboTimer > 0.3f or
					state == AnimState::ROLL or
					state == AnimState::BLOCK or
					!isLanding)
				{
					secondComboAttack = false;
					secondComboTimer = 0.0f;
				}

				else if (INPUT->KeyDown(VK_LBUTTON))
				{
					state = AnimState::SECOND_ATTACK;
					secondComboAttack = false;
					secondComboTimer = 0.0f;
				}
			}

			if (thirdComboAttack)
			{
				thirdComboTimer += DELTA;

				if (thirdComboTimer > 0.3f or
					state == AnimState::ROLL or
					state == AnimState::BLOCK or
					!isLanding)
				{
					thirdComboAttack = false;
					thirdComboTimer = 0.0f;
				}

				else if (INPUT->KeyDown(VK_LBUTTON))
				{
					state = AnimState::THIRD_ATTACK;
					thirdComboAttack = false;
					thirdComboTimer = 0.0f;
				}
			}

		}

		if (weapon == WeaponState::Axe)
		{
			if (secondComboAttack)
			{
				secondComboTimer += DELTA;

				if (secondComboTimer > 0.3f or
					state == AnimState::ROLL or
					state == AnimState::BLOCK or
					!isLanding)
				{
					secondComboAttack = false;
					secondComboTimer = 0.0f;
				}

				else if (INPUT->KeyDown(VK_LBUTTON))
				{
					state = AnimState::SECOND_WEAPON_SECOND_ATTACK;
					secondComboAttack = false;
					secondComboTimer = 0.0f;
				}
			}

			if (thirdComboAttack)
			{
				thirdComboTimer += DELTA;

				if (thirdComboTimer > 0.3f or
					state == AnimState::ROLL or
					state == AnimState::BLOCK or
					!isLanding)
				{
					thirdComboAttack = false;
					thirdComboTimer = 0.0f;
				}

				else if (INPUT->KeyDown(VK_LBUTTON))
				{
					state = AnimState::SECOND_WEAPON_THIRD_ATTACK;
					thirdComboAttack = false;
					thirdComboTimer = 0.0f;
				}
			}

		}

	}

	if (state == AnimState::IDLE)
	{
		inputDir = Vector3();

		if (!camLock)
		{
			if (INPUT->KeyDown('W') or
				INPUT->KeyDown('A') or 
				INPUT->KeyDown('S') or 
				INPUT->KeyDown('D'))
				state = AnimState::WALK;
		}


		if (INPUT->KeyPress(VK_SPACE))
		{
			roolTimer += DELTA;
		}

		if (INPUT->KeyUp(VK_SPACE) and roolTimer <= 0.3)
		{
			rollDirection = inputDir;
			state = AnimState::ROLL;
			roolTimer = 0;
		}

		else if (INPUT->KeyUp(VK_SPACE) and roolTimer >= 0.3)
		{
			roolTimer = 0;
		}

		if (not isLanding)
			state = AnimState::JUMP_STAND;

		//if (camLock)
		//{
		//	if (INPUT->KeyPress(VK_SPACE))
		//		state = AnimState::RUN_FRONT;
		//	else if (INPUT->KeyPress('W') && !INPUT->KeyPress('A') && !INPUT->KeyPress('D'))
		//		state = AnimState::WALK;
		//	else if (INPUT->KeyPress('A') && !INPUT->KeyPress('W') && !INPUT->KeyPress('S'))
		//		state = AnimState::LEFT_WALK;
		//	else if (INPUT->KeyPress('D') && !INPUT->KeyPress('W') && !INPUT->KeyPress('S'))
		//		state = AnimState::RIGHT_WALK;
		//	else if (INPUT->KeyPress('S') && !INPUT->KeyPress('A') && !INPUT->KeyPress('D'))
		//		state = AnimState::WALK_BACK;
		//	else if (INPUT->KeyPress('W') && INPUT->KeyPress('D'))
		//		state = AnimState::RIGHT_WALK;
		//	else if (INPUT->KeyPress('W') && INPUT->KeyPress('A'))
		//		state = AnimState::LEFT_WALK;
		//	else if (INPUT->KeyPress('S') && INPUT->KeyPress('D'))
		//		state = AnimState::RIGHT_WALK;
		//	else if (INPUT->KeyPress('S') && INPUT->KeyPress('A'))
		//		state = AnimState::LEFT_WALK;
		//}

	}
	else if (state == AnimState::WALK)
	{
		WASD();
		ChangeRotation();

		if (INPUT->KeyPress(VK_SPACE))
			state = AnimState::RUN_FRONT;

		if (not INPUT->KeyPress('W') and
			not INPUT->KeyPress('A') and
			not INPUT->KeyPress('S') and
			not INPUT->KeyPress('D'))
			state = AnimState::IDLE;

		if (not isLanding)
			state = AnimState::JUMP_RUN;

		//if (camLock)
		//{
		//	if (INPUT->KeyPress(VK_SPACE))
		//		state = AnimState::RUN_FRONT;
		//	else if (INPUT->KeyPress('W') && !INPUT->KeyPress('A') && !INPUT->KeyPress('D'))
		//		state = AnimState::WALK;
		//	else if (INPUT->KeyPress('A') && !INPUT->KeyPress('W') && !INPUT->KeyPress('S'))
		//		state = AnimState::LEFT_WALK;
		//	else if (INPUT->KeyPress('D') && !INPUT->KeyPress('W') && !INPUT->KeyPress('S'))
		//		state = AnimState::RIGHT_WALK;
		//	else if (INPUT->KeyPress('S') && !INPUT->KeyPress('A') && !INPUT->KeyPress('D'))
		//		state = AnimState::WALK_BACK;
		//	else if (INPUT->KeyPress('W') && INPUT->KeyPress('D'))
		//		state = AnimState::RIGHT_WALK;
		//	else if (INPUT->KeyPress('W') && INPUT->KeyPress('A'))
		//		state = AnimState::LEFT_WALK;
		//	else if (INPUT->KeyPress('S') && INPUT->KeyPress('D'))
		//		state = AnimState::RIGHT_WALK;
		//	else if (INPUT->KeyPress('S') && INPUT->KeyPress('A'))
		//		state = AnimState::LEFT_WALK;
		//}

	}
	else if (state == AnimState::RUN_FRONT)
	{
		WASD();
		ChangeRotation();

		if (INPUT->KeyPress(VK_SPACE))
			roolTimer += DELTA;

		if (INPUT->KeyUp(VK_SPACE) and roolTimer <= 0.3)
		{
			rollDirection = inputDir;
			state = AnimState::ROLL;
			roolTimer = 0;
			return;
		}

		if (INPUT->KeyUp(VK_SPACE) and roolTimer >= 0.3)
		{
			state = AnimState::WALK;
			roolTimer = 0;
			return;
		}

		if (not INPUT->KeyPress('W') and
			not INPUT->KeyPress('A') and
			not INPUT->KeyPress('S') and
			not INPUT->KeyPress('D'))
			state = AnimState::IDLE;

		if (not isLanding)
			state = AnimState::JUMP_RUN;
		//if (camLock)
		//{
		//	if (INPUT->KeyPress(VK_SPACE))
		//		state = AnimState::RUN_FRONT;
		//	else if (INPUT->KeyPress('W') && !INPUT->KeyPress('A') && !INPUT->KeyPress('D'))
		//		state = AnimState::WALK;
		//	else if (INPUT->KeyPress('A') && !INPUT->KeyPress('W') && !INPUT->KeyPress('S'))
		//		state = AnimState::LEFT_WALK;
		//	else if (INPUT->KeyPress('D') && !INPUT->KeyPress('W') && !INPUT->KeyPress('S'))
		//		state = AnimState::RIGHT_WALK;
		//	else if (INPUT->KeyPress('S') && !INPUT->KeyPress('A') && !INPUT->KeyPress('D'))
		//		state = AnimState::WALK_BACK;
		//	else if (INPUT->KeyPress('W') && INPUT->KeyPress('D'))
		//		state = AnimState::RIGHT_WALK;
		//	else if (INPUT->KeyPress('W') && INPUT->KeyPress('A'))
		//		state = AnimState::LEFT_WALK;
		//	else if (INPUT->KeyPress('S') && INPUT->KeyPress('D'))
		//		state = AnimState::RIGHT_WALK;
		//	else if (INPUT->KeyPress('S') && INPUT->KeyPress('A'))
		//		state = AnimState::LEFT_WALK;
		//}
	}
	else if (state == AnimState::JUMP_STAND)
	{
		if (isLanding)
		{
			state = AnimState::WALK;
		}
	}
	else if (state == AnimState::JUMP_RUN)
	{
		WASD();
		ChangeRotation();

		if (isLanding)
			state = AnimState::RUN_FRONT;
	}
	else if (state == AnimState::ROLL)
	{
		secondComboAttack = false;
		//root->anim->aniScale = 1.3;
		
		root->MoveWorldPos(rollDirection * ROLL_SPEED * DELTA);

		if (rollDirection == Vector3{})
			root->MoveWorldPos(root->Find("Offset")->GetForward() * -ROLL_SPEED * DELTA);

		float currentRotation = root->Find("Offset")->rotation.y;
		float camRotation = root->Find("Campoint")->rotation.y;
		//float camRotation = root->Find("Socket")->rotation.y;

		//if (roolInput)
		//{
		//	if (INPUT->KeyPress('W'))
		//		root->Find("Offset")->rotation.y = camRotation;
		//	if (INPUT->KeyPress('S'))
		//		root->Find("Offset")->rotation.y = camRotation + 180 * TORADIAN;
		//	if (INPUT->KeyPress('D'))
		//		root->Find("Offset")->rotation.y = camRotation + 90 * TORADIAN;
		//	if (INPUT->KeyPress('A'))
		//		root->Find("Offset")->rotation.y = camRotation - 90 * TORADIAN;

		//	if (INPUT->KeyPress('S') && INPUT->KeyPress('D'))
		//		root->Find("Offset")->rotation.y = camRotation + 135 * TORADIAN;
		//	if (INPUT->KeyPress('S') && INPUT->KeyPress('A'))
		//		root->Find("Offset")->rotation.y = camRotation - 135 * TORADIAN;
		//	if (INPUT->KeyPress('W') && INPUT->KeyPress('D'))
		//		root->Find("Offset")->rotation.y = camRotation + 45 * TORADIAN;
		//	if (INPUT->KeyPress('W') && INPUT->KeyPress('A'))
		//		root->Find("Offset")->rotation.y = camRotation - 45 * TORADIAN;
		//	roolInput = false;
		//}

		if (root->anim->currentAnimator.currentFrame == root->anim->playList[(int)AnimState::ROLL]->frameMax - 2)
		{
			root->anim->currentAnimator.currentFrame = 0;
			//root->anim->aniScale = 1.0f;
			//if (camLock && !INPUT->KeyPress(VK_SPACE))
			//{
			//	lerpAmount = 0.0f; 
			//	lerpAfterRoll = true; 
			//}
			state = AnimState::WALK;
		}
	}
	else if (state == AnimState::NORMAL_ATTACK)
	{
		if (root->anim->currentAnimator.currentFrame == root->anim->playList[(int)AnimState::NORMAL_ATTACK]->frameMax - 2)
		{
			root->anim->currentAnimator.currentFrame = 0;
			secondComboAttack = true;
			state = AnimState::IDLE;
		}
	}
	else if (state == AnimState::SECOND_ATTACK)
	{
		//root->anim->aniScale = 1.3;
		if (root->anim->currentAnimator.currentFrame == root->anim->playList[(int)AnimState::SECOND_ATTACK]->frameMax - 100)
		{
			root->anim->currentAnimator.currentFrame = 0;
			//root->anim->aniScale = 1.0f;
			thirdComboAttack = true;
			state = AnimState::WALK;
		}
	}
	else if (state == AnimState::THIRD_ATTACK)
	{
		//root->anim->aniScale = 1.7;
		if (root->anim->currentAnimator.currentFrame == root->anim->playList[(int)AnimState::THIRD_ATTACK]->frameMax - 40)
		{
			root->anim->currentAnimator.currentFrame = 0;
			//root->anim->aniScale = 1.0f;
			state = AnimState::WALK;
		}
	}
	else if (state == AnimState::DRINKING)
	{
		//root->anim->aniScale = 1.3;
		if (root->anim->currentAnimator.currentFrame == root->anim->playList[(int)AnimState::DRINKING]->frameMax - 8)
		{
			root->anim->currentAnimator.currentFrame = 0;
			//root->anim->aniScale = 1.0f;
			state = AnimState::WALK;
		}
	}
	else if (state == AnimState::STRONG_ATTACK)
	{
		//root->anim->aniScale = 1.3;
		if (root->anim->currentAnimator.currentFrame == root->anim->playList[(int)AnimState::STRONG_ATTACK]->frameMax - 2)
		{
			root->anim->currentAnimator.currentFrame = 0;
			//root->anim->aniScale = 1.0f;
			state = AnimState::WALK;
		}
	}
	else if (state == AnimState::LEFT_WALK or
			 state == AnimState::RIGHT_WALK or
			 state == AnimState::WALK_BACK)
	{
		WASD();
		ChangeRotation();
		speed = 10.0f;

		if (INPUT->KeyPress(VK_SPACE))
		{
			state = AnimState::RUN_FRONT;
			return;
		}

		//else if (INPUT->KeyPress('W') && !INPUT->KeyPress('A') && !INPUT->KeyPress('D'))
		//	state = AnimState::WALK;
		//else if (INPUT->KeyPress('A') && !INPUT->KeyPress('W') && !INPUT->KeyPress('S'))
		//	state = AnimState::LEFT_WALK;
		//else if (INPUT->KeyPress('D') && !INPUT->KeyPress('W') && !INPUT->KeyPress('S'))
		//	state = AnimState::RIGHT_WALK;
		//else if (INPUT->KeyPress('S') && !INPUT->KeyPress('A') && !INPUT->KeyPress('D'))
		//	state = AnimState::WALK_BACK;
		//else if (INPUT->KeyPress('W') && INPUT->KeyPress('D'))
		//	state = AnimState::RIGHT_WALK;
		//else if (INPUT->KeyPress('W') && INPUT->KeyPress('A'))
		//	state = AnimState::LEFT_WALK;
		//else if (INPUT->KeyPress('S') && INPUT->KeyPress('D'))
		//	state = AnimState::RIGHT_WALK;
		//else if (INPUT->KeyPress('S') && INPUT->KeyPress('A'))
		//	state = AnimState::LEFT_WALK;

		if (!camLock)
			state = AnimState::WALK;

		if (not INPUT->KeyPress('W') and
			not INPUT->KeyPress('A') and
			not INPUT->KeyPress('S') and
			not INPUT->KeyPress('D'))
			state = AnimState::IDLE;

		if (not isLanding)
			state = AnimState::JUMP_RUN;
	}
	else if (state == AnimState::SECOND_WEAPON_FIRST_ATTACK)
	{
		//root->anim->aniScale = 1.3;
		if (root->anim->currentAnimator.currentFrame == root->anim->playList[(int)AnimState::SECOND_WEAPON_FIRST_ATTACK]->frameMax - 10)
		{
			//root->anim->aniScale = 1.0;
			root->anim->currentAnimator.currentFrame = 0;
			secondComboAttack = true;
			state = AnimState::IDLE;
		}
	}
	else if (state == AnimState::SECOND_WEAPON_SECOND_ATTACK)
	{
		// root->anim->aniScale = 1.3;
		if (root->anim->currentAnimator.currentFrame == root->anim->playList[(int)AnimState::SECOND_WEAPON_SECOND_ATTACK]->frameMax - 40)
		{
			root->anim->currentAnimator.currentFrame = 0;
			//root->anim->aniScale = 1.0f;
			thirdComboAttack = true;
			state = AnimState::WALK;
		}
	}
	else if (state == AnimState::SECOND_WEAPON_THIRD_ATTACK)
	{
		//root->anim->aniScale = 1.7;
		if (root->anim->currentAnimator.currentFrame == root->anim->playList[(int)AnimState::SECOND_WEAPON_THIRD_ATTACK]->frameMax - 40)
		{
			root->anim->currentAnimator.currentFrame = 0;
			//root->anim->aniScale = 1.0f;
			state = AnimState::WALK;
		}
	}
	else if (state == AnimState::EQUIP)
	{
		//root->anim->aniScale = 1.7;
		//if (root->anim->currentAnimator.currentFrame == root->anim->playList[(int)AnimState::EQUIP]->frameMax - 35)
		if (root->anim->GetPlayTime() >= 0.5f && !isWeaponCange)
		{
			if (weapon == WeaponState::Sword)
			{
				cout << "무기교체 : Sword -> Axe" << endl;
				cout << "무기상태[0:Sword/1:Axe] : " << static_cast<int>(weapon) << endl;
				sword->visible = false;
				axe->visible = true;
				weapon = WeaponState::Axe;
				root->Find("AttackCol")->collider->scale = Vector3(50.0f, 40.0f, 5.0f); // 도끼콜라이더확장
				isWeaponCange = true;
				axe->Update();
			}

			else if (weapon == WeaponState::Axe)
			{
				cout << "무기교체 : Axe -> Sword" << endl;
				cout << "무기상태[0:Sword/1:Axe] : " << static_cast<int>(weapon) << endl;
				axe->visible = false;
				sword->visible = true;
				weapon = WeaponState::Sword;
				root->Find("AttackCol")->collider->scale = Vector3(40.0f, 5.0f, 5.0f); // 칼콜라이더축소
				isWeaponCange = true;
				sword->Update();
			}
		}

		//if (root->anim->currentAnimator.currentFrame == root->anim->playList[(int)AnimState::EQUIP]->frameMax - 2)
		if (root->anim->GetPlayTime() >= 1.0f)
		{
			//root->anim->aniScale = 1.0f;
			root->anim->currentAnimator.currentFrame = 0;
			state = AnimState::WALK;
		}
	}

	//cout << (int)weapon << endl;
	root->MoveWorldPos(inputDir * 2 * speed * DELTA);

	AnimationMachine();
	Trail();

	//if (lerpAfterRoll && lerpAmount < 10.0f)
	//{
	//	lerpAmount += DELTA /2;
	//	if (lerpAmount > 1.0f)
	//	{
	//		lerpAmount = 1.0f;
	//		lerpAfterRoll = false;
	//	}
	//
	//	float currentRotation = root->Find("Offset")->rotation.y;
	//	root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, root->Find("Socket")->rotation.y, lerpAmount, dist);
	//}

	root->Update();
	if (weapon == WeaponState::Sword)
		sword->Update();
	if (weapon == WeaponState::Axe)
		axe->Update();
	//lock->Update();
	//armor->Update();
	Lefthand->Update();

	inputDir = Vector3{};
	//Camera::main->Update();

}

void APlayer::LateUpdate()
{
	
}

void APlayer::RenderHierarchy()
{
	root->RenderHierarchy();
	sword->RenderHierarchy();
	axe->RenderHierarchy();
	//armor->RenderHierarchy();
	Lefthand->RenderHierarchy();
	//Cam->RenderHierarchy();
	//lock->RenderHierarchy();
	//trail->RenderDetail();
}

void APlayer::Render()
{
	//Camera::main->Set();

	//Camera::main->view = Matrix::CreateLookAt(Camera::main->GetWorldPos(),
	//	root->GetWorldPos()+Vector3(0,10,0), Vector3(0, 1, 0));
	////기본 프로젝션
	//Camera::main->proj = Matrix::CreatePerspectiveFieldOfView(
	//	Camera::main->fov, Camera::main->width / Camera::main->height, Camera::main->nearZ, Camera::main->farZ);
	//Camera::main->SetShadow();

	////&& !INPUT->KeyPress(VK_SPACE)

	//if (camLock)
	//{
	//	Vector3 dis = player->Find("CamLoc")->GetWorldPos() - Camera::main->GetWorldPos();
	//	Camera::main->MoveWorldPos(dis * 0.01f);
	//}


	root->Render();
	if (weapon == WeaponState::Sword)
		sword->Render();
	if (weapon == WeaponState::Axe)
		axe->Render();
	//armor->Render();
	Lefthand->Render();
	//lock->Render();
	trail->Render();
}

void APlayer::WASD()
{
	if (!camLock)
	{
		//if (INPUT->KeyPress('W') or
		//	INPUT->KeyPress('S') or 
		//	INPUT->KeyPress('A') or 
		//	INPUT->KeyPress('D'))
		//	inputDir -= root->Find("Offset")->GetForward() * FORWARD_SPEED * DELTA;
		if (INPUT->KeyPress('W'))
			inputDir -= root->Find("Offset")->GetForward() * FORWARD_SPEED * DELTA;
	}

	//else if (camLock)
	//{
	//	if (INPUT->KeyPress('D'))
	//	{
	//		inputDir -= root->Find("Socket")->GetRight() * FORWARD_SPEED * DELTA;
	//	}
	//	if (INPUT->KeyPress('A'))
	//	{
	//		inputDir += root->Find("Socket")->GetRight() * FORWARD_SPEED * DELTA;
	//	}
	//	if (INPUT->KeyPress('W'))
	//	{
	//		inputDir -= root->Find("Socket")->GetForward() * FORWARD_SPEED * DELTA;
	//	}
	//	if (INPUT->KeyPress('S'))
	//	{
	//		inputDir += root->Find("Socket")->GetForward() * FORWARD_SPEED * DELTA;
	//	}

	//	else if (!INPUT->KeyPress(VK_SPACE) &&
	//		 INPUT->KeyPress('W') or
	//		INPUT->KeyPress('A') or
	//		INPUT->KeyPress('S') or
	//		INPUT->KeyPress('D'))
	//		//root->Find("Offset")->rotation.y = root->Find("Socket")->rotation.y;
	//		root->Find("Offset")->rotation.y = root->Find("Campoint")->rotation.y;
	//}

	inputDir.Normalize();
}

void APlayer::OutOfRange()
{
	root->MoveWorldPos(-inputDir * speed * DELTA);

	root->Update();
	inputDir = Vector3();
}

void APlayer::OnFloor(Vector3 Hit)
{
	isLanding = true;
	gravity = 0.0f;
	root->SetWorldPosY(Hit.y);
	root->Update();
}

void APlayer::Release()
{
	//Cam->Release();
}

void APlayer::AnimationMachine()
{
	if (prevState != state)
	{
		if (state == AnimState::IDLE)
			root->anim->ChangeAnimation(AnimationState::LOOP, (int)AnimState::IDLE, 0.3);
		else if (state == AnimState::WALK)
		{
			root->anim->ChangeAnimation(AnimationState::LOOP, (int)AnimState::WALK, 0.1);
			speed = 10.0f;
		}
		else if (state == AnimState::RUN_FRONT)
		{
			root->anim->ChangeAnimation(AnimationState::LOOP, (int)AnimState::RUN_FRONT, 0.1);
			speed = 18.0f;
		}
		else if (state == AnimState::ROLL)
		{
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, (int)AnimState::ROLL, 0.1);
			roolInput = true;
		}
		else if (state == AnimState::JUMP_STAND)
		{
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, (int)AnimState::JUMP_STAND, 0.1);
			speed = 0.5f;
		}
		else if (state == AnimState::JUMP_RUN)
		{
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, (int)AnimState::JUMP_RUN, 0.1);
			speed = 0.5f;
		}
		else if (state == AnimState::NORMAL_ATTACK)
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, (int)AnimState::NORMAL_ATTACK, 0.1);
		else if (state == AnimState::SECOND_ATTACK)
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, (int)AnimState::SECOND_ATTACK, 0.1);
		else if (state == AnimState::THIRD_ATTACK)
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, (int)AnimState::THIRD_ATTACK, 0.1);
		else if (state == AnimState::DRINKING)
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, (int)AnimState::DRINKING, 0.1);
		else if (state == AnimState::STRONG_ATTACK)
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, (int)AnimState::STRONG_ATTACK, 0.1);
		else if (state == AnimState::LEFT_WALK)
			root->anim->ChangeAnimation(AnimationState::LOOP, (int)AnimState::LEFT_WALK, 0.1);
		else if (state == AnimState::RIGHT_WALK)
			root->anim->ChangeAnimation(AnimationState::LOOP, (int)AnimState::RIGHT_WALK, 0.1);
		else if (state == AnimState::WALK_BACK)
			root->anim->ChangeAnimation(AnimationState::LOOP, (int)AnimState::WALK_BACK, 0.1);
		else if (state == AnimState::BIG_SIDE_ATTACK)
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, (int)AnimState::BIG_SIDE_ATTACK, 0.1);
		else if (state == AnimState::SECOND_WEAPON_FIRST_ATTACK)
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, (int)AnimState::SECOND_WEAPON_FIRST_ATTACK, 0.1);
		else if (state == AnimState::SECOND_WEAPON_SECOND_ATTACK)
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, (int)AnimState::SECOND_WEAPON_SECOND_ATTACK, 0.1);
		else if (state == AnimState::SECOND_WEAPON_THIRD_ATTACK)
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, (int)AnimState::SECOND_WEAPON_THIRD_ATTACK, 0.1);
		else if (state == AnimState::SECOND_WEAPON_STRONG_ATTACK)
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, (int)AnimState::SECOND_WEAPON_STRONG_ATTACK, 0.1);
		else if (state == AnimState::EQUIP)
			root->anim->ChangeAnimation(AnimationState::ONCE_LAST, (int)AnimState::EQUIP, 0.1);

		prevState = state;
	}
}

void APlayer::ChangeRotation()
{
	//if (INPUT->KeyDown('S')) // S키누르면 뒤로돌기
	//	root->Find("Offset")->rotation.y *= -1;

	if (INPUT->KeyPress('D')) // D키누르면 회전값 서서히 증가
		root->Find("Offset")->rotation.y += 180 * TORADIAN * DELTA;
	if (INPUT->KeyPress('A')) // A키누르면 회전값 서서히 감소
		root->Find("Offset")->rotation.y -= 180 * TORADIAN * DELTA;


	//if (!camLock)
	//{
	//	if (state == AnimState::JUMP_RUN)
	//		rotationTimer += DELTA / 2;
	//	else
	//		rotationTimer += DELTA;

	//	float currentRotation = root->Find("Offset")->rotation.y;
	//	float camRotation = root->Find("Campoint")->rotation.y;
	//	//float camRotation = root->Find("Socket")->rotation.y;

	//	if (rotationTimer >= 1)
	//		rotationTimer = 0;

	//	if (rotationTimer <= 1)
	//	{
	//		if (INPUT->KeyPress('W'))
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation, rotationTimer, dist);
	//		if (INPUT->KeyPress('S')) 
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation + 180 * TORADIAN, rotationTimer, dist);
	//		if (INPUT->KeyPress('D'))
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation + 90 * TORADIAN, rotationTimer, dist);
	//		if (INPUT->KeyPress('A'))
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation - 90 * TORADIAN, rotationTimer, dist);

	//		if (INPUT->KeyPress('S') && INPUT->KeyPress('D'))
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation + 135 * TORADIAN, rotationTimer, dist);
	//		if (INPUT->KeyPress('S') && INPUT->KeyPress('A'))
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation - 135 * TORADIAN, rotationTimer, dist);
	//		if (INPUT->KeyPress('W') && INPUT->KeyPress('D'))
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation + 45 * TORADIAN, rotationTimer, dist);
	//		if (INPUT->KeyPress('W') && INPUT->KeyPress('A'))
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation - 45 * TORADIAN, rotationTimer, dist);

	//		if (INPUT->KeyDown('W') or
	//			INPUT->KeyDown('A') or
	//			INPUT->KeyDown('S') or
	//			INPUT->KeyDown('D'))
	//			rotationTimer = 0;
	//	}
	//}
	//else if ((camLock && INPUT->KeyPress(VK_SPACE)))
	//{
	//	if (state == AnimState::JUMP_RUN)
	//		rotationTimer += DELTA / 2;
	//	
	//	if (INPUT->KeyPress('W') or
	//		INPUT->KeyPress('A') or
	//		INPUT->KeyPress('S') or
	//		INPUT->KeyPress('D'))
	//	{
	//		rotationTimer += DELTA * 10;	
	//	}

	//	float currentRotation = root->Find("Offset")->rotation.y;
	//	float camRotation = root->Find("Campoint")->rotation.y;
	//	//float camRotation = root->Find("Socket")->rotation.y;

	//	if (rotationTimer >= 1)
	//		rotationTimer = 1;

	//	if (rotationTimer <= 1)
	//	{
	//		if (INPUT->KeyPress('W'))
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation, rotationTimer, dist);
	//		if (INPUT->KeyPress('S'))
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation + 180 * TORADIAN, rotationTimer, dist);
	//		if (INPUT->KeyPress('D'))
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation + 90 * TORADIAN, rotationTimer, dist);
	//		if (INPUT->KeyPress('A'))
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation - 90 * TORADIAN, rotationTimer, dist);

	//		if (INPUT->KeyPress('S') && INPUT->KeyPress('D'))
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation + 135 * TORADIAN, rotationTimer, dist);
	//		if (INPUT->KeyPress('S') && INPUT->KeyPress('A'))
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation - 135 * TORADIAN, rotationTimer, dist);
	//		if (INPUT->KeyPress('W') && INPUT->KeyPress('D'))
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation + 45 * TORADIAN, rotationTimer, dist);
	//		if (INPUT->KeyPress('W') && INPUT->KeyPress('A'))
	//			root->Find("Offset")->rotation.y = Util::LerpRotation(currentRotation, camRotation - 45 * TORADIAN, rotationTimer, dist);

	//		if (INPUT->KeyDown('W') or
	//			INPUT->KeyDown('A') or
	//			INPUT->KeyDown('S') or
	//			INPUT->KeyDown('D'))
	//			rotationTimer = 0;
	//	}
	//	Matrix R = Matrix::CreateLookAt(root->GetWorldPos(),
	//		lock->GetWorldPos(), Vector3(0, 1, 0));

	//	R = R.Transpose();

	//	Quaternion q = Quaternion::CreateFromRotationMatrix(R);
	//	Vector3 temp = Util::QuaternionToYawPtichRoll(q);
	//	//root->rotation = Util::QuaternionToYawPtichRoll(q);
	//	root->rotation.y = temp.y;
	//}
	//else if (camLock)
	//{
	//	Matrix R = Matrix::CreateLookAt(root->GetWorldPos(),
	//		lock->GetWorldPos(), Vector3(0, 1, 0));

	//	R = R.Transpose();

	//	Quaternion q = Quaternion::CreateFromRotationMatrix(R);
	//	Vector3 temp = Util::QuaternionToYawPtichRoll(q);
	//	//root->rotation = Util::QuaternionToYawPtichRoll(q);
	//	root->rotation.y = temp.y;
	//}

}

void APlayer::Trail()
{
	trail->Update();
	if (state == AnimState::NORMAL_ATTACK or
		state == AnimState::SECOND_ATTACK or
		state == AnimState::THIRD_ATTACK or
		state == AnimState::STRONG_ATTACK or
		state == AnimState::BIG_SIDE_ATTACK or
		state == AnimState::SECOND_WEAPON_FIRST_ATTACK or
		state == AnimState::SECOND_WEAPON_SECOND_ATTACK or
		state == AnimState::SECOND_WEAPON_THIRD_ATTACK or
		state == AnimState::SECOND_WEAPON_STRONG_ATTACK)
	{
		trail->Bottom = root->Find("BottomTrail");
		trail->Top = root->Find("TopTrail");
		trailEffect = true;
	}
	else
		trailEffect = false;


	if (trailEffect)
	{
		if (not trail->isPlaying)
		{
			if (root->anim->GetPlayTime() > 0.3 and root->anim->GetPlayTime() < 0.7)
				trail->Play();
		}
		else
		{
			if (root->anim->GetPlayTime() > 0.7)
				trail->Stop();
		}
	}
	if (!trailEffect)
		trail->Stop();
}

void APlayer::HealHP(int temp)
{
	hp += temp;
	if (hp >= maxhp) hp = maxhp;
}

void APlayer::HealMP(int temp)
{
	mp += temp;
	if (mp >= maxmp) mp = maxmp;
}

void APlayer::Set_Rhandfile(string file)
{
	sword->LoadFile(file);
}

void APlayer::Set_Lhandfile(string file)
{
	Lefthand->LoadFile(file);
}

void APlayer::SetArmorfile(string file)
{
	armor->LoadFile(file);
}

void APlayer::CollOn()
{
	if (!root->Find("AttackCol")->collider->colOnOff) // 콜라이더가 꺼져있다면
	{
		if (TIMER->GetTick(collonzerotime, 1.0f))
		{
			//cout << "플레이어 공격용 콜라이더 ON" << endl;
			root->Find("AttackCol")->collider->colOnOff = true;
		}
	}
	if (!root->Find("mixamorig:Hips")->collider->colOnOff) // 콜라이더가 꺼져있다면
	{
		if (TIMER->GetTick(collonzerotime2, 1.0f))
		{
			//cout << "플레이어 피격용 콜라이더 ON" << endl;
			root->Find("mixamorig:Hips")->collider->colOnOff = true;
		}
	}

}

void APlayer::GetDameged(int damage)
{
	root->Find("mixamorig:Hips")->collider->colOnOff = false;
	hp -= damage;
	isDameged = true;
}

void APlayer::SetPower()
{
	// 공격모션마다 속도&파워(공격력)조절
	// 별도모션에 속도를 조절하려면 case를 추가한뒤 설정
	switch (root->anim->GetPlayNum())
	{
	case 2: // STRONG_ATTACK
	{
		root->anim->aniScale = middle_speed;
		power = 8;
	}
		break;
	case 7: // NORMAL_ATTACK
	{
		root->anim->aniScale = middle_speed;
		power = 5;
	}
		break;
	case 20: // BIG_SIDE_ATTACK
	{
		root->anim->aniScale = middle_speed;
		power = 10;
	}
		break;
	case 28: // SECOND_ATTACK
	{
		root->anim->aniScale = high_speed;
		power = 5;
	}
		break;
	case 29: // THIRD_ATTACK (특히느려서 좀더 빠르게조절)
	{
		root->anim->aniScale = high_speed + 1.0f;
		power = 5;
	}
		break;
	case 31: // JUMP_ATTACK
	{
		root->anim->aniScale = middle_speed;
		power = 5;
	}
		break;
	case 32: // SECOND_WEAPON_FIRST_ATTACK (도끼)
	{
		root->anim->aniScale = middle_speed;
		power = 10;
	}
		break;
	case 33: // SECOND_WEAPON_SECOND_ATTACK (도끼)
	{
		root->anim->aniScale = middle_speed;
		power = 6;
	}
		break;
	case 34: // SECOND_WEAPON_THIRD_ATTACK (도끼)
	{
		root->anim->aniScale = middle_speed;
		power = 10;
	}
		break;
	case 35: // SECOND_WEAPON_STRONG_ATTACK (도끼)
	{
		root->anim->aniScale = middle_speed;
		power = 15;
	}
		break;
	default: // 공격모션이 아닌 모든 애니매이션은 default로 속도는 1이다.
		root->anim->aniScale = low_speed;
		break;
	}
}

void APlayer::SetAtkCol()
{
	if (root->anim->GetPlayNum() == 2 ||	// STRONG_ATTACK
		root->anim->GetPlayNum() == 7 ||	// NORMAL_ATTACK
		root->anim->GetPlayNum() == 20 ||	// BIG_SIDE_ATTACK
		root->anim->GetPlayNum() == 28 ||	// SECOND_ATTACK
		root->anim->GetPlayNum() == 29 ||	// THIRD_ATTACK
		root->anim->GetPlayNum() == 31 ||	// JUMP_ATTACK
		root->anim->GetPlayNum() == 32 ||	// SECOND_WEAPON_FIRST_ATTACK
		root->anim->GetPlayNum() == 33 ||	// SECOND_WEAPON_SECOND_ATTACK
		root->anim->GetPlayNum() == 34 ||	// SECOND_WEAPON_THIRD_ATTACK
		root->anim->GetPlayNum() == 35		// SECOND_WEAPON_STRONG_ATTACK
		)
	{
		if (root->anim->GetPlayTime() > 0.5 &&
			root->anim->GetPlayTime() < 0.7)
			root->Find("AttackCol")->collider->colOnOff = true;
	}
	else
		root->Find("AttackCol")->collider->colOnOff = false;
}
