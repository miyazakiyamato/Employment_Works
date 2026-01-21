#include "GameSceneStateStart.h"
#include "GameScene.h"
#include "TimeManager.h"
#include "GameStartUI.h"
#include "CameraManager.h"
#include "GameSceneStateBattle.h" 

GameSceneStateStart::~GameSceneStateStart() {
	if (gameStartUI_) {
		gameStartUI_->SetIsDead(true);
	}
}

void GameSceneStateStart::Initialize(GameScene* gameScene) {
	BaseSceneState::Initialize(gameScene);

	// Game Start UI
	std::unique_ptr<GameStartUI> newUI = std::make_unique<GameStartUI>();
	newUI->Initialize(duration_);
	gameStartUI_ = newUI.get();
	gameScene_->GetUIList().push_back(std::move(newUI));

	TimeManager::GetInstance()->SetDeltaTimeSpeedStart(0.0f, duration_ * 3.0f);
}

void GameSceneStateStart::Update() {
	auto stageManager = gameScene_->GetStageManager();
	auto bulletManager = gameScene_->GetBulletManager();
	auto particleSystem = gameScene_->GetParticleSystem();
	auto collisionManager = gameScene_->GetCollisionManager();
	auto& uiList = gameScene_->GetUIList();
	auto player = stageManager->GetPlayer();

	//ステージ
	stageManager->Update();

	bulletManager->Update();

	//当たり判定
	//衝突マネージャのリストクリアする
	collisionManager->Reset();
	//全てのコライダーを衝突マネージャのリストに登録する
	stageManager->SetStageCollisions(collisionManager);
	bulletManager->AddCollider(collisionManager);
	//リスト内の総当たり判定
	collisionManager->CheckAllCollisions();

	collisionManager->UpdateWorldTransform();

	particleSystem->Update();

	for (auto& ui : uiList) {
		ui->Update();
	}
	
	if (gameStartUI_->ShouldClose()) {
		// Battleへ遷移
		gameScene_->ChangeState(std::make_unique<GameSceneStateBattle>());
	}
}

void GameSceneStateStart::Draw() {
	auto stageManager = gameScene_->GetStageManager();
	auto bulletManager = gameScene_->GetBulletManager();
	auto particleSystem = gameScene_->GetParticleSystem();
	auto collisionManager = gameScene_->GetCollisionManager();
	auto& uiList = gameScene_->GetUIList();

	//Object3dの描画
	//ステージ
	stageManager->Draw();
	bulletManager->Draw();

	//当たり判定の表示
	collisionManager->Draw();

	//ラインの描画
	//Line3dManager::GetInstance()->Draw();

	//Particleの描画
	particleSystem->Draw();

	//Spriteの描画
	for (auto& ui : uiList) {
		ui->Draw();
	}
}
