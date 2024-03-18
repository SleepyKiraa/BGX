#include "../plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "femboyutils.h"
#include "Ashe.h"

namespace ashe
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
			TreeEntry* ebush;
		}

		namespace rsettings
		{
			TreeEntry* SemiR;
		}

		namespace harass
		{
			TreeEntry* wHarass;
		}

		namespace killsteal
		{
			TreeEntry* ksW;
		}

		namespace misc
		{
			TreeEntry* antigapcloser;
			TreeEntry* Interrupter;
		}

		namespace hitchance
		{
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
			TreeEntry* LcW;
			TreeEntry* JcW;
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

	void createmenu()
	{
		mainMenu = menu->create_tab("femboy.Ashe", "[Femboy] Ashe");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("femboy.Ashe.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("femboy.Ashe.qsettings", "Ranger´s Focus - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qCombo = qsettings->add_checkbox("femboy Ashe.useq", "Enable Q in Combo", true);
			}

			const auto wsettings = combotab->add_tab("femboy.Ashe.wsettings", "Volley - [W]");
			wsettings->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			{
				settings::wsettings::wCombo = wsettings->add_checkbox("femboy.Ashe.usew", "Enable W in Combo", true);
			}

			const auto esettings = combotab->add_tab("femboy.Ashe.esettings", "Hawkshot - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::ebush = esettings->add_checkbox("femboy.Ashe.ebush", "Auto E if Target enters a Bush", true);
			}

			const auto rsettings = combotab->add_tab("femboy.Ashe.rsettings", "Enchanted Crystal Arrow - [R]");
			rsettings->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			{
				settings::rsettings::SemiR = rsettings->add_hotkey("femboy.Ashe.semir", "Semi R", TreeHotkeyMode::Hold, 0x54, false, true);
			}
		}

		const auto harrastab = mainMenu->add_tab("femboy.Ashe.harass", "Harass");
		{
			settings::harass::wHarass = harrastab->add_checkbox("femboy.Ashe.wharass", "Enable W Harass", true);
		}

		const auto cleartab = mainMenu->add_tab("femboy.Ashe.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("femboy.Ashe.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcW = cleartab->add_checkbox("femboy.Ashe.wclear", "Enable W Lane Clear", true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcW = cleartab->add_checkbox("femboy.Ashe.wjclear", "Enable W Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("femboy.Ashe.ks", "Killsteal");
		{
			settings::killsteal::ksW = killstealtab->add_checkbox("femboy.Ashe.ksw", "Enable W Killsteal", true);
		}

		const auto misctab = mainMenu->add_tab("femboy.Ashe.misc", "Misc");
		{
			settings::misc::antigapcloser = misctab->add_checkbox("femboy.Ashe.antigap", "Enable Anti Gapcloser", true);
			settings::misc::Interrupter = misctab->add_checkbox("femboy.Ashe.interrupt", "Enable Interrupter", true);
		}

		const auto hitchancetab = mainMenu->add_tab("femboy.Ashe.hitchance", "Hitchance");
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

		settings::hitchance::wHitChance = hitchancetab->add_combobox("femboy.Ashe.whitchance", "W Hitchance", combo_elemts, 2);
		settings::hitchance::rHitChance = hitchancetab->add_combobox("femboy.Ashe.rhitchance", "R Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("femboy.Ashe.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			float defaultcolorW[] = { 0.f, 0.f, 1.f, 1.f };
			float defaultcolorE[] = { 0.f, 1.f, 0.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };
			const auto qdrawtab = drawTab->add_tab("femboy.Ashe.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("femboy.Ashe.qrange", "Draw Q Range", true);
				settings::draws::qColor = qdrawtab->add_colorpick("femboy.Ashe.qcolor", "Q Draw Color", defaultcolorQ);
			}

			const auto wdrawtab = drawTab->add_tab("femboy.Ashe.draww", "W Draw Settings");
			{
				settings::draws::wRange = wdrawtab->add_checkbox("femboy.Ashe.wrange", "Draw W Range", true);
				settings::draws::wColor = wdrawtab->add_colorpick("femboy.Ashe.wcolor", "W Draw Color", defaultcolorW);

			}

			const auto edrawtab = drawTab->add_tab("femboy.Ashe.drawe", "E Draw Settings");
			{
				settings::draws::eRange = edrawtab->add_checkbox("femboy.Ashe.erange", "Draw E Range", true);
				settings::draws::eColor = edrawtab->add_colorpick("femboy.Ashe.ecolor", "E Draw Color", defaultcolorE);
			}

			const auto rdrawtab = drawTab->add_tab("femboy.Ashe.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("femboy.Ashe.rrange", "Draw R Range", true);
				settings::draws::rColor = rdrawtab->add_colorpick("femboy.Ashe.rcolor", "R Draw Color", defaultcolorR);
			}

			const auto miscdrawTab = drawTab->add_tab("femboy.Ashe.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("femboy.Ashe.damage", "Draw Damage Indicator", true);
				settings::draws::legCircles = miscdrawTab->add_checkbox("femboy.Ashe.3dcircle", "Enable 3D Circle", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("femboy.Ashe.aaindicator", "Draw AA Counter", true);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Femboy] Ashe");
		Permashow::Instance.AddElement("Semi R", settings::rsettings::SemiR);
		Permashow::Instance.AddElement("Farm", settings::clear::farmtoggle);
	}

	void afteraa(game_object_script target)
	{
		if (settings::qsettings::qCombo->get_bool())
		{
			if (q->is_ready())
			{
				if (myhero->has_buff(buff_hash("asheqcastready")))
				{
					if (target != nullptr)
					{
						if (target->is_ai_hero() && target->is_enemy() && target->is_valid_target() && myhero->is_in_auto_attack_range(target))
						{
							q->cast();
						}
					}
				}
			}
		}
	}

	void WLogic()
	{
		auto wtarget = target_selector->get_target(w->range(), damage_type::physical);

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

	void ELogic()
	{

		if (settings::esettings::ebush->get_bool())
		{
			if (e->is_ready())
			{
				auto etarget = e->get_target();
				if (etarget != nullptr)
				{
					auto epred = e->get_prediction(etarget);
					if (navmesh->get_collision_flag(epred.get_cast_position()) == nav_collision_flags::grass)
					{
						e->cast(epred.get_cast_position());
					}
				}
			}
		}
	}

	void SemiR()
	{
		auto rtarget = target_selector->get_target(r->range(), damage_type::magical);

		if (r->is_ready())
		{
			if (rtarget != nullptr)
			{
				if (rtarget->is_valid_target())
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

	void HarassW()
	{
		auto wtarget = target_selector->get_target(w->range(), damage_type::physical);

		if (settings::harass::wHarass->get_bool())
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

	void killsteal(const game_object_script& enemy)
	{
		if (!w->is_ready()) return;
		if (!settings::killsteal::ksW->get_bool()) return;
		if (!enemy->is_valid_target(w->range())) return;

		if (enemy->get_real_health() <= WDamage(enemy))
		{
			auto wpred = w->get_prediction(enemy);
			if (wpred.hitchance >= hit_chance::high)
			{
				w->cast(wpred.get_cast_position());
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

		for (auto& minionW : minions_list)
		{
			if (minionW->get_distance(myhero) > w->range())
				continue;

			if (settings::clear::LcW->get_bool() && w->is_ready())
			{
				w->cast(minionW->get_position());
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

		for (auto& JungleminionW : minions_list)
		{
			if (JungleminionW->get_distance(myhero) > w->range())
				continue;

			if (settings::clear::JcW->get_bool() && w->is_ready())
			{
				w->cast(JungleminionW->get_position());
			}
		}
	}

	void killstealloop()
	{
		for (auto&& enemy : entitylist->get_enemy_heroes())
		{
			if (enemy == nullptr || !enemy->is_valid_target()) continue;

			killsteal(enemy);
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
				drawCircle(myhero->get_position(), r->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::rColor->get_color());
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
	
	void update()
	{
		if (settings::rsettings::SemiR->get_bool())
		{
			myhero->issue_order(hud->get_hud_input_logic()->get_game_cursor_position());

			SemiR();
		}

		if (orbwalker->combo_mode())
		{
			WLogic();
		}

		if (orbwalker->lane_clear_mode())
		{
			laneclear();
			jungleclear();
		}

		if (orbwalker->harass())
		{
			HarassW();
		}

		killstealloop();
		ELogic();

		interrupter();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 0);
		q->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 1200);
		w->set_skillshot(0.25f + 0.066f, 100.f, 2000.f, { collisionable_objects::yasuo_wall, collisionable_objects::minions }, skillshot_type::skillshot_cone);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 2000);
		e->set_skillshot(0.25f + 0.066f, 500.f, 1400.f, {}, skillshot_type::skillshot_line);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 2500);
		r->set_skillshot(0.25f + 0.066f, 130.f, 1500.f, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		r->set_spell_lock(false);

		createmenu();

		antigapcloser::add_event_handler(antigap);
		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_draw>::add_callback(on_draw_real, event_prority::high);
		event_handler<events::on_env_draw>::add_callback(on_draw, event_prority::high);
		event_handler<events::on_after_attack_orbwalker>::add_callback(afteraa, event_prority::high);
	}

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		antigapcloser::remove_event_handler(antigap);
		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_draw>::remove_handler(on_draw_real);
		event_handler<events::on_env_draw>::remove_handler(on_draw);
		event_handler<events::on_after_attack_orbwalker>::remove_handler(afteraa);
	}

	static constexpr float WRawDamage[]{ 0, 20, 35, 50, 65, 80 };
	static constexpr float RRawDamage[]{ 0, 200, 400, 600 };

	static float WDamage(const game_object_script& target)
	{
		auto wlevel = w->level();
		auto wdamage = WRawDamage[wlevel] + 1.0f * myhero->get_total_attack_damage();
		return damagelib->calculate_damage_on_unit(myhero, target, damage_type::physical, wdamage);
	}

	static float RDamage(const game_object_script& target)
	{
		auto rlevel = r->level();
		auto rdamage = RRawDamage[rlevel] + 1.2f * myhero->get_total_ability_power();
		return damagelib->calculate_damage_on_unit(myhero, target, damage_type::magical, rdamage);
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
			Damage += WDamage(target);
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

	void interrupter()
	{
		if (settings::misc::Interrupter->get_bool())
		{
			for (auto& enemy : entitylist->get_enemy_heroes())
			{
				if (r->is_ready())
				{
					if (enemy->is_valid_target(r->range()) && enemy != nullptr && enemy->is_enemy() && enemy->is_casting_interruptible_spell() == 2)
					{
						if (myhero->get_distance(enemy) <= r->range())
						{
							auto rpred = r->get_prediction(enemy);
							if (rpred.hitchance >= hit_chance::high)
							{
								r->cast(rpred.get_cast_position());
							}
						}
					}
				}
			}
		}
	}

	void antigap(game_object_script sender, antigapcloser::antigapcloser_args* args)
	{
		if (settings::misc::antigapcloser->get_bool())
		{
			if (r->is_ready())
			{
				if (sender->is_enemy() && sender->is_valid())
				{
					if (args->end_position.distance(myhero) < 350)
					{
						auto rpred = r->get_prediction(sender);
						if (rpred.hitchance >= hit_chance::high)
						{
							r->cast(rpred.get_cast_position());
						}
					}
				}
			}
		}
	}
}
