#include "../plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "femboyutils.h"
#include "Xerath.h"

namespace xerath
{
	script_spell* q;
	script_spell* w;
	script_spell* e;
	script_spell* r;

	TreeTab* mainMenu;
	namespace settings {
		namespace qsettings
		{
			TreeEntry* qCombo;
			TreeEntry* qWaitForW;
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
			TreeEntry* r2Combo;
			TreeEntry* r2mode;
			TreeEntry* disableEvade;
			TreeEntry* r2Key;
			TreeEntry* r2cursorRange;
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
			TreeEntry* r2HitChance;
		}

		namespace draws {
			TreeEntry* qRange;
			TreeEntry* wRange;
			TreeEntry* eRange;
			TreeEntry* rRange;
			TreeEntry* Qcolor;
			TreeEntry* Wcolor;
			TreeEntry* Ecolor;
			TreeEntry* Rcolor;
			TreeEntry* Damage;
			TreeEntry* KillableText;
			TreeEntry* legCircles;
			TreeEntry* drawcursorrange;
			TreeEntry* TextSize;
			TreeEntry* NeededAA;
		}

		namespace clear
		{
			TreeEntry* farmtoggle;
			TreeEntry* LcQ;
			TreeEntry* LcW;
			TreeEntry* LcWminions;
			TreeEntry* JcQ;
			TreeEntry* JcW;
		}

		namespace misc
		{
			TreeEntry* antigap;
			TreeEntry* antigaphp;
			TreeEntry* interrupter;
			TreeEntry* AutoBlueward;
		}
	}

	void createmenu()
	{
		mainMenu = menu->create_tab("femboy.Xerath", "[Femboy] Xerath");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("femboy.Xerath.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("femboy.Xerath.qsettings", "Arcanopulse - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qCombo = qsettings->add_checkbox("femboy.Xerath.useq", "Enable Q in Combo", true);
				settings::qsettings::qWaitForW = qsettings->add_checkbox("femboy.Xerath.waitw", "Wait for W before Casting Q", true);
				settings::qsettings::qMode = qsettings->add_combobox("femboy.Xerath.qmode", "Q Mode", { {"Fast", nullptr}, {"Full", nullptr}, {"Mixed", nullptr} }, 2, true);
			}

			const auto wsettings = combotab->add_tab("femboy.Xerath.wsettings", "Eye of Destruction - [W]");
			wsettings->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			{
				settings::wsettings::wCombo = wsettings->add_checkbox("femboy.Xerath.usew", "Enable W in Combo", true);
				settings::wsettings::autoW = wsettings->add_checkbox("femboy.Xerath.autow", "Enable Auto W on Dash/CC", true);
			}

			const auto esettings = combotab->add_tab("femboy.Xerath.esettings", "Shocking Orb - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::eCombo = esettings->add_checkbox("femboy.Xerath.usee", "Enable E in Combo", true);
				settings::esettings::autoE = esettings->add_checkbox("femboy.Xerath.autoe", "Enable Auto E on Dash/CC", true);
			}

			const auto rsettings = combotab->add_tab("femboy.Xerath.rsettings", "Rite of the Arcane - [R]");
			rsettings->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			{
				settings::rsettings::r2Combo = rsettings->add_checkbox("femboy.Xerath.user", "Enable R Usage", true);
				settings::rsettings::disableEvade = rsettings->add_checkbox("femboy.Xerath.disevade", "Disable Evade while in R", false);
				settings::rsettings::r2cursorRange = rsettings->add_slider("femboy.Xerath.range", "Cursor Range to get Target", 1000, 500, 2000, true);
				settings::rsettings::r2Key = rsettings->add_hotkey("femboy.Xerath.key", "R2 Tap Key", TreeHotkeyMode::Hold, 0x54, false, true);
				settings::rsettings::r2mode = rsettings->add_combobox("femboy.Xerath.rmode", "R Mode", { {"Auto", nullptr}, {"Tap Key", nullptr} }, 1);
			}
		}

		const auto cleartab = mainMenu->add_tab("femboy.Xerath.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("femboy.Xerath.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcQ = cleartab->add_checkbox("femboy.Xerath.qclear", "Enable Q Lane Clear", true);
			settings::clear::LcW = cleartab->add_checkbox("femboy.Xerath.wclear", "Enable W Lane Clear", true);
			settings::clear::LcWminions = cleartab->add_slider("femboy.Xerath.wmin", "Min Minions to W Clear", 2, 1, 5);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcQ = cleartab->add_checkbox("femboy.Xerath.qjclear", "Enable Q Jungle Clear", true);
			settings::clear::JcW = cleartab->add_checkbox("femboy.Xerath.wjclear", "Enable W Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("femboy.Xerath.ks", "Killsteal");
		{
			settings::killsteal::ksQ = killstealtab->add_checkbox("femboy.Xerath.ksq", "Enable Q Killsteal", true);
			settings::killsteal::ksW = killstealtab->add_checkbox("femboy.Xerath.ksw", "Enable W Killsteal", true);
			settings::killsteal::ksE = killstealtab->add_checkbox("femboy.Xerath.kse", "Enable E Killsteal", true);
		}

		const auto misctab = mainMenu->add_tab("femboy.Xerath.misc", "Misc");
		{
			settings::misc::antigap = misctab->add_checkbox("femboy.Xerath.anti", "Enable Anti Gapcloser", true);
			settings::misc::antigaphp = misctab->add_slider("femboy.Xerath.hp", "Min HP % to Anti Gapclose", 50, 1, 100, true);
			settings::misc::interrupter = misctab->add_checkbox("femboy.Xerath.inter", "Enable Interrupter", true);
		}

		const auto hitchancetab = mainMenu->add_tab("femboy.Xerath.hitchance", "Hitchance");
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

		settings::hitchance::qHitChance = hitchancetab->add_combobox("femboy.Xerath.qhitchance", "Q Hitchance", combo_elemts, 2);
		settings::hitchance::wHitChance = hitchancetab->add_combobox("femboy.Xerath.whitchance", "W Hitchance", combo_elemts, 2);
		settings::hitchance::eHitChance = hitchancetab->add_combobox("femboy.Xerath.ehitchance", "E Hitchance", combo_elemts, 2);
		settings::hitchance::r2HitChance = hitchancetab->add_combobox("femboy.Xerath.rhitchance", "R Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("femboy.Xerath.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			float defaultcolorW[] = { 0.f, 0.f, 1.f, 1.f };
			float defaultcolorE[] = { 0.f, 1.f, 0.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };
			const auto qdrawtab = drawTab->add_tab("femboy.Xerath.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("femboy.Xerath.qrange", "Draw Q Range", true);
				settings::draws::Qcolor = qdrawtab->add_colorpick("femboy.Xerath.qcolor", "Q Draw Color", defaultcolorQ);
			}

			const auto wdrawtab = drawTab->add_tab("femboy.Xerath.draww", "W Draw Settings");
			{
				settings::draws::wRange = wdrawtab->add_checkbox("femboy.Xerath.wrange", "Draw W Range", true);
				settings::draws::Wcolor = wdrawtab->add_colorpick("femboy.Xerath.wcolor", "W Draw Color", defaultcolorW);

			}

			const auto edrawtab = drawTab->add_tab("femboy.Xerath.drawe", "E Draw Settings");
			{
				settings::draws::eRange = edrawtab->add_checkbox("femboy.Xerath.erange", "Draw E Range", true);
				settings::draws::Ecolor = edrawtab->add_colorpick("femboy.Xerath.ecolor", "E Draw Color", defaultcolorE);
			}

			const auto rdrawtab = drawTab->add_tab("femboy.Xerath.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("femboy.Xerath.rrange", "Draw R Range", true);
				settings::draws::drawcursorrange = rdrawtab->add_checkbox("femboy.Xerath.cusorrange", "Draw Cursor Range", true);
				settings::draws::Rcolor = rdrawtab->add_colorpick("femboy.Xerath.rcolor", "R Draw Color", defaultcolorR);
			}

			const auto miscdrawTab = drawTab->add_tab("femboy.Xerath.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("femboy.Xerath.damage", "Draw Damage Indicator", true);
				settings::draws::legCircles = miscdrawTab->add_checkbox("femboy.Xerath.3dcircle", "Enable 3D Circle", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("femboy.Xerath.aaindicator", "Draw AA Counter", true);
				miscdrawTab->add_separator("seperator9", "Kill Text Settings");
				settings::draws::KillableText = miscdrawTab->add_checkbox("femboy.Xerath.drawkill", "Draw Killable Text", true);
				settings::draws::TextSize = miscdrawTab->add_slider("femboy.Xerath.textsize", "Text Size", 22, 5, 35);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Femboy] Xerath");
		Permashow::Instance.AddElement("Farm", settings::clear::farmtoggle);
		Permashow::Instance.AddElement("R2 Key", settings::rsettings::r2Key);
	}

	hit_chance getPredIntFromSettings(int hitchance)
	{
		// Get hitchance from settings value
		return static_cast<hit_chance>(hitchance + 3);
	}

	void drawCircle(vector pos, int radius, int quality, bool legsense, unsigned long color, int thickness = 1)
	{
		if (legsense)
		{
			draw_manager->add_circle_with_glow(pos, color, radius, thickness, glow_data(0.1f, 0.75f, 0.f, 1.f));
			return;
		}
		const auto points = geometry::geometry::circle_points(pos, radius, quality);
		for (int i = 0; i < points.size(); i++)
		{
			const int next_index = (i + 1) % points.size();
			const auto start = points[i];
			const auto end = points[next_index];

			vector screenPosStart;
			renderer->world_to_screen(start, screenPosStart);
			vector screenPosEnd;
			renderer->world_to_screen(end, screenPosEnd);
			if (!renderer->is_on_screen(screenPosStart, 50) && !renderer->is_on_screen(screenPosEnd, 50))
				continue;

			draw_manager->add_line(points[i].set_z(pos.z), points[next_index].set_z(pos.z), color, thickness);
		}
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

	void QLogic()
	{
		auto qtarget = target_selector->get_target(q->charged_max_range, damage_type::magical);

		if (settings::qsettings::qWaitForW->get_bool())
		{
			if (qtarget != nullptr)
			{
				if (w->is_ready() && qtarget->get_distance(myhero) < w->range())
				{
					return;
				}
			}
		}

		if (settings::qsettings::qCombo->get_bool())
		{
			if (!myhero->has_buff(buff_hash("XerathLocusOfPower2")))
			{
				if (q->is_ready())
				{
					if (qtarget != nullptr)
					{
						if (qtarget->is_valid_target())
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
								//auto& p = q2PredictionList[qtarget->get_handle()];
								auto p = q->get_prediction(qtarget);
								if (p.hitchance >= getPredIntFromSettings(settings::hitchance::qHitChance->get_int()))
								{
									if (settings::qsettings::qMode->get_int() == 0)
									{
										myhero->update_charged_spell(q->get_slot(), p.get_cast_position(), true, false);
									}

									if (settings::qsettings::qMode->get_int() == 1)
									{
										if (q->range() == q->charged_max_range)
										{
											myhero->update_charged_spell(q->get_slot(), p.get_cast_position(), true, false);
										}
									}

									if (settings::qsettings::qMode->get_int() == 2)
									{
										if (myhero->is_in_auto_attack_range(qtarget))
										{
											myhero->update_charged_spell(q->get_slot(), p.get_cast_position(), true, false);
										}

										if (!myhero->is_in_auto_attack_range(qtarget))
										{
											if (q->range() == q->charged_max_range)
											{
												myhero->update_charged_spell(q->get_slot(), p.get_cast_position(), true, false);
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
	}

	void WLogic()
	{
		if (myhero->has_buff(buff_hash("XerathArcanopulseChargeUp")))
		{
			return;
		}

		if (settings::wsettings::wCombo->get_bool())
		{
			if (!myhero->has_buff(buff_hash("XerathLocusOfPower2")))
			{
				if (w->is_ready())
				{
					auto wtarget = target_selector->get_target(w->range(), damage_type::magical);
					if (wtarget != nullptr)
					{
						if (wtarget->is_valid_target())
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
	}

	void ELogic()
	{
		if (myhero->has_buff(buff_hash("XerathArcanopulseChargeUp")))
		{
			return;
		}

		if (!myhero->has_buff(buff_hash("XerathLocusOfPower2")))
		{
			if (settings::esettings::eCombo->get_bool())
			{
				if (e->is_ready())
				{
					auto etarget = target_selector->get_target(e->range(), damage_type::magical);
					if (etarget != nullptr)
					{
						if (etarget->is_valid_target())
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
	}

	void autoW()
	{
if (myhero->has_buff(buff_hash("XerathLocusOfPower2")))
{
	return;
}

if (myhero->has_buff(buff_hash("XerathArcanopulseChargeUp")))
{
	return;
}

if (settings::wsettings::autoW->get_bool())
{
	if (w->is_ready())
	{
		auto wtarget = target_selector->get_target(w->range(), damage_type::magical);
		if (wtarget != nullptr)
		{
			if (wtarget->is_valid_target())
			{
				if (wtarget->is_dashing())
				{
					auto wpred = w->get_prediction(wtarget);
					if (wpred.hitchance >= hit_chance::dashing)
					{
						w->cast(wpred.get_cast_position());
						console->print("[FemboyAio] Casting W on Dash");
					}
				}

				if (wtarget->has_buff_type(buff_type::Stun) || wtarget->has_buff_type(buff_type::Snare))
				{
					auto wpred = w->get_prediction(wtarget);
					if (wpred.hitchance >= hit_chance::immobile)
					{
						w->cast(wpred.get_cast_position());
						console->print("[FemboyAio] Casting W on CC");
					}
				}
			}
		}
	}
}
	}

	void autoE()
	{
		if (myhero->has_buff(buff_hash("XerathLocusOfPower2")))
		{
			return;
		}

		if (myhero->has_buff(buff_hash("XerathArcanopulseChargeUp")))
		{
			return;
		}

		if (settings::esettings::autoE->get_bool())
		{
			if (e->is_ready())
			{
				auto etarget = target_selector->get_target(e->range(), damage_type::magical);
				if (etarget != nullptr)
				{
					if (etarget->is_valid_target())
					{
						if (etarget->is_dashing())
						{
							auto epred = e->get_prediction(etarget);
							if (epred.hitchance >= hit_chance::dashing)
							{
								e->cast(epred.get_cast_position());
								console->print("[FemboyAio] Casting E On Dash");
							}
						}

						if (etarget->has_buff_type(buff_type::Stun) || etarget->has_buff_type(buff_type::Snare))
						{
							auto epred = e->get_prediction(etarget);
							if (epred.hitchance >= hit_chance::immobile)
							{
								e->cast(epred.get_cast_position());
								console->print("[FemboyAio] Casting E on CC");
							}
						}
					}
				}
			}
		}
	}

	void RLogic()
	{
		if (settings::rsettings::r2Combo->get_bool())
		{
			if (myhero->has_buff(buff_hash("XerathLocusOfPower2")))
			{
				auto rtarget = target_selector->get_target(r->range(), damage_type::magical);
				if (rtarget != nullptr)
				{
					if (rtarget->get_distance(myhero) <= r->range())
					{
						if (hud->get_hud_input_logic()->get_game_cursor_position().distance(rtarget->get_position()) <= settings::rsettings::r2cursorRange->get_int())
						{
							auto rpred = r->get_prediction(rtarget);
							if (rpred.hitchance >= getPredIntFromSettings(settings::hitchance::r2HitChance->get_int()))
							{
								if (settings::rsettings::r2mode->get_int() == 0)
								{
									r->cast(rpred.get_cast_position());
								}

								if (settings::rsettings::r2mode->get_int() == 1)
								{
									if (settings::rsettings::r2Key->get_bool())
									{
										r->cast(rpred.get_cast_position());
									}
								}
							}

							if (rpred.hitchance >= hit_chance::dashing)
							{
								r->cast(rpred.get_cast_position());
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

		if (enemy->get_real_health() <= qDamage(enemy))
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
				auto p = q->get_prediction(enemy);
				if (p.hitchance >= getPredIntFromSettings(settings::hitchance::qHitChance->get_int()))
				{				
					if (myhero->is_in_auto_attack_range(enemy))
					{
						myhero->update_charged_spell(q->get_slot(), p.get_cast_position(), true, false);
					}

					if (!myhero->is_in_auto_attack_range(enemy))
					{
						if (q->range() == q->charged_max_range)
						{
							myhero->update_charged_spell(q->get_slot(), p.get_cast_position(), true, false);
						}
					}
				}
			}
		}
	}

	void killstealW(const game_object_script& enemy)
	{
		if (!w->is_ready()) return;
		if (!settings::killsteal::ksW->get_bool()) return;
		if (!enemy->is_valid_target(w->range())) return;
		if (myhero->has_buff(buff_hash("XerathLocusOfPower2"))) return;

		if (enemy->get_real_health() <= wDamage(enemy))
		{
			auto wpred = w->get_prediction(enemy);
			if (wpred.hitchance >= hit_chance::high)
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
		if (myhero->has_buff(buff_hash("XerathLocusOfPower2"))) return;

		if (enemy->get_real_health() <= eDamage(enemy))
		{
			auto epred = e->get_prediction(enemy);
			if (epred.hitchance >= hit_chance::high)
			{
				e->cast(epred.get_cast_position());
			}
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
					myhero->update_charged_spell(q->get_slot(), minionQ->get_position(), true, false);
				}
			}
		}

		for (auto& minionW : minions_list)
		{
			if (minionW->get_distance(myhero) > w->range())
				continue;


			if (settings::clear::LcW->get_bool() && w->is_ready())
			{
				w->cast_on_best_farm_position(settings::clear::LcWminions->get_int(), false);
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
					myhero->update_charged_spell(q->get_slot(), Jungleminionq->get_position(), true, false);
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

	void on_env_draw()
	{
		if (settings::draws::qRange->get_bool())
		{
			if (q->level() >= 1)
			{
				drawCircle(myhero->get_position(), q->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::Qcolor->get_color(), 2);
			}
		}

		if (settings::draws::wRange->get_bool())
		{
			if (w->level() >= 1)
			{
				drawCircle(myhero->get_position(), w->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::Wcolor->get_color(), 2);
			}
		}

		if (settings::draws::eRange->get_bool())
		{
			if (e->level() >= 1)
			{
				drawCircle(myhero->get_position(), e->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::Ecolor->get_color(), 2);
			}
		}

		if (settings::draws::drawcursorrange->get_bool())
		{
			if (myhero->has_buff(buff_hash("XerathLocusOfPower2")))
			{
				drawCircle(hud->get_hud_input_logic()->get_game_cursor_position(), settings::rsettings::r2cursorRange->get_int(), 100, settings::draws::legCircles->get_bool(), settings::draws::Rcolor->get_color(), 1);
			}
		}
	}

	void on_draw_real()
	{
		if (settings::draws::rRange->get_bool())
		{
			if (r->level() >= 1)
			{
				draw_manager->draw_circle_on_minimap(myhero->get_position(), r->range(), settings::draws::Rcolor->get_color(), 2, 100);
			}
		}

		for (const auto& target : entitylist->get_enemy_heroes())
		{
			if (!target->is_valid()) continue;

			if (target->is_visible_on_screen() && target->is_hpbar_recently_rendered() && !target->is_dead())
			{
				if (settings::draws::Damage->get_bool())
				{
					draw_dmg_rl(target, GetDamage(target), MAKE_COLOR(255, 170, 0, 150));
				}

				if (settings::draws::NeededAA->get_bool())
				{
					femboyutils::AATracker();
				}
			}
		}

		if (settings::draws::KillableText->get_bool())
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
								if (rDamage(target) * 4 >= target->get_real_health())
								{
									int num = (int)target->get_real_health();
									std::string numstr = std::to_string(num);
									int firstdigit = std::stoi(numstr.substr(0, 4));
									const auto key = index++;
									const auto position = vector(568.f, 551.f + (key * 30));
									draw_manager->add_text_on_screen(position, MAKE_COLOR(255, 0, 0, 255), settings::draws::TextSize->get_int(), "%s is Killable HP Left: %i", target->get_name_cstr(), firstdigit);
								}
							}

							if (r->level() == 2)
							{
								if (rDamage(target) * 5 >= target->get_real_health())
								{
									int num = (int)target->get_real_health();
									std::string numstr = std::to_string(num);
									int firstdigit = std::stoi(numstr.substr(0, 4));
									const auto key = index++;
									const auto position = vector(568.f, 551.f + (key * 30));
									draw_manager->add_text_on_screen(position, MAKE_COLOR(255, 0, 0, 255), settings::draws::TextSize->get_int(), "%s is Killable HP Left: %i", target->get_name_cstr(), firstdigit);
								}
							}

							if (r->level() == 3)
							{								
								if (rDamage(target) * 6 >= target->get_real_health())
								{
									int num = (int)target->get_real_health();
									std::string numstr = std::to_string(num);
									int firstdigit = std::stoi(numstr.substr(0, 4));
									const auto key = index++;
									const auto position = vector(568.f, 551.f  + (key * 30));
									draw_manager->add_text_on_screen(position, MAKE_COLOR(255, 0, 0, 255), settings::draws::TextSize->get_int(), "%s is Killable HP Left: %i", target->get_name_cstr(), firstdigit);
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
		if (settings::rsettings::disableEvade->get_bool())
		{
			if (myhero->has_buff(buff_hash("XerathLocusOfPower2")))
			{
				evade->disable_evade();
			}
			else
			{
				evade->enable_evade();
			}
		}

		if (myhero->has_buff(buff_hash("XerathLocusOfPower2")))
		{
			orbwalker->set_movement(false);
			orbwalker->set_attack(false);
		}
		else
		{
			orbwalker->set_movement(true);
			orbwalker->set_attack(true);
		}

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
		autoE();
		autoW();		
		interrupter();
		killstealloop();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 735.f);
		q->set_skillshot(0.55f + 0.066f, 70.f, FLT_MAX, {}, skillshot_type::skillshot_line);
		q->set_charged(735.f, 1450.f, 1.7f);
		q->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 1000.f);
		w->set_skillshot(0.55f + 0.066f, 125.f, FLT_MAX, {}, skillshot_type::skillshot_circle);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 1125.f);
		e->set_skillshot(0.25f + 0.066f, 60.f, 1400.f, { collisionable_objects::yasuo_wall, collisionable_objects::minions }, skillshot_type::skillshot_line);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 5000.f);
		r->set_skillshot(0.5f + 0.066f, 100.f, FLT_MAX, {}, skillshot_type::skillshot_circle);
		r->set_spell_lock(false);

		createmenu();

		antigapcloser::add_event_handler(antigap);
		event_handler<events::on_env_draw>::add_callback(update, event_prority::highest);
		event_handler<events::on_env_draw>::add_callback(on_env_draw, event_prority::high);
		event_handler<events::on_draw>::add_callback(on_draw_real, event_prority::high);
	}	

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		antigapcloser::remove_event_handler(antigap);
		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_env_draw>::remove_handler(on_env_draw);
		event_handler<events::on_draw>::remove_handler(on_draw_real);
	}

	float qrawdamage[] = { 70.f, 110.f, 150.f, 190.f, 230.f };
	float qDamage(const game_object_script& target)
	{
		if (q->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = qrawdamage[q->level() - 1] + myhero->get_total_ability_power() * 0.85f;
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float wrawdamage[] = { 60.f, 95.f, 130.f, 165.f, 200.f };
	float wDamage(const game_object_script& target)
	{
		if (w->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = wrawdamage[w->level() - 1] + myhero->get_total_ability_power() * 0.6f;
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float erawdamage[] = { 80.f, 110.f, 140.f, 170.f, 200.f };
	float eDamage(const game_object_script& target)
	{
		if (e->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = erawdamage[e->level() - 1] + myhero->get_total_ability_power() * 0.45f;
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float rrawdamage[] = { 180.f, 230.f, 280.f };
	float rstackdamage[] = { 20.f, 25.f, 30.f };
	float rDamage(const game_object_script& target)
	{
		if (r->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = rrawdamage[r->level() - 1] + myhero->get_total_ability_power() * 0.4f + rstackdamage[r->level() - 1] * myhero->get_buff_count(buff_hash("xerathrrampup"));
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float GetDamage(const game_object_script& target)
	{
		auto Damage = 0;

		if (q->is_ready())
		{
			Damage += qDamage(target);
		}

		if (w->is_ready())
		{
			Damage += wDamage(target);
		}

		if (e->is_ready())
		{
			Damage += eDamage(target);
		}

		if (r->is_ready())
		{
			if (r->level() == 1)
			{
				Damage += rDamage(target) * 4;
			}
			else if (r->level() == 2)
			{
				Damage += rDamage(target) * 5;
			}
			else if (r->level() == 3)
			{
				Damage += rDamage(target) * 6;
			}
		}

		return Damage;
	}

	void antigap(game_object_script sender, antigapcloser::antigapcloser_args* args)
	{
		if (myhero->get_health_percent() > settings::misc::antigaphp->get_int())
		{
			return;
		}

		if (settings::misc::antigap->get_bool())
		{
			if (e->is_ready())
			{
				if (sender->is_enemy() && sender->is_valid())
				{
					if (args->end_position.distance(myhero) <= 450)
					{
						e->cast(args->end_position);
						console->print("[FemboyAio] Casting E on Gap Closer End Position");
					}
				}
			}
		}
	}

	void interrupter()
	{
		if (settings::misc::interrupter->get_bool())
		{
			for (auto&& enemy : entitylist->get_enemy_heroes())
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
								e->cast(enemy);
								console->print("[FemboyAio] Casting E on %s", enemy->get_name_cstr());
							}
						}
					}
				}
			}
		}
	}
}