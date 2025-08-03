#pragma once
#include "BaseBullet.h"
#include <memory>

class CollisionManager;
class BulletManager{
public:
	BulletManager() = default;
	~BulletManager() = default;
	// 終了
	void Finalize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 弾を追加
	void AddBullet(std::unique_ptr<BaseBullet> bullet);
	// コライダーを追加
	void AddCollider(CollisionManager* collisionManager);
private:
	std::list<std::unique_ptr<BaseBullet>> bullets_; // 弾のリスト
};

