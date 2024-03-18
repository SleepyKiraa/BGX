#include "plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "Varus.h"
#include "Spaceutils.h"

namespace varus
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
			TreeEntry* qStacks;
			TreeEntry* qMode;
			TreeEntry* SemiQ;
		}

		namespace wsettings
		{
			TreeEntry* autoW;
			TreeEntry* HP;
		}

		namespace esettings
		{
			TreeEntry* eCombo;
			TreeEntry* eStacks;
		}

		namespace rsettings
		{
			TreeEntry* rCombo;
			TreeEntry* semiR;
		}

		namespace killsteal
		{
			TreeEntry* ksQ;
			TreeEntry* ksE;
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
			TreeEntry* eRange;
			TreeEntry* rRange;
			TreeEntry* qColor;
			TreeEntry* q2Color;
			TreeEntry* qThicness;
			TreeEntry* eColor;
			TreeEntry* e2Color;
			TreeEntry* eThicness;
			TreeEntry* rColor;
			TreeEntry* r2Color;
			TreeEntry* rThicness;
			TreeEntry* Damage;
			TreeEntry* NeededAA;
		}

		namespace clear
		{
			TreeEntry* farmtoggle;
			TreeEntry* LcE;
			TreeEntry* LcEMinions;
			TreeEntry* JcE;
		}

		namespace misc
		{
			TreeEntry* antigapcloser;
			TreeEntry* antigapcloserHP;
		}
	}

	hit_chance getPredIntFromSettings(int hitchance)
	{
		// Get hitchance from settings value
		return static_cast<hit_chance>(hitchance + 3);
	}

	void createmenu()
	{
		mainMenu = menu->create_tab("space.varus", "[Spaceglide] Varus");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("space.varus.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("space.varus.qsettings", "Piercing Arrow - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qMode = qsettings->add_combobox("space.varus.qmode", "Q Mode", { {"Fast", nullptr}, {"Full", nullptr}, {"Mixed", nullptr} }, 2, true);
				settings::qsettings::qCombo = qsettings->add_checkbox("space.varus.useq", "Enable Q in Combo", true);
				settings::qsettings::qStacks = qsettings->add_slider("space.varus.stacksq", "W Stacks", 3, 1, 3, true);
				settings::qsettings::SemiQ = qsettings->add_hotkey("space.varus.semiq", "Semi Q", TreeHotkeyMode::Hold, 0x47, false, true);
			}

			const auto wsettings = combotab->add_tab("space.varus.wsettings", "Blighted Quiver - [W]");
			wsettings->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			{
				settings::wsettings::autoW = wsettings->add_checkbox("space.varus.usew", "Enable W in Combo", true);
				settings::wsettings::HP = wsettings->add_slider("space.varus.hppercent", "HP % to use W", 50, 1, 100, true);
			}

			const auto esettings = combotab->add_tab("space.varus.esettings", "Hail of Arrows - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::eCombo = esettings->add_checkbox("space.varus.usee", "Enable E in Combo", true);
				settings::esettings::eStacks = esettings->add_slider("space.varus.stackse", "W Stacks", 2, 1, 3, true);
			}

			const auto rsettings = combotab->add_tab("space.varus.rsettings", "Chain of Corruption - [R]");
			rsettings->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			{
				settings::rsettings::rCombo = rsettings->add_checkbox("space.varus.user", "Enable R in Combo", true);
				settings::rsettings::semiR = rsettings->add_hotkey("space.varus.rkey", "Semi R", TreeHotkeyMode::Hold, 0x54, false);
			}
		}

		const auto cleartab = mainMenu->add_tab("space.varus.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("space.varus.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcE = cleartab->add_checkbox("space.varus.eclear", "Enable E Lane Clear", true);
			settings::clear::LcEMinions = cleartab->add_slider("space.varus.emins", "Minions to E Clear", 3, 1, 5, true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcE = cleartab->add_checkbox("space.varus.ejclear", "Enable E Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("space.varus.ks", "Killsteal");
		{
			settings::killsteal::ksQ = killstealtab->add_checkbox("space.varus.ksq", "Enable Q Killsteal", true);
			settings::killsteal::ksE = killstealtab->add_checkbox("space.varus.kse", "Enable E Killsteal", true);
		}

		const auto misctab = mainMenu->add_tab("space.varus.misc", "Misc");
		{
			settings::misc::antigapcloser = misctab->add_checkbox("space.varus.anti", "Enable Anti Gapcloser", true);
			settings::misc::antigapcloserHP = misctab->add_slider("space.varus.hp", "Min HP % to Anti Gapclose", 50, 1, 100, true);
		}

		const auto hitchancetab = mainMenu->add_tab("space.varus.hitchance", "Hitchance");
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

		settings::hitchance::qHitChance = hitchancetab->add_combobox("space.varus.qhitchance", "Q Hitchance", combo_elemts, 2);
		settings::hitchance::eHitChance = hitchancetab->add_combobox("space.varus.ehitchance", "E Hitchance", combo_elemts, 2);
		settings::hitchance::rHitChance = hitchancetab->add_combobox("space.varus.rhitchance", "R Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("space.varus.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			float defaultcolorQ2[] = { 1.f, 0.f, 1.f, 1.f };
			float defaultcolorE[] = { 0.f, 1.f, 0.f, 1.f };
			float defaultcolorE2[] = { 1.f, 0.f, 1.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };
			float defaultcolorR2[] = { 1.f, 0.f, 1.f, 1.f };

			const auto qdrawtab = drawTab->add_tab("space.varus.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("space.varus.qrange", "Draw Q Range", true);
				settings::draws::qColor = qdrawtab->add_colorpick("space.varus.qcolor", "Q Draw Color", defaultcolorQ);
				settings::draws::q2Color = qdrawtab->add_colorpick("space.varus.q2color", "Q2 Draw Color", defaultcolorQ2);
				settings::draws::qThicness = qdrawtab->add_slider("space.varus.thicknessq", "Q circle Thickness", 2, 1, 6, true);
			}

			const auto edrawtab = drawTab->add_tab("space.varus.drawe", "E Draw Settings");
			{
				settings::draws::eRange = edrawtab->add_checkbox("space.varus.erange", "Draw E Range", true);
				settings::draws::eColor = edrawtab->add_colorpick("space.varus.ecolor", "E Draw Color", defaultcolorE);
				settings::draws::e2Color = edrawtab->add_colorpick("space.varus.e2color", "E2 Draw Color", defaultcolorE2);
				settings::draws::eThicness = edrawtab->add_slider("space.varus.thicknesse", "E circle Thickness", 2, 1, 6, true);
			}

			const auto rdrawtab = drawTab->add_tab("space.varus.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("space.varus.rrange", "Draw R Range", true);
				settings::draws::rColor = rdrawtab->add_colorpick("space.varus.rcolor", "R Draw Color", defaultcolorR);
				settings::draws::r2Color = rdrawtab->add_colorpick("space.varus.r2color", "R2 Draw Color", defaultcolorR2);
				settings::draws::rThicness = rdrawtab->add_slider("space.varus.thicknessr", "R circle Thickness", 2, 1, 6, true);
			}

			const auto miscdrawTab = drawTab->add_tab("space.varus.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("space.varus.damage", "Draw Damage Indicator", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("space.varus.aaindicator", "Draw AA Counter", true);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Spaceglide] Varus");
		Permashow::Instance.AddElement("Farm", settings::clear::farmtoggle);
		Permashow::Instance.AddElement("Semi Q", settings::qsettings::SemiQ);
		Permashow::Instance.AddElement("Semi R", settings::rsettings::semiR);
	}

	void QLogic()
	{
		auto qtarget = target_selector->get_target(q->charged_max_range, damage_type::physical);

		if (settings::qsettings::qCombo->get_bool())
		{
			if (q->is_ready())
			{
				if (qtarget != nullptr)
				{
					if (qtarget->is_valid())
					{
						if (!q->is_charging() && qtarget->get_buff_count(buff_hash("VarusWDebuff")) >= settings::qsettings::qStacks->get_int())
						{
							q->start_charging();
							return;
						}

						if (q->is_charging())
						{
							auto pred = q->get_prediction(qtarget);
							if (pred.hitchance >= getPredIntFromSettings(settings::hitchance::qHitChance->get_int()))
							{
								if (settings::qsettings::qMode->get_int() == 0)
								{
									myhero->update_charged_spell(q->get_slot(), pred.get_cast_position(), true, false);
								}

								if (settings::qsettings::qMode->get_int() == 1)
								{
									if (q->range() == q->charged_max_range)
									{
										myhero->update_charged_spell(q->get_slot(), pred.get_cast_position(), true, false);
									}
								}

								if (settings::qsettings::qMode->get_int() == 2)
								{
									if (myhero->is_in_auto_attack_range(qtarget))
									{
										myhero->update_charged_spell(q->get_slot(), pred.get_cast_position(), true, false);
									}

									if (!myhero->is_in_auto_attack_range(qtarget))
									{
										if (q->range() == q->charged_max_range)
										{
											myhero->update_charged_spell(q->get_slot(), pred.get_cast_position(), true, false);
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

	void SemiQ()
	{
		auto target = target_selector->get_target(q->charged_max_range, damage_type::physical);

		if (q->is_ready())
		{
			if (target != nullptr)
			{
				if (target->is_valid())
				{
					if (!q->is_charging())
					{
						q->start_charging();
						return;
					}

					if (q->is_charging())
					{
						auto qpred = q->get_prediction(target);
						if (qpred.hitchance >= getPredIntFromSettings(settings::hitchance::qHitChance->get_int()))
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

	void Wlogic()
	{
		if (settings::wsettings::autoW->get_bool())
		{
			if (w->is_ready() && q->is_ready())
			{
				auto targets = target_selector->get_target(q->charged_max_range, damage_type::physical);
				if (targets != nullptr)
				{
					if (targets->is_valid())
					{
						if (targets->get_health_percent() <= settings::wsettings::HP->get_int())
						{
							if (myhero->get_spell(spellslot::w)->get_icon_index() == 0)
							{
								if (targets->get_buff_count(buff_hash("VarusWDebuff")) == 3)
								{
									w->cast();
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
		if (settings::esettings::eCombo->get_bool())
		{
			if (e->is_ready())
			{
				auto etarget = target_selector->get_target(e->range(), damage_type::physical);
				if (etarget != nullptr)
				{
					if (etarget->is_valid())
					{
						if (etarget->get_buff_count(buff_hash("VarusWDebuff")) >= settings::esettings::eStacks->get_int())
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

	void RLogic()
	{
		auto rtarget = target_selector->get_target(r->range(), damage_type::magical);

		if (rtarget != nullptr)
		{
			if (rtarget->is_valid())
			{
				if (q->is_ready())
				{
					if (qFullDamage(rtarget) >= rtarget->get_real_health())
					{
						return;
					}
				}
			}
		}

		if (rtarget != nullptr)
		{
			if (rtarget->is_valid())
			{
				if (rtarget->get_distance(myhero) <= myhero->get_attack_range() + myhero->get_bounding_radius())
				{
					if (myhero->get_auto_attack_damage(rtarget) * 2 >= rtarget->get_real_health())
					{
						return;
					}
				}
			}
		}

		if (settings::rsettings::rCombo->get_bool())
		{
			if (r->is_ready())
			{				
				if (rtarget != nullptr)
				{
					if (rtarget->is_valid())
					{
						if (GetDamageforROnly(rtarget) >= rtarget->get_real_health())
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
		if (!enemy->is_valid_target(q->charged_max_range)) return;

		if (enemy->get_distance(myhero) <= myhero->get_attackRange() + myhero->get_bounding_radius())
		{
			return;
		}
		
		if (enemy->get_real_health() <= qFullDamage(enemy) + wDamage(enemy))
		{
			if (!q->is_charging())
			{
				q->start_charging();
				return;
			}

			if (q->is_charging())
			{
				if (q->range() == q->charged_max_range)
				{
					auto pred = q->get_prediction(enemy);
					if (pred.hitchance >= getPredIntFromSettings(settings::hitchance::qHitChance->get_int()))
					{
						myhero->update_charged_spell(q->get_slot(), pred.get_cast_position(), true, false);
					}
				}
			}
		}
	}

	void killstealE(const game_object_script& enemy)
	{
		if (!e->is_ready()) return;
		if (!settings::killsteal::ksE->get_bool()) return;
		if (!enemy->is_valid_target(e->range())) return;
		if (q->is_charging()) return;

		if (enemy->get_real_health() <= eDamage(enemy) + wDamage(enemy))
		{
			auto epred = e->get_prediction(enemy);
			if (epred.hitchance >= getPredIntFromSettings(settings::hitchance::eHitChance->get_int()))
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

		for (auto& minionE : minions_list)
		{
			if (minionE->get_distance(myhero) > e->range())
				continue;


			if (settings::clear::LcE->get_bool() && e->is_ready())
			{
				e->cast_on_best_farm_position(settings::clear::LcEMinions->get_int(), false);
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
	}

	void killstealloop()
	{
		for (auto&& enemy : entitylist->get_enemy_heroes())
		{
			if (enemy == nullptr || !enemy->is_valid_target()) continue;

			killstealE(enemy);
			killstealQ(enemy);
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
	}

	void update()
	{
		Wlogic();

		if (orbwalker->combo_mode())
		{
			RLogic();
			ELogic();
			QLogic();
		}

		if (orbwalker->lane_clear_mode())
		{
			laneclear();
			jungleclear();
		}

		if (settings::qsettings::SemiQ->get_bool())
		{
			myhero->issue_order(hud->get_hud_input_logic()->get_game_cursor_position());

			SemiQ();
		}

		if (settings::rsettings::semiR->get_bool())
		{
			myhero->issue_order(hud->get_hud_input_logic()->get_game_cursor_position());

			SemiR();
		}
		
		killstealloop();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 895.f);
		q->set_skillshot(0.25f + 0.066f, 70.f, 1900.f, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		q->set_charged(895.f, 1595.f, 1.5f);
		q->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 0.f);

		e = plugin_sdk->register_spell(spellslot::e, 925.f);
		e->set_skillshot(0.25f + 0.066f, 100.f, FLT_MAX, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_circle);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 1370.f);
		r->set_skillshot(0.25f + 0.066f, 120.f, 1500.f, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		r->set_spell_lock(false);

		createmenu();

		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_env_draw>::add_callback(on_draw_env, event_prority::high);
		event_handler<events::on_draw>::add_callback(on_draw, event_prority::high);
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
		event_handler<events::on_draw>::remove_handler(on_draw);
		antigapcloser::remove_event_handler(antigap);
	}

	float qfastdamage[] = { 10.f, 46.67f, 83.33f, 120.f, 156.67f };
	float qfastscale[] = { 0.8333f, 0.8667f, 0.90f, 0.9333f, 0.9667f };

	float qfastDamage(const game_object_script& target)
	{
		if (q->level() == 0) return 0.f;
		damage_input input;
		input.raw_physical_damage = qfastdamage[q->level() - 1] + qfastscale[q->level() - 1] * myhero->get_total_attack_damage();
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float qFulldamage[] = { 15.f, 70.f, 125.f, 180.f, 235.f };
	float qFullsccale[] = { 1.25f, 1.30f, 1.35f, 1.40f, 1.45f };

	float qFullDamage(const game_object_script& target)
	{
		if (q->level() == 0) return 0.f;
		damage_input input;
		input.raw_physical_damage = qFulldamage[q->level() - 1] + qFullsccale[q->level() - 1] * myhero->get_total_attack_damage();
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float wDamage(const game_object_script& target)
	{
		return target->get_buff_count(buff_hash("VarusWDebuff")) * w->get_damage(target);
	}

	float edamage[] = { 60.f, 100.f, 140.f, 180.f, 220.f };

	float eDamage(const game_object_script& target)
	{
		if (e->level() == 0) return 0.f;
		damage_input input;
		input.raw_physical_damage = edamage[e->level() - 1] + 0.90f * (myhero->get_total_attack_damage() - myhero->get_base_attack_damage());
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float rdamage[] = { 150.f, 250.f, 350.f };

	float rDamage(const game_object_script& target)
	{
		if (r->level() == 0) return 0.f;
		damage_input input;
		input.raw_magical_damage = rdamage[r->level() - 1] + 1.f * myhero->get_total_ability_power();
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float GetDamage(const game_object_script& target)
	{
		auto Damage = 0;

		if (!q->is_charging())
		{
			Damage += qfastDamage(target);
		}
		else if (q->is_charging())
		{
			if (q->range() == q->charged_max_range)
			{
				Damage += qFullDamage(target);
			}
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
			Damage += rDamage(target);
		}

		return Damage;
	}

	float GetDamageforROnly(const game_object_script& target)
	{
		auto Damage = 0;

		if (q->is_ready())
		{
			Damage += qFullDamage(target);
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
			Damage += rDamage(target);
		}

		return Damage;
	}

	void antigap(const game_object_script sender, antigapcloser::antigapcloser_args* args)
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
			if (r->is_ready())
			{
				if (sender->is_enemy() && sender->is_valid())
				{
					if (args->end_position.distance(myhero) <= 450.f)
					{
						r->cast(args->end_position);
						console->print("[Spaceglide] Casting R on Gap Closer End Position Sender: %s", sender->get_base_skin_name().c_str());
					}
				}
			}
		}
	}
}