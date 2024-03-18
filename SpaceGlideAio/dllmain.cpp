#include "plugin_sdk/plugin_sdk.hpp"
#include "Varus.h"
#include "Xerath.h"
#include "Vayne.h"
#include "Lux.h"
#include "Yone.h"

PLUGIN_NAME("Spaceglide AIO")
SUPPORTED_CHAMPIONS(champion_id::Varus, champion_id::Xerath, champion_id::Vayne, champion_id::Lux, champion_id::Yone);

PLUGIN_API bool on_sdk_load(plugin_sdk_core* plugin_sdk_good)
{
	DECLARE_GLOBALS(plugin_sdk_good);
	switch (myhero->get_champion())
	{
	case champion_id::Varus:
		varus::load();
		console->print("[Spaceglide] Varus Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[Spaceglide]</font> <font color=\"#ee00cb\">Varus Loaded!</font>");
		return true;

	case champion_id::Xerath:
		xerath::load();
		console->print("[Spaceglide] Xerath Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[Spaceglide]</font> <font color=\"#ee00cb\">Xerath Loaded!</font>");
		return true;

	case champion_id::Vayne:
		vayne::load();
		console->print("[Spaceglide] Vayne Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[Spaceglide]</font> <font color=\"#ee00cb\">Vayne Loaded!</font>");
		return true;

	case champion_id::Lux:
		lux::load();
		console->print("[Spaceglide] Lux Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[Spaceglide]</font> <font color=\"#ee00cb\">Lux Loaded!</font>");
		return true;

	case champion_id::Yone:
		yone::load();
		console->print("[Spaceglide] Yone Loaded!");
		myhero->print_chat(0, "<font color=\"#ff00cb\">[Spaceglide]</font> <font color=\"#ee00cb\">Yone Loaded!</font>");
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
	case champion_id::Varus:
		varus::unload();
		return;

	case champion_id::Xerath:
		xerath::unload();
		return;

	case champion_id::Vayne:
		vayne::unload();
		return;

	case champion_id::Lux:
		lux::unload();
		return;

	case champion_id::Yone:
		yone::unload();
		return;

	default:
		break;
	}
}