#pragma once
#include <vector>
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "ParticleEmitter.h"
#include "CollisionManager.h"
#include "AccelerationField.h"
#include "Skydome.h"
#include "Ground.h"

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
private://メンバ変数
	//衝突マネージャ
	std::unique_ptr<CollisionManager> collisionManager_;

	bool isAccelerationField = false;
	std::unique_ptr<AccelerationField> accelerationField_ = nullptr;

	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//地面
	std::unique_ptr<Ground> ground_ = nullptr;

	std::vector<std::unique_ptr<Sprite>> sprites_;
	std::unique_ptr<ParticleEmitter> particleEmitter_ = nullptr;
};

