#include "../plugin_sdk/plugin_sdk.hpp"

namespace xerath
{	
	inline void jungleclear();
	inline void laneclear();
	inline void killstealloop();
	inline void killstealQ(const game_object_script& enemy);
	inline void killstealW(const game_object_script& enemy);
	inline void killstealE(const game_object_script& enemy);
	inline void antigap(game_object_script sender, antigapcloser::antigapcloser_args* args);
	inline void interrupter();
	inline void RLogic();
	inline void QLogic();
	inline void ELogic();
	inline void autoE();
	inline void autoW();
	inline void update();
	inline void WLogic();
	inline void on_env_draw();
	inline void on_draw_real();
	inline float GetDamage(const game_object_script& target);
	inline float rDamage(const game_object_script& target);
	inline float wDamage(const game_object_script& target);
	inline float qDamage(const game_object_script& target);
	inline float eDamage(const game_object_script& target);
	inline hit_chance getPredIntFromSettings(int hitchance);
	inline void drawCircle(vector pos, int radius, int quality, bool legsense, unsigned long color, int thickness);
	inline void draw_dmg_rl(const game_object_script& target, const float damage, unsigned long color);
	inline void createmenu();
	void load();
	void unload();
}