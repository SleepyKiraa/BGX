#include "plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "Xerath.h"
#include "Spaceutils.h"

namespace xerath
{
	script_spell* q;
	script_spell* w;
	script_spell* e;
	script_spell* r;

	TreeTab* mainMenu;
	namespace settings
	{
		namespace qsettings
		{
			TreeEntry* qCombo;
			TreeEntry* waitforW;
			TreeEntry* qMode;
		}

		namespace wsettings
		{
			TreeEntry* wCombo;
			TreeEntry* autoW;
		}

		namespace esettings
		{
			TreeEntry* eCombo;
			TreeEntry* autoE;
		}

		namespace rsettings
		{
			TreeEntry* rCombo;
			TreeEntry* rMode;
			TreeEntry* rTapKey;
			TreeEntry* rCursorRange;
		}

		namespace killsteal
		{
			TreeEntry* ksQ;
			TreeEntry* ksW;
			TreeEntry* ksE;
		}

		namespace hitchance
		{
			TreeEntry* qHitChance;
			TreeEntry* wHitChance;
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
			TreeEntry* r2Color;
			TreeEntry* rThicness;
			TreeEntry* Damage;
			TreeEntry* NeededAA;
			TreeEntry* rKillableText;
			TreeEntry* rkillableTextSize;
			TreeEntry* rcursorrange;
		}

		namespace clear
		{
			TreeEntry* farmtoggle;
			TreeEntry* LcQ;
			TreeEntry* LcW;
			TreeEntry* LcWMinions;
			TreeEntry* JcQ;
			TreeEntry* JcW;
			TreeEntry* JcE;
		}

		namespace misc
		{
			TreeEntry* antigapcloser;
			TreeEntry* antigapcloserHP;
			TreeEntry* Interrupter;
		}
	}	

	void createmenu()
	{
		mainMenu = menu->create_tab("space.xerath", "[Spaceglide] Xerath");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("space.xerath.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("space.xerath.qsettings", "Arcanopulse - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qMode = qsettings->add_combobox("space.xerath.qmode", "Q Mode", { {"Fast", nullptr}, {"Full", nullptr}, {"Mixed", nullptr} }, 2, true);
				settings::qsettings::qCombo = qsettings->add_checkbox("space.xerath.useq", "Enable Q in Combo", true);
				settings::qsettings::waitforW = qsettings->add_checkbox("space.xerath.watiw", "Wait for W", true);
			}

			const auto wsettings = combotab->add_tab("space.xerath.wsettings", "Eye of Destruction - [W]");
			wsettings->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			{
				settings::wsettings::wCombo = wsettings->add_checkbox("space.xerath.usew", "Enable W in Combo", true);
				settings::wsettings::autoW = wsettings->add_checkbox("space.xerath.autow", "Auto W on Dash/CC", true);
			}

			const auto esettings = combotab->add_tab("space.xerath.esettings", "Shocking Orb - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::eCombo = esettings->add_checkbox("space.xerath.usee", "Enable E in Combo", true);
				settings::esettings::autoE = esettings->add_checkbox("space.xerath.autoe", "Auto E on Dash/CC", true);
			}

			const auto rsettings = combotab->add_tab("space.xerath.rsettings", "Rite of the Arcane - [R]");
			rsettings->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			{
				settings::rsettings::rMode = rsettings->add_combobox("space.xerath.rmode", "R Mode", { {"Auto", nullptr}, {"Tap Key", nullptr} }, 1, true);
				settings::rsettings::rCombo = rsettings->add_checkbox("space.xerath.user", "Enable R in Combo", true);
				settings::rsettings::rCursorRange = rsettings->add_slider("space.xerath.range", "Cursor Range", 500, 300, 1000, true);
				settings::rsettings::rTapKey = rsettings->add_hotkey("space.xerath.key", "Tap Key", TreeHotkeyMode::Hold, 0x54, false, true);
			}
		}

		const auto cleartab = mainMenu->add_tab("space.xerath.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("space.xerath.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcQ = cleartab->add_checkbox("space.xerath.lcq", "Enable Q Lane Clear", true);
			settings::clear::LcW = cleartab->add_checkbox("space.xerath.wclear", "Enable W Lane Clear", true);
			settings::clear::LcWMinions = cleartab->add_slider("space.xerath.wmins", "Minions to W Clear", 3, 1, 5, true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcQ = cleartab->add_checkbox("space.xerath.jcq", "Enable Q Jungle Clear", true);
			settings::clear::JcW = cleartab->add_checkbox("space.xerath.jcw", "Enable W Jungle Clear", true);
			settings::clear::JcE = cleartab->add_checkbox("space.xerath.ejclear", "Enable E Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("space.xerath.ks", "Killsteal");
		{
			settings::killsteal::ksQ = killstealtab->add_checkbox("space.xerath.ksq", "Enable Q Killsteal", true);
			settings::killsteal::ksW = killstealtab->add_checkbox("space.xerath.ksw", "Enable W Killsteal", true);
			settings::killsteal::ksE = killstealtab->add_checkbox("space.xerath.kse", "Enable E Killsteal", true);
		}

		const auto misctab = mainMenu->add_tab("space.xerath.misc", "Misc");
		{
			settings::misc::antigapcloser = misctab->add_checkbox("space.xerath.anti", "Enable Anti Gapcloser", true);
			settings::misc::antigapcloserHP = misctab->add_slider("space.xerath.hp", "Min HP % to Anti Gapclose", 50, 1, 100, true);
			settings::misc::Interrupter = misctab->add_checkbox("space.xerath.inter", "Enable Interrupter", true);
		}

		const auto hitchancetab = mainMenu->add_tab("space.xerath.hitchance", "Hitchance");
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

		settings::hitchance::qHitChance = hitchancetab->add_combobox("space.xerath.qhitchance", "Q Hitchance", combo_elemts, 2);
		settings::hitchance::wHitChance = hitchancetab->add_combobox("space.xerath.whitchance", "W Hitchance", combo_elemts, 2);
		settings::hitchance::eHitChance = hitchancetab->add_combobox("space.xerath.ehitchance", "E Hitchance", combo_elemts, 2);
		settings::hitchance::rHitChance = hitchancetab->add_combobox("space.xerath.rhitchance", "R Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("space.xerath.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			float defaultcolorQ2[] = { 1.f, 0.f, 1.f, 1.f };
			float defaultcolorW[] = { 0.f, 0.f, 1.f, 1.f };
			float defaultcolorW2[] = { 1.f, 0.f, 1.f, 1.f };
			float defaultcolorE[] = { 0.f, 1.f, 0.f, 1.f };
			float defaultcolorE2[] = { 1.f, 0.f, 1.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };
			float defaultcolorR2[] = { 1.f, 0.f, 1.f, 1.f };

			const auto qdrawtab = drawTab->add_tab("space.xerath.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("space.xerath.qrange", "Draw Q Range", true);
				settings::draws::qColor = qdrawtab->add_colorpick("space.xerath.qcolor", "Q Draw Color", defaultcolorQ);
				settings::draws::q2Color = qdrawtab->add_colorpick("space.xerath.q2color", "Q2 Draw Color", defaultcolorQ2);
				settings::draws::qThicness = qdrawtab->add_slider("space.xerath.thicknessq", "Q circle Thickness", 2, 1, 6, true);
			}

			const auto wdrawtab = drawTab->add_tab("space.xerath.draww", "W Draw Settings");
			{
				settings::draws::wRange = wdrawtab->add_checkbox("space.xerath.wrange", "Draw W Range", true);
				settings::draws::wColor = wdrawtab->add_colorpick("space.xerath.wcolor", "W Draw Color", defaultcolorW);
				settings::draws::w2Color = wdrawtab->add_colorpick("space.xerath.w2color", "W2 Draw Color", defaultcolorW2);
				settings::draws::wThicness = wdrawtab->add_slider("space.xerath.thicknessw", "W circle Thickness", 2, 1, 6, true);
			}

			const auto edrawtab = drawTab->add_tab("space.xerath.drawe", "E Draw Settings");
			{
				settings::draws::eRange = edrawtab->add_checkbox("space.xerath.erange", "Draw E Range", true);
				settings::draws::eColor = edrawtab->add_colorpick("space.xerath.ecolor", "E Draw Color", defaultcolorE);
				settings::draws::e2Color = edrawtab->add_colorpick("space.xerath.e2color", "E2 Draw Color", defaultcolorE2);
				settings::draws::eThicness = edrawtab->add_slider("space.xerath.thicknesse", "E circle Thickness", 2, 1, 6, true);
			}

			const auto rdrawtab = drawTab->add_tab("space.xerath.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("space.xerath.rrange", "Draw R Range", true);
				settings::draws::rColor = rdrawtab->add_colorpick("space.xerath.rcolor", "R Draw Color", defaultcolorR);
				settings::draws::r2Color = rdrawtab->add_colorpick("space.xerath.r2color", "R2 Draw Color", defaultcolorR2);
				settings::draws::rThicness = rdrawtab->add_slider("space.xerath.thicknessr", "R circle Thickness", 2, 1, 6, true);
				settings::draws::rcursorrange = rdrawtab->add_checkbox("space.xerath.cursor", "Draw Cursor Range", true);
			}

			const auto miscdrawTab = drawTab->add_tab("space.xerath.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("space.xerath.damage", "Draw Damage Indicator", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("space.xerath.aaindicator", "Draw AA Counter", true);
				miscdrawTab->add_separator("sep44", "Killable Text Settings");
				settings::draws::rKillableText = miscdrawTab->add_checkbox("space.xerath.killtext", "Draw Killable R Text", true);
				settings::draws::rkillableTextSize = miscdrawTab->add_slider("space.xerath.textsize", "Text Size", 22, 5, 35);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Spaceglide] Xerath");
		Permashow::Instance.AddElement("Farm", settings::clear::farmtoggle);
		Permashow::Instance.AddElement("R2 Key", settings::rsettings::rTapKey);
	}

	hit_chance getPredIntFromSettings(int hitchance)
	{
		// Get hitchance from settings value
		return static_cast<hit_chance>(hitchance + 3);
	}

	void QLogic()
	{
		auto qtarget = target_selector->get_target(q->charged_max_range, damage_type::magical);

		if (myhero->has_buff(buff_hash("XerathLocusOfPower2")))
		{
			return;
		}

		if (settings::qsettings::waitforW->get_bool())
		{
			if (w->is_ready())
			{
				if (qtarget != nullptr)
				{
					if (qtarget->get_distance(myhero) <= w->range())
					{
						return;
					}
				}
			}
		}

		if (settings::qsettings::qCombo->get_bool())
		{
			if (q->is_ready())
			{
				if (qtarget != nullptr)
				{
					if (qtarget->is_valid())
					{
						if (!myhero->has_buff(buff_hash("XerathArcanopulseChargeUp")))
						{
							if (!q->is_charging())
							{
								q->start_charging();
								return;
							}
						}

						if (myhero->has_buff(buff_hash("XerathArcanopulseChargeUp")))
						{
							auto qpred = q->get_prediction(qtarget);
							if (qpred.hitchance >= getPredIntFromSettings(settings::hitchance::qHitChance->get_int()))
							{
								if (settings::qsettings::qMode->get_int() == 0)
								{
									myhero->update_charged_spell(q->get_slot(), qpred.get_cast_position(), true, false);
								}

								if (settings::qsettings::qMode->get_int() == 1)
								{
									if (q->range() == q->charged_max_range)
									{
										myhero->update_charged_spell(q->get_slot(), qpred.get_cast_position(), true, false);
									}
								}

								if (settings::qsettings::qMode->get_int() == 2)
								{
									if (qtarget->get_distance(myhero) <= q->charged_min_range)
									{
										myhero->update_charged_spell(q->get_slot(), qpred.get_cast_position(), true, false);
									}

									if (!myhero->is_in_auto_attack_range(qtarget))
									{
										if (q->range() == q->charged_max_range)
										{
											myhero->update_charged_spell(q->get_slot(), qpred.get_cast_position(), true, false);
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

	void WLogic()
	{
		if (myhero->has_buff(buff_hash("XerathLocusOfPower2")))
		{
			return;
		}

		if (settings::wsettings::wCombo->get_bool())
		{
			if (w->is_ready())
			{
				auto wtarget = target_selector->get_target(w->range(), damage_type::magical);
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

	void ELogic()
	{
		if (myhero->has_buff(buff_hash("XerathLocusOfPower2")))
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
			if (myhero->has_buff(buff_hash("XerathLocusOfPower2")))
			{
				auto rtarget = target_selector->get_target(r->range(), damage_type::magical);
				if (rtarget != nullptr)
				{
					if (rtarget->is_valid())
					{
						if (rtarget->get_distance(myhero) <= r->range())
						{
							if (hud->get_hud_input_logic()->get_game_cursor_position().distance(rtarget->get_position()) <= settings::rsettings::rCursorRange->get_int())
							{
								auto rpred = r->get_prediction(rtarget);
								if (rpred.hitchance >= getPredIntFromSettings(settings::hitchance::rHitChance->get_int()))
								{
									if (settings::rsettings::rMode->get_int() == 0)
									{
										r->cast(rpred.get_cast_position());
									}

									if (settings::rsettings::rMode->get_int() == 1)
									{
										if (settings::rsettings::rTapKey->get_bool())
										{
											r->cast(rpred.get_cast_position());
										}
									}
								}

								if (rtarget->is_dashing())
								{
									if (rpred.hitchance >= hit_chance::high)
									{
										r->cast(rpred.get_cast_position());
										console->print("Casting R On Dashing Target: %s", rtarget->get_base_skin_name().c_str());
									}
								}
							}
						}
					}
				}
			}
		}
	}

	void killstealQ(const game_object_script& enemy)
	{
		if (!q->is_ready()) return;
		if (!settings::killsteal::ksQ->get_bool()) return;
		if (!enemy->is_valid_target(q->charged_max_range)) return;
		if (myhero->has_buff(buff_hash("XerathLocusOfPower2"))) return;

		if (!myhero->has_buff(buff_hash("XerathArcanopulseChargeUp")))
		{
			if (enemy->get_real_health() <= QDamage(enemy))
			{
				if (!q->is_charging())
				{
					q->start_charging();
					return;
				}
			}
		}

		if (myhero->has_buff(buff_hash("XerathArcanopulseChargeUp")))
		{
			auto qpred = q->get_prediction(enemy);
			if (qpred.hitchance >= getPredIntFromSettings(settings::hitchance::qHitChance->get_int()))
			{
				if (q->range() == q->charged_max_range)
				{
					myhero->update_charged_spell(q->get_slot(), qpred.get_cast_position(), true, false);
				}
			}
		}
	}

	void killstealW(const game_object_script& enemy)
	{
		if (!w->is_ready()) return;
		if (!settings::killsteal::ksW->get_bool()) return;
		if (!enemy->is_valid_target(w->range())) return;
		if (q->is_charging()) return;

		if (enemy->get_real_health() <= WDamage(enemy))
		{
			auto wpred = w->get_prediction(enemy);
			if (wpred.hitchance >= getPredIntFromSettings(settings::hitchance::wHitChance->get_int()))
			{
				w->cast(wpred.get_cast_position());
			}
		}
	}

	void killstealE(const game_object_script& enemy)
	{
		if (!e->is_ready()) return;
		if (!settings::killsteal::ksE->get_bool()) return;
		if (!enemy->is_valid_target(e->range())) return;
		if (q->is_charging()) return;

		if (enemy->get_real_health() <= EDamage(enemy))
		{
			auto epred = e->get_prediction(enemy);
			if (epred.hitchance >= getPredIntFromSettings(settings::hitchance::eHitChance->get_int()))
			{
				e->cast(epred.get_cast_position());
			}
		}
	}

	void killstealloop()
	{
		for (auto&& enemy : entitylist->get_enemy_heroes())
		{
			if (enemy == nullptr || !enemy->is_valid_target()) continue;

			killstealQ(enemy);
			killstealW(enemy);
			killstealE(enemy);			
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

		for (auto& minionQ : minions_list)
		{
			if (minionQ->get_distance(myhero) > q->range())
				continue;

			if (settings::clear::LcQ->get_bool() && q->is_ready())
			{
				if (!myhero->has_buff(buff_hash("XerathArcanopulseChargeUp")))
				{
					if (!q->is_charging())
					{
						q->start_charging();
						return;
					}
				}

				if (myhero->has_buff(buff_hash("XerathArcanopulseChargeUp")))
				{
					if (q->is_charging())
					{
						myhero->update_charged_spell(q->get_slot(), minionQ->get_position(), true, false);
					}
				}
			}					
		}

		for (auto& minionW : minions_list)
		{
			if (minionW->get_distance(myhero) > w->range())
				continue;


			if (settings::clear::LcW->get_bool() && w->is_ready())
			{
				w->cast_on_best_farm_position(settings::clear::LcWMinions->get_int(), false);
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

		for (auto& Jungleminionq : minions_list)
		{
			if (Jungleminionq->get_distance(myhero) > q->range())
				continue;


			if (settings::clear::JcQ->get_bool() && q->is_ready())
			{
				if (!myhero->has_buff(buff_hash("XerathArcanopulseChargeUp")))
				{
					if (!q->is_charging())
					{
						q->start_charging();
						return;
					}
				}				

				if (myhero->has_buff(buff_hash("XerathArcanopulseChargeUp")))
				{
					if (q->is_charging())
					{
						myhero->update_charged_spell(q->get_slot(), Jungleminionq->get_position(), true, false);
					}
				}
			}
		}

		for (auto& Jungleminionw : minions_list)
		{
			if (Jungleminionw->get_distance(myhero) > w->range())
				continue;


			if (settings::clear::JcW->get_bool() && w->is_ready())
			{
				w->cast(Jungleminionw->get_position());
			}
		}

		for (auto& Jungleminione : minions_list)
		{
			if (Jungleminione->get_distance(myhero) > e->range())
				continue;


			if (settings::clear::JcE->get_bool() && e->is_ready())
			{
				e->cast(Jungleminione->get_position());
			}
		}
	}

	void on_draw_env()
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
		}

		if (settings::draws::rcursorrange->get_bool())
		{
			if (myhero->has_buff(buff_hash("XerathLocusOfPower2")))
			{
				draw_manager->add_circle_with_glow_gradient(hud->get_hud_input_logic()->get_game_cursor_position(), settings::draws::rColor->get_color(), settings::draws::r2Color->get_color(), settings::rsettings::rCursorRange->get_int(), settings::draws::rThicness->get_int(), glow);
			}
		}
	}

	void on_draw_real()
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
							if (r->level() == 1)
							{
								if (RDamage(target) * 4 >= target->get_real_health())
								{
									int num = (int)target->get_real_health();
									std::string numstr = std::to_string(num);
									int firstdigit = std::stoi(numstr.substr(0, 4));
									const auto key = index++;
									const auto position = vector(568.f, 551.f + (key * 30));
									draw_manager->add_text_on_screen(position, MAKE_COLOR(255, 0, 0, 255), settings::draws::rkillableTextSize->get_int(), "%s is Killable HP Left: %i", target->get_base_skin_name().c_str(), firstdigit);
								}
							}

							if (r->level() == 2)
							{
								if (RDamage(target) * 5 >= target->get_real_health())
								{
									int num = (int)target->get_real_health();
									std::string numstr = std::to_string(num);
									int firstdigit = std::stoi(numstr.substr(0, 4));
									const auto key = index++;
									const auto position = vector(568.f, 551.f + (key * 30));
									draw_manager->add_text_on_screen(position, MAKE_COLOR(255, 0, 0, 255), settings::draws::rkillableTextSize->get_int(), "%s is Killable HP Left: %i", target->get_base_skin_name().c_str(), firstdigit);
								}
							}

							if (r->level() == 3)
							{
								if (RDamage(target) * 6 >= target->get_real_health())
								{
									int num = (int)target->get_real_health();
									std::string numstr = std::to_string(num);
									int firstdigit = std::stoi(numstr.substr(0, 4));
									const auto key = index++;
									const auto position = vector(568.f, 551.f + (key * 30));
									draw_manager->add_text_on_screen(position, MAKE_COLOR(255, 0, 0, 255), settings::draws::rkillableTextSize->get_int(), "%s is Killable HP Left: %i", target->get_base_skin_name().c_str(), firstdigit);
								}
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
			WLogic();
			ELogic();
			QLogic();
		}

		if (orbwalker->lane_clear_mode())
		{
			laneclear();
			jungleclear();
		}

		RLogic();
		AutoW();
		AutoE();
		Interrupter();
		killstealloop();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 735.f);
		q->set_skillshot(0.55f + 0.066f, 70.f, FLT_MAX, {}, skillshot_type::skillshot_line);
		q->set_charged(735.f, 1450.f, 1.7f);
		q->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 1000.f);
		w->set_skillshot(0.6f + 0.066f, 125.f, FLT_MAX, {}, skillshot_type::skillshot_circle);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 1125.f);
		e->set_skillshot(0.25f + 0.066f, 60.f, 1400.f, { collisionable_objects::yasuo_wall, collisionable_objects::minions }, skillshot_type::skillshot_line);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 5000.f);
		r->set_skillshot(0.25f + 0.066f, 100.f, FLT_MAX, {}, skillshot_type::skillshot_circle);
		r->set_spell_lock(false);

		createmenu();

		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_env_draw>::add_callback(on_draw_env, event_prority::high);
		event_handler<events::on_draw>::add_callback(on_draw_real, event_prority::high);
		event_handler<events::on_process_spell_cast>::add_callback(onprocess, event_prority::high);
		antigapcloser::add_event_handler(antigap);
	}

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		Permashow::Instance.Destroy();

		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_env_draw>::remove_handler(on_draw_env);
		event_handler<events::on_draw>::remove_handler(on_draw_real);
		event_handler<events::on_process_spell_cast>::remove_handler(onprocess);
		antigapcloser::remove_event_handler(antigap);
	}

	float qdamage[] = { 70.f, 110.f, 150.f, 190.f, 230.f };

	float QDamage(const game_object_script& target)
	{
		if (q->level() == 0) return 0.f;
		damage_input input;
		input.raw_magical_damage = qdamage[q->level() - 1] + 0.85f * myhero->get_total_ability_power();
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float wdamage[] = { 100.02f, 158.365f, 216.71f, 275.055f, 333.4f };

	float WDamage(const game_object_script& target)
	{
		if (w->level() == 0) return 0.f;
		damage_input input;
		input.raw_magical_damage = wdamage[w->level() - 1] + 1.f * myhero->get_total_ability_power();
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float edamage[] = { 80.f, 110.f, 140.f, 170.f, 200.f };

	float EDamage(const game_object_script& target)
	{
		if (e->level() == 0) return 0.f;
		damage_input input;
		input.raw_magical_damage = edamage[e->level() - 1] + 0.45f * myhero->get_total_ability_power();
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float rdamage[] = { 180.f, 230.f, 280.f };
	float rampupdamage[] = { 20.f, 25.f, 30.f };

	float RDamage(const game_object_script& target)
	{
		if (r->level() == 0) return 0.f;
		damage_input input;
		input.raw_magical_damage = rdamage[r->level() - 1] + 0.40f * myhero->get_total_ability_power() + rampupdamage[r->level() - 1]  * myhero->get_buff_count(buff_hash("xerathrrampup"));
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
			Damage += WDamage(target);
		}

		if (e->is_ready())
		{
			Damage += EDamage(target);
		}

		if (r->is_ready())
		{
			if (r->level() == 1)
			{
				Damage += RDamage(target) * 4;
			}
			else if (r->level() == 2)
			{
				Damage += RDamage(target) * 5;
			}
			else if (r->level() == 3)
			{
				Damage += RDamage(target) * 6;
			}
		}

		return Damage;
	}

	void AutoW()
	{
		if (settings::wsettings::autoW->get_bool())
		{
			if (w->is_ready())
			{
				auto wtarget = target_selector->get_target(w->range(), damage_type::magical);
				if (wtarget != nullptr)
				{
					if (wtarget->is_valid())
					{
						if (wtarget->has_buff_type(buff_type::Stun) || wtarget->has_buff_type(buff_type::Snare))
						{
							w->cast(wtarget->get_position());
							console->print("[Spaceglide] Casting W on CC Target: %s", wtarget->get_base_skin_name().c_str());
						}

						auto wpred = w->get_prediction(wtarget);
						if (wtarget->is_dashing())
						{
							if (wpred.hitchance >= hit_chance::high)
							{
								w->cast(wpred.get_cast_position());
								console->print("[Spaceglide] Casting W on Dash Target: %s", wtarget->get_base_skin_name().c_str());
							}
						}
					}
				}
			}
		}
	}

	void AutoE()
	{
		if (settings::esettings::autoE->get_bool())
		{
			if (e->is_ready())
			{
				auto etarget = target_selector->get_target(e->range(), damage_type::magical);
				if (etarget != nullptr)
				{
					if (etarget->is_valid())
					{
						if (etarget->has_buff_type(buff_type::Stun) || etarget->has_buff_type(buff_type::Snare))
						{
							auto epred = e->get_prediction(etarget);
							if (epred.hitchance >= hit_chance::high)
							{
								e->cast(epred.get_cast_position());
							}
							console->print("[Spaceglide] Casting E on CC Target: %s", etarget->get_base_skin_name().c_str());
						}

						auto epred = e->get_prediction(etarget);
						if (etarget->is_dashing())
						{
							if (epred.hitchance >= hit_chance::high)
							{
								e->cast(epred.get_cast_position());
								console->print("[Spaceglide] Casting E on Dash Target: %s", etarget->get_base_skin_name().c_str());
							}
						}
					}
				}
			}
		}
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
		}

		if (settings::misc::antigapcloser->get_bool())
		{
			if (e->is_ready())
			{
				if (sender->is_enemy() && sender->is_valid())
				{
					if (args->end_position.distance(myhero) <= 350)
					{
						e->cast(args->end_position);
						console->print("[Spaceglide] Casting E on Gap Closer End Position Sender: %s", sender->get_base_skin_name().c_str());
					}
				}
			}
		}
	}

	void Interrupter()
	{
		if (settings::misc::Interrupter->get_bool())
		{
			for (auto& enemy : entitylist->get_enemy_heroes())
			{
				if (e->is_ready())
				{
					if (enemy->is_enemy() && enemy->is_casting_interruptible_spell() == 2)
					{
						if (enemy->get_distance(myhero) <= e->range())
						{
							auto epred = e->get_prediction(enemy);
							if (epred.hitchance >= hit_chance::high)
							{
								e->cast(epred.get_cast_position());
								console->print("[Spaceglide] Interrupting %s", enemy->get_base_skin_name().c_str());
							}
						}
					}
				}
			}
		}
	}

	void onprocess(game_object_script sender, spell_instance_script spell)
	{
		if (settings::misc::antigapcloser->get_bool())
		{
			if (e->is_ready())
			{
				if (sender != nullptr)
				{
					if (sender->get_base_skin_name() == "Alistar")
					{
						if (spell->get_spellslot() == spellslot::w)
						{
							if (spell->get_last_target_id() == myhero->get_id())
							{
								e->cast(sender);
								console->print("[Spaceglide] Casting E on Alistar W");
							}
						}
					}

					if (sender->get_base_skin_name() == "Jax")
					{
						if (spell->get_spellslot() == spellslot::q)
						{
							if (spell->get_last_target_id() == myhero->get_id())
							{
								e->cast(sender);
								console->print("[Spaceglide] Casting E on Jax Q");
							}
						}
					}

					if (sender->get_base_skin_name() == "Pantheon")
					{
						if (spell->get_spellslot() == spellslot::w)
						{
							if (spell->get_last_target_id() == myhero->get_id())
							{
								e->cast(sender);
								console->print("[Spaceglide] Casting E on Pantheon W");
							}
						}
					}

					if (sender->get_base_skin_name() == "Briar")
					{
						if (spell->get_spellslot() == spellslot::q)
						{
							if (spell->get_last_target_id() == myhero->get_id())
							{
								e->cast(sender);
								console->print("[Spaceglide] Casting E on Briar Q");
							}
						}
					}
				}
			}
		}
	}
}