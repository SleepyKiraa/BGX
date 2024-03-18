#include "../plugin_sdk/plugin_sdk.hpp"

namespace ashe
{
	inline void HarassW();
	inline void killstealloop();
	inline void antigap(game_object_script sender, antigapcloser::antigapcloser_args* args);
	inline void interrupter();
	inline void laneclear();
	inline void jungleclear();
	inline void killsteal(const game_object_script& enemy);
	inline void SemiR();
	inline void ELogic();
	inline float RDamage(const game_object_script& target);
	inline float WDamage(const game_object_script& target);
	inline float GetDamage(const game_object_script& target);
	inline void on_draw_real();
	inline void on_draw();
	inline void WLogic();
	inline void update();
	inline void afteraa(game_object_script target);
	inline hit_chance getPredIntFromSettings(int hitchance);
	inline void drawCircle(vector pos, int radius, int quality, bool legsense, unsigned long color, int thickness);
	inline void draw_dmg_rl(const game_object_script& target, const float damage, unsigned long color);
	inline void createmenu();
	void load();
	void unload();
}