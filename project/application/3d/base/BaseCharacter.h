#pragma once
#include "Collider.h"
#include "Object3d.h"
#include "BaseCharacterState.h"

class BaseCharacter : public Collider{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	void ChangeState(std::unique_ptr<BaseCharacterState> newState);
protected:
	// ワールドデータ
	std::unique_ptr<Object3d> object3d_ = nullptr;
	// キャラクター状態
	std::unique_ptr<BaseCharacterState> state_ = nullptr;
public:
	virtual Vector3 GetCenterPosition() const override;
	Object3d* GetObject3d() { return object3d_.get(); }
};

