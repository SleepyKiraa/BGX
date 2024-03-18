#include "../plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "femboyutils.h"
#include "Corki.h"

namespace corki
{
	script_spell* q;
	script_spell* w;
	script_spell* e;
	script_spell* r;
	script_spell* r2;

	TreeTab* mainMenu;
	namespace settings {
		namespace qsettings
		{
			TreeEntry* qCombo;
		}

		namespace wsettings
		{
			TreeEntry* wevade;
			TreeEntry* wevadedangerlevel;
		}

		namespace esettings
		{
			TreeEntry* eafteraa;
		}

		namespace rsettings
		{
			TreeEntry* rCombo;
			TreeEntry* semiR;
		}

		namespace killsteal
		{
			TreeEntry* ksQ;
			TreeEntry* ksR;
		}

		namespace hitchance
		{
			TreeEntry* qHitChance;
			TreeEntry* rHitChance;
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
			TreeEntry* drawBuffTime;
		}

		namespace clear
		{
			TreeEntry* farmtoggle;
			TreeEntry* LcQ;
			TreeEntry* LcQmin;
			TreeEntry* LcE;
			TreeEntry* JcQ;
			TreeEntry* JcE;
		}
	}

	void createmenu()
	{
		mainMenu = menu->create_tab("femboy.Corki", "[Femboy] Corki");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("femboy.Corki.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("femboy.Corki.qsettings", "Phosphorus Bomb - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qCombo = qsettings->add_checkbox("femboy.Corki.useq", "Enable Q in Combo", true);
			}

			const auto wsettings = combotab->add_tab("femboy.Corki.wsettings", "Valkyrie - [W]");
			wsettings->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			{
				settings::wsettings::wevade = wsettings->add_checkbox("femboy.Corki.evadew", "Use W to Evade", true);
				settings::wsettings::wevadedangerlevel = wsettings->add_slider("femboy.Corki.evadelevel", "Evade Min Danager Level to use W", 5, 1, 5, true);
			}

			const auto esettings = combotab->add_tab("femboy.Corki.esettings", "Gatling Gun - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::eafteraa = esettings->add_checkbox("femboy.Corki.usee", "Enable E in Combo", true);
			}

			const auto rsettings = combotab->add_tab("femboy.Corki.rsettings", "Missile Barrage - [R]");
			rsettings->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			{
				settings::rsettings::rCombo = rsettings->add_checkbox("femboy.Corki.useR", "Enable R in Combo", true);
				settings::rsettings::semiR = rsettings->add_hotkey("femboy.Corki.semir", "Semi R", TreeHotkeyMode::Hold, 0x54, false, true);
			}
		}

		const auto cleartab = mainMenu->add_tab("femboy.Corki.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("femboy.Corki.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcQ = cleartab->add_checkbox("femboy.Corki.qclear", "Enable Q Lane Clear", true);
			settings::clear::LcQmin = cleartab->add_slider("femboy.Corki.minminions", "Min Minions to Q Clear", 2, 1, 5, true);
			settings::clear::LcE = cleartab->add_checkbox("femboy.Corki.eclear", "Enable E Lane Clear", true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcQ = cleartab->add_checkbox("femboy.Corki.qjclear", "Enable Q Jungle Clear", true);
			settings::clear::JcE = cleartab->add_checkbox("femboy.Corki.ejclear", "Enable E Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("femboy.Corki.ks", "Killsteal");
		{
			settings::killsteal::ksQ = killstealtab->add_checkbox("femboy.Corki.ksq", "Enable Q Killsteal", true);
			settings::killsteal::ksR = killstealtab->add_checkbox("femboy.Corki.ksr", "Enable R Killsteal", true);
		}

		const auto hitchancetab = mainMenu->add_tab("femboy.Corki.hitchance", "Hitchance");
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

		settings::hitchance::qHitChance = hitchancetab->add_combobox("femboy.Corki.qhitchance", "Q Hitchance", combo_elemts, 2);
		settings::hitchance::rHitChance = hitchancetab->add_combobox("femboy.Corki.rhitchance", "R Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("femboy.Corki.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			float defaultcolorW[] = { 0.f, 0.f, 1.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };
			const auto qdrawtab = drawTab->add_tab("femboy.Corki.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("femboy.Jhin.qrange", "Draw Q Range", true);
				settings::draws::qColor = qdrawtab->add_colorpick("femboy.Jhin.qcolor", "Q Draw Color", defaultcolorQ);
			}

			const auto wdrawtab = drawTab->add_tab("femboy.Corki.draww", "W Draw Settings");
			{
				settings::draws::wRange = wdrawtab->add_checkbox("femboy.Corki.wrange", "Draw W Range", true);
				settings::draws::wColor = wdrawtab->add_colorpick("femboy.Corki.wcolor", "W Draw Color", defaultcolorW);

			}

			const auto rdrawtab = drawTab->add_tab("femboy.Corki.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("femboy.Corki.rrange", "Draw R Range", true);
				settings::draws::rColor = rdrawtab->add_colorpick("femboy.Corki.rcolor", "R Draw Color", defaultcolorR);
			}

			const auto miscdrawTab = drawTab->add_tab("femboy.Corki.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("femboy.Corki.damage", "Draw Damage Indicator", true);
				settings::draws::legCircles = miscdrawTab->add_checkbox("femboy.Corki.3dcircle", "Enable 3D Circle", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("femboy.Corki.aaindicator", "Draw AA Counter", true);
				settings::draws::drawBuffTime = miscdrawTab->add_checkbox("femboy.Corki.drawbufftime", "Draw Buff Time", true);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Femboy] Corki");
		Permashow::Instance.AddElement("Semi R", settings::rsettings::semiR);
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

	void QLogic()
	{
		if (settings::qsettings::qCombo->get_bool())
		{
			if (q->is_ready())
			{
				auto qtarget = target_selector->get_target(q->range(), damage_type::magical);
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

	void Wevade()
	{
		if (settings::wsettings::wevade->get_bool())
		{
			if (w->is_ready())
			{
				for (auto skillshots : evade->get_skillshots())
				{
					if (skillshots.spell_data_danger_level >= settings::wsettings::wevadedangerlevel->get_int() && skillshots.polygon.is_inside(myhero->get_position()) && skillshots.polygon.is_outside(hud->get_hud_input_logic()->get_game_cursor_position()))
					{
						w->cast(hud->get_hud_input_logic()->get_game_cursor_position());
					}
				}
			}
		}
	}

	void afteraa(game_object_script target)
	{
		if (!orbwalker->combo_mode())
		{
			return;
		}

		if (settings::esettings::eafteraa->get_bool())
		{
			if (e->is_ready())
			{
				if (target != nullptr)
				{
					if (target->is_ai_hero() && target->is_enemy() && target->is_valid_target())
					{
						e->cast();
					}
				}
			}
		}
	}

	void RLogic()
	{
		auto R1Target = target_selector->get_target(r->range(), damage_type::magical);
		auto R2Target = target_selector->get_target(r2->range(), damage_type::magical);

		if (myhero->is_winding_up())
		{
			return;
		}

		if (settings::rsettings::rCombo->get_bool())
		{
			if (r->is_ready())
			{
				if (!myhero->has_buff(buff_hash("mbcheck2")))
				{
					if (R1Target != nullptr)
					{
						if (R1Target->is_valid_target())
						{
							auto r1pred = r->get_prediction(R1Target);
							if (r1pred.hitchance >= getPredIntFromSettings(settings::hitchance::rHitChance->get_int()))
							{
								r->cast(r1pred.get_cast_position());
							}
						}
					}
				}

				if (myhero->has_buff(buff_hash("mbcheck2")))
				{
					if (R2Target != nullptr)
					{
						if (R2Target->is_valid_target())
						{
							auto r2pred = r2->get_prediction(R2Target);
							if (r2pred.hitchance >= getPredIntFromSettings(settings::hitchance::rHitChance->get_int()))
							{
								r2->cast(r2pred.get_cast_position());
							}
						}
					}
				}
			}
		}
	}

	void SemiR1()
	{
		if (r->is_ready() && !myhero->has_buff(buff_hash("mbcheck2")))
		{
			auto rtarget = target_selector->get_target(r->range(), damage_type::magical);
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

	void SemiR2()
	{
		if (r2->is_ready() && myhero->has_buff(buff_hash("mbcheck2")))
		{
			auto r2target = target_selector->get_target(r2->range(), damage_type::magical);
			if (r2target != nullptr)
			{
				if (r2target->is_valid_target())
				{
					auto r2pred = r2->get_prediction(r2target);
					if (r2pred.hitchance >= getPredIntFromSettings(settings::hitchance::rHitChance->get_int()))
					{
						r2->cast(r2pred.get_cast_position());
					}
				}
			}
		}
	}

	void KillstealQ(const game_object_script& enemy)
	{
		if (!q->is_ready()) return;
		if (!settings::killsteal::ksQ->get_bool()) return;
		if (!enemy->is_valid_target(q->range())) return;

		if (enemy->get_real_health() <= QDamage(enemy))
		{
			auto qpred = q->get_prediction(enemy);
			if (qpred.hitchance >= hit_chance::high)
			{
				q->cast(qpred.get_cast_position());
			}
		}
	}

	void KillstealR(const game_object_script& enemy)
	{
		if (!r->is_ready()) return;
		if (!settings::killsteal::ksR->get_bool()) return;

		if (!myhero->has_buff(buff_hash("mbcheck2")))
		{
			if (enemy->is_valid_target(r->range()))
			{
				if (enemy->get_real_health() <= RDamage(enemy))
				{
					auto rpred = r->get_prediction(enemy);
					if (rpred.hitchance >= hit_chance::high)
					{
						r->cast(rpred.get_cast_position());
					}
				}
			}
		}

		if (myhero->has_buff(buff_hash("mbcheck2")))
		{
			if (enemy->is_valid_target(r2->range()))
			{
				if (enemy->get_real_health() <= R2Damage(enemy))
				{
					auto r2pred = r2->get_prediction(enemy);
					if (r2pred.hitchance >= hit_chance::high)
					{
						r2->cast(r2pred.get_cast_position());
					}
				}
			}
		}
	}

	void killstealloop()
	{
		for (auto&& enemy : entitylist->get_enemy_heroes())
		{
			if (enemy == nullptr || !enemy->is_valid_target()) continue;

			KillstealQ(enemy);
			KillstealR(enemy);
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
				q->cast_on_best_farm_position(settings::clear::LcQmin->get_int(), false);
			}
		}

		for (auto& minionE : minions_list)
		{
			if (minionE->get_distance(myhero) > myhero->get_attackRange())
				continue;

			if (settings::clear::LcE->get_bool() && e->is_ready())
			{
				e->cast();
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

		for (auto& JungleminionE : minions_list)
		{
			if (JungleminionE->get_distance(myhero) > myhero->get_attackRange())
				continue;

			if (settings::clear::JcE->get_bool() && e->is_ready())
			{
				e->cast();
			}
		}
	}

	void on_draw_env()
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

		if (settings::draws::rRange->get_bool())
		{
			if (r->level() >= 1)
			{
				if (!myhero->has_buff(buff_hash("mbcheck2")))
				{
					drawCircle(myhero->get_position(), r->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::rColor->get_color(), 1);
				}

				if (myhero->has_buff(buff_hash("mbcheck2")))
				{
					drawCircle(myhero->get_position(), r2->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::rColor->get_color(), 1);
				}
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

		if (settings::draws::drawBuffTime->get_bool())
		{
			auto buff = myhero->get_buff(buff_hash("corkiloaded"));
			if (buff != nullptr)
			{
				int num = (int)buff->get_end() - (int)gametime->get_time();
				std::string numstr = std::to_string(num);
				int firstdigit = std::stoi(numstr.substr(0, 2));
				const auto position = vector(688.f, 551.f);
				draw_manager->add_text_on_screen(position, MAKE_COLOR(255, 255, 255, 255), 22, "Passive Time: %i", firstdigit);
			}
		}
	}

	void update()
	{
		if (settings::rsettings::semiR->get_bool())
		{
			myhero->issue_order(hud->get_hud_input_logic()->get_game_cursor_position());

			SemiR1();
			SemiR2();
		}

		if (orbwalker->combo_mode())
		{
			RLogic();
			QLogic();
		}

		if (orbwalker->lane_clear_mode())
		{
			laneclear();
			jungleclear();
		}

		killstealloop();
		Wevade();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 825.f);
		q->set_skillshot(0.25f + 0.066f, 130.f, 1000.f, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_circle);
		q->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 600.f);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 0.f);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 1300.f);
		r->set_skillshot(0.175f + 0.066f, 40.f, 2000.f, { collisionable_objects::minions, collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		r->set_spell_lock(false);

		r2 = plugin_sdk->register_spell(spellslot::r, 1500.f);
		r2->set_skillshot(0.175f + 0.066f, 40.f, 2000.f, { collisionable_objects::minions, collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		r2->set_spell_lock(false);

		createmenu();

		event_handler<events::on_after_attack_orbwalker>::add_callback(afteraa, event_prority::high);
		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_env_draw>::add_callback(on_draw_env, event_prority::high);
		event_handler<events::on_draw>::add_callback(on_draw_real, event_prority::high);
	}

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);
		plugin_sdk->remove_spell(r2);

		event_handler<events::on_after_attack_orbwalker>::remove_handler(afteraa);
		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_env_draw>::remove_handler(on_draw_env);
		event_handler<events::on_draw>::remove_handler(on_draw_real);
	}

	float QRawDamage[] { 75, 120, 210, 255 };

	float QDamage(const game_object_script& target)
	{
		if (q->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = QRawDamage[q->level() - 1] + myhero->get_total_ability_power() * 0.5f + myhero->get_total_attack_damage() * 0.7f;
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float RRawDamage[]{ 80, 115, 150 };
	float RBonusDamage[]{ 0.15, 0.45, 0.75 };

	float RDamage(const game_object_script& target)
	{
		if (r->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = RRawDamage[r->level() - 1] + myhero->get_total_ability_power() * 0.12f + RBonusDamage[r->level() - 1] * myhero->get_total_attack_damage();
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float R2RawDamage[]{ 160, 230, 300 };
	float R2BonusDamage[]{ 0.30, 0.90, 1.50 };

	float R2Damage(const game_object_script& target)
	{
		if (r->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = R2RawDamage[r->level() - 1] + myhero->get_total_ability_power() * 0.24f + R2BonusDamage[r->level() - 1] * myhero->get_total_attack_damage();
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

		if (r->is_ready() && !myhero->has_buff(buff_hash("mbcheck2")))
		{
			Damage += RDamage(target);
		}
		else if (r->is_ready() && myhero->has_buff(buff_hash("mbcheck2")))
		{
			Damage += R2Damage(target);
		}

		return Damage;
	}
}