#include "GameSceneStateWin.h"
#include "GameScene.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "StageManager.h"
#include "BulletManager.h"
#include "CollisionManager.h"
#include "ParticleSystem.h"
#include "Line3D.h"
#include "Easing.h"
#include "EmitterSphere.h"

GameSceneStateWin::~GameSceneStateWin() {
	if (playerWinUI_) {
		playerWinUI_->SetIsDead(true);
	}
}

void GameSceneStateWin::Initialize(GameScene* gameScene) {
	BaseSceneState::Initialize(gameScene);

	// Player Win UI
	std::unique_ptr<PlayerWinUI> newUI = std::make_unique<PlayerWinUI>();
	newUI->Initialize();
	playerWinUI_ = newUI.get();
	gameScene_->GetUIList().push_back(std::move(newUI));

	auto particleSystem = gameScene_->GetParticleSystem();
	auto player = gameScene_->GetStageManager()->GetPlayer();
	float scale = Easing::EaseOutBounce(counter_ / duration_, 1.0f, 0.0f);

	// エフェクト処理
	if (particleSystem->FindEmitter("emitterHit")) {
		static_cast<EmitterSphere*>(particleSystem->FindEmitter("emitterHit"))->SetTranslate(player->GetObject3d()->GetCenterPosition());
		if (scale >= 1.0f) {
			particleSystem->Emit("emitterHit");
		}
	}
	// カメラ追従停止
	if (player) {
		player->StopCameraFollow();
	}
}

void GameSceneStateWin::Update() {
	auto stageManager = gameScene_->GetStageManager();
	auto bulletManager = gameScene_->GetBulletManager();
	auto particleSystem = gameScene_->GetParticleSystem();
	auto& uiList = gameScene_->GetUIList();
	auto player = gameScene_->GetStageManager()->GetPlayer();
	//勝った時の処理
	counter_ += TimeManager::GetInstance()->kFlamTime_;

	if (counter_ < duration_) {

	}
	else {
		// 次のシーンへ
		SceneManager::GetInstance()->ChangeScene("CLEAR");
		SceneManager::GetInstance()->ChangeTransition("FADE");
	}
	//ステージ
	stageManager->Update();
	//弾管理者
	bulletManager->Update();
	//パーティクルシステム
	particleSystem->Update();

	// UIの更新
	for (auto& ui : uiList) {
		ui->Update();
	}
}

void GameSceneStateWin::Draw() {
	// 背景描画
	auto stageManager = gameScene_->GetStageManager();
	auto bulletManager = gameScene_->GetBulletManager();
	auto particleSystem = gameScene_->GetParticleSystem();
	auto& uiList = gameScene_->GetUIList();

	stageManager->Draw();
	bulletManager->Draw();
	particleSystem->Draw();

	// UI描画
	for (auto& ui : uiList) {
		ui->Draw();
	}
}
