#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "GameOverScene.h"
#include "FadeInScene.h"
#include "FadeOutScene.h"
#include "GameStartScene.h"
#include "PlayerDeathScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE") {
		newScene = new TitleScene();
	} else if (sceneName == "GAME") {
		newScene = new GameScene();
	} else if (sceneName == "CLEAR") {
		newScene = new ClearScene();
	} else if (sceneName == "GAMEOVER") {
		newScene = new GameOverScene();
	} else if (sceneName == "FADE_IN") {
		newScene = new FadeInScene();
	} else if (sceneName == "FADE_OUT") {
		newScene = new FadeOutScene();
	} else if (sceneName == "GAME_START") {
		newScene = new GameStartScene();
	} else if (sceneName == "PLAYER_DEATH") {
		newScene = new PlayerDeathScene();
	} else {
		assert(0 && "不明なシーン名");
	}
	return newScene;
}
