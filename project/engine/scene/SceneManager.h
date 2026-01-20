#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include "BaseTransition.h"
#include "AbstractTransitionFactory.h"
#include "PipelineManager.h"
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
	void ChangeScene(std::string sceneName);

	/// <summary>
	/// シーン遷移エフェクト変更
	/// 次のシーン遷移エフェクトを変更
	/// </summary>
	void ChangeTransition(std::string transitionName);
private:
	/// <summary>
	/// シーン遷移開始
	/// シーン遷移開始とシーンの変更を行う
	/// </summary>
	void SceneTransition();

		// --- シングルトン ---
	static SceneManager* instance;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(SceneManager&) = delete;
	SceneManager& operator=(SceneManager&) = delete;

		// --- メンバ変数 ---
	//今のシーン
	std::unique_ptr<BaseScene> scene_ = nullptr;
	//次のシーン
	std::unique_ptr<BaseScene> nextScene_ = nullptr;
	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;

	// シーン遷移エフェクト
	std::unique_ptr<BaseTransition> transition_ = nullptr;
	// シーン遷移エフェクトファクトリー
	AbstractTransitionFactory* transitionFactory_ = nullptr;
public:
		// --- ゲッター ---
	/// シーン遷移エフェクトの取得
	BaseTransition* GetTransition() const { return transition_.get(); }
		// --- セッター ---
	/// <summary>
	/// シーンファクトリーの設定
	/// 初期化時に必ず設定する必要がある
	/// </summary>
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
	/// <summary>
	/// シーン遷移エフェクトファクトリーの設定
	///	初期化時に必ず設定する必要がある
	/// </summary>
	void SetTransitionFactory(AbstractTransitionFactory* transitionFactory) { transitionFactory_ = transitionFactory; }
};