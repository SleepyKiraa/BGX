#include "../plugin_sdk/plugin_sdk.hpp"

namespace zeri
{
	inline void killstealloop();
	inline void killstealQ(const game_object_script& enemy);
	inline void killstealW(const game_object_script& enemy);
	inline void killstealR(const game_object_script& enemy);
	inline void attackturretwithq();
	inline void laneclear();
	inline void jungleclear();
	inline void QLogic();
	inline void RLogic();
	inline void SemiW();
	inline void WLogic();
	inline float GetDamage(const game_object_script& target);
	inline float RDamage(const game_object_script& target);
	inline float WDamage(const game_object_script& target);
	inline float QDamage(const game_object_script& target);
	inline void AATracker();
	inline void UpdateQRange();
	inline void on_env_draw();
	inline void update();
	inline void beforeaa(game_object_script target, bool* process);
	inline hit_chance getPredIntFromSettings(int hitchance);
	inline void drawCircle(vector pos, int radius, int quality, bool legsense, unsigned long color, int thickness);
	inline void draw_dmg_rl(const game_object_script& target, const float damage, unsigned long color);
	inline void createmenu();
	void load();
	void unload();
}