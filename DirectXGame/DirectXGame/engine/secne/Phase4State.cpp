#include "Phase4State.h"
#include "GameScene.h"

void Phase4State::Enter(GameScene* scene)
{
	scene->GetEnemy()->SetMaxDashCount(4);
}

void Phase4State::Update(GameScene*)
{
}
