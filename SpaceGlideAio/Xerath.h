#include "plugin_sdk/plugin_sdk.hpp"

namespace xerath
{
	inline void onprocess(game_object_script sender, spell_instance_script spell);
	inline void laneclear();
	inline void jungleclear();
	inline void killstealQ(const game_object_script& enemy);
	inline void killstealW(const game_object_script& enemy);
	inline void killstealE(const game_object_script& enemy);
	inline void killstealloop();
	inline void antigap(game_object_script sender, antigapcloser::antigapcloser_args* args);
	inline void Interrupter();
	inline void AutoE();
	inline void AutoW();
	inline void RLogic();
	inline void QLogic();
	inline void ELogic();
	inline void WLogic();
	inline float GetDamage(const game_object_script& target);
	inline void update();
	inline float QDamage(const game_object_script& target);
	inline float WDamage(const game_object_script& target);
	inline float EDamage(const game_object_script& target);
	inline float RDamage(const game_object_script& target);
	inline void on_draw_real();
	inline void on_draw_env();
	inline void createmenu();
	inline hit_chance getPredIntFromSettings(int hitchance);
	void load();
	void unload();
}
