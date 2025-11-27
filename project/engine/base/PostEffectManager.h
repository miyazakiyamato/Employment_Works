#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "DirectXCommon.h"
#include "SrvUavManager.h"
#include "Matrix4x4.h"

/// <summary>
/// ポストエフェクトマネージャクラス
/// 画面全体にかけるエフェクト（ポストプロセス）を管理・実行する
/// </summary>
class PostEffectManager {
private:
		// --- namespace省略 ---
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
		// --- 構造体定義 ---
	/// <summary>
	/// ポストエフェクト用定数バッファ構造体
	/// </summary>
	struct Material {
		Matrix4x4 projectionInverseMatrix;
		float depthSensitivity = 1.0f;
		float threshold;   // マスクの閾値
		float padding[2];  // 16バイト境界アライメント
		Vector3 edgeColor; // エッジの色
		float time = 0.0f; // 時間変数
	};

		// --- メンバ関数 ---
	/// <summary>
	/// インスタンス取得
	/// </summary>
	static PostEffectManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(DirectXCommon* dxCommon, SrvUavManager* srvUavManager);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGuiでのパラメータ編集用更新
	/// </summary>
	void ImGuiUpdate();

	/// <summary>
	/// ポストエフェクトの追加
	/// </summary>
	void AddPostEffect(const std::string& name);

	/// <summary>
	/// ポストエフェクトの削除
	/// </summary>
	void RemovePostEffect(const std::string& name);

private:
		// --- シングルトン ---
	static PostEffectManager* instance;

	PostEffectManager() = default;
	~PostEffectManager() = default;
	PostEffectManager(PostEffectManager&) = delete;
	PostEffectManager& operator=(PostEffectManager&) = delete;

		// --- メンバ変数 ---
	DirectXCommon* dxCommon_ = nullptr;
	SrvUavManager* srvUavManager_ = nullptr;

	std::string offScreenName_;         //!< オフスクリーンのパイプライン名
	std::string postEffectName_ = "Fullscreen"; //!< ポストエフェクト描画用パイプライン名
	std::string maskTextureName_ = "noise0.png"; //!< マスクテクスチャの名前

	ComPtr<ID3D12Resource> materialResource;
	ComPtr<ID3D12Resource> effectOrderResource; //!< エフェクト順序のリソース
	Material* material_ = nullptr;

public:
		// --- ゲッター ---
	float GetDepthSensitivity() const { return material_->depthSensitivity; }
	float GetThreshold() const { return material_->threshold; }
	Vector3 GetEdgeColor() const { return material_->edgeColor; }
		// --- セッター ---
	void SetDepthSensitivity(float sensitivity) { material_->depthSensitivity = sensitivity; }
	void SetThreshold(float threshold) { material_->threshold = threshold; }
	void SetEdgeColor(const Vector3& color) { material_->edgeColor = color; }
	void SetMaskTextureName(const std::string& name) { maskTextureName_ = name; }
};