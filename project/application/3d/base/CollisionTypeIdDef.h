#pragma once

#include <cstdint>

//コリジョン種別ID定義
/// <summary>
/// コリジョン種別ID定義
/// </summary>
enum class CollisionTypeIdDef : uint32_t {
	kDefault,
	kPlayer,
	kPlayerWeapon,
	kPlayerBullet,
	kEnemy,
	kEnemyBullet,
	kEnemyWeapon,
	kEvent,
};