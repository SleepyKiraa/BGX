#include "plugin_sdk/plugin_sdk.hpp"

namespace varus
{
	inline void Wlogic();
	inline void antigap(const game_object_script sender, antigapcloser::antigapcloser_args* args);
	inline void killstealloop();
	inline void jungleclear();
	inline void laneclear();
	inline void killstealE(const game_object_script& enemy);
	inline void killstealQ(const game_object_script& enemy);
	inline void QLogic();
	inline void SemiQ();
	inline void SemiR();
	inline float wDamage(const game_object_script& target);
	inline void RLogic();
	inline float GetDamageforROnly(const game_object_script& target);
	inline void ELogic();
	inline float rDamage(const game_object_script& target);
	inline float GetDamage(const game_object_script& target);
	inline void update();
	inline void on_draw();
	inline void on_draw_env();
	inline float qfastDamage(const game_object_script& target);
	inline float qFullDamage(const game_object_script& target);
	inline float eDamage(const game_object_script& target);
	inline hit_chance getPredIntFromSettings(int hitchance);
	inline void createmenu();
	void load();
	void unload();
}