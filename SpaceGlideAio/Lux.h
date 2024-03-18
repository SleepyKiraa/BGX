#include "plugin_sdk/plugin_sdk.hpp"

namespace lux
{
	inline void killstealE2(const game_object_script& enemy);
	inline void laneclear();
	inline void jungleclear();
	inline void killstealloop();
	inline void killstealR(const game_object_script& enemy);
	inline void killstealE1(const game_object_script& enemy);
	inline void killstealQ(const game_object_script& enemy);
	inline void autoQ();
	inline void antigap(game_object_script sender, antigapcloser::antigapcloser_args* args);
	inline void SemiR();
	inline void E2Logic();
	inline void ELogic();
	inline void WLogicMe();
	inline void WLogicAllys();
	inline bool wwhitelist(game_object_script ally);
	inline void SemiQ();
	inline void QLogic();
	inline void on_draw();
	inline void on_env_draw();
	inline void ondelete(game_object_script obj);
	inline void oncreate(game_object_script obj);
	inline void update();
	inline float GetDamage(const game_object_script& target);
	inline float WShield();
	inline float QDamage(const game_object_script& target);
	inline float EDamage(const game_object_script& target);
	inline float RDamage(const game_object_script& target);
	inline hit_chance getPredIntFromSettings(int hitchance);
	inline void createmenu();
	void load();
	void unload();
}