#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ClearScene.h"
#include "GameOverScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceveName)
{
	//次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceveName == "TITLE") {
		newScene = new TitleScene();
	}else if (sceveName == "GAME") {
		newScene = new GameScene();
	} else if (sceveName == "CLEAR") {
		newScene = new ClearScene();
	} else if (sceveName == "GAMEOVER") {
		newScene = new GameOverScene();
	}
	return newScene;
}
