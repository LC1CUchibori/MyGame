#include "Phase1State.h"
#include "GameScene.h"

void Phase1State::Enter(GameScene* scene)
{
	scene->GetEnemy()->SetMaxDashCount(1);
}

void Phase1State::Update(GameScene*)
{
}

