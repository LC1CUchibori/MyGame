#pragma once
#include "PhaseState.h"

class Phase2State : public PhaseState {
public:
	void Enter(GameScene* scene) override;
	void Update(GameScene* scene) override;
};