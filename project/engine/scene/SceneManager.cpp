#include "SceneManager.h"
#include <cassert>

std::unique_ptr<SceneManager> SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance(){
	//インスタンスがなければ生成
	if (instance == nullptr) {
		instance.reset(new SceneManager);
	}
	return instance.get();
}

void SceneManager::Finalize() {
	//インスタンスの削除
	instance.reset();
}

void SceneManager::Update() {
	//シーン遷移の開始とシーンの変更処理
	SceneTransition();

	//シーンの更新
	scene_->Update();
	//シーン遷移エフェクトの更新
	if (transition_) {
		transition_->Update();
	}
}

void SceneManager::Draw() {
	//シーンの描画
	scene_->Draw();
	
	//シーン遷移エフェクトの描画
	if (transition_) {
		transition_->Draw();
	}
}

void SceneManager::ChangeScene(std::string sceneName) {
	assert(sceneFactory_);
	if (nextScene_ != nullptr ||
		transition_ != nullptr) { return; }

	if (scene_ == nullptr) {
		//最初のシーンの場合は即座にセットする
		scene_ = std::move(sceneFactory_->CreateScene(sceneName));
		scene_->Initialize();
		scene_->SetSceneManager(this);
		return;
	}

	nextScene_ = std::move(sceneFactory_->CreateScene(sceneName));
}

void SceneManager::ChangeTransition(std::string transitionName){
	assert(transitionFactory_);
	if (transition_ != nullptr) { return; }

	transition_ = std::move(transitionFactory_->CreateTransition(transitionName));
	transition_->Initialize();
	transition_->SetType(BaseTransition::Type::kOut);
}

void SceneManager::SceneTransition(){
	//シーン遷移エフェクトが終了しているなら
	if (transition_ && transition_->GetType() == BaseTransition::Type::kIn) {
		if (transition_->IsFinished()) {
			//シーン遷移エフェクトの終了
			transition_->Finalize();
			transition_.reset();
		}
		return;
	}

	//次のシーンの予約がないならreturn
	if (nextScene_ == nullptr) { return; }

	// transition_が存在しないなら
	if (!transition_ ||
		transition_->GetType() == BaseTransition::Type::kOut && transition_->IsFinished()) {

		//シーンの切り替え
		scene_ = std::move(nextScene_);
		//次のシーンを初期化する
		scene_->Initialize();
		scene_->SetSceneManager(this);

		if (transition_) {
			//シーン遷移エフェクトの初期化
			transition_->Finalize();
			transition_->Initialize();
			transition_->SetType(BaseTransition::Type::kIn);
		}
	}
}
