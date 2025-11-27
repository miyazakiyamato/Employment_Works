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
	/// 追加・削除リクエストの処理後、各シーンのUpdateを呼ぶ
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// 各シーンのDrawを呼ぶ
	/// </summary>
	void Draw();

	/// <summary>
	/// シーン追加
	/// 次のフレーム開始時にファクトリを使ってシーンを生成・追加する
	/// </summary>
	/// <param name="sceneName">追加するシーン名</param>
	void AddScene(const std::string& sceneName);

	/// <summary>
	/// シーン削除
	/// 次のフレーム開始時に指定されたシーンを破棄する
	/// </summary>
	/// <param name="sceneName">削除するシーン名</param>
	void RemoveScene(const std::string& sceneName);

	/// <summary>
	/// シーン生存確認
	/// </summary>
	/// <param name="sceneName">確認するシーン名</param>
	/// <returns>シーンが存在すればtrue</returns>
	bool IsSceneAlive(const std::string& sceneName);

	/// <summary>
	/// シーン終了確認
	/// シーンクラス内の終了フラグ(IsFinished)を確認する
	/// </summary>
	/// <param name="sceneName">確認するシーン名</param>
	/// <returns>終了フラグが立っていればtrue</returns>
	bool IsSceneFinished(const std::string& sceneName);

private:
		// --- シングルトン ---
	static SceneManager* instance;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(SceneManager&) = delete;
	SceneManager& operator=(SceneManager&) = delete;

		// --- メンバ変数 ---
	// 管理中のシーン (名前 -> インスタンス)
	std::unordered_map<std::string, std::unique_ptr<BaseScene>> scenes_;

	// 追加・削除予約リスト
	std::list<std::string> addSceneNames_;
	std::list<std::string> removeSceneNames_;

	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;

public:
		// --- ゲッター ---
	/// <summary>
	/// シーンの取得
	/// </summary>
	BaseScene* GetScene(const std::string& sceneName);

		// --- セッター ---
	/// <summary>
	/// シーンファクトリーの設定
	/// 初期化時に必ず設定する必要がある
	/// </summary>
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
};