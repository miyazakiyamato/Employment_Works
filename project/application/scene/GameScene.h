#pragma once
#include <vector>
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "ParticleSystem.h"
#include "CollisionManager.h"
#include "AccelerationField.h"
#include "StageManager.h"
#include "Player.h"
#include "BulletManager.h"
#include "BaseEnemy.h"
#include "RailCamera.h"
#include "HpUI.h"

class GameScene : public BaseScene {
public://メンバ関数
	//初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	//毎フレーム更新
	void Update() override;
	//描画
	void Draw() override;

private:
	//衝突判定と応答 
	void CheckAllCollisions();
	//クリア判定
	void ClearCheck();
private://メンバ変数
	//衝突マネージャ
	std::unique_ptr<CollisionManager> collisionManager_;

	//ステージマネージャ
	std::unique_ptr<StageManager> stageManager_;
	//プレイヤー
	Player* player_ = nullptr;
	//HPのUI
	std::unique_ptr<HpUI> hpUI_ = nullptr;

	//スプライト
	std::vector< std::unique_ptr<Sprite>> sprites_;
	//パーティクルシステム
	std::unique_ptr<ParticleSystem> particleSystem_ = nullptr;
	//バレットマネージャー
	std::unique_ptr<BulletManager> bulletManager_ = nullptr;
	public:
		ParticleSystem* GetParticleSystem() { return particleSystem_.get(); }
};

