#include "BossPhaseState.h"
#include "GameScene.h"

void BossPhaseState::Enter(GameScene* scene)
{
	scene->GetEnemy()->SetMaxDashCount(5);
}

void BossPhaseState::Update(GameScene* scene)
{
	scene->LastPhase();
}