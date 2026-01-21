#include "GameSceneStateBattle.h"
#include "GameScene.h"
#include "StageManager.h"
#include "BulletManager.h"
#include "ParticleSystem.h"
#include "CollisionManager.h"
#include "Line3D.h"
#include "BaseUI.h"
#include "SceneManager.h"
#include "GameSceneStateWin.h"
#include "GameSceneStateDeath.h"
#include "PlayerStateLeave.h"
#include "TextureManager.h"

void GameSceneStateBattle::Initialize(GameScene* gameScene) {
	BaseSceneState::Initialize(gameScene);
}

void GameSceneStateBattle::Update() {
	auto stageManager = gameScene_->GetStageManager();
	auto bulletManager = gameScene_->GetBulletManager();
	auto particleSystem = gameScene_->GetParticleSystem();
	auto collisionManager = gameScene_->GetCollisionManager();
	auto& uiList = gameScene_->GetUIList();
	auto player = stageManager->GetPlayer();
	auto sceneManager = SceneManager::GetInstance();

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

	//クリアチェック
	if (!player->GetIsAlive()) {
		// GAMEOVER遷移 -> StateDeath
		gameScene_->ChangeState(std::make_unique<GameSceneStateDeath>());
	}
	else if (stageManager->GetRailCamera()->GetIsFinished()) {
		// CLEAR遷移 -> StateWin
		player->ChangeState(std::make_unique<PlayerStateLeave>(player));
		gameScene_->ChangeState(std::make_unique<GameSceneStateWin>());
	}
}

void GameSceneStateBattle::Draw() {
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
	Line3dManager::GetInstance()->Draw();

	//Particleの描画
	particleSystem->Draw();

	//Spriteの描画
	for (auto& ui : uiList) {
		ui->Draw();
	}
}
