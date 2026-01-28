#include "PauseStateSelect.h"
#include "PauseScene.h"
#include "PauseStateEnd.h"
#include "Input.h"

void PauseStateSelect::Initialize(PauseScene* scene) {
	BaseSceneState<PauseScene>::Initialize(scene);
}

void PauseStateSelect::Update() {
	auto input = Input::GetInstance();
	int currentSelection = scene_->GetSelectionIndex();

	// Navigation
	if (input->TriggerKey(DIK_UP) || input->TriggerKey(DIK_W) || input->TriggerControllerButton(XINPUT_GAMEPAD_DPAD_UP)) {
		currentSelection--;
		if (currentSelection < 0) currentSelection = 1;
		scene_->SetSelectionIndex(currentSelection);
	}
	else if (input->TriggerKey(DIK_DOWN) || input->TriggerKey(DIK_S) || input->TriggerControllerButton(XINPUT_GAMEPAD_DPAD_DOWN)) {
		currentSelection++;
		if (currentSelection > 1) currentSelection = 0;
		scene_->SetSelectionIndex(currentSelection);
	}

	// Action
	if (input->TriggerKey(DIK_SPACE) || input->TriggerControllerButton(XINPUT_GAMEPAD_A)) {
		if (currentSelection == 0) {
			// Resume (keepPlaying)
			scene_->ChangeState(std::make_unique<PauseStateEnd>(0));
			return;
		}
		else if (currentSelection == 1) {
			// Title (backToTitle)
			scene_->ChangeState(std::make_unique<PauseStateEnd>(2));
			return;
		}
	}

	// Update Sprites
	auto bgSprite = scene_->GetBgSprite();
	if (bgSprite) {
		bgSprite->SetColor({ 0.0f, 0.0f, 0.0f, 0.5f });
		bgSprite->SetSize({ 1280.0f, 720.0f });
		bgSprite->Update();
	}

	auto pauseTitle = scene_->GetPauseTitleSprite();
	if (pauseTitle) {
		pauseTitle->SetSize({ 400.0f, 100.0f }); // 4:1 ratio
		pauseTitle->Update();
	}
	
	// Pulsing Animation Calculation
	static float pulseTimer = 0.0f;
	pulseTimer += 1.0f / 60.0f; // Approx increment per frame
	float pulseScale = 1.0f + 0.1f * std::sin(pulseTimer * 5.0f); // +/- 10% scale

	const auto& buttons = scene_->GetButtonSprites();
	for (int i = 0; i < buttons.size(); ++i) {
		float baseW = 240.0f;
		float baseH = 50.0f;
		
		if (i == currentSelection) {
			// Apply pulsing to base size
			// 240 * (1.1 ~ 0.9) approx range 264 ~ 216
			buttons[i]->SetSize({ baseW * pulseScale, baseH * pulseScale });
		} else {
			buttons[i]->SetSize({ baseW, baseH });
		}
		buttons[i]->Update();
	}
}
