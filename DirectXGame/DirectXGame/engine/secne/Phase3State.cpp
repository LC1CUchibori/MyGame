#include "Phase3State.h"
#include "GameScene.h"

void Phase3State::Enter(GameScene* scene)
{
	scene->GetEnemy()->SetMaxDashCount(3);
}

void Phase3State::Update(GameScene*)
{
}

