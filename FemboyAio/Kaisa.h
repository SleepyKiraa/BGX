#include "../plugin_sdk/plugin_sdk.hpp"

namespace kaisa
{
	inline void laneclear();
	inline void jungleclear();
	inline void killstealloop();
	inline void killstealQ(const game_object_script& enemy);
	inline void killstealW(const game_object_script& enemy);
	inline void RLogic();
	inline void afteraa(game_object_script target);
	inline void SemiW();
	inline void WLogic();
	inline void QLogic();
	inline float GetDamage(const game_object_script& target);
	inline float wDamage(const game_object_script& target);
	inline float qDamage(const game_object_script& target);
	inline void on_env_draw();
	inline void on_draw();
	inline void draw_dmg_rl(const game_object_script& target, const float damage, unsigned long color);
	inline void drawCircle(vector pos, int radius, int quality, bool legsense, unsigned long color, int thickness);
	inline hit_chance getPredIntFromSettings(int hitchance);
	inline void update();
	inline void createmenu();
	void load();
	void unload();
}
