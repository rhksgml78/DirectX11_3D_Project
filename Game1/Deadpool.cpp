#include "stdafx.h"
#include "Deadpool.h"

void Deadpool::Init()
{
    root = Actor::Create();
    root->LoadFile("Normal.xml");
    dead = (Actor*)root->Find("RootNode");
    //Cam = (Camera*)root->Find("Camera");
    Camera::main = Cam;
    state = DeadState::IDLE;
    isLanding = true;
    speed = 10.0f;
    gravity = 0.0f;
    root->anim->ChangeAnimation(AnimationState::LOOP, 0);

    /*static int count = 0;
    string key = "coin";
    key += to_string(count);
    SOUND->AddSound("coin.wav", key.c_str());
    count++;*/
}

void Deadpool::Update()
{
    if (INPUT->KeyPress(VK_RBUTTON))
    {
        Vector3 Rot;
        Rot.x = INPUT->movePosition.y * 0.001f;
        Rot.y = INPUT->movePosition.x * 0.001f;
        root->rotation.y += Rot.y;
    }
    
    
    lastPos = root->GetWorldPos();

    if (state == DeadState::IDLE)
    {
        inputDir = Vector3();


        if (INPUT->KeyDown(VK_SPACE))
        {
            gravity = -10.0f;
            isLanding = false;
        }

        //idle to walk
        if (INPUT->KeyDown('W') or
            INPUT->KeyDown('A') or
            INPUT->KeyDown('S') or
            INPUT->KeyDown('D'))
        {
            state = DeadState::WALK;
            root->anim->ChangeAnimation(AnimationState::LOOP, 1);
        }
        //idle to jump
        /*if (not isLanding)
        {
            state = DeadState::JUMP;
        }*/
    }
    else if (state == DeadState::WALK)
    {
        WASD();


        //walk to idle
        if (not INPUT->KeyPress('W') and
            not INPUT->KeyPress('A') and
            not INPUT->KeyPress('S') and
            not INPUT->KeyPress('D'))
        {
            state = DeadState::IDLE;
            root->anim->ChangeAnimation(AnimationState::LOOP, 0);
        }
        //walk to jump
        /*if (not isLanding)
        {
            state = DeadState::JUMP;
        }*/
       
    }
    else if (state == DeadState::JUMP)
    {
        //WASD();
      

        //jump to idle
        if (isLanding)
        {
            state = DeadState::IDLE;
        }

    }
    //Cam->ControlMainCam();

   /* if (not isLanding)
    {
        gravity += 10.0f * DELTA;
    }*/


    root->MoveWorldPos(inputDir * speed * DELTA);
    //oot->MoveWorldPos(-UP * gravity * DELTA);


    root->Update();
}

void Deadpool::Hi()
{
    root->RenderHierarchy();
    //dead->RenderHierarchy();
}

void Deadpool::Render()
{
    root->Render();
}

void Deadpool::WASD()
{
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

void Deadpool::OutOfRange()
{
    //원래 입력 반대방향으로 움직이기
    root->MoveWorldPos(-inputDir * speed * DELTA);

    root->Update();
    inputDir = Vector3();


}

void Deadpool::OnFloor(Vector3 Hit)
{
    isLanding = true;
    gravity = 0.0f;
    root->SetWorldPosY(Hit.y);
    root->Update();
}
