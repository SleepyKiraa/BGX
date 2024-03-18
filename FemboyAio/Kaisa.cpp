#include "../plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "femboyutils.h"
#include "Kaisa.h"

namespace kaisa
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
			TreeEntry* qOnlyIsolated;
		}

		namespace wsettings
		{
			TreeEntry* wCombo;
			TreeEntry* wNotInAARange;
			TreeEntry* SemiW;
		}

		namespace esettings
		{
			TreeEntry* eCombo;
			TreeEntry* eOnlyEvolved;
		}

		namespace rsettings
		{
			TreeEntry* rCombo;
			TreeEntry* minIncommingDamageToR;
		}

		namespace killsteal
		{
			TreeEntry* ksQ;
			TreeEntry* ksW;
		}

		namespace hitchance
		{
			TreeEntry* wHitChance;
		}

		namespace draws
		{
			TreeEntry* qRange;
			TreeEntry* wRange;
			TreeEntry* rRange;
			TreeEntry* qColor;
			TreeEntry* wColor;
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
			TreeEntry* JcW;
		}

		namespace misc
		{
			TreeEntry* fastevolve;
		}
	}

	void createmenu()
	{
		mainMenu = menu->create_tab("femboy.Kaisa", "[Femboy] Kaisa");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("femboy.Kaisa.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("femboy.Kaisa.qsettings", "Icathian Rain - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qCombo = qsettings->add_checkbox("femboy.Kaisa.useq", "Enable Q in Combo", true);
				settings::qsettings::qOnlyIsolated = qsettings->add_checkbox("femboy.Kaisa.iso", "Only Q if Target is Isolated", false);
			}

			const auto wsettings = combotab->add_tab("femboy.Kaisa.wsettings", "Void Seeker - [W]");
			wsettings->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			{
				settings::wsettings::wCombo = wsettings->add_checkbox("femboy.Kaisa.usew", "Enable W in Combo", true);
				settings::wsettings::wNotInAARange = wsettings->add_checkbox("femboy.Kaisa.noaa", "Dont W if Target is in AA Range", true);
				settings::wsettings::SemiW = wsettings->add_hotkey("femboy.Kaisa.semiw", "Semi W", TreeHotkeyMode::Hold, 0x54, false, true);
			}

			const auto esettings = combotab->add_tab("femboy.Kaisa.esettings", "Supercharge - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::eCombo = esettings->add_checkbox("femboy.Kaisa.usee", "Enable E in Combo", true);
				settings::esettings::eOnlyEvolved = esettings->add_checkbox("femboy.Kaisa.eevo", "Only Use E if Evolved", true);
			}

			const auto rsettings = combotab->add_tab("femboy.Kaisa.rsettings", "Killer Instinct - [R]");
			rsettings->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			{
				settings::rsettings::rCombo = rsettings->add_checkbox("femboy.Kaisa.user", "Enable R in Combo", true);
				settings::rsettings::minIncommingDamageToR = rsettings->add_slider("femboy.Kaisa.damage", "Min Incomming Damage to R", 200, 100, 1000, true);
			}
		}

		const auto cleartab = mainMenu->add_tab("femboy.Kaisa.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("femboy.Kaisa.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcQ = cleartab->add_checkbox("femboy.Kaisa.qclear", "Enable Q Lane Clear", true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcQ = cleartab->add_checkbox("femboy.Kaisa.qjclear", "Enable Q Jungle Clear", true);
			settings::clear::JcW = cleartab->add_checkbox("femboy.Kaisa.wjclear", "Enable W Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("femboy.Kaisa.ks", "Killsteal");
		{
			settings::killsteal::ksQ = killstealtab->add_checkbox("femboy.Kaisa.ksq", "Enable Q Killsteal", true);
			settings::killsteal::ksW = killstealtab->add_checkbox("femboy.Kaisa.ksw", "Enable W Killsteal", true);
		}

		const auto misctab = mainMenu->add_tab("femboy.Kaisa.misc", "Misc");
		{
			settings::misc::fastevolve = misctab->add_checkbox("femboy.Kaisa.fast", "Fast Evolve Spells", true);
		}

		const auto hitchancetab = mainMenu->add_tab("femboy.Kaisa.hitchance", "Hitchance");
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

		settings::hitchance::wHitChance = hitchancetab->add_combobox("femboy.Kaisa.whitchance", "W Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("femboy.Kaisa.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			float defaultcolorW[] = { 0.f, 0.f, 1.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };
			const auto qdrawtab = drawTab->add_tab("femboy.Kaisa.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("femboy.Kaisa.qrange", "Draw Q Range", true);
				settings::draws::qColor = qdrawtab->add_colorpick("femboy.Kaisa.qcolor", "Q Draw Color", defaultcolorQ);
			}

			const auto wdrawtab = drawTab->add_tab("femboy.Kaisa.draww", "W Draw Settings");
			{
				settings::draws::wRange = wdrawtab->add_checkbox("femboy.Kaisa.wrange", "Draw W Range", true);
				settings::draws::wColor = wdrawtab->add_colorpick("femboy.Kaisa.wcolor", "W Draw Color", defaultcolorW);

			}

			const auto rdrawtab = drawTab->add_tab("femboy.Kaisa.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("femboy.Kaisa.rrange", "Draw R Range", true);
				settings::draws::rColor = rdrawtab->add_colorpick("femboy.Kaisa.rcolor", "R Draw Color", defaultcolorR);
			}

			const auto miscdrawTab = drawTab->add_tab("femboy.Kaisa.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("femboy.Kaisa.damage", "Draw Damage Indicator", true);
				settings::draws::legCircles = miscdrawTab->add_checkbox("femboy.Kaisa.3dcircle", "Enable 3D Circle", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("femboy.Kaisa.aaindicator", "Draw AA Counter", true);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Femboy] Kaisa");
		Permashow::Instance.AddElement("Farm", settings::clear::farmtoggle);
		Permashow::Instance.AddElement("Semi W", settings::wsettings::SemiW);
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
		if (settings::qsettings::qOnlyIsolated->get_bool())
		{
			if (myhero->count_enemies_in_range(q->range()) > 1 || femboyutils::count_minions(myhero->get_position(), q->range(), "enemy") > 0)
			{
				return;
			}
		}

		if (settings::qsettings::qCombo->get_bool())
		{
			if (q->is_ready())
			{
				auto qtarget = target_selector->get_target(q->range(), damage_type::physical);
				if (qtarget != nullptr)
				{
					if (qtarget->is_valid_target())
					{
						q->cast();
					}
				}
			}
		}
	}

	void WLogic()
	{
		auto wtarget = target_selector->get_target(w->range(), damage_type::magical);

		if (settings::wsettings::wNotInAARange->get_bool())
		{
			if (wtarget != nullptr)
			{
				if (wtarget->is_valid_target())
				{
					if (myhero->is_in_auto_attack_range(wtarget))
					{
						return;
					}
				}
			}
		}

		if (settings::wsettings::wCombo->get_bool())
		{
			if (w->is_ready())
			{
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

	void SemiW()
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

	void afteraa(game_object_script target)
	{
		if (settings::esettings::eOnlyEvolved->get_bool())
		{
			if (!myhero->has_buff(buff_hash("KaisaEEvolved")))
			{
				return;
			}
		}

		if (!orbwalker->combo_mode())
		{
			return;
		}

		if (settings::esettings::eCombo->get_bool())
		{
			if (e->is_ready())
			{
				e->cast();
			}
		}
	}

	void RLogic()
	{
		if (settings::rsettings::rCombo->get_bool())
		{
			if (r->is_ready())
			{
				for (auto&& target : entitylist->get_enemy_heroes())
				{
					if (target->is_valid_target(r->range()))
					{
						if (target->has_buff(buff_hash("kaisapassivemarker")))
						{
							if (health_prediction->get_incoming_damage(myhero, 0.5f, true) >= settings::rsettings::minIncommingDamageToR->get_int())
							{
								if (!target->is_facing(myhero))
								{
									r->cast(target->get_position().extend(myhero->get_position(), -350));
								}
								else if (target->is_facing(myhero))
								{
									r->cast(target->get_position().extend(myhero->get_position(), +250));
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
		if (!enemy->is_valid_target(q->range())) return;

		if (enemy->get_real_health() <= qDamage(enemy))
		{
			if (myhero->count_enemies_in_range(q->range()) == 1 && femboyutils::count_minions(myhero->get_position(), q->range(), "enemy") == 0)
			{
				q->cast();
			}
		}
	}

	void killstealW(const game_object_script& enemy)
	{
		if (!w->is_ready()) return;
		if (!settings::killsteal::ksW->get_bool()) return;
		if (!enemy->is_valid_target(w->range())) return;

		if (enemy->get_real_health() <= wDamage(enemy))
		{
			auto wpred = w->get_prediction(enemy);
			if (wpred.hitchance >= hit_chance::high)
			{
				w->cast(wpred.get_cast_position());
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
				q->cast();
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
				q->cast();
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

	void on_env_draw()
	{
		if (settings::draws::qRange->get_bool())
		{
			if (q->level() >= 1)
			{
				drawCircle(myhero->get_position(), q->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::qColor->get_color());
			}
		}
	}

	void on_draw()
	{
		if (settings::draws::wRange->get_bool())
		{
			if (w->level() >= 1)
			{
				draw_manager->draw_circle_on_minimap(myhero->get_position(), w->range(), settings::draws::wColor->get_color(), 2, 100);
			}
		}

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
					draw_dmg_rl(target, GetDamage(target), MAKE_COLOR(255, 170, 0, 150));
				}

				if (settings::draws::NeededAA->get_bool())
				{
					femboyutils::AATracker();
				}
			}
		}
	}

	void update()
	{
		if (r->level() == 1)
		{
			r->set_range(1500.f);
		}
		else if (r->level() == 2)
		{
			r->set_range(2250.f);
		}
		else if (r->level() == 3)
		{
			r->set_range(3000.f);
		}

		if (orbwalker->combo_mode())
		{
			RLogic();
			QLogic();
			WLogic();
		}

		if (orbwalker->lane_clear_mode())
		{
			laneclear();
			jungleclear();
		}

		if (settings::wsettings::SemiW->get_bool())
		{
			myhero->issue_order(hud->get_hud_input_logic()->get_game_cursor_position());

			SemiW();
		}

		if (settings::misc::fastevolve->get_bool())
		{
			if (myhero->get_evolve_points() != 0)
			{
				myhero->cast_spell(spellslot::recall);
				myhero->evolve_spell(spellslot::e);
				myhero->evolve_spell(spellslot::q);
				myhero->evolve_spell(spellslot::w);
			}
		}

		killstealloop();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 600.f + myhero->get_bounding_radius());
		q->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 3000.f);
		w->set_skillshot(0.4f + 0.066f, 100.f, 1750.f, { collisionable_objects::minions, collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 0.f);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 1500.f);
		r->set_spell_lock(false);

		createmenu();

		event_handler<events::on_after_attack_orbwalker>::add_callback(afteraa, event_prority::high);
		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_env_draw>::add_callback(on_env_draw, event_prority::high);
		event_handler<events::on_draw>::add_callback(on_draw, event_prority::high);
	}

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		event_handler<events::on_after_attack_orbwalker>::remove_handler(afteraa);
		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_draw>::remove_handler(on_draw);
		event_handler<events::on_env_draw>::remove_handler(on_env_draw);
	}

	float qrawDamage[] = { 90.f, 123.f, 157.f, 191.f, 225.f };

	float qDamage(const game_object_script& target)
	{
		if (q->level() == 0) return 0.f;

		damage_input input;
		input.raw_physical_damage = qrawDamage[q->level() - 1] + myhero->mPercentBonusPhysicalDamageMod() * 1.12f + myhero->get_total_ability_power() * 0.45f;
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float wrawDamage[] = { 30.f, 55.f, 80.f, 105.f, 130.f };

	float wDamage(const game_object_script& target)
	{
		if (w->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = wrawDamage[w->level() - 1] + myhero->get_total_attack_damage() * 1.30f + myhero->get_total_ability_power() * 0.45f;
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

		return Damage;
	}
}