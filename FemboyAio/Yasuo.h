#include "../plugin_sdk/plugin_sdk.hpp"

namespace yasuo
{
	inline void onprocess(game_object_script sender, spell_instance_script spell);
	inline void blockspellswithW(game_object_script sender, spell_instance_script spell);
	inline void laneclear();
	inline void jungleclear();
	inline void ELogic();
	inline void eqlogic();
	inline void QLogic();
	inline void AutoQDash();
	inline void antigap(game_object_script sender, antigapcloser::antigapcloser_args* args);
	inline void interrupter();
	inline void RLogic();
	inline void update();
	inline void killstealloop();
	inline void killstealQ(const game_object_script& enemy);
	inline void killstealQ3(const game_object_script& enemy);
	inline void killstealFQ(const game_object_script& enemy);
	inline void killstealE(const game_object_script& enemy);
	inline void on_draw_real();
	inline void on_draw_env();
	inline hit_chance getPredIntFromSettings(int hitchance);
	inline void drawCircle(vector pos, int radius, int quality, bool legsense, unsigned long color, int thickness);
	inline void draw_dmg_rl(const game_object_script& target, const float damage, unsigned long color);
	inline float GetDamage(const game_object_script& target);
	inline float qDamage(const game_object_script& target);
	inline float eDamage(const game_object_script& target);
	inline float rDamage(const game_object_script& target);
	inline void createmenu();
	void load();
	void unload();
}
