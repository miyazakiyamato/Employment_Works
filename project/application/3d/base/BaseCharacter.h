#pragma once
#include "Collider.h"
#include "Object3d.h"
#include "BaseCharacterState.h"

/// <summary>
/// キャラクター基底クラス
/// </summary>
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

	/// <summary>
	/// 状態変更
	/// </summary>
	/// <param name="newState">新しい状態</param>
	void ChangeState(std::unique_ptr<BaseCharacterState> newState);
protected:
	// ワールドデータ
	std::unique_ptr<Object3d> object3d_ = nullptr;
	// キャラクター状態
	std::unique_ptr<BaseCharacterState> state_ = nullptr;
public:
	/// <summary>
	/// 中心座標取得
	/// </summary>
	/// <returns>中心座標</returns>
	virtual Vector3 GetCenterPosition() const override;
	
	/// <summary>
	/// 3Dオブジェクト取得
	/// </summary>
	/// <returns>3Dオブジェクト</returns>
	Object3d* GetObject3d() { return object3d_.get(); }
};

