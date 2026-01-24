#pragma once
#include <vector>
#include <memory>
#include <string>

#include "Skydome.h"
#include "Ground.h"
#include "Object3d.h"
#include "LevelDataManager.h"
#include "RailCamera.h"
#include "BaseEnemy.h"
#include "Player.h"
#include "CollisionManager.h"
#include "BulletManager.h"
#include "BaseEventObject.h"

class ParticleSystem;

/// <summary>
/// ステージマネージャ
/// ステージ上のオブジェクト（敵、イベント、背景など）を管理する
/// </summary>
class StageManager {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="bulletManager">弾マネージャ(エネミー用)</param>
	/// <param name="particleSystem">パーティクル(エネミー用)</param>
	void Initialize(BulletManager* bulletManager, ParticleSystem* particleSystem);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	
	/// <summary>
	/// コリジョンの追加
	/// </summary>
	/// <param name="collisionManager">コリジョンマネージャー</param>
	void SetStageCollisions(CollisionManager* collisionManager);

	/// <summary>
	/// レールカメラ取得
	/// </summary>
	RailCamera* GetRailCamera() const { return railCamera_.get(); }

	/// <summary>
	/// プレイヤーの取得
	/// </summary>
	/// <returns></returns>
	Player* GetPlayer() const { return player_.get(); }

	/// <summary>
	/// 敵の追加
	/// </summary>
	/// <param name="enemy">追加するエネミー</param>
	/// <param name="position">出現位置</param>
	void AddEnemy(std::unique_ptr<BaseEnemy> enemy, const Vector3& position);

	/// <summary>
	/// イベントオブジェクトの追加
	/// </summary>
	/// <param name="eventObject">追加するイベントオブジェクト</param>
	void AddEventObject(std::unique_ptr<BaseEventObject> eventObject);

private:
	void LoadBackgroundObject(const std::unique_ptr<ObjectData>& objectData);
	void LoadEnemyObject(const std::unique_ptr<ObjectData>& objectData);
	void LoadEventObject(const std::unique_ptr<ObjectData>& objectData);
	void LoadCameraObject(const std::unique_ptr<ObjectData>& objectData, std::vector<Vector3>& railCameraPoints);

	// 天球
	std::unique_ptr<Skydome> skydome_;
	// 地面
	std::unique_ptr<Ground> ground_;
	// 3Dオブジェクト(背景など)
	std::vector<std::unique_ptr<Object3d>> object3ds_;
	// レベルデータマネージャ
	std::unique_ptr<LevelDataManager> levelDataManager_;
	// レールカメラ
	std::unique_ptr<RailCamera> railCamera_;
	// プレイヤー
	std::unique_ptr<Player> player_;
	// エネミー
	std::vector<std::unique_ptr<BaseEnemy>> enemies_;
	// イベントオブジェクト
	std::vector<std::unique_ptr<BaseEventObject>> eventObjects_;

	// 借りてくるもの
	BulletManager* bulletManager_ = nullptr;
	ParticleSystem* particleSystem_ = nullptr;
};
