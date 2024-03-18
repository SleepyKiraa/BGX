#pragma once
#include "../plugin_sdk/plugin_sdk.hpp"

namespace femboyutils
{
	bool is_valid_minion(const game_object_script& obj);
	int count_minions(const vector& position, const float range, const std::string& team);
	void AATracker();
}