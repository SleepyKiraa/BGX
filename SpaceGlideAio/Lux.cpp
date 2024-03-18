#include "plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "Lux.h"
#include "Spaceutils.h"

namespace lux
{
	script_spell* q;
	script_spell* qsemi;
	script_spell* w;
	script_spell* e;
	script_spell* r;
	game_object_script LuxE = nullptr;
	vector epos;
	bool ecasted = false;

	TreeTab* mainMenu;
	namespace settings
	{
		namespace qsettings
		{
			TreeEntry* qCombo;
			TreeEntry* autoQ;
			TreeEntry* semiQ;
		}

		namespace wsettings
		{
			TreeEntry* AutoWShield;
			TreeEntry* AutoWAllys;
			TreeEntry* MinManaPercent;
			std::map<std::uint32_t, TreeEntry*> wWhiteList;
		}

		namespace esettings
		{
			TreeEntry* eCombo;
		}

		namespace rSettings
		{
			TreeEntry* semiR;
		}

		namespace killsteal
		{
			TreeEntry* ksQ;
			TreeEntry* ksE;
			TreeEntry* ksR;
		}

		namespace hitchance
		{
			TreeEntry* qHitChance;
			TreeEntry* eHitChance;
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
			TreeEntry* Damage;
			TreeEntry* NeededAA;
			TreeEntry* rKillableText;
			TreeEntry* rKillableTextSize;
		}

		namespace clear
		{
			TreeEntry* farmtoggle;
			TreeEntry* LcE;
			TreeEntry* LcEMin;
			TreeEntry* JcQ;
			TreeEntry* JcE;
		}

		namespace misc
		{
			TreeEntry* antigapcloser;
			TreeEntry* antigapcloserHP;
		}
	}

	void createmenu()
	{
		mainMenu = menu->create_tab("space.lux", "[Spaceglide] Lux");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("space.lux.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("space.lux.qsettings", "Light Binding - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qCombo = qsettings->add_checkbox("space.lux.useq", "Enable Q in Combo", true);
				settings::qsettings::autoQ = qsettings->add_checkbox("space.lux.autoq", "Auto Q on Dash/CC", true);
				settings::qsettings::semiQ = qsettings->add_hotkey("space.lux.semiq", "Semi Q", TreeHotkeyMode::Hold, 0x47, false, true);
				settings::qsettings::semiQ->set_tooltip("Semi Q Dosent Have Collision Checks For Minions!");
			}

			const auto wsettings = combotab->add_tab("space.lux.wsettings", "Prismatic Barrier - [W]");
			wsettings->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			{
				settings::wsettings::AutoWShield = wsettings->add_checkbox("space.lux.usew", "Auto W on Incomming Damage", true);
				settings::wsettings::AutoWAllys = wsettings->add_checkbox("space.lux.allyw", "Auto W Allys on Incomming Damage", true);
				settings::wsettings::MinManaPercent = wsettings->add_slider("space.lux.manapercent", "Min Mana % to use W", 50, 1, 100, true);
				auto wwhitelisttab = wsettings->add_tab("space.lux.whitelist", "W Ally Whitelist");
				{
					for (auto&& allys : entitylist->get_ally_heroes())
					{
						if (!allys->is_me())
						{
							settings::wsettings::wWhiteList[allys->get_network_id()] = wwhitelisttab->add_checkbox(std::to_string(allys->get_network_id()), allys->get_model(), true, true);
							settings::wsettings::wWhiteList[allys->get_network_id()]->set_texture(allys->get_square_icon_portrait());
						}
					}
				}
			}

			const auto esettings = combotab->add_tab("space.lux.esettings", "Lucent Singularity - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::eCombo = esettings->add_checkbox("space.lux.usee", "Enable E in Combo", true);
			}

			const auto rsettings = combotab->add_tab("space.lux.rsettings", "Final Spark - [R]");
			rsettings->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			{
				settings::rSettings::semiR = rsettings->add_hotkey("space.lux.semir", "Semi R", TreeHotkeyMode::Hold, 0x54, false, true);
			}
		}

		const auto cleartab = mainMenu->add_tab("space.lux.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("space.lux.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcE = cleartab->add_checkbox("space.lux.lce", "Enable E Lane Clear", true);
			settings::clear::LcEMin = cleartab->add_slider("space.lux.slider", "Min Minions to E", 2, 1, 5, true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcQ = cleartab->add_checkbox("space.lux.jcq", "Enable Q Jungle Clear", true);
			settings::clear::JcE = cleartab->add_checkbox("space.lux.jce", "Enable E Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("space.lux.ks", "Killsteal");
		{
			settings::killsteal::ksQ = killstealtab->add_checkbox("space.lux.ksq", "Enable Q Killsteal", true);
			settings::killsteal::ksE = killstealtab->add_checkbox("space.lux.kse", "Enable E Killsteal", true);
			settings::killsteal::ksR = killstealtab->add_checkbox("space.lux.ksr", "Enable R Killsteal", true);
		}

		const auto misctab = mainMenu->add_tab("space.lux.misc", "Misc");
		{
			settings::misc::antigapcloser = misctab->add_checkbox("space.lux.anti", "Enable Anti Gapcloser", true);
			settings::misc::antigapcloserHP = misctab->add_slider("space.lux.hp", "Min HP % to Anti Gapclose", 50, 1, 100, true);
		}

		const auto hitchancetab = mainMenu->add_tab("space.lux.hitchance", "Hitchance");
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

		settings::hitchance::qHitChance = hitchancetab->add_combobox("space.lux.qhitchance", "Q Hitchance", combo_elemts, 2);
		settings::hitchance::eHitChance = hitchancetab->add_combobox("space.lux.ehitchance", "E Hitchance", combo_elemts, 2);
		settings::hitchance::rHitChance = hitchancetab->add_combobox("space.lux.rhitchance", "R Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("space.lux.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			float defaultcolorQ2[] = { 1.f, 0.f, 1.f, 1.f };
			float defaultcolorW[] = { 0.f, 0.f, 1.f, 1.f };
			float defaultcolorW2[] = { 1.f, 0.f, 1.f, 1.f };
			float defaultcolorE[] = { 0.f, 1.f, 0.f, 1.f };
			float defaultcolorE2[] = { 1.f, 0.f, 1.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };

			const auto qdrawtab = drawTab->add_tab("space.lux.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("space.lux.qrange", "Draw Q Range", true);
				settings::draws::qColor = qdrawtab->add_colorpick("space.lux.qcolor", "Q Draw Color", defaultcolorQ);
				settings::draws::q2Color = qdrawtab->add_colorpick("space.lux.q2color", "Q2 Draw Color", defaultcolorQ2);
				settings::draws::qThicness = qdrawtab->add_slider("space.lux.thicknessq", "Q circle Thickness", 2, 1, 6, true);
			}

			const auto wdrawtab = drawTab->add_tab("space.lux.draww", "W Draw Settings");
			{
				settings::draws::wRange = wdrawtab->add_checkbox("space.lux.wrange", "Draw W Range", true);
				settings::draws::wColor = wdrawtab->add_colorpick("space.lux.wcolor", "W Draw Color", defaultcolorW);
				settings::draws::w2Color = wdrawtab->add_colorpick("space.lux.w2color", "W2 Draw Color", defaultcolorW2);
				settings::draws::wThicness = wdrawtab->add_slider("space.lux.thicknessw", "W circle Thickness", 2, 1, 6, true);
			}

			const auto edrawtab = drawTab->add_tab("space.lux.drawe", "E Draw Settings");
			{
				settings::draws::eRange = edrawtab->add_checkbox("space.lux.erange", "Draw E Range", true);
				settings::draws::eColor = edrawtab->add_colorpick("space.lux.ecolor", "E Draw Color", defaultcolorE);
				settings::draws::e2Color = edrawtab->add_colorpick("space.lux.e2color", "E2 Draw Color", defaultcolorE2);
				settings::draws::eThicness = edrawtab->add_slider("space.lux.thicknesse", "E circle Thickness", 2, 1, 6, true);
			}

			const auto rdrawtab = drawTab->add_tab("space.lux.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("space.lux.rrange", "Draw R Range", true);
				settings::draws::rColor = rdrawtab->add_colorpick("space.lux.rcolor", "R Draw Color", defaultcolorR);
			}

			const auto miscdrawTab = drawTab->add_tab("space.lux.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("space.lux.damage", "Draw Damage Indicator", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("space.lux.aaindicator", "Draw AA Counter", true);
				miscdrawTab->add_separator("sep44", "Killable Text Settings");
				settings::draws::rKillableText = miscdrawTab->add_checkbox("space.lux.killtext", "Draw Killable R Text", true);
				settings::draws::rKillableTextSize = miscdrawTab->add_slider("space.lux.textsize", "Text Size", 22, 5, 35);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Spaceglide] Lux");
		Permashow::Instance.AddElement("Farm", settings::clear::farmtoggle);
		Permashow::Instance.AddElement("Semi Q", settings::qsettings::semiQ);
		Permashow::Instance.AddElement("Semi R", settings::rSettings::semiR);
	}

	hit_chance getPredIntFromSettings(int hitchance)
	{
		// Get hitchance from settings value
		return static_cast<hit_chance>(hitchance + 3);
	}

	void QLogic()
	{
		if (settings::qsettings::qCombo->get_bool())
		{
			if (q->is_ready())
			{
				auto qtarget = target_selector->get_target(q->range(), damage_type::magical);
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
		}
	}

	void SemiQ()
	{
		if (q->is_ready())
		{
			auto qtarget = target_selector->get_target(q->range(), damage_type::magical);
			if (qtarget != nullptr)
			{
				if (qtarget->is_valid())
				{
					auto qpred = qsemi->get_prediction(qtarget);
					if (qpred.hitchance >= getPredIntFromSettings(settings::hitchance::qHitChance->get_int()))
					{
						qsemi->cast(qpred.get_cast_position());
					}
				}
			}
		}
	}

	void WLogicMe()
	{
		if (settings::wsettings::AutoWShield->get_bool())
		{
			if (w->is_ready())
			{
				if (myhero->get_mana_percent() >= settings::wsettings::MinManaPercent->get_int())
				{
					if (health_prediction->get_incoming_damage(myhero, 1.f, true) >= myhero->get_health())
					{
						if (myhero->get_health() + WShield() >= health_prediction->get_incoming_damage(myhero, 1.f, true))
						{
							w->cast(myhero->get_position());
						}
					}
				}
			}
		}
	}

	void WLogicAllys()
	{
		if (settings::wsettings::AutoWAllys->get_bool())
		{
			for (auto allys : entitylist->get_ally_heroes())
			{
				if (allys != nullptr)
				{
					if (allys->is_valid())
					{
						if (wwhitelist(allys))
						{
							if (myhero->get_mana_percent() >= settings::wsettings::MinManaPercent->get_int() && !allys->is_dead())
							{
								if (allys->get_distance(myhero) <= w->range())
								{
									if (health_prediction->get_incoming_damage(allys, 1.f, true) >= allys->get_health())
									{
										if (allys->get_health() + WShield() >= health_prediction->get_incoming_damage(allys, 1.f, true))
										{
											w->cast(allys->get_position());
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	void ELogic()
	{
		if (ecasted == true)
		{
			return;
		}

		if (settings::esettings::eCombo->get_bool())
		{
			if (e->is_ready())
			{
				auto etarget = target_selector->get_target(e->range(), damage_type::magical);
				if (etarget != nullptr)
				{
					if (etarget->is_valid())
					{
						auto epred = e->get_prediction(etarget);
						if (epred.hitchance >= getPredIntFromSettings(settings::hitchance::eHitChance->get_int()))
						{
							e->cast(epred.get_cast_position());
							ecasted = true;
							console->print("[Spaceglide] Lux E Casted = true");
						}
					}
				}
			}
		}
	}

	void E2Logic()
	{
		if (LuxE != nullptr)
		{
			if (epos.count_enemies_in_range(310) > 0)
			{
				e->cast(myhero);
				ecasted = false;
				console->print("[Spaceglide] Lux E Casted = false");
			}
		}
	}

	void SemiR()
	{
		if (r->is_ready())
		{
			auto rtarget = target_selector->get_target(r->range(), damage_type::magical);
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

	void killstealQ(const game_object_script& enemy)
	{
		if (!q->is_ready()) return;
		if (!settings::killsteal::ksQ->get_bool()) return;
		if (!enemy->is_valid_target(q->range())) return;

		if (enemy->get_real_health() <= QDamage(enemy))
		{
			auto qpred = q->get_prediction(enemy);
			if (qpred.hitchance >= getPredIntFromSettings(settings::hitchance::qHitChance->get_int()))
			{
				q->cast(qpred.get_cast_position());
			}
		}
	}

	void killstealE1(const game_object_script& enemy)
	{
		if (!e->is_ready()) return;
		if (!settings::killsteal::ksE->get_bool()) return;
		if (!enemy->is_valid_target(e->range())) return;
		if (ecasted == true) return;

		if (enemy->get_real_health() <= EDamage(enemy))
		{
			auto epred = e->get_prediction(enemy);
			if (epred.hitchance >= getPredIntFromSettings(settings::hitchance::eHitChance->get_int()))
			{
				e->cast(epred.get_cast_position());
			}
		}
	}

	void killstealE2(const game_object_script& enemy)
	{
		if (!e->is_ready()) return;
		if (!settings::killsteal::ksE->get_bool()) return;
		if (!enemy->is_valid_target(e->range())) return;
		if (LuxE == nullptr) return;

		if (enemy->get_real_health() <= EDamage(enemy))
		{
			auto epred = e->get_prediction(enemy);
			if (epred.hitchance >= getPredIntFromSettings(settings::hitchance::eHitChance->get_int()))
			{
				if (epos.count_enemies_in_range(310) > 0)
				{
					e->cast(epred.get_cast_position());
					ecasted = false;
				}
			}
		}
	}

	void killstealR(const game_object_script& enemy)
	{
		if (!r->is_ready()) return;
		if (!settings::killsteal::ksR->get_bool()) return;
		if (!enemy->is_valid_target(r->range())) return;
		if (q->is_ready() && enemy->get_distance(myhero) < q->range() && QDamage(enemy) > enemy->get_real_health()) return;
		if (e->is_ready() && enemy->get_distance(myhero) < e->range() && EDamage(enemy) > enemy->get_real_health()) return;
		if (enemy->get_distance(myhero) < myhero->get_attack_range() + myhero->get_bounding_radius()) return;

		if (enemy->get_real_health() <= RDamage(enemy))
		{
			auto rpred = r->get_prediction(enemy);
			if (rpred.hitchance >= getPredIntFromSettings(settings::hitchance::rHitChance->get_int()))
			{
				r->cast(rpred.get_cast_position());
			}
		}
	}

	void killstealloop()
	{
		for (auto&& enemy : entitylist->get_enemy_heroes())
		{
			if (enemy == nullptr || !enemy->is_valid_target()) continue;

			killstealE1(enemy);
			killstealE2(enemy);
			killstealQ(enemy);
			killstealR(enemy);
		}
	}

	void laneclear()
	{
		if (!settings::clear::farmtoggle->get_bool())
		{
			return;
		}

		if (entitylist->get_enemy_minions().empty())
			return;

		std::vector<game_object_script> minions_list;
		minions_list.reserve(entitylist->get_enemy_minions().size());
		minions_list.insert(minions_list.end(), entitylist->get_enemy_minions().begin(), entitylist->get_enemy_minions().end());

		for (auto& minionE : minions_list)
		{
			if (minionE->get_distance(myhero) > e->range())
				continue;


			if (settings::clear::LcE->get_bool() && e->is_ready())
			{
				e->cast_on_best_farm_position(settings::clear::LcEMin->get_int(), false);
			}
		}
	}

	void jungleclear()
	{
		if (!settings::clear::farmtoggle->get_bool())
		{
			return;
		}

		if (entitylist->get_jugnle_mobs_minions().empty())
			return;

		std::vector<game_object_script> minions_list;
		minions_list.reserve(entitylist->get_jugnle_mobs_minions().size());
		minions_list.insert(minions_list.end(), entitylist->get_jugnle_mobs_minions().begin(), entitylist->get_jugnle_mobs_minions().end());

		for (auto& Jungleminione : minions_list)
		{
			if (Jungleminione->get_distance(myhero) > e->range())
				continue;


			if (settings::clear::JcE->get_bool() && e->is_ready())
			{
				e->cast(Jungleminione->get_position());
			}
		}

		for (auto& Jungleminionq : minions_list)
		{
			if (Jungleminionq->get_distance(myhero) > q->range())
				continue;


			if (settings::clear::JcQ->get_bool() && q->is_ready())
			{
				q->cast(Jungleminionq->get_position());
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
				draw_manager->add_circle_with_glow_gradient(myhero->get_position(), settings::draws::qColor->get_color(), settings::draws::q2Color->get_color(), q->range(), settings::draws::qThicness->get_int(), glow);
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

			if (LuxE != nullptr)
			{
				if (epos.count_enemies_in_range(310) == 0)
				{
					draw_manager->add_circle_with_glow(LuxE->get_position(), MAKE_COLOR(255, 0, 0, 255), 310.f, 2, glow);
				}

				if (epos.count_enemies_in_range(310) > 0)
				{
					draw_manager->add_circle_with_glow(LuxE->get_position(), MAKE_COLOR(0, 255, 0, 255), 310.f, 2, glow);
				}
			}
		}
	}

	void on_draw()
	{
		if (settings::draws::rRange->get_bool())
		{
			if (r->level() >= 1)
			{
				draw_manager->draw_circle_on_minimap(myhero->get_position(), r->range(), settings::draws::rColor->get_color(), 2, 100);
			}
		}

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

		if (settings::draws::rKillableText->get_bool())
		{
			if (r->level() > 0 && r->is_ready())
			{
				int index = 0;
				for (const auto& target : entitylist->get_enemy_heroes())
				{
					if (target != nullptr)
					{
						if (!target->is_dead())
						{
							if (RDamage(target) >= target->get_real_health())
							{
								int num = (int)target->get_real_health();
								std::string numstr = std::to_string(num);
								int firstdigit = std::stoi(numstr.substr(0, 4));
								const auto key = index++;
								const auto position = vector(568.f, 551.f + (key * 30));
								draw_manager->add_text_on_screen(position, MAKE_COLOR(255, 0, 0, 255), settings::draws::rKillableTextSize->get_int(), "%s is Killable HP Left: %i", target->get_base_skin_name().c_str(), firstdigit);
							}
						}
					}
				}
			}
		}
	}


	void update()
	{
		if (orbwalker->combo_mode())
		{
			ELogic();
			QLogic();
		}

		if (orbwalker->lane_clear_mode())
		{
			laneclear();
			jungleclear();
		}

		if (settings::qsettings::semiQ->get_bool())
		{
			myhero->issue_order(hud->get_hud_input_logic()->get_game_cursor_position());

			SemiQ();
		}

		if (settings::rSettings::semiR->get_bool())
		{
			myhero->issue_order(hud->get_hud_input_logic()->get_game_cursor_position());

			SemiR();
		}

		killstealloop();
		autoQ();
		E2Logic();
		WLogicMe();
		WLogicAllys();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 1300.f);
		q->set_skillshot(0.25f + 0.066f, 70.f, 1200.f, { collisionable_objects::yasuo_wall, collisionable_objects::minions }, skillshot_type::skillshot_line);
		q->set_spell_lock(false);

		qsemi = plugin_sdk->register_spell(spellslot::q, 1300.f);
		qsemi->set_skillshot(0.25f + 0.066f, 70.f, 1200.f, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		qsemi->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 1175.f);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 1100.f);
		e->set_skillshot(0.25f + 0.066f, 150.f, 1200.f, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_circle);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 3400.f);
		r->set_skillshot(1.f, 100.f, FLT_MAX, {}, skillshot_type::skillshot_line);
		r->set_spell_lock(false);

		createmenu();

		antigapcloser::add_event_handler(antigap);
		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_create_object>::add_callback(oncreate, event_prority::high);
		event_handler<events::on_delete_object>::add_callback(ondelete, event_prority::high);
		event_handler<events::on_env_draw>::add_callback(on_env_draw, event_prority::high);
		event_handler<events::on_draw>::add_callback(on_draw, event_prority::high);
	}

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(qsemi);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		Permashow::Instance.Destroy();

		antigapcloser::remove_event_handler(antigap);
		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_create_object>::remove_handler(oncreate);
		event_handler<events::on_delete_object>::remove_handler(ondelete);
		event_handler<events::on_env_draw>::remove_handler(on_env_draw);
		event_handler<events::on_draw>::remove_handler(on_draw);
	}

	float qdamage[] = { 80.f, 120.f, 160.f, 200.f, 240.f };

	float QDamage(const game_object_script& target)
	{
		if (q->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = qdamage[q->level() - 1] + 0.60f * myhero->get_total_ability_power();
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float wshield[] = { 40.f, 55.f, 70.f, 85.f, 100.f };
	
	float WShield()
	{
		if (w->level() == 0) return 0.f;
		const float shieldscale = wshield[w->level() - 1] + 0.40 * myhero->get_total_ability_power();
		return shieldscale;
	}

	float edamage[] = { 65.f, 115.f, 165.f, 215.f, 265.f };

	float EDamage(const game_object_script& target)
	{
		if (e->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = edamage[e->level() - 1] + 0.8f * myhero->get_total_ability_power();
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float rdamage[] = { 300.f, 400.f, 500.f };

	float RDamage(const game_object_script& target)
	{
		if (r->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = rdamage[r->level() - 1] + 1.20f * myhero->get_total_ability_power();
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
			Damage += EDamage(target);
		}

		if (r->is_ready())
		{
			Damage += RDamage(target);
		}

		return Damage;
	}

	void oncreate(game_object_script obj)
	{
		if (obj->get_name().find("E_tar_aoe_green") != std::string::npos)
		{
			LuxE = obj;
			epos = obj->get_position();
			console->print("[Spaceglide] Lux E Created");
		}
	}

	void ondelete(game_object_script obj)
	{
		if (obj->get_name().find("E_tar_nova") != std::string::npos)
		{
			LuxE = nullptr;
			ecasted = false;
			console->print("[Spaceglide] Lux E Deleted");
			console->print("[Spaceglide] Lux E Casted = false");
		}
	}

	bool wwhitelist(game_object_script ally)
	{
		auto it = settings::wsettings::wWhiteList.find(ally->get_network_id());
		if (it == settings::wsettings::wWhiteList.end())
			return false;

		return it->second->get_bool();
	}

	void antigap(game_object_script sender, antigapcloser::antigapcloser_args* args)
	{
		if (myhero->get_health_percent() > settings::misc::antigapcloserHP->get_int())
		{
			return;
		}

		if (sender != nullptr)
		{
			if (sender->has_buff_type(buff_type::SpellImmunity) || sender->has_buff_type(buff_type::SpellShield))
			{
				return;
			}

			if (settings::misc::antigapcloser->get_bool())
			{
				if (q->is_ready())
				{
					if (sender->is_enemy() && sender->is_valid())
					{
						if (args->end_position.distance(myhero) < 350.f)
						{
							q->cast(args->end_position);
							console->print("[Spaceglide] Casting Q on Gap Closer End Position Sender: %s", sender->get_base_skin_name().c_str());
						}
					}
				}
			}

		}
	}

	void autoQ()
	{
		if (settings::qsettings::autoQ->get_bool())
		{
			if (q->is_ready())
			{
				auto qtarget = target_selector->get_target(q->range(), damage_type::magical);
				if (qtarget != nullptr)
				{
					if (qtarget->is_valid())
					{
						if (qtarget->has_buff_type(buff_type::Stun) || qtarget->has_buff_type(buff_type::Snare))
						{
							auto qpred = q->get_prediction(qtarget);
							if (qpred.hitchance >= hit_chance::high)
							{
								q->cast(qpred.get_cast_position());
								console->print("[Spaceglide] Casting Q on CC Target: %s", qtarget->get_base_skin_name().c_str());
							}
						}

						auto qpred = q->get_prediction(qtarget);
						if (qtarget->is_dashing())
						{
							if (qpred.hitchance >= hit_chance::high)
							{
								q->cast(qpred.get_cast_position());
								console->print("[Spaceglide] Casting Q on Dash Target: %s", qtarget->get_base_skin_name().c_str());
							}
						}
					}
				}
			}
		}
	}
}