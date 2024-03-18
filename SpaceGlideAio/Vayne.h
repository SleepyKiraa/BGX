#include "plugin_sdk/plugin_sdk.hpp"

namespace vayne
{
	inline void ondocast(game_object_script sender, spell_instance_script spell);
	inline void QLogic();
	inline void RLogic();
	inline void afteraa(game_object_script target);
	inline void ELogic();
	inline void killstealloop();
	inline void KillstealE(const game_object_script& enemy);
	inline void interrupter();
	inline void antigap(const game_object_script sender, antigapcloser::antigapcloser_args* args);
	inline void on_draw();
	inline void on_env_draw();
	inline float GetDamage(const game_object_script& target);
	inline void update();
	inline float EDamage(const game_object_script& target);
	inline void createmenu();
	inline hit_chance getPredIntFromSettings(int hitchance);
	inline bool condemcheck(vector start, const game_object_script& target);
	void load();
	void unload();
}
