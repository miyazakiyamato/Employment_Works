#pragma once
#include "BaseCharacter.h"
//#include "LockOn.h"
#include "ContactRecord.h"
#include "Sprite.h"

class Input;
class Camera;
class GameScene;
class Player : public BaseCharacter{
public://メンバ関数
	Player() {}

	~Player() {}
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	//衝突を検知したら呼び出されるコールバック関数
	void OnCollision([[maybe_unused]] Collider* other) override;
private://ローカル関数
	void Move();
	void CameraMove();
	void Attack();
private://メンバ変数
	Input* input_ = nullptr;
	Camera* camera_ = nullptr;
	GameScene* gameScene_ = nullptr;
	//モデルオブジェクト
	std::unique_ptr<Object3d> model_ = nullptr;
	//3Dレティクル
	std::unique_ptr<Object3d> reticle3D_ = nullptr;
	std::unique_ptr<Sprite> reticle2D_ = nullptr;

	Vector3 velocity_{};
	float moveSpeed_ = 0.5f;

	float destinationAngleX_ = 0.0f;
	float destinationAngleY_ = 0.0f;

	bool isCollision = false;

	int hp_ = 5;

	//ロックオン
	//LockOn* lockOn_ = nullptr;
public://ゲッターセッター
	//void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }
	Vector3 GetWorldPosition();
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	int GetHp() { return hp_; }
};

