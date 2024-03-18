#include "../plugin_sdk/plugin_sdk.hpp"

namespace sivir
{
	inline void HarassQ();
	inline void on_env_draw();
	inline void on_draw();
	inline void laneclear();
	inline void jungleclear();
	inline void killstealloop();
	inline void killstealQ(const game_object_script& enemy);
	inline void evadee();
	inline void afteraa(game_object_script target);
	inline void update();
	inline void QLogic();
	inline hit_chance getPredIntFromSettings(int hitchance);
	inline void drawCircle(vector pos, int radius, int quality, bool legsense, unsigned long color, int thickness);
	inline void draw_dmg_rl(const game_object_script& target, const float damage, unsigned long color);
	inline float GetDamage(const game_object_script& target);
	inline float qDamage(const game_object_script& target);
	inline void createmenu();
	void load();
	void unload();
}