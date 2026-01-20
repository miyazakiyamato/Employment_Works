#pragma once
#include "AbstractTransitionFactory.h"

/// <summary>
/// トランジションシーン工場
/// </summary>
class TransitionFactory : public AbstractTransitionFactory {
public:
	/// <summary>
	/// トランジションシーン生成
	/// </summary>
	/// <param name="sceneName">シーン遷移エフェクト名</param>
	/// <returns>生成したシーン遷移エフェクト</returns>
	std::unique_ptr<BaseTransition> CreateTransition(const std::string& transitionName) override;
};
