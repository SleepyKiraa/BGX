#include "../plugin_sdk/plugin_sdk.hpp"

namespace twitch
{
	inline void update();
	inline float GetDamage(const game_object_script& target);
	inline float eDamage(const game_object_script& target);
	inline hit_chance getPredIntFromSettings(int hitchance);
	inline void draw_dmg_rl(const game_object_script& target, const float damage, unsigned long color);
	inline void createmenu();
	void load();
	void unload();
}
