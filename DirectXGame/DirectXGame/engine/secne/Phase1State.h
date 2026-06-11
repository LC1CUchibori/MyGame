#pragma once
#include "PhaseState.h"

class Phase1State : public PhaseState {
public:
	void Enter(GameScene* scene) override;
	void Update(GameScene* scene) override;
};