#include "../plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "femboyutils.h"
#include "Twitch.h"

namespace twitch
{
	script_spell* q;
	script_spell* w;
	script_spell* e;
	script_spell* r;

	TreeTab* mainMenu;
	namespace settings {
		namespace qsettings
		{
			TreeEntry* AutoqAfterKill;
		}

		namespace wsettings
		{
			TreeEntry* wCombo;
			TreeEntry* DontWinvis;
			TreeEntry* DontWinR;
		}

		namespace esettings
		{
			TreeEntry* eCombo;
			TreeEntry* MinStacksToE;
			TreeEntry* AutoEOnDeath;
		}

		namespace killsteal
		{
			TreeEntry * ksE;
		}

		namespace hitchance
		{
			TreeEntry* wHitChance;
		}

		namespace draws
		{
			TreeEntry* wRange;
			TreeEntry* wThickness;
			TreeEntry* eRange;
			TreeEntry* eThickness;
			TreeEntry* rRange;
			TreeEntry* rThickness;
			TreeEntry* Wcolor;
			TreeEntry* W2color;
			TreeEntry* Ecolor;
			TreeEntry* E2color;
			TreeEntry* Rcolor;
			TreeEntry* R2color;
			TreeEntry* Damage;
			TreeEntry* legCircles;
			TreeEntry* NeededAA;
			TreeEntry* BuffTime;
		}

		namespace clear
		{
			TreeEntry* farmtoggle;
			TreeEntry* LcW;
			TreeEntry* JcE;
		}

		namespace misc
		{
			TreeEntry* antigap;
			TreeEntry* antigapHp;
			TreeEntry* InvisBackport;
		}
	}

	void createmenu()
	{
		mainMenu = menu->create_tab("femboy.Twitch", "[Femboy] Twitch");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("femboy.Twitch.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("femboy.Twitch.qsettings", "Ambush - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::AutoqAfterKill = qsettings->add_checkbox("femboy.Twitch.autoq", "Auto Q After Kill", true);
			}

			const auto wsettings = combotab->add_tab("femboy.Twitch.wsettings", "Venom Cask - [W]");
			wsettings->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			{
				settings::wsettings::wCombo = wsettings->add_checkbox("femboy.Twitch.usew", "Enable W in Combo", true);
				settings::wsettings::DontWinvis = wsettings->add_checkbox("femboy.Twitch.dontinvis", "Dont Use W While in Q", true);
				settings::wsettings::DontWinR = wsettings->add_checkbox("femboy.Twitch.dontinr", "Dont Use W While R is Active", true);
			}

			const auto esettings = combotab->add_tab("femboy.Twitch.esettings", "Contaminate - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::eCombo = esettings->add_checkbox("femboy.Twitch.usee", "Enable E in Combo", true);
				settings::esettings::MinStacksToE = esettings->add_slider("femboy.Twtich.stacks", "Min Stacks to use E", 7, 1, 7);
				settings::esettings::AutoEOnDeath = esettings->add_checkbox("femboy.Twitch.autoe", "Auto E Before Death", true);
			}
		}

		const auto cleartab = mainMenu->add_tab("femboy.Twitch.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("femboy.Twitch.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcW = cleartab->add_checkbox("femboy.Twitch.qclear", "Enable Q Lane Clear", true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcE = cleartab->add_checkbox("femboy.Twitch.qjclear", "Enable E Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("femboy.Twitch.ks", "Killsteal");
		{
			settings::killsteal::ksE = killstealtab->add_checkbox("femboy.Twitch.kse", "Enable E Killsteal", true);
		}

		const auto misctab = mainMenu->add_tab("femboy.Twitch.misc", "Misc");
		{
			settings::misc::antigap = misctab->add_checkbox("femboy.Twitch.anti", "Enable Anti Gapcloser", true);
			settings::misc::antigapHp = misctab->add_slider("femboy.Twitch.hp", "Min HP % to Anti Gapclose", 50, 1, 100, true);
			settings::misc::InvisBackport = misctab->add_hotkey("femboy.Twitch.invisport", "Invis Backport", TreeHotkeyMode::Hold, 0x47, false, true);
		}

		const auto hitchancetab = mainMenu->add_tab("femboy.Twitch.hitchance", "Hitchance");
		std::vector<std::pair<std::string, void*>> combo_elemts = {};
		for (int i = 3; i < 9; i++)
		{
			const auto hitchance = static_cast<hit_chance>(i);

			std::string hitchance_str;
			switch (hitchance)
			{
			case hit_chance::immobile:
				hitchance_str = "Immobile";
				break;

			case hit_chance::dashing:
				hitchance_str = "Dashing";
				break;

			case hit_chance::very_high:
				hitchance_str = "Very High";
				break;
			case hit_chance::high:
				hitchance_str = "High";
				break;
			case hit_chance::medium:
				hitchance_str = "Medium";
				break;
			case hit_chance::low:
				hitchance_str = "Low";
				break;
			case hit_chance::impossible:
				hitchance_str = "Impossible";
				break;
			case hit_chance::out_of_range:
				hitchance_str = "Out Of Range";
				break;
			case hit_chance::collision:
				hitchance_str = "Collision";
				break;
			}

			combo_elemts.emplace_back(hitchance_str, nullptr);
		}

		settings::hitchance::wHitChance = hitchancetab->add_combobox("femboy.Twitch.whitchance", "W Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("femboy.Twitch.draw", "Draws");
		{
			float defaultcolorW[] = { 0.f, 0.f, 1.f, 1.f };
			float defaultcolorW2[] = { 1.f, 0.f, 1.f, 1.f };
			float defaultcolorE[] = { 0.f, 1.f, 0.f, 1.f };
			float defaultcolorE2[] = { 1.f, 0.f, 1.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };
			float defaultcolorR2[] = { 1.f, 0.f, 1.f, 1.f };

			const auto wdrawtab = drawTab->add_tab("femboy.Twitch.draww", "W Draw Settings");
			{
				settings::draws::wRange = wdrawtab->add_checkbox("femboy.Twitch.wrange", "Draw W Range", true);
				settings::draws::Wcolor = wdrawtab->add_colorpick("femboy.Twitch.wcolor", "W Draw Color", defaultcolorW);
				settings::draws::W2color = wdrawtab->add_colorpick("femboy.Twitch.w2color", "W2 Draw Color", defaultcolorW2);
				settings::draws::wThickness = wdrawtab->add_slider("femboy.Twitch.thicknessw", "W circle Thickness", 2, 1, 6, true);

			}

			const auto edrawtab = drawTab->add_tab("femboy.Twitch.drawe", "E Draw Settings");
			{
				settings::draws::eRange = edrawtab->add_checkbox("femboy.Twitch.erange", "Draw E Range", true);
				settings::draws::Ecolor = edrawtab->add_colorpick("femboy.Twitch.ecolor", "E Draw Color", defaultcolorE);
				settings::draws::E2color = edrawtab->add_colorpick("femboy.Twitch.e2color", "E2 Draw Color", defaultcolorE2);
				settings::draws::eThickness = edrawtab->add_slider("femboy.Twitch.thicknesse", "E circle Thickness", 2, 1, 6, true);
			}

			const auto rdrawtab = drawTab->add_tab("femboy.Twitch.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("femboy.Twitch.rrange", "Draw R Range", true);
				settings::draws::Rcolor = rdrawtab->add_colorpick("femboy.Twitch.rcolor", "R Draw Color", defaultcolorR);
				settings::draws::R2color = rdrawtab->add_colorpick("femboy.Twitch.r2color", "R2 Draw Color", defaultcolorR2);
				settings::draws::rThickness = rdrawtab->add_slider("femboy.Twitch.thicknessr", "R circle Thickness", 2, 1, 6, true);
			}

			const auto miscdrawTab = drawTab->add_tab("femboy.Twitch.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("femboy.Twitch.damage", "Draw Damage Indicator", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("femboy.Twitch.aaindicator", "Draw AA Counter", true);
				settings::draws::BuffTime = miscdrawTab->add_checkbox("femboy.Twitch.bufftime", "Draw Buff Time", true);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Femboy] Twitch");
		Permashow::Instance.AddElement("Farm", settings::clear::farmtoggle);
		Permashow::Instance.AddElement("Invis Backport", settings::misc::InvisBackport);
	}

	hit_chance getPredIntFromSettings(int hitchance)
	{
		// Get hitchance from settings value
		return static_cast<hit_chance>(hitchance + 3);
	}

	void draw_dmg_rl(const game_object_script& target, const float damage, unsigned long color)
	{
		// Draw damage on HP bar from right to left
		if (target != nullptr && target->is_valid() && target->is_hpbar_recently_rendered())
		{
			auto bar_pos = target->get_hpbar_pos();

			if (bar_pos.is_valid() && !target->is_dead() && target->is_visible())
			{
				const auto health = target->get_real_health();

				bar_pos = vector(bar_pos.x + (105 * (health / target->get_max_health())), bar_pos.y -= 10);

				auto damage_size = (105 * (damage / target->get_max_health()));

				if (damage >= health)
				{
					damage_size = (105 * (health / target->get_max_health()));
				}

				if (damage_size > 105)
				{
					damage_size = 105;
				}

				const auto size = vector(bar_pos.x + (damage_size * -1), bar_pos.y + 11);

				draw_manager->add_filled_rect(bar_pos, size, color);
			}
		}
	}

	void on_draw_env()
	{
		auto glow = glow_data(0.2, 0.6, 0.2, 0.6);

		if (settings::draws::wRange->get_bool())
		{
			if (w->level() >= 1)
			{
				draw_manager->add_circle_with_glow_gradient(myhero->get_position(), settings::draws::Wcolor->get_color(),settings::draws::W2color->get_color(), w->range(), settings::draws::wThickness->get_int(), glow);
			}
		}

		if (settings::draws::eRange->get_bool())
		{
			if (e->level() >= 1)
			{
				draw_manager->add_circle_with_glow_gradient(myhero->get_position(), settings::draws::Ecolor->get_color(), settings::draws::E2color->get_color(), e->range(), settings::draws::eThickness->get_int(), glow);
			}
		}

		if (settings::draws::rRange->get_bool())
		{
			if (r->level() >= 1)
			{
				draw_manager->add_circle_with_glow_gradient(myhero->get_position(), settings::draws::Rcolor->get_color(), settings::draws::R2color->get_color(), r->range(), settings::draws::rThickness->get_int(), glow);
			}
		}
	}

	void update()
	{
		auto etarget = target_selector->get_target(e->range(), damage_type::physical);
		if (etarget != nullptr)
		{
			std::string str = std::to_string(eDamage(etarget));
			const char* mychar = str.c_str();
			console->print(mychar);
		}
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 0.f);
		q->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 950.f);
		w->set_skillshot(0.25f + 0.066f, 80.f, 1400.f, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_circle);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 1200.f);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, myhero->get_attackRange() + 300.f);
		r->set_spell_lock(false);

		createmenu();

		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_env_draw>::add_callback(on_draw_env, event_prority::high);
	}

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_env_draw>::remove_handler(on_draw_env);
	}

	float erawdamage[] = { 20.f, 30.f, 40.f, 50.f, 60.f };
	float estackdamage[] = { 15.f, 20.f, 25.f, 30.f, 35.f };
	float eDamage(const game_object_script& target)
	{
		if (e->level() == 0) return 0.f;

		auto ebuff = target->get_buff_count(buff_hash("TwitchDeadlyVenom"));
		if (ebuff == 0)
		{
			return 0.f;
		}

		auto ebasedamage = erawdamage[e->level() - 1];
		auto stackdamage = estackdamage[e->level() - 1] + myhero->mPercentBasePhysicalDamageAsFlatBonusMod() * 2.10f + myhero->get_total_ability_power() * 0.30f;

		damage_input input;
		input.raw_physical_damage = ebasedamage + stackdamage * (ebuff);
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float GetDamage(const game_object_script& target)
	{
		auto Damage = 0;

		if (q->is_ready())
		{
			Damage += q->get_damage(target);
		}

		if (w->is_ready())
		{
			Damage += w->get_damage(target);
		}

		if (e->is_ready())
		{
			Damage += eDamage(target);
		}

		if (r->is_ready())
		{
			Damage += r->get_damage(target);
		}

		return Damage;
	}
}