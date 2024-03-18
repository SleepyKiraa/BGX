#include "../plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "femboyutils.h"
#include "ezreal.h"

namespace ezreal
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
			TreeEntry* AutoQ;
		}

		namespace wsettings
		{
			TreeEntry* wCombo;
			TreeEntry* wAfterAA;
		}

		namespace esettings
		{
			TreeEntry* useEtoEvade;
			TreeEntry* dangerlevel;
		}

		namespace rsettings
		{
			TreeEntry* semiR;
			TreeEntry* rRange;
			TreeEntry* autoRonCC;
		}

		namespace harass
		{
			TreeEntry* qHarass;
		}

		namespace killsteal
		{
			TreeEntry* ksQ;
			TreeEntry* ksR;
		}

		namespace hitchance
		{
			TreeEntry* qHitChance;
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
			TreeEntry* wColor;
			TreeEntry* eColor;
			TreeEntry* rColor;
			TreeEntry* Damage;
			TreeEntry* legCircles;
			TreeEntry* NeededAA;
		}

		namespace clear
		{
			TreeEntry* farmtoggle;
			TreeEntry* LcQ;
			TreeEntry* JcQ;
		}
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

	float GetDamage(const game_object_script& target)
	{
		auto Damage = 0;
		if (q->is_ready())
		{
			Damage += q->get_damage(target);
		}

		if (e->is_ready())
		{
			Damage += e->get_damage(target);
		}

		if (r->is_ready())
		{
			Damage += r->get_damage(target);
		}

		return Damage;
	}

	void createmenu()
	{
		mainMenu = menu->create_tab("femboy.Ezreal", "[Femboy] Ezreal");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("femboy.ezreal.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("femboy.Ezreal.qsettings", "Mystic Shot - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qCombo = qsettings->add_checkbox("femboy Ezreal.useq", "Enable Q in Combo", true);
				settings::qsettings::AutoQ = qsettings->add_checkbox("femboy.Ezreal.autoq", "Auto Q on Dash/CC", true);
			}

			const auto wsettings = combotab->add_tab("femboy.Ezreal.wsettings", "Essence Flux - [W]");
			wsettings->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			{
				settings::wsettings::wCombo = wsettings->add_checkbox("femboy.Ezreal.usew", "Enable W in Combo", true);
				settings::wsettings::wAfterAA = wsettings->add_checkbox("femboy.Ezreal.wafteraa", "Use W only After AA", false);
			}

			const auto esettings = combotab->add_tab("femboy.Ezreal.esettings", "Arcane Shift - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::useEtoEvade = esettings->add_checkbox("femboy.Ezreal.eevade", "Use E to Evade Spells", true);
				settings::esettings::dangerlevel = esettings->add_slider("femboy.Ezreal.edangerlevel", "Evade Min Danger Level to use E", 5, 1, 5);
			}

			const auto rsettings = combotab->add_tab("femboy.Ezreal.rsettings", "Trueshot Barrage - [R]");
			rsettings->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			{
				settings::rsettings::autoRonCC = rsettings->add_checkbox("femboy.Ezreal.autoR", "Auto R on CC Targets", true);
				settings::rsettings::semiR = rsettings->add_hotkey("femboy.Ezreal.semir", "Semi R", TreeHotkeyMode::Hold, 0x54, false, true);
				settings::rsettings::rRange = rsettings->add_slider("femboy.Ezreal.range", "Adjust R Range", 1500, 500, 2500);
			}
		}

		const auto harasstab = mainMenu->add_tab("femboy.Ezreal.harass", "Harass");
		{
			settings::harass::qHarass = harasstab->add_checkbox("femboy.Ezreal.qharass", "Enable Q in Harass", true);
		}

		const auto cleartab = mainMenu->add_tab("femboy.Ezreal.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("femboy.Ezreal.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcQ = cleartab->add_checkbox("femboy.Ezreal.qclear", "Enable Q Lane Clear", true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcQ = cleartab->add_checkbox("femboy.Ezreal.qjclear", "Enable Q Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("femboy.Ezreal.ks", "Killsteal");
		{
			settings::killsteal::ksQ = killstealtab->add_checkbox("femboy.Ezreal.ksq", "Enable Q Killsteal", true);
			settings::killsteal::ksR = killstealtab->add_checkbox("femboy.Ezreal.ksr", "Enable R Killsteal", true);
		}

		const auto hitchancetab = mainMenu->add_tab("femboy.Ezreal.hitchance", "Hitchance");
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

		settings::hitchance::qHitChance = hitchancetab->add_combobox("femboy.Ezreal.qhitchance", "Q Hitchance", combo_elemts, 2);
		settings::hitchance::wHitChance = hitchancetab->add_combobox("femboy.Ezreal.whitchance", "W Hitchance", combo_elemts, 2);
		settings::hitchance::rHitChance = hitchancetab->add_combobox("femboy.Ezreal.rhitchance", "R Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("femboy.Ezreal.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			float defaultcolorW[] = { 0.f, 0.f, 1.f, 1.f };
			float defaultcolorE[] = { 0.f, 1.f, 0.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };
			const auto qdrawtab = drawTab->add_tab("femboy.Ezreal.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("femboy.Ezreal.qrange", "Draw Q Range", true);
				settings::draws::qColor = qdrawtab->add_colorpick("femboy.Ezreal.qcolor", "Q Draw Color", defaultcolorQ);
			}

			const auto wdrawtab = drawTab->add_tab("femboy.Ezreal.draww", "W Draw Settings");
			{
				settings::draws::wRange = wdrawtab->add_checkbox("femboy.Ezreal.wrange", "Draw W Range", true);
				settings::draws::wColor = wdrawtab->add_colorpick("femboy.Ezreal.wcolor", "W Draw Color", defaultcolorW);

			}

			const auto edrawtab = drawTab->add_tab("femboy.Ezreal.drawe", "E Draw Settings");
			{
				settings::draws::eRange = edrawtab->add_checkbox("femboy.Ezreal.erange", "Draw E Range", true);
				settings::draws::eColor = edrawtab->add_colorpick("femboy.Ezreal.ecolor", "E Draw Color", defaultcolorE);
			}

			const auto rdrawtab = drawTab->add_tab("femboy.Ezreal.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("femboy.Ezreal.rrange", "Draw R Range", true);
				settings::draws::rColor = rdrawtab->add_colorpick("femboy.Ezreal.rcolor", "R Draw Color", defaultcolorR);
			}

			const auto miscdrawTab = drawTab->add_tab("femboy.Ezreal.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("femboy.Ezreal.damage", "Draw Damage Indicator", true);
				settings::draws::legCircles = miscdrawTab->add_checkbox("femboy.Ezreal.3dcircle", "Enable 3D Circle", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("femboy.Ezreal.aaindicator", "Draw AA Counter", true);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Femboy] Ezreal");
		Permashow::Instance.AddElement("Semi R", settings::rsettings::semiR);
		Permashow::Instance.AddElement("Farm", settings::clear::farmtoggle);
	}

	void QLogic()
	{
		auto qtarget = target_selector->get_target(q->range(), damage_type::physical);

		if (settings::qsettings::qCombo->get_bool())
		{
			if (qtarget != nullptr)
			{
				if (q->is_ready())
				{
					if (qtarget->is_valid_target())
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

	void AutoQ()
	{
		auto qtarget = target_selector->get_target(q->range(), damage_type::physical);
		if (settings::qsettings::AutoQ->get_bool())
		{
			if (q->is_ready())
			{
				auto qtarget = target_selector->get_target(q->range(), damage_type::physical);
				if (qtarget != nullptr)
				{
					if (qtarget->is_valid_target())
					{
						auto qpred = q->get_prediction(qtarget);
						if (qpred.hitchance >= hit_chance::dashing)
						{
							q->cast(qpred.get_cast_position());
						}
					}
				}
			}
		}
	}

	void Wlogic()
	{
		auto wtarget = target_selector->get_target(w->range(), damage_type::magical);

		if (settings::wsettings::wCombo->get_bool() && !settings::wsettings::wAfterAA->get_bool())
		{
			if (wtarget != nullptr)
			{
				if (w->is_ready())
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

	void autoR()
	{
		auto rtarget = target_selector->get_target(settings::rsettings::rRange->get_int(), damage_type::magical);

		if (settings::rsettings::autoRonCC->get_bool())
		{
			if (r->is_ready())
			{
				if (rtarget != nullptr)
				{
					if (!myhero->is_in_auto_attack_range(rtarget))
					{
						if (rtarget->has_buff_type(buff_type::Stun) || rtarget->has_buff_type(buff_type::Snare))
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

	void afteraa(game_object_script target)
	{
		if (settings::wsettings::wAfterAA->get_bool())
		{
			if (w->is_ready())
			{
				if (target != nullptr)
				{
					if (target->is_ai_hero() && target->is_enemy() && target->is_valid_target() && w->is_in_range(target))
					{
						auto wpred = w->get_prediction(target);
						if (wpred.hitchance >= getPredIntFromSettings(settings::hitchance::wHitChance->get_int()))
						{
							w->cast(wpred.get_cast_position());
						}
					}
				}
			}
		}
	}

	void SemiR()
	{
		auto rtarget = target_selector->get_target(settings::rsettings::rRange->get_int(), damage_type::magical);

		if (rtarget != nullptr)
		{
			if (rtarget->has_buff_type(buff_type::Invulnerability) || rtarget->has_buff_type(buff_type::UnKillable))
			{
				return;
			}
		}

		if (rtarget != nullptr)
		{
			if (rtarget->is_valid_target() && r->is_ready())
			{
				if (!rtarget->is_dead())
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

	void evadeing()
	{
		if (settings::esettings::useEtoEvade->get_bool())
		{
			for (auto skillshots : evade->get_skillshots())
			{
				if (skillshots.spell_data_danger_level >= settings::esettings::dangerlevel->get_int() && skillshots.polygon.is_inside(myhero->get_position()) && skillshots.polygon.is_outside(hud->get_hud_input_logic()->get_game_cursor_position()))
				{
					e->cast(hud->get_hud_input_logic()->get_game_cursor_position());
				}
			}
		}
	}

	void qHarass()
	{
		auto qtarget = target_selector->get_target(q->range(), damage_type::physical);

		if (settings::harass::qHarass->get_bool())
		{
			if (qtarget != nullptr)
			{
				if (q->is_ready())
				{
					if (qtarget->is_valid_target())
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

	void killstealQ(const game_object_script& enemy)
	{
		if (!q->is_ready()) return;
		if (!settings::killsteal::ksQ->get_bool()) return;
		if (!enemy->is_valid_target(q->range())) return;

		if (enemy->get_real_health() <= q->get_damage(enemy))
		{
			auto qpred = q->get_prediction(enemy);
			if (qpred.hitchance >= hit_chance::high)
			{
				q->cast(qpred.get_cast_position());
			}
		}
	}

	void killstealR(const game_object_script& enemy)
	{
		if (!enemy->is_valid_target(settings::rsettings::rRange->get_int())) return;
		if (myhero->is_in_auto_attack_range(enemy)) return;
		if (!r->is_ready()) return;
		if (!settings::killsteal::ksR->get_bool()) return;

		if (enemy->get_real_health() <= r->get_damage(enemy))
		{
			auto rpred = r->get_prediction(enemy);
			if (rpred.hitchance >= hit_chance::high)
			{
				r->cast(rpred.get_cast_position());
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
				q->cast(minionQ->get_position());
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

		for (auto& JungleminionQ : minions_list)
		{
			if (JungleminionQ->get_distance(myhero) > q->range())
				continue;

			if (settings::clear::JcQ->get_bool() && q->is_ready())
			{
				q->cast(JungleminionQ->get_position());
			}
		}
	}

	void on_draw()
	{
		if (settings::draws::qRange->get_bool())
		{
			if (q->level() >= 1)
			{
				drawCircle(myhero->get_position(), q->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::qColor->get_color());
			}
		}

		if (settings::draws::wRange->get_bool())
		{
			if (w->level() >= 1)
			{
				drawCircle(myhero->get_position(), w->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::wColor->get_color());
			}		
		}

		if (settings::draws::eRange->get_bool())
		{
			if (e->level() >= 1)
			{
				drawCircle(myhero->get_position(), e->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::eColor->get_color());
			}			
		}

		if (settings::draws::rRange->get_bool())
		{
			if (r->level() >= 1)
			{
				auto rRangeslider = settings::rsettings::rRange->get_int();
				drawCircle(myhero->get_position(), rRangeslider, 100, settings::draws::legCircles->get_bool(), settings::draws::rColor->get_color());
			}
		}
	}

	void on_draw_real()
	{
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
	}

	void killstealloop()
	{
		for (auto&& enemy : entitylist->get_enemy_heroes())
		{
			if (enemy == nullptr || !enemy->is_valid_target()) continue;

			killstealQ(enemy);
			killstealR(enemy);
		}
	}

	void onUpdate()
	{
		if (settings::rsettings::semiR->get_bool())
		{
			myhero->issue_order(hud->get_hud_input_logic()->get_game_cursor_position());

			SemiR();
		}

		if (orbwalker->combo_mode())
		{
			Wlogic();
			QLogic();
		}

		if (orbwalker->lane_clear_mode())
		{
			laneclear();
			jungleclear();
		}

		if (orbwalker->harass())
		{
			qHarass();
		}

		AutoQ();
		killstealloop();
		autoR();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 1200);
		q->set_skillshot(0.25f + 0.066f, 60.f, 2000.f, { collisionable_objects::minions, collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		q->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 1200);
		w->set_skillshot(0.25f + 0.066f, 80.f, 1700.f, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 475);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 2500);
		r->set_skillshot(1.f + 0.066f, 160.f, 2000.f, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		r->set_spell_lock(false);

		createmenu();

		event_handler<events::on_update>::add_callback(onUpdate, event_prority::highest);
		event_handler<events::on_draw>::add_callback(on_draw_real, event_prority::high);
		event_handler<events::on_env_draw>::add_callback(on_draw, event_prority::high);
		event_handler<events::on_after_attack_orbwalker>::add_callback(afteraa, event_prority::high);
		event_handler<events::on_preupdate>::add_callback(evadeing, event_prority::highest);
	}

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		event_handler<events::on_update>::remove_handler(onUpdate);
		event_handler<events::on_draw>::remove_handler(on_draw_real);
		event_handler<events::on_env_draw>::remove_handler(on_draw);
		event_handler<events::on_after_attack_orbwalker>::remove_handler(afteraa);
		event_handler<events::on_preupdate>::remove_handler(evadeing);
	}
}