#include "../plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "femboyutils.h"
#include "Sivir.h"

namespace sivir
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
		}

		namespace wsettings
		{
			TreeEntry* wCombo;
		}

		namespace esettings
		{
			TreeEntry* eEvade;
			TreeEntry* eDangerLevel;
		}

		namespace harass
		{
			TreeEntry* qHarass;
		}

		namespace killsteal
		{
			TreeEntry* ksQ;
		}

		namespace hitchance
		{
			TreeEntry* qHitChance;
		}

		namespace draws
		{
			TreeEntry* qRange;
			TreeEntry* qColor;
			TreeEntry* Damage;
			TreeEntry* legCircles;
			TreeEntry* NeededAA;
			TreeEntry* drawBuffs;
			TreeEntry* killListXOffset;
			TreeEntry* killListYOffset;
			TreeEntry* TextSize;
		}

		namespace clear
		{
			TreeEntry* farmtoggle;
			TreeEntry* LcQ;
			TreeEntry* JcQ;
			TreeEntry* JcW;
		}
	}

	void createmenu()
	{
		mainMenu = menu->create_tab("femboy.Sivir", "[Femboy] Sivir");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("femboy.Sivir.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("femboy.Sivir.qsettings", "Boomerang Blade - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qCombo = qsettings->add_checkbox("femboy.Sivir.useq", "Enable Q in Combo", true);
			}

			const auto wsettings = combotab->add_tab("femboy.Sivir.wsettings", "Ricochet - [W]");
			wsettings->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			{
				settings::wsettings::wCombo = wsettings->add_checkbox("femboy.Sivir.usew", "Enable W in Combo", true);
			}

			const auto esettings = combotab->add_tab("femboy.Sivir.esettings", "Spell Shield - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::eEvade = esettings->add_checkbox("femboy.Sivir.evade", "Use E to Evade Spells", true);
				settings::esettings::eDangerLevel = esettings->add_slider("femboy.Sivir.danger", "Evade Min Danger Level to use E", 5, 1, 5);
			}
		}

		const auto harrastab = mainMenu->add_tab("femboy.Sivir.harass", "Harass");
		{
			settings::harass::qHarass = harrastab->add_checkbox("femboy.Sivir.qharass", "Enable Q Harass", true);
		}

		const auto cleartab = mainMenu->add_tab("femboy.Sivir.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("femboy.Sivir.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcQ = cleartab->add_checkbox("femboy.Sivir.qclear", "Enable Q Lane Clear", true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcQ = cleartab->add_checkbox("femboy.Sivir.qjclear", "Enable Q Jungle Clear", true);
			settings::clear::JcW = cleartab->add_checkbox("femboy.Sivir.wjclear", "Enable W Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("femboy.Sivir.ks", "Killsteal");
		{
			settings::killsteal::ksQ = killstealtab->add_checkbox("femboy.Sivir.ksq", "Enable Q Killsteal", true);
		}

		const auto hitchancetab = mainMenu->add_tab("femboy.Sivir.hitchance", "Hitchance");
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

		settings::hitchance::qHitChance = hitchancetab->add_combobox("femboy.Sivir.qhitchance", "Q Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("femboy.Sivir.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			const auto qdrawtab = drawTab->add_tab("femboy.Sivir.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("femboy.Sivir.qrange", "Draw Q Range", true);
				settings::draws::qColor = qdrawtab->add_colorpick("femboy.Sivir.qcolor", "Q Draw Color", defaultcolorQ);
			}

			const auto miscdrawTab = drawTab->add_tab("femboy.Sivir.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("femboy.Sivir.damage", "Draw Damage Indicator", true);
				settings::draws::legCircles = miscdrawTab->add_checkbox("femboy.Sivir.3dcircle", "Enable 3D Circle", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("femboy.Sivir.aaindicator", "Draw AA Counter", true);
				miscdrawTab->add_separator("seperator9", "Buff Draw Settings");
				settings::draws::drawBuffs = miscdrawTab->add_checkbox("femboy.Sivir.buff", "Draw Buff Timer", true);
				settings::draws::killListXOffset = miscdrawTab->add_slider("femboy.Sivir.xoffset", "Horizontal position", -568, -2000, 2000);
				settings::draws::killListYOffset = miscdrawTab->add_slider("femboy.Sivir.yoffset", "Vertical position", 551, -2000, 2000);
				settings::draws::TextSize = miscdrawTab->add_slider("femboy.Sivir.textsize", "Text Size", 22, 5, 35);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Femboy] Sivir");
		Permashow::Instance.AddElement("Farm", settings::clear::farmtoggle);
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

		if (settings::draws::drawBuffs->get_bool())
		{
			int index = 0;
			auto buff = myhero->get_buff(buff_hash("SivirR"));
			if (buff != nullptr)
			{
				int num = (int)buff->get_end() - (int)gametime->get_time();
				std::string numstr = std::to_string(num);
				int firstdigit = std::stoi(numstr.substr(0, 2));
				const auto key = index++;
				const auto position = vector(1350.f + settings::draws::killListXOffset->get_int(), 80.f + settings::draws::killListYOffset->get_int() + (key * 50));
				draw_manager->add_text_on_screen(position, MAKE_COLOR(255, 255, 255, 255), settings::draws::TextSize->get_int(), "R Time: %i", firstdigit);
			}
		}
	}

	void QLogic()
	{
		if (settings::qsettings::qCombo->get_bool())
		{
			if (q->is_ready())
			{
				auto qtarget = target_selector->get_target(q->range(), damage_type::physical);
				if (qtarget != nullptr)
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

	void afteraa(game_object_script target)
	{
		if (settings::wsettings::wCombo->get_bool())
		{
			if (w->is_ready())
			{
				if (target->is_ai_hero() && target->is_enemy())
				{
					w->cast();
					orbwalker->reset_auto_attack_timer();
				}
			}
		}
	}

	void evadee()
	{
		if (settings::esettings::eEvade->get_bool())
		{
			if (e->is_ready())
			{
				for (auto skillshots : evade->get_skillshots())
				{
					if (skillshots.spell_data_danger_level >= settings::esettings::eDangerLevel->get_int() && skillshots.polygon.is_inside(myhero->get_position()))
					{
						e->cast();
						console->print("Spell Detected Blocking!");
					}
				}
			}
		}
	}

	void HarassQ()
	{
		if (settings::harass::qHarass->get_bool())
		{
			if (q->is_ready())
			{
				auto qtarget = target_selector->get_target(q->range(), damage_type::physical);
				if (qtarget != nullptr)
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

		if (enemy->get_real_health() <= qDamage(enemy))
		{
			auto qpred = q->get_prediction(enemy);
			if (qpred.hitchance >= hit_chance::high)
			{
				q->cast(qpred.get_cast_position());
			}
		}
	}

	void killstealloop()
	{
		for (auto&& enemy : entitylist->get_enemy_heroes())
		{
			if (enemy == nullptr || !enemy->is_valid_target()) continue;

			killstealQ(enemy);
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

		for (auto& Jungleminionq : minions_list)
		{
			if (Jungleminionq->get_distance(myhero) > q->range())
				continue;

			if (settings::clear::JcQ->get_bool() && q->is_ready())
			{
				q->cast(Jungleminionq->get_position());
			}
		}

		for (auto& Jungleminionw : minions_list)
		{
			if (!myhero->is_in_auto_attack_range(Jungleminionw))
				continue;

			if (settings::clear::JcW->get_bool() && w->is_ready())
			{
				w->cast();
			}
		}
	}

	void update()
	{
		if (orbwalker->combo_mode())
		{
			QLogic();
		}

		if (orbwalker->lane_clear_mode())
		{
			laneclear();
			jungleclear();
		}

		if (orbwalker->harass())
		{
			HarassQ();
		}

		killstealloop();
		evadee();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 1250.f);
		q->set_skillshot(0.25f + 0.066f, 90.f, 1450.f, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		q->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 0.f);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 0.f);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 0.f);
		r->set_spell_lock(false);

		createmenu();

		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_after_attack_orbwalker>::add_callback(afteraa, event_prority::high);
		event_handler<events::on_env_draw>::add_callback(on_env_draw, event_prority::high);
		event_handler<events::on_draw>::add_callback(on_draw, event_prority::high);
	}

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_after_attack_orbwalker>::remove_handler(afteraa);
		event_handler<events::on_draw>::remove_handler(on_draw);
		event_handler<events::on_env_draw>::remove_handler(on_env_draw);
	}

	float qrawDamage[] = { 30.f, 60.f, 90.f, 120.f, 150.f };
	float qBonusAD[] = { 1.6f, 1.7f, 1.8f, 1.9f, 2.f };

	float qDamage(const game_object_script& target)
	{
		if (q->level() == 0) return 0.f;

		damage_input input;
		input.raw_physical_damage = qrawDamage[q->level() - 1] + qBonusAD[q->level() - 1] * myhero->get_total_attack_damage() + 1.2f * myhero->get_total_ability_power();
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
			Damage += w->get_damage(target);
		}

		return Damage;
	}
}