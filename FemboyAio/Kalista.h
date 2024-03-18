#include "../plugin_sdk/plugin_sdk.hpp"

namespace kalista
{
	inline void onbuffgain2(game_object_script sender, buff_instance_script buff);
	inline void laneclear();
	inline void jungleclear();
	inline void Rlogic();
	inline void onbuffgain(game_object_script sender, buff_instance_script buff);
	inline void ELogic();
	inline void QLogic();
	inline void killstealQ(const game_object_script& enemy);
	inline void killstealloop();
	inline void killstealE(const game_object_script& enemy);
	inline void on_draw_env();
	inline void on_draw_real();
	inline float GetDamage(const game_object_script& target);
	inline void update();
	inline float eDamage(const game_object_script& target);
	inline float qDamage(const game_object_script& target);
	inline void createmenu();
	inline void draw_dmg_rl(const game_object_script& target, const float damage, unsigned long color);
	inline void drawCircle(vector pos, int radius, int quality, bool legsense, unsigned long color, int thickness);
	inline hit_chance getPredIntFromSettings(int hitchance);
	void load();
	void unload();
}