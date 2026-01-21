#include "GameSceneStateDeath.h"
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

void GameSceneStateDeath::Initialize(GameScene* gameScene) {
	BaseSceneState::Initialize(gameScene);

	// Player Death UI
	playerDeathUI_ = std::make_unique<PlayerDeathUI>();
	playerDeathUI_->Initialize();
}

void GameSceneStateDeath::Update() {
	auto stageManager = gameScene_->GetStageManager();
	auto bulletManager = gameScene_->GetBulletManager();
	auto particleSystem = gameScene_->GetParticleSystem();
	auto& uiList = gameScene_->GetUIList();
	auto player = gameScene_->GetStageManager()->GetPlayer();

	//死んだ時の処理
	counter_ += TimeManager::GetInstance()->kFlamTime_;

	if (counter_ < duration_) {
		float scale = Easing::EaseOutBounce(counter_ / duration_, 1.0f, 0.0f);
		player->GetObject3d()->SetScale({ scale,scale ,scale });
		
		if (particleSystem->FindEmitter("hitEffect")) {
			static_cast<EmitterSphere*>(particleSystem->FindEmitter("hitEffect"))->SetTranslate(player->GetObject3d()->GetCenterPosition());
			if (int(scale * 100.0f) % 5 < 1 && scale > 0.1f) {
				particleSystem->Emit("hitEffect");
			}
		}
	}
	else {
		// 次のシーンへ
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
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
	playerDeathUI_->Update();
}

void GameSceneStateDeath::Draw() {
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
	playerDeathUI_->Draw();
}
