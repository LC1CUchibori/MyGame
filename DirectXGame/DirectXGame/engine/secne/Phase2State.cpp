#include "Phase2State.h"
#include "GameScene.h"

void Phase2State::Enter(GameScene* scene)
{
	scene->GetEnemy()->SetMaxDashCount(2);
}

void Phase2State::Update(GameScene* )
{
}

