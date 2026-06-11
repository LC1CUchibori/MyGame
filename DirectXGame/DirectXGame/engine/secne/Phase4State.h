#pragma once
#include "PhaseState.h"

class Phase4State : public PhaseState {
public:
	void Enter(GameScene* scene) override;
	void Update(GameScene* scene) override;
};