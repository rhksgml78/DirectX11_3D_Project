#include "stdafx.h"
#include "Player.h"

void Player::Init()
{
    root = Actor::Create();
    //root->LoadFile("player_temp.xml");
    root->LoadFile("player_temp.xml");
    dead = (Actor*)root->Find("RootNode");

    Cam = (Camera*)root->Find("Cam");
    Camera::main = Cam;
    iscontrol = false;
    /*
    플레이어 ( 카메라 넣기 필수 )
    0 - 기본
    1 - 뛰기
    2 - 걷기
    3 - 뒤로 걷기
    4 - 위협
    5 - 기어다니기
    6 - 매달려있기
    7 - 앉아있기
    8 - 벽에머리박기
    */

    state = PlayerState::IDLE;
    isLanding = true;
    speed = 20.0f;
    gravity = 0.0f;
    root->anim->ChangeAnimation(AnimationState::LOOP, 0);

    SOUND->AddSound("playerrun2.wav", "MOVE", true);
}

void Player::Update()
{
    if (state != PlayerState::DIE and iscontrol)
    {
        if (INPUT->KeyPress(VK_RBUTTON))
        {
            Vector3 Rot;
            Rot.x = INPUT->movePosition.y * 0.001f;
            Rot.y = INPUT->movePosition.x * 0.001f;
            root->rotation.y += Rot.y;
        }

    }

    lastPos = root->GetWorldPos();

    if (state == PlayerState::IDLE and iscontrol)
    {

        inputDir = Vector3();


        if (INPUT->KeyDown(VK_SPACE))
        {
            gravity = -10.0f;
            isLanding = false;
        }

        //idle to walk
        if (INPUT->KeyDown('W'))
        {
            SOUND->Play("MOVE");

            state = PlayerState::WALK;
            root->anim->ChangeAnimation(AnimationState::LOOP, 2);
        }
        if (INPUT->KeyDown('S'))
        {
            SOUND->Play("MOVE");

            state = PlayerState::BACKWALK;
            root->anim->ChangeAnimation(AnimationState::LOOP, 3);
        }
        //idle to jump
        /*if (not isLanding)
        {
            state = DeadState::JUMP;
        }*/
    }
    else if (state == PlayerState::WALK and iscontrol)
    {
        if (iscontrol)
        {
            WASD();
        }

        //walk to idle
        if (not INPUT->KeyPress('W'))
        {
            SOUND->Stop("MOVE");
            state = PlayerState::IDLE;
            root->anim->ChangeAnimation(AnimationState::LOOP, 0);
        }
        //walk to jump
        /*if (not isLanding)
        {
            state = DeadState::JUMP;
        }*/

    }
    else if (state == PlayerState::BACKWALK and iscontrol)
    {
        //SOUND->Play("MOVE");

        if (iscontrol)
        {
            WASD();
        }

        if (not INPUT->KeyPress('S'))
        {
            SOUND->Stop("MOVE");
            state = PlayerState::IDLE;
            root->anim->ChangeAnimation(AnimationState::LOOP, 0);
        }
    }
    if (state == PlayerState::ENDINGMOVE and !iscontrol)
    {
        root->anim->ChangeAnimation(AnimationState::LOOP, 2);
    }
    if (state == PlayerState::ENDINGROAR and !iscontrol)
    {
        root->anim->ChangeAnimation(AnimationState::ONCE_LAST, 4);
    }


    if (state != PlayerState::DIE and iscontrol)
    {
        if (INPUT->KeyPress('Q'))
        {
            root->rotation.y -= 90.0f * TORADIAN * DELTA;
        }
        else if (INPUT->KeyPress('E'))
        {
            root->rotation.y += 90.0f * TORADIAN * DELTA;
        }
    }
    else if (state == PlayerState::DIE and iscontrol)
    {
        //SOUND->Stop("MOVE");

        if (root->Find("Cam")->rotation.x < 60 * TORADIAN)
        {
            root->Find("Cam")->rotation.x += 15 * TORADIAN * DELTA;
        }
    }

    //else if (state == DeadState::JUMP)
    //{
    //    //WASD();
    //  

    //    //jump to idle
    //    if (isLanding)
    //    {
    //        state = DeadState::IDLE;
    //    }

    //}
    //Cam->ControlMainCam();

   /* if (not isLanding)
    {
        gravity += 10.0f * DELTA;
    }*/

    if (state == PlayerState::WALK and iscontrol)
    {
        root->MoveWorldPos(inputDir * speed * DELTA);
    }
    else if (state == PlayerState::BACKWALK and iscontrol)
    {
        root->MoveWorldPos(inputDir * speed * 0.3f * DELTA);
    }

    //oot->MoveWorldPos(-UP * gravity * DELTA);


    root->Update();
}

void Player::Hi()
{
    root->RenderHierarchy();
}

void Player::Render()
{
    root->Render();
}

void Player::WASD()
{
    inputDir = Vector3();

    if (INPUT->KeyPress('W'))
    {
        inputDir += root->GetForward();
    }
    else if (INPUT->KeyPress('S'))
    {
        inputDir += -root->GetForward();
    }
    /* inputDir = Vector3();

     if (INPUT->KeyPress('W'))
     {
         inputDir += root->GetForward();
     }
     else if (INPUT->KeyPress('S'))
     {
         inputDir += -root->GetForward();
     }
     if (INPUT->KeyPress('A'))
     {
         inputDir += -root->GetRight();
     }
     else if (INPUT->KeyPress('D'))
     {
         inputDir += root->GetRight();
     }*/
     /*if (INPUT->KeyDown(VK_SPACE))
     {
         gravity = -10.0f;
         isLanding = false;
     }*/
}

void Player::OutOfRange()
{
    //원래 입력 반대방향으로 움직이기
    root->MoveWorldPos(-inputDir * speed * 1.5f * DELTA);

    root->Update();
    inputDir = Vector3();


}

void Player::OnFloor(Vector3 Hit)
{
    isLanding = true;
    gravity = 0.0f;
    root->SetWorldPosY(Hit.y);
    root->Update();
}
