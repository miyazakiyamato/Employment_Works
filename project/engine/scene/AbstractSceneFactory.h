#pragma once
#include <string>
#include "BaseScene.h"

/// <summary>
/// シーン工場基底クラス (Abstract Factory)
/// シーン名に基づいて具体的なシーンインスタンスを生成するインターフェース
/// </summary>
class AbstractSceneFactory {
public:
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~AbstractSceneFactory() = default;

	/// <summary>
	/// シーン生成
	/// 指定された名前のシーンオブジェクトを作成する
	/// </summary>
	/// <param name="sceneName">生成したいシーンの名前</param>
	/// <returns>生成されたシーンへのポインタ</returns>
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};