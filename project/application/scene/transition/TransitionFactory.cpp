#include "TransitionFactory.h"
#include "FadeTransition.h"
#include <cassert>

std::unique_ptr<BaseTransition> TransitionFactory::CreateTransition(const std::string& transitionName)
{
	//次のシーンを生成
	std::unique_ptr<BaseTransition> newTransition = nullptr;

	if (transitionName == "FADE") {
		newTransition = std::make_unique<FadeTransition>();
	}else {
		assert(0 && "不明なシーン名");
	}
	return std::move(newTransition);
}
