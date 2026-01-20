#include "SceneManager.h"
#include <cassert>

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance(){
	//インスタンスがなければ生成
	if (instance == nullptr) {
		instance = new SceneManager;
	}
	return instance;
}

void SceneManager::Finalize() {
	//シーンの終了
	scene_->Finalize();
	delete scene_;

	//インスタンスの削除
	delete instance;
	instance = nullptr;
}

void SceneManager::Update() {
	//次のシーンの予約があるなら
	if (nextScene_) {
		//旧シーンの終了
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}
		//シーンの切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;
		//次のシーンを初期化する
		scene_->Initialize();
		scene_->SetSceneManager(this);
	}

	//シーンの更新
	scene_->Update();
}

void SceneManager::Draw() {
	//シーンの描画
	scene_->Draw();
}

void SceneManager::ChangeScene(std::string SceneName) {
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	nextScene_ = sceneFactory_->CreateScene(SceneName);
}