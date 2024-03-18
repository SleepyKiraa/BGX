#include "../plugin_sdk/plugin_sdk.hpp"
#include "ezreal.h"
#include "Ashe.h"
#include "Jhin.h"
#include "Tristana.h"
#include "Orianna.h"
#include "Kaisa.h"
#include "Sivir.h"
#include "Xerath.h"
#include "Corki.h"
#include "Yasuo.h"
#include "Kalista.h"
#include "Zeri.h"
#include "Twitch.h"

PLUGIN_NAME("FemboyAio")
SUPPORTED_CHAMPIONS(champion_id::Ezreal, champion_id::Ashe, champion_id::Jhin, champion_id::Tristana, champion_id::Orianna, champion_id::Kaisa, champion_id::Sivir, champion_id::Xerath, champion_id::Corki, champion_id::Yasuo, champion_id::Kalista, champion_id::Zeri, champion_id::Twitch);

PLUGIN_API bool on_sdk_load(plugin_sdk_core* plugin_sdk_good)
{
	DECLARE_GLOBALS(plugin_sdk_good);
	console->print("[FemboyAIO] Thanks to Kiri and Yorick for Their Template and Opensource Aio!");
	switch (myhero->get_champion())
	{
	case champion_id::Ezreal:
		ezreal::load();
		console->print("[Femboy] Ezreal Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[FemboyAio]</font> <font color=\"#ee00cb\">Ezreal Loaded!</font>");
		return true;

	case champion_id::Ashe:
		ashe::load();
		console->print("[Femboy] Ashe Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[FemboyAio]</font> <font color=\"#ee00cb\">Ashe Loaded!</font>");
		return true;

	case champion_id::Jhin:
		jhin::load();
		console->print("[Femboy] Jhin Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[FemboyAio]</font> <font color=\"#ee00cb\">Jhin Loaded!</font>");
		return true;

	case champion_id::Tristana:
		tristana::load();
		console->print("[Femboy] Tristana Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[FemboyAio]</font> <font color=\"#ee00cb\">Tristana Loaded!</font>");
		return true;

	case champion_id::Orianna:
		orianna::load();
		console->print("[Femboy] Orianna Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[FemboyAio]</font> <font color=\"#ee00cb\">Orianna Loaded!</font>");
		return true;

	case champion_id::Kaisa:
		kaisa::load();
		console->print("[Femboy] Kaisa Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[FemboyAio]</font> <font color=\"#ee00cb\">Kaisa Loaded!</font>");
		return true;

	case champion_id::Sivir:
		sivir::load();
		console->print("[Femboy] Sivir Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[FemboyAio]</font> <font color=\"#ee00cb\">Sivir Loaded!</font>");
		return true;

	case champion_id::Xerath:
		xerath::load();
		console->print("[Femboy] Xerath Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[FemboyAio]</font> <font color=\"#ee00cb\">Xerath Loaded!</font>");
		return true;

	case champion_id::Corki:
		corki::load();
		console->print("[Femboy] Corki Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[FemboyAio]</font> <font color=\"#ee00cb\">Corki Loaded!</font>");
		return true;

	case champion_id::Yasuo:
		yasuo::load();
		console->print("[Femboy] Yasuo Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[FemboyAio]</font> <font color=\"#ee00cb\">Yasuo Loaded!</font>");
		return true;

	case champion_id::Kalista:
		kalista::load();
		console->print("[Femboy] Kalista Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[FemboyAio]</font> <font color=\"#ee00cb\">Kalista Loaded!</font>");
		return true;

	case champion_id::Zeri:
		zeri::load();
		console->print("[Femboy] Zeri Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[FemboyAio]</font> <font color=\"#ee00cb\">Zeri Loaded!</font>");
		return true;

	case champion_id::Twitch:
		twitch::load();
		console->print("[Femboy] Twitch Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[FemboyAio]</font> <font color=\"#ee00cb\">Twitch Loaded!</font>");
		return true;

	default:		
		break;
	}	

	return false;
}

PLUGIN_API void on_sdk_unload()
{
	switch (myhero->get_champion())
	{
	case champion_id::Ezreal:
		ezreal::unload();
		return;

	case champion_id::Ashe:
		ashe::unload();
		return;

	case champion_id::Jhin:
		jhin::unload();
		return;

	case champion_id::Tristana:
		tristana::unload();
		return;

	case champion_id::Orianna:
		orianna::unload();
		return;

	case champion_id::Kaisa:
		kaisa::unload();
		return;

	case champion_id::Sivir:
		sivir::unload();
		return;

	case champion_id::Xerath:
		xerath::unload();
		return;

	case champion_id::Corki:
		corki::unload();
		return;

	case champion_id::Yasuo:
		yasuo::unload();
		return;

	case champion_id::Kalista:
		kalista::unload();
		return;

	case champion_id::Zeri:
		zeri::unload();
		return;

	case champion_id::Twitch:
		twitch::unload();
		return;

	default:
		break;
	}
}