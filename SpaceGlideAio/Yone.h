#include "plugin_sdk/plugin_sdk.hpp"

namespace yone
{
	inline void AutoE();
	inline void WLogic();
	inline void QLogic();
	inline void on_draw();
	inline void RLogic();
	inline float GetDamage(const game_object_script& target);
	inline float QDamage(const game_object_script& target);
	inline float RDamage(const game_object_script& target);
	inline void on_env_draw();
	inline void ondelete(game_object_script obj);
	inline void oncreate(game_object_script obj);
	inline void update();
	inline void createmenu();
	inline hit_chance getPredIntFromSettings(int hitchance);
	void load();
	void unload();
}