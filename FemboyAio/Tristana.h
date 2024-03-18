#include "../plugin_sdk/plugin_sdk.hpp"

namespace tristana
{
	inline void interrupter();
	inline void antigap(game_object_script sender, antigapcloser::antigapcloser_args* args);
	inline void killstealloop();
	inline void killstealER(const game_object_script& enemy);
	inline void killstealW(const game_object_script& enemy);
	inline float GetDamage(const game_object_script& target);
	inline float wDamage(const game_object_script& target);
	inline float EDamage(const game_object_script& target);
	inline float RDamage(const game_object_script& target);
	inline void beforeaa(game_object_script target, bool* process);
	inline void on_draw_real();
	inline void on_draw();
	inline void update();
	void load();
	void unload();
	inline hit_chance getPredIntFromSettings(int hitchance);
	inline void drawCircle(vector pos, int radius, int quality, bool legsense, unsigned long color, int thickness);
	inline void draw_dmg_rl(const game_object_script& target, const float damage, unsigned long color);
}