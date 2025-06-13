#pragma once
#include <vector>
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "ParticleSystem.h"
#include "CollisionManager.h"
#include "AccelerationField.h"
#include "Skydome.h"
#include "Ground.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"

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

	void AddPlayerBullet(std::unique_ptr<PlayerBullet> playerBullet);

private:
	//衝突判定と応答 
	void CheckAllCollisions();
private://メンバ変数
	//衝突マネージャ
	std::unique_ptr<CollisionManager> collisionManager_;

	bool isAccelerationField = false;
	std::unique_ptr<AccelerationField> accelerationField_ = nullptr;

	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//地面
	std::unique_ptr<Ground> ground_ = nullptr;
	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	std::list<std::unique_ptr<PlayerBullet>> playerBullets_;
	//エネミー
	std::vector<std::unique_ptr<Enemy>> enemies_;
	std::vector< std::unique_ptr<Sprite>> sprites_;
	std::unique_ptr<ParticleSystem> particleSystem_ = nullptr;
};

