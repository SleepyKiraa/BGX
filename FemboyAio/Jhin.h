#include "../plugin_sdk/plugin_sdk.hpp"

namespace jhin
{
	inline void qHarass();
	inline void antigap(game_object_script sender, antigapcloser::antigapcloser_args* args);
	inline void killstealQ(const game_object_script& enemy);
	inline void killstealW(const game_object_script& enemy);
	inline void killstealloop();
	inline void laneclear();
	inline void jungleclear();
	inline void ELogic();
	inline void SemiW();
	inline void update();
	inline void WLogic();
	inline void afterAA(game_object_script target);
	inline float GetDamage(const game_object_script& target);
	inline float WDamage(const game_object_script& target);
	inline float EDamage(const game_object_script& target);
	inline float RDamage(const game_object_script& target);
	inline void createmenu();
	inline void on_draw();
	inline void on_draw_real();
	inline void draw_dmg_rl(const game_object_script& target, const float damage, unsigned long color);
	inline void drawCircle(vector pos, int radius, int quality, bool legsense, unsigned long color, int thickness);
	inline hit_chance getPredIntFromSettings(int hitchance);
	void load();
	void unload();
}