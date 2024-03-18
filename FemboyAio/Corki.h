#include "../plugin_sdk/plugin_sdk.hpp"

namespace corki
{
	inline void jungleclear();
	inline void laneclear();
	inline void killstealloop();
	inline void KillstealQ(const game_object_script& enemy);
	inline void KillstealR(const game_object_script& enemy);
	inline void afteraa(game_object_script target);
	inline void Wevade();
	inline void RLogic();
	inline void QLogic();
	inline void SemiR1();
	inline void SemiR2();
	inline void update();
	inline float GetDamage(const game_object_script& target);
	inline float QDamage(const game_object_script& target);
	inline float RDamage(const game_object_script& target);
	inline float R2Damage(const game_object_script& target);
	inline void on_draw_real();
	inline void on_draw_env();
	inline hit_chance getPredIntFromSettings(int hitchance);
	inline void drawCircle(vector pos, int radius, int quality, bool legsense, unsigned long color, int thickness);
	inline void draw_dmg_rl(const game_object_script& target, const float damage, unsigned long color);
	inline void createmenu();
	void load();
	void unload();
}