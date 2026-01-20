#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "GameOverScene.h"
#include "DParticleScene.h"

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
	//次のシーンを生成
	std::unique_ptr<BaseScene> newScene = nullptr;

	if (sceneName == "TITLE") {
		newScene = std::make_unique<TitleScene>();
	} else if (sceneName == "GAME") {
		newScene = std::make_unique<GameScene>();
	} else if (sceneName == "CLEAR") {
		newScene = std::make_unique<ClearScene>();
	} else if (sceneName == "GAMEOVER") {
		newScene = std::make_unique<GameOverScene>();
	} else if (sceneName == "D_PARTICLE") {
		newScene = std::make_unique<DParticleScene>();
	}else {
		assert(0 && "不明なシーン名");
	}
	return std::move(newScene);
}
