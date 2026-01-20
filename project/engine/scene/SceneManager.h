#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include <PipelineManager.h>
#include <unordered_map>
#include <memory>

/// <summary>
/// シーンマネージャクラス
/// 複数のシーンを管理し、切り替えや並行実行を制御するシングルトン
/// </summary>
class SceneManager {
public:
		// --- メンバ関数 ---
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	static SceneManager* GetInstance();

	/// <summary>
	/// 終了処理
	/// 管理している全シーンの破棄を行う
	/// </summary>
	void Finalize();

	/// <summary>
	/// 毎フレーム更新
	/// シーン変更後、シーンのUpdateを呼ぶ
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// シーンのDrawを呼ぶ
	/// </summary>
	void Draw();

	/// <summary>
	/// シーン変更
	/// 次のシーン予約する
	/// </summary>
	void ChangeScene(std::string SceneName);
private:
		// --- シングルトン ---
	static SceneManager* instance;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(SceneManager&) = delete;
	SceneManager& operator=(SceneManager&) = delete;

		// --- メンバ変数 ---
	//今のシーン
	BaseScene* scene_ = nullptr;

	//次のシーン
	BaseScene* nextScene_ = nullptr;

	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;
public:
		// --- セッター ---
	/// <summary>
	/// シーンファクトリーの設定
	/// 初期化時に必ず設定する必要がある
	/// </summary>
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
};