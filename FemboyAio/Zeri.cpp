#include "../plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "femboyutils.h"
#include "Zeri.h"

namespace zeri
{
	script_spell* q;
	script_spell* q2;
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
			TreeEntry* semiW;
		}

		namespace rsettings
		{
			TreeEntry* rCombo;
			TreeEntry* minEne;
		}

		namespace killsteal
		{
			TreeEntry* ksQ;
			TreeEntry* ksW;
			TreeEntry* ksR;
		}

		namespace hitchance
		{
			TreeEntry* qHitChance;
			TreeEntry* wHitChance;
		}

		namespace draws
		{
			TreeEntry* qRange;
			TreeEntry* wRange;
			TreeEntry* eRange;
			TreeEntry* rRange;
			TreeEntry* Qcolor;
			TreeEntry* Wcolor;
			TreeEntry* Ecolor;
			TreeEntry* Rcolor;
			TreeEntry* Damage;
			TreeEntry* legCircles;
			TreeEntry* NeededAA;
			TreeEntry* buffs;
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
			TreeEntry* DisableAAWithoutPassive;
		}
	}

	void createmenu()
	{
		mainMenu = menu->create_tab("femboy.Zeri", "[Femboy] Zeri");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("femboy.Zeri.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("femboy.Zeri.qsettings", "Burst Fire - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qCombo = qsettings->add_checkbox("femboy.Zeri.useq", "Enable Q in Combo", true);
			}

			const auto wsettings = combotab->add_tab("femboy.Zeri.wsettings", "Ultrashock Laser - [W]");
			wsettings->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			{
				settings::wsettings::wCombo = wsettings->add_checkbox("femboy.Zeri.usew", "Enable W in Combo", true);
				settings::wsettings::semiW = wsettings->add_hotkey("femboy.Zeri.semiw", "Semi W", TreeHotkeyMode::Hold, 0x54, false, true);
			}

			const auto rsettings = combotab->add_tab("femboy.Zeri.rsettings", "Lightning Crash - [R]");
			rsettings->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			{
				settings::rsettings::rCombo = rsettings->add_checkbox("femboy.Zeri.user", "Enable R in Combo", true);
				settings::rsettings::minEne = rsettings->add_slider("femboy.Zeri.enemys", "Min Enemys in R Range", 2, 1, 5, true);
			}
		}

		const auto cleartab = mainMenu->add_tab("femboy.Zeri.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("femboy.Zeri.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcQ = cleartab->add_checkbox("femboy.Zeri.qclear", "Enable Q Lane Clear", true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcQ = cleartab->add_checkbox("femboy.Zeri.qjclear", "Enable Q Jungle Clear", true);
			settings::clear::JcW = cleartab->add_checkbox("femboy.Zeri.wjclear", "Enable W Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("femboy.Zeri.ks", "Killsteal");
		{
			settings::killsteal::ksQ = killstealtab->add_checkbox("femboy.Zeri.ksq", "Enable Q Killsteal", true);
			settings::killsteal::ksW = killstealtab->add_checkbox("femboy.Zeri.ksw", "Enable W Killsteal", true);
			settings::killsteal::ksR = killstealtab->add_checkbox("femboy.Zeri.ksr", "Enable R Killsteal", false);
		}

		const auto misctab = mainMenu->add_tab("femboy.Zeri.misc", "Misc");
		{
			settings::misc::DisableAAWithoutPassive = misctab->add_checkbox("femboy.Zeri.anti", "Disable AA when Passive is not Ready", true);
		}

		const auto hitchancetab = mainMenu->add_tab("femboy.Zeri.hitchance", "Hitchance");
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

		settings::hitchance::qHitChance = hitchancetab->add_combobox("femboy.Zeri.qhitchance", "Q Hitchance", combo_elemts, 2);
		settings::hitchance::wHitChance = hitchancetab->add_combobox("femboy.Zeri.whitchance", "W Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("femboy.Zeri.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			float defaultcolorW[] = { 0.f, 0.f, 1.f, 1.f };
			float defaultcolorE[] = { 0.f, 1.f, 0.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };

			const auto qdrawtab = drawTab->add_tab("femboy.Zeri.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("femboy.Kalista.qrange", "Draw Q Range", true);
				settings::draws::Qcolor = qdrawtab->add_colorpick("femboy.Kalista.qcolor", "Q Draw Color", defaultcolorQ);
			}

			const auto wdrawtab = drawTab->add_tab("femboy.Zeri.draww", "W Draw Settings");
			{
				settings::draws::wRange = wdrawtab->add_checkbox("femboy.Zeri.wrange", "Draw W Range", true);
				settings::draws::Wcolor = wdrawtab->add_colorpick("femboy.Zeri.wcolor", "W Draw Color", defaultcolorW);
			}

			const auto edrawtab = drawTab->add_tab("femboy.Zeri.drawe", "E Draw Settings");
			{
				settings::draws::eRange = edrawtab->add_checkbox("femboy.Zeri.erange", "Draw E Range", true);
				settings::draws::Ecolor = edrawtab->add_colorpick("femboy.Zeri.ecolor", "E Draw Color", defaultcolorE);
			}

			const auto rdrawtab = drawTab->add_tab("femboy.Zeri.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("femboy.Zeri.rrange", "Draw R Range", true);
				settings::draws::Rcolor = rdrawtab->add_colorpick("femboy.Zeri.rcolor", "R Draw Color", defaultcolorR);
			}

			const auto miscdrawTab = drawTab->add_tab("femboy.Zeri.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("femboy.Zeri.damage", "Draw Damage Indicator", true);
				settings::draws::legCircles = miscdrawTab->add_checkbox("femboy.Zeri.3dcircle", "Enable 3D Circle", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("femboy.Zeri.aaindicator", "Draw AA Counter", true);
				settings::draws::buffs = miscdrawTab->add_checkbox("femboy.Zeri.buffs", "Draw Buffs", true);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Femboy] Zeri");
		Permashow::Instance.AddElement("Farm", settings::clear::farmtoggle);
		Permashow::Instance.AddElement("Semi W", settings::wsettings::semiW);
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
		if (settings::qsettings::qCombo->get_bool())
		{
			if (q->is_ready())
			{
				auto qtarget = target_selector->get_target(q->range(), damage_type::physical);
				if (qtarget != nullptr)
				{
					if (qtarget->is_valid_target())
					{
						if (!myhero->has_buff(buff_hash("ZeriESpecialRounds")))
						{
							auto qpred = q->get_prediction(qtarget);
							if (qpred.hitchance >= getPredIntFromSettings(settings::hitchance::qHitChance->get_int()))
							{
								q->cast(qpred.get_cast_position());
							}
						}

						if (myhero->has_buff(buff_hash("ZeriESpecialRounds")))
						{
							auto q2pred = q2->get_prediction(qtarget);
							if (q2pred.hitchance >= getPredIntFromSettings(settings::hitchance::qHitChance->get_int()))
							{
								q2->cast(q2pred.get_cast_position());
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
					if (wtarget->is_valid_target())
					{
						if (wtarget->get_distance(myhero) > myhero->get_attackRange())
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

	void SemiW()
	{
		if (w->is_ready())
		{
			auto wtarget = target_selector->get_target(w->range(), damage_type::physical);
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

	void RLogic()
	{
		if (settings::rsettings::rCombo->get_bool())
		{
			if (r->is_ready())
			{
				if (myhero->count_enemies_in_range(r->range()) >= settings::rsettings::minEne->get_int())
				{
					r->cast();
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
			if (!myhero->has_buff(buff_hash("ZeriESpecialRounds")))
			{
				auto qpred = q->get_prediction(enemy);
				if (qpred.hitchance >= hit_chance::high)
				{
					q->cast(qpred.get_cast_position());
				}
			}

			if (myhero->has_buff(buff_hash("ZeriESpecialRounds")))
			{
				auto q2pred = q2->get_prediction(enemy);
				if (q2pred.hitchance >= hit_chance::high)
				{
					q2->cast(q2pred.get_cast_position());
				}
			}
		}
	}

	void killstealW(const game_object_script& enemy)
	{
		if (!w->is_ready()) return;
		if (!settings::killsteal::ksW->get_bool()) return;
		if (!enemy->is_valid_target(w->range())) return;

		if (enemy->get_real_health() <= WDamage(enemy))
		{
			if (enemy->get_distance(myhero) >= myhero->get_attackRange())
			{
				auto wpred = w->get_prediction(enemy);
				if (wpred.hitchance >= hit_chance::high)
				{
					w->cast(wpred.get_cast_position());
				}
			}
		}
	}

	void killstealR(const game_object_script& enemy)
	{
		if (!r->is_ready()) return;
		if (!settings::killsteal::ksR->get_bool()) return;
		if (!enemy->is_valid_target(r->range())) return;

		if (enemy->get_real_health() <= RDamage(enemy))
		{
			r->cast();
		}
	}

	void killstealloop()
	{
		for (auto&& enemy : entitylist->get_enemy_heroes())
		{
			if (enemy == nullptr || !enemy->is_valid_target()) continue;

			killstealQ(enemy);
			killstealW(enemy);
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

	void on_env_draw()
	{
		if (settings::draws::qRange->get_bool())
		{
			if (q->level() >= 1)
			{
				drawCircle(myhero->get_position(), q->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::Qcolor->get_color());
			}
		}

		if (settings::draws::wRange->get_bool())
		{
			if (w->level() >= 1)
			{
				drawCircle(myhero->get_position(), w->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::Wcolor->get_color());
			}
		}

		if (settings::draws::eRange->get_bool())
		{
			if (e->level() >= 1)
			{
				drawCircle(myhero->get_position(), e->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::Ecolor->get_color());
			}
		}

		if (settings::draws::rRange->get_bool())
		{
			if (r->level() >= 1)
			{
				drawCircle(myhero->get_position(), r->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::Rcolor->get_color());
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
					AATracker();
				}
			}
		}

		if (settings::draws::buffs->get_bool())
		{
			auto buff1 = myhero->get_buff(buff_hash("ZeriESpecialRounds"));
			auto buff2 = myhero->get_buff(buff_hash("ZeriR"));
			if (buff1 != nullptr)
			{
				int num = (int)buff1->get_end() - (int)gametime->get_time();
				std::string numstr = std::to_string(num);
				int firstdigit = std::stoi(numstr.substr(0, 2));
				const auto pos = vector(800.f, 590.f);
				draw_manager->add_text_on_screen(pos, MAKE_COLOR(255, 255, 255, 255), 22, "E Time: %i", firstdigit);
			}

			if (buff2 != nullptr)
			{
				int num = (int)buff2->get_end() - (int)gametime->get_time();
				std::string numstr = std::to_string(num);
				int firstdigit = std::stoi(numstr.substr(0, 2));
				const auto pos = vector(800.f, 620.f);
				const auto pos2 = vector(800.f, 590.f);
				if (buff1 == nullptr)
				{
					draw_manager->add_text_on_screen(pos2, MAKE_COLOR(255, 255, 255, 255), 22, "R Time: %i", firstdigit);
				}
				else if (buff1 != nullptr)
				{
					draw_manager->add_text_on_screen(pos, MAKE_COLOR(255, 255, 255, 255), 22, "R Time: %i", firstdigit);
				}
			}
		}
	}

	void update()
	{
		UpdateQRange();

		if (orbwalker->combo_mode())
		{
			QLogic();
			RLogic();
			WLogic();
		}

		if (settings::wsettings::semiW->get_bool())
		{
			myhero->issue_order(hud->get_hud_input_logic()->get_game_cursor_position());

			SemiW();
		}

		if (orbwalker->lane_clear_mode())
		{
			laneclear();
			jungleclear();
			attackturretwithq();
		}

		killstealloop();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 750.f);
		q->set_skillshot(0.25f + 0.066f, 40.f, 2600.f, { collisionable_objects::yasuo_wall, collisionable_objects::minions }, skillshot_type::skillshot_line);
		q->set_spell_lock(false);

		q2 = plugin_sdk->register_spell(spellslot::q, 750.f);
		q2->set_skillshot(0.25f + 0.066f, 40.f, 3400.f, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		q2->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 1200.f);
		w->set_skillshot(0.5f + 0.066f, 40.f, 2500.f, { collisionable_objects::yasuo_wall, collisionable_objects::minions }, skillshot_type::skillshot_line);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 300.f);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 825.f);
		r->set_spell_lock(false);

		createmenu();

		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_before_attack_orbwalker>::add_callback(beforeaa, event_prority::high);
		event_handler<events::on_env_draw>::add_callback(on_env_draw, event_prority::high);
		event_handler<events::on_draw>::add_callback(on_draw_real, event_prority::high);
	}

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(q2);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_before_attack_orbwalker>::remove_handler(beforeaa);
		event_handler<events::on_env_draw>::remove_handler(on_env_draw);
		event_handler<events::on_draw>::remove_handler(on_draw_real);
	}

	void UpdateQRange()
	{
		if (myhero->get_buff_count(buff_hash("ASSETS/Perks/Styles/Precision/LethalTempo/LethalTempo.lua")) == 6)
		{
			console->print("Found Lethal Tempo!");
			q->set_range(750.f + 50.f);
		}
		else
		{
			q->set_range(750.f);
		}

		if (myhero->has_item(ItemId::Rapid_Firecannon) != spellslot::invalid)
		{
			if (myhero->get_buff_count(buff_hash("itemstatikshankcharge")) == 100)
			{
				console->print("Rapid Fire Found");
				q->set_range(750.f + 150.f);
			}
			else
			{
				q->set_range(750.f);
			}
		}
	}

	void beforeaa(game_object_script target, bool* process)
	{
		if (settings::misc::DisableAAWithoutPassive->get_bool())
		{
			if (!myhero->has_buff(buff_hash("ZeriQPassiveReady")))
			{
				*process = false;
			}
		}
	}

	void AATracker()
	{
		for (const auto& target : entitylist->get_enemy_heroes())
		{
			if (target != nullptr && target->is_visible_on_screen() && !target->is_dead())
			{
				int num = (int)(target->get_health() / QDamage(target));
				std::string numstr = std::to_string(num);
				int firstdigit = std::stoi(numstr.substr(0, 2));
				draw_manager->add_text(target->get_position(), MAKE_COLOR(255, 255, 255, 255), 22, "Q Left: %i", firstdigit);
			}
		}
	}

	float qrawdamage[] = { 15.f, 17.f, 19.f, 21.f, 23.f };
	float qadscaling[] = { 1.04f, 1.08f, 1.12f, 1.16f, 1.20f };
	float QDamage(const game_object_script& target)
	{
		if (q->level() == 0) return 0.f;

		damage_input input;
		input.raw_physical_damage = qrawdamage[q->level() - 1] + qadscaling[q->level() - 1] * myhero->get_total_attack_damage();
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float wrawdamage[] = { 30.f, 70.f, 110.f, 150.f, 190.f };
	float WDamage(const game_object_script& target)
	{
		if (w->level() == 0) return 0.f;

		damage_input input;
		input.raw_physical_damage = wrawdamage[w->level() - 1] + myhero->get_total_attack_damage() * 1.30f + myhero->get_total_ability_power() * 0.25f;
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float rrawdamage[] = { 175.f, 275.f, 375.f };
	float RDamage(const game_object_script& target)
	{
		if (r->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = rrawdamage[r->level() - 1] + myhero->mPercentBonusPhysicalDamageMod() * 0.85f + myhero->get_total_ability_power() * 1.10f;
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
			Damage += e->get_damage(target);
		}

		if (r->is_ready())
		{
			Damage += RDamage(target);
		}

		return Damage;
	}

	void attackturretwithq()
	{
		if (q->is_ready())
		{
			for (auto& turret : entitylist->get_enemy_turrets())
			{
				if (turret != nullptr)
				{
					if (turret->get_distance(myhero) <= q->range())
					{
						if (!turret->is_dead())
						{
							if (myhero->count_enemies_in_range(q->range()) <= 1)
							{
								q->cast(turret->get_position());
							}
						}
					}
				}
			}

			for (auto& inhib : entitylist->get_enemy_inhibitors())
			{
				if (inhib != nullptr)
				{
					if (inhib->get_distance(myhero) <= q->range())
					{
						if (!inhib->is_dead())
						{
							if (myhero->count_enemies_in_range(q->range()) <= 1)
							{
								q->cast(inhib->get_position());
							}
						}
					}
				}
			}
		}
	}
}