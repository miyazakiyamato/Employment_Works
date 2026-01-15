#pragma once
#include "Sprite.h"

/// <summary>
/// UI基底クラス
/// </summary>
class BaseUI {
public:
	virtual ~BaseUI() = default;



	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// ImGui更新
	/// </summary>
	virtual void ImGuiUpdate() {}
};
