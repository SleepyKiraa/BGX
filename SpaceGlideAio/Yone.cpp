#include "plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "Spaceutils.h"
#include "Yone.h"

namespace yone
{
	script_spell* q;
	script_spell* q3;
	script_spell* w;
	script_spell* e;
	script_spell* r;
	bool canExecute = false;

	TreeTab* mainMenu;
	namespace settings
	{
		namespace qsettings
		{
			TreeEntry* qCombo;
			TreeEntry* q3Combo;
			TreeEntry* q3Turret;
		}

		namespace wsettings
		{
			TreeEntry* wCombo;
		}

		namespace esettings
		{
			TreeEntry* autoEexcute;
			TreeEntry* eBeforeQ3;
			TreeEntry* eBeforeR;
		}

		namespace rsettings
		{
			TreeEntry* rCombo;
			TreeEntry* raoeMin;
			TreeEntry* semiR;
		}

		namespace killsteal
		{
			TreeEntry* ksQ;
			TreeEntry* ksQ3;
			TreeEntry* ksW;
		}

		namespace hitchance
		{
			TreeEntry* qHitChance;
			TreeEntry* q3HitChance;
			TreeEntry* wHitChance;
			TreeEntry* rHitChance;
		}

		namespace draws
		{
			TreeEntry* qRange;
			TreeEntry* wRange;
			TreeEntry* eRange;
			TreeEntry* rRange;
			TreeEntry* qColor;
			TreeEntry* q2Color;
			TreeEntry* qThicness;
			TreeEntry* wColor;
			TreeEntry* w2Color;
			TreeEntry* wThicness;
			TreeEntry* eColor;
			TreeEntry* e2Color;
			TreeEntry* eThicness;
			TreeEntry* rColor;
			TreeEntry* r2Color;
			TreeEntry* rThicness;
			TreeEntry* Damage;
			TreeEntry* NeededAA;
			TreeEntry* BuffTime;
		}

		namespace clear
		{
			TreeEntry* farmtoggle;
			TreeEntry* LcQ;
			TreeEntry* LcW;
			TreeEntry* JcQ;
			TreeEntry* JcW;
		}

		namespace misc
		{
			TreeEntry* Interrupter;
		}
	}

	void createmenu()
	{
		mainMenu = menu->create_tab("space.yone", "[Spaceglide] Yone");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("space.yone.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("space.yone.qsettings", "Mortal Steel - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qCombo = qsettings->add_checkbox("space.yone.useQ", "Enable Q in Combo", true);
				settings::qsettings::q3Combo = qsettings->add_checkbox("space.yone.useQ3", "Enable Q3 in Combo", true);
				settings::qsettings::q3Turret = qsettings->add_hotkey("space.yone.q3turret", "Dont Dash into Turret Range", TreeHotkeyMode::Toggle, 0x47, true, true);
			}

			const auto wsettings = combotab->add_tab("space.yone.wsettings", "Spirit Cleave - [W]");
			wsettings->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			{
				settings::wsettings::wCombo = wsettings->add_checkbox("space.yone.useW", "Enable W in Combo", true);
			}

			const auto esettings = combotab->add_tab("space.yone.esettings", "Soul Unbound - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::autoEexcute = esettings->add_checkbox("space.yone.execute", "Auto E If Enemy is Executable", true);
				settings::esettings::eBeforeQ3 = esettings->add_checkbox("space.yone.eq3", "Use E Before Casting Q3", true);
				settings::esettings::eBeforeR = esettings->add_checkbox("space.yone.er", "Use E Before Casting R", true);
			}

			const auto rsettings = combotab->add_tab("space.yone.rsettings", "Fate Sealed - [R]");
			rsettings->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			{
				settings::rsettings::rCombo = rsettings->add_checkbox("space.yone.useR", "Enable R in Combo", true);
				settings::rsettings::raoeMin = rsettings->add_slider("space.yone.aoehits", "Min Targets to Hit", 2, 1, 5);
				settings::rsettings::semiR = rsettings->add_hotkey("space.yone.semir", "Semi R", TreeHotkeyMode::Hold, 0x54, false, true);
			}
		}

		const auto cleartab = mainMenu->add_tab("space.yone.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("space.yone.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcQ = cleartab->add_checkbox("space.yone.lcq", "Enable Q Lane Clear", true);
			settings::clear::LcW = cleartab->add_checkbox("space.yone.wclear", "Enable W Lane Clear", true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcQ = cleartab->add_checkbox("space.yone.jcq", "Enable Q Jungle Clear", true);
			settings::clear::JcW = cleartab->add_checkbox("space.yone.jcw", "Enable W Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("space.yone.ks", "Killsteal");
		{
			settings::killsteal::ksQ = killstealtab->add_checkbox("space.yone.ksq", "Enable Q Killsteal", true);
			settings::killsteal::ksQ3 = killstealtab->add_checkbox("space.yone.ksq3", "Enable Q3 Killsteal", true);
			settings::killsteal::ksW = killstealtab->add_checkbox("space.yone.ksw", "Enable W Killsteal", true);
		}

		const auto misctab = mainMenu->add_tab("space.yone.misc", "Misc");
		{
			settings::misc::Interrupter = misctab->add_checkbox("space.yone.inter", "Enable Interrupter", true);
		}

		const auto hitchancetab = mainMenu->add_tab("space.yone.hitchance", "Hitchance");
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

		settings::hitchance::qHitChance = hitchancetab->add_combobox("space.yone.qhitchance", "Q Hitchance", combo_elemts, 2);
		settings::hitchance::q3HitChance = hitchancetab->add_combobox("space.yone.q3hitchance", "Q3 Hitchance", combo_elemts, 2);
		settings::hitchance::wHitChance = hitchancetab->add_combobox("space.yone.whitchance", "W Hitchance", combo_elemts, 2);
		settings::hitchance::rHitChance = hitchancetab->add_combobox("space.yone.rhitchance", "R Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("space.yone.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			float defaultcolorQ2[] = { 1.f, 0.f, 1.f, 1.f };
			float defaultcolorW[] = { 0.f, 0.f, 1.f, 1.f };
			float defaultcolorW2[] = { 1.f, 0.f, 1.f, 1.f };
			float defaultcolorE[] = { 0.f, 1.f, 0.f, 1.f };
			float defaultcolorE2[] = { 1.f, 0.f, 1.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };
			float defaultcolorR2[] = { 1.f, 0.f, 1.f, 1.f };

			const auto qdrawtab = drawTab->add_tab("space.yone.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("space.yone.qrange", "Draw Q Range", true);
				settings::draws::qColor = qdrawtab->add_colorpick("space.yone.qcolor", "Q Draw Color", defaultcolorQ);
				settings::draws::q2Color = qdrawtab->add_colorpick("space.yone.q2color", "Q2 Draw Color", defaultcolorQ2);
				settings::draws::qThicness = qdrawtab->add_slider("space.yone.thicknessq", "Q circle Thickness", 2, 1, 6, true);
			}

			const auto wdrawtab = drawTab->add_tab("space.yone.draww", "W Draw Settings");
			{
				settings::draws::wRange = wdrawtab->add_checkbox("space.yone.wrange", "Draw W Range", true);
				settings::draws::wColor = wdrawtab->add_colorpick("space.yone.wcolor", "W Draw Color", defaultcolorW);
				settings::draws::w2Color = wdrawtab->add_colorpick("space.yone.w2color", "W2 Draw Color", defaultcolorW2);
				settings::draws::wThicness = wdrawtab->add_slider("space.yone.thicknessw", "W circle Thickness", 2, 1, 6, true);
			}

			const auto edrawtab = drawTab->add_tab("space.yone.drawe", "E Draw Settings");
			{
				settings::draws::eRange = edrawtab->add_checkbox("space.yone.erange", "Draw E Range", true);
				settings::draws::eColor = edrawtab->add_colorpick("space.yone.ecolor", "E Draw Color", defaultcolorE);
				settings::draws::e2Color = edrawtab->add_colorpick("space.yone.e2color", "E2 Draw Color", defaultcolorE2);
				settings::draws::eThicness = edrawtab->add_slider("space.yone.thicknesse", "E circle Thickness", 2, 1, 6, true);
			}

			const auto rdrawtab = drawTab->add_tab("space.yone.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("space.yone.rrange", "Draw R Range", true);
				settings::draws::rColor = rdrawtab->add_colorpick("space.yone.rcolor", "R Draw Color", defaultcolorR);
				settings::draws::r2Color = rdrawtab->add_colorpick("space.yone.r2color", "R2 Draw Color", defaultcolorR2);
				settings::draws::rThicness = rdrawtab->add_slider("space.yone.thicknessr", "R circle Thickness", 2, 1, 6, true);
			}

			const auto miscdrawTab = drawTab->add_tab("space.yone.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("space.yone.damage", "Draw Damage Indicator", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("space.yone.aaindicator", "Draw AA Counter", true);
				settings::draws::BuffTime = miscdrawTab->add_checkbox("space.yone.bufftext", "Draw Buff Times", true);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Spaceglide] Yone");
		Permashow::Instance.AddElement("Farm", settings::clear::farmtoggle);
		Permashow::Instance.AddElement("Dont Q3 in Tower", settings::qsettings::q3Turret);
		Permashow::Instance.AddElement("Semi R", settings::rsettings::semiR);
	}

	hit_chance getPredIntFromSettings(int hitchance)
	{
		// Get hitchance from settings value
		return static_cast<hit_chance>(hitchance + 3);
	}

	void QLogic()
	{
		auto qtarget = target_selector->get_target(q->range(), damage_type::physical);
		auto q3target = target_selector->get_target(q3->range() + e->range(), damage_type::physical);

		if (myhero->is_winding_up())
		{
			return;
		}

		if (settings::qsettings::q3Turret->get_bool())
		{
			if (q3target != nullptr)
			{
				if (q3target != nullptr && myhero->has_buff(buff_hash("YoneQ3Ready")))
				{
					auto posafterQ3 = myhero->get_position().extend(q3target->get_position(), 475);
					if (posafterQ3.is_under_enemy_turret())
					{
						return;
					}
				}
			}
		}

		if (settings::qsettings::qCombo)
		{
			if (q->is_ready())
			{
				if (!myhero->has_buff(buff_hash("YoneQ3Ready")))
				{
					if (qtarget != nullptr)
					{
						if (qtarget->is_valid())
						{
							auto qpred = q->get_prediction(qtarget);
							if (qpred.hitchance >= getPredIntFromSettings(settings::hitchance::qHitChance->get_int()))
							{
								q->cast(qpred.get_cast_position());
							}
						}
					}
				}

				if (myhero->has_buff(buff_hash("YoneQ3Ready")))
				{
					if (q3target != nullptr)
					{
						if (q3target->is_valid())
						{
							auto q3pred = q3->get_prediction(q3target);
							if (q3pred.hitchance >= getPredIntFromSettings(settings::hitchance::q3HitChance->get_int()))
							{
								if (settings::esettings::eBeforeQ3->get_bool())
								{
									if (!myhero->get_buff(buff_hash("YoneE")))
									{
										e->cast(q3target->get_position());
									}
								}

								q3->cast(q3pred.get_cast_position());
							}
						}
					}
				}
			}
		}
	}

	void WLogic()
	{
		if (settings::wsettings::wCombo->get_bool())
		{
			if (w->is_ready())
			{
				auto wtarget = target_selector->get_target(w->range(), damage_type::physical);
				if (wtarget != nullptr)
				{
					if (wtarget->is_valid())
					{
						auto wpred = w->get_prediction(wtarget);
						if (wpred.hitchance >= getPredIntFromSettings(settings::hitchance::wHitChance->get_int()))
						{
							w->cast(wpred.get_cast_position());
						}
					}
				}
			}
		}
	}

	void AutoE()
	{
		if (settings::esettings::autoEexcute->get_bool())
		{
			if (myhero->has_buff(buff_hash("YoneE")))
			{
				for (auto& enemies : entitylist->get_enemy_heroes())
				{
					if (enemies != nullptr)
					{
						if (!enemies->is_dead())
						{
							if (enemies->has_buff(buff_hash("YoneEDeathmark")))
							{
								if (canExecute)
								{
									e->cast();
									console->print("[Spaceglide] Executed %s", enemies->get_base_skin_name().c_str());
								}
							}
						}
					}
				}
			}
		}
	}

	void RLogic()
	{
		if (settings::rsettings::rCombo->get_bool())
		{
			if (r->is_ready())
			{
				auto rtarget = target_selector->get_target(r->range(), damage_type::physical);
				if (rtarget != nullptr)
				{
					if (rtarget->is_valid())
					{
						auto rpred = r->get_prediction(rtarget);
						if (rpred.hitchance >= getPredIntFromSettings(settings::hitchance::rHitChance->get_int()))
						{
							r->cast(rpred.get_cast_position());
						}
					}
				}
			}
		}
	}

	void SemiR()
	{
		if (r->is_ready())
		{
			auto rtarget = target_selector->get_target(r->range(), damage_type::physical);
			if (rtarget != nullptr)
			{
				if (rtarget->is_valid())
				{
					auto rpred = r->get_prediction(rtarget);
					if (rpred.hitchance >= getPredIntFromSettings(settings::hitchance::rHitChance->get_int()))
					{
						r->cast(rpred.get_cast_position());
					}
				}
			}
		}
	}

	void on_env_draw()
	{
		auto glow = glow_data(0.2, 0.6, 0.2, 0.6);

		if (settings::draws::qRange->get_bool())
		{
			if (q->level() >= 1)
			{
				if (q->name() != "YoneQ3")
				{
					draw_manager->add_circle_with_glow_gradient(myhero->get_position(), settings::draws::qColor->get_color(), settings::draws::q2Color->get_color(), q->range(), settings::draws::qThicness->get_int(), glow);
				}

				if (q->name() == "YoneQ3")
				{
					draw_manager->add_circle_with_glow_gradient(myhero->get_position(), settings::draws::qColor->get_color(), settings::draws::q2Color->get_color(), q3->range(), settings::draws::qThicness->get_int(), glow);
				}
			}
		}

		if (settings::draws::wRange->get_bool())
		{
			if (w->level() >= 1)
			{
				draw_manager->add_circle_with_glow_gradient(myhero->get_position(), settings::draws::wColor->get_color(), settings::draws::w2Color->get_color(), w->range(), settings::draws::wThicness->get_int(), glow);
			}
		}

		if (settings::draws::eRange->get_bool())
		{
			if (e->level() >= 1)
			{
				draw_manager->add_circle_with_glow_gradient(myhero->get_position(), settings::draws::eColor->get_color(), settings::draws::e2Color->get_color(), e->range(), settings::draws::eThicness->get_int(), glow);
			}
		}

		if (settings::draws::rRange->get_bool())
		{
			if (r->level() >= 1)
			{
				draw_manager->add_circle_with_glow_gradient(myhero->get_position(), settings::draws::rColor->get_color(), settings::draws::r2Color->get_color(), r->range(), settings::draws::rThicness->get_int(), glow);
			}
		}
	}

	void on_draw()
	{
		for (const auto& target : entitylist->get_enemy_heroes())
		{
			if (!target->is_valid()) continue;

			if (target->is_visible_on_screen() && target->is_hpbar_recently_rendered() && !target->is_dead())
			{
				if (settings::draws::Damage->get_bool())
				{
					spaceutils::draw_dmg_rl(target, GetDamage(target), MAKE_COLOR(255, 170, 0, 150));
				}

				if (settings::draws::NeededAA->get_bool())
				{
					spaceutils::AATracker();
				}
			}
		}

		if (settings::draws::BuffTime->get_bool())
		{
			auto buff = myhero->get_buff(buff_hash("YoneQ3Ready"));
			if (buff != nullptr)
			{
				int num = (int)buff->get_end() - (int)gametime->get_time();
				std::string numstr = std::to_string(num);
				int firstdigit = std::stoi(numstr.substr(0, 2));
				const auto pos = vector(800.f, 590.f);
				draw_manager->add_text_on_screen(pos, MAKE_COLOR(255, 255, 255, 255), 22, "Q Time: %i", firstdigit);
			}
		}
	}

	void update()
	{
		if (orbwalker->combo_mode())
		{
			RLogic();
			QLogic();
			WLogic();
		}

		if (settings::rsettings::semiR->get_bool())
		{
			myhero->issue_order(hud->get_hud_input_logic()->get_game_cursor_position());
			SemiR();
		}

		AutoE();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 450.f);
		q->set_skillshot(0.25f + 0.066f, 40.f, FLT_MAX, {}, skillshot_type::skillshot_line);
		q->set_spell_lock(false);

		q3 = plugin_sdk->register_spell(spellslot::q, 1050.f);
		q3->set_skillshot(0.25f + 0.066f, 80.f, 1500.f, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		q3->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 600.f);
		w->set_skillshot(0.25f + 0.066f, 150.f, FLT_MAX, {}, skillshot_type::skillshot_cone);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 300.f);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 1000.f);
		r->set_skillshot(0.75f + 0.066f, 100.f, FLT_MAX, {}, skillshot_type::skillshot_line);
		r->set_spell_lock(false);

		createmenu();

		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_env_draw>::add_callback(on_env_draw, event_prority::high);
		event_handler<events::on_draw>::add_callback(on_draw, event_prority::high);
		event_handler<events::on_create_object>::add_callback(oncreate, event_prority::high);
		event_handler<events::on_delete_object>::add_callback(ondelete, event_prority::high);
	}	

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(q3);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		Permashow::Instance.Destroy();

		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_env_draw>::remove_handler(on_env_draw);
		event_handler<events::on_draw>::remove_handler(on_draw);
		event_handler<events::on_create_object>::remove_handler(oncreate);
		event_handler<events::on_delete_object>::remove_handler(ondelete);
	}

	void oncreate(game_object_script obj)
	{
		if (obj->get_emitter())
		{
			if (obj->is_enemy())
			{
				if (obj->get_name().find("Mark_Execute") != std::string::npos)
				{
					canExecute = true;
					console->print("[Spaceglide] Can Execute = true");
				}
			}
		}
	}

	void ondelete(game_object_script obj)
	{
		if (obj->get_emitter())
		{
			if (obj->get_name().find("E_Dash_Head") != std::string::npos)
			{
				canExecute = false;
				console->print("[Spaceglide] Execute Bool Resetted!");
			}
		}
	}

	float qdamage[] = { 20.f, 40.f, 60.f, 80.f, 100.f };

	float QDamage(const game_object_script& target)
	{
		if (q->level() == 0) return 0.f;

		damage_input input;
		input.raw_physical_damage = qdamage[q->level() - 1] + 1.05 * myhero->get_total_attack_damage();
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float rdamage[] = { 200.f, 400.f, 600.f };

	float RDamage(const game_object_script& target)
	{
		if (r->level() == 0) return 0.f;

		damage_input input;
		input.raw_physical_damage = rdamage[r->level() - 1] + 0.80f * (myhero->get_total_attack_damage() - myhero->get_base_attack_damage());
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float GetDamage(const game_object_script& target)
	{
		auto Damage = 0;

		if (q->is_ready())
		{
			Damage += QDamage(target);
		}

		if (w->is_ready())
		{
			Damage += w->get_damage(target);
		}

		if (e->is_ready())
		{
			Damage += e->get_damage(target);
		}

		if (r->is_ready())
		{
			Damage += RDamage(target);
		}

		return Damage;
	}
}