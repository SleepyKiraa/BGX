#include "../plugin_sdk/plugin_sdk.hpp"

namespace ezreal
{
	inline void AutoQ();
	inline void qHarass();
	inline void killstealloop();
	inline void killstealQ(const game_object_script& enemy);
	inline void killstealR(const game_object_script& enemy);
	inline void laneclear();
	inline void jungleclear();
	inline void evadeing();
	inline void autoR();
	inline void afteraa(game_object_script target);
	inline void Wlogic();
	inline void QLogic();
	inline void SemiR();
	inline void on_draw_real();
	inline void on_draw();
	inline float GetDamage(const game_object_script& target);
	inline hit_chance getPredIntFromSettings(int hitchance);
	inline void drawCircle(vector pos, int radius, int quality, bool legsense, unsigned long color, int thickness);
	inline void draw_dmg_rl(const game_object_script& target, const float damage, unsigned long color);
	inline void createmenu();
	void onUpdate();
	void load();
	void unload();
}