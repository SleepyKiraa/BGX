#include "../plugin_sdk/plugin_sdk.hpp"

namespace orianna
{
	inline void interrupter();
	inline void laneclear();
	inline void jungleclear();
	inline void killstealloop();
	inline void killstealQ(const game_object_script& enemy);
	inline void killstealW(const game_object_script& enemy);
	inline void killstealR(const game_object_script& enemy);
	inline void SemiE();
	inline void RLogic();
	inline void ELogic();
	inline void WLogic();
	inline void QLogic();
	inline float GetDamage(const game_object_script& target);
	inline float rDamage(const game_object_script& target);
	inline float wDamage(const game_object_script& target);
	inline float qDamage(const game_object_script& target);
	void update();
	inline void on_env_draw();
	inline void on_draw_real();
	inline hit_chance getPredIntFromSettings(int hitchance);
	inline void drawCircle(vector pos, int radius, int quality, bool legsense, unsigned long color, int thickness);
	inline void draw_dmg_rl(const game_object_script& target, const float damage, unsigned long color);
	inline void update();
	inline void oncreate(game_object_script obj);
	inline void createmenu();
	void load();
	void unload();
}