#pragma once
#include "../2DFrameWork/framework.h"
#pragma comment (lib,"../Build/2DFrameWork/2DFrameWork.lib")

extern PostEffect* post;
#define MONSTERNUM 3

//Observer Pattern
#include "IObserver.h"
#include "ISubject.h"
//Subject
#include "AttackSubject.h"
#include "CollisionSubject.h"
//Observer
#include "AttackObserver.h"
#include "CollisionObserver.h"

//Map
#include "Map.h"

// item & inventory
#include "GItem.h"
#include "Item.h"
#include "GameUI.h"

// BTCommand
#include "Command.h"

// BlackBoard
#include "BlackBoard.h"

#include "CommandIntro.h"
#include "CommandAttack.h"
#include "CommandChase.h"
#include "CommandHit.h"
#include "CommandJump.h"
#include "CommandMove.h"
#include "CommandSearch.h"


// Node
#include "BTNode.h"
#include "BTIntroNode.h"
#include "BTAttackNode.h"
#include "BTSearchNode.h"
#include "BTChaseNode.h"
#include "BTMoveNode.h"
#include "BTJumpNode.h"


// BTBranch
#include "BTSelectorNode.h"
#include "BTSequnceNode.h"
#include "BTDecoratorNode.h"

// Behavior Tree
#include "BehaviorTree.h"

// Actor
#include "APlayer.h"

// Monster Skill
#include "RocksEffect.h" // Boss Monster
#include "RocksEffectVt.h" // Boss Monster


// Monster
#include "Monster.h"
#include "MonsterBoss.h"
#include "MonsterType1.h"
#include "MonsterType2.h"



// Scene
#include "LoadingScene.h"
#include "Scene2.h"
#include "BossScene.h"
#include "SKH_TestScene.h"
#include "StudyScene.h"
#include "DeferredTestScene.h"




