#include "../plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "femboyutils.h"
#include "Orianna.h"

namespace orianna
{
	script_spell* q;
	script_spell* w;
	script_spell* e;
	script_spell* r;
	game_object_script oriball = nullptr;
	vector ballpos;

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
			TreeEntry* AutoE;
			TreeEntry* SemiE;
			TreeEntry* incommingmindamage;
		}

		namespace rsettings
		{
			TreeEntry* rCombo;
			TreeEntry* minEneinR;
			TreeEntry* rHP;
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
			TreeEntry* LcW;
			TreeEntry* JcQ;
			TreeEntry* JcW;
		}

		namespace misc
		{
			TreeEntry* interrupter;
		}
	}

	void createmenu()
	{
		mainMenu = menu->create_tab("femboy.Orianna", "[Femboy] Orianna");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("femboy.Orianna.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("femboy.Orianna.qsettings", "Command: Attack - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qCombo = qsettings->add_checkbox("femboy.Orianna.useq", "Enable Q in Combo", true);
			}

			const auto wsettings = combotab->add_tab("femboy.Orianna.wsettings", "Command: Dissonance - [W]");
			wsettings->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			{
				settings::wsettings::wCombo = wsettings->add_checkbox("femboy.Orianna.usew", "Enable W in Combo", true);
			}

			const auto esettings = combotab->add_tab("femboy.Orianna.esettings", "Command: Protect - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::AutoE = esettings->add_checkbox("femboy.Orianna.usee", "Enable E Shielding", true);
				settings::esettings::incommingmindamage = esettings->add_slider("femboy.Orianna.damage", "Min Incomming Damage to Auto Shield", 200, 100, 1000, true);
				settings::esettings::SemiE = esettings->add_hotkey("femboy.Orianna.key", "Semi E", TreeHotkeyMode::Hold, 0x45, false, true);
			}

			const auto rsettings = combotab->add_tab("femboy.Orianna.rsettings", "Command: Shockwave - [R]");
			rsettings->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			{
				settings::rsettings::rCombo = rsettings->add_checkbox("femboy.Orianna.user", "Enable R in Combo", true);
				settings::rsettings::minEneinR = rsettings->add_slider("femboy.Orianna.minenEnemies", "Min Enemies in R Range to use R", 2, 1, 5, true);
				settings::rsettings::rHP = rsettings->add_slider("femboy.Orianna.health", "Health % to Use R on Solo Target", 50, 1, 100);
			}
		}

		const auto cleartab = mainMenu->add_tab("femboy.Orianna.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("femboy.Orianna.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcQ = cleartab->add_checkbox("femboy.Orianna.qclear", "Enable Q Lane Clear", true);
			settings::clear::LcW = cleartab->add_checkbox("femboy.Orianna.wclear", "Enable W Lane Clear", true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcQ = cleartab->add_checkbox("femboy.Orianna.qjclear", "Enable Q Jungle Clear", true);
			settings::clear::JcW = cleartab->add_checkbox("femboy.Orianna.wjclear", "Enable W Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("femboy.Orianna.ks", "Killsteal");
		{
			settings::killsteal::ksQ = killstealtab->add_checkbox("femboy.Orianna.ksq", "Enable Q Killsteal", true);
			settings::killsteal::ksW = killstealtab->add_checkbox("femboy.Orianna.ksw", "Enable W Killsteal", true);
			settings::killsteal::ksR = killstealtab->add_checkbox("femboy.Orianna.ksr", "Enable R Killsteal", true);
		}

		const auto misctab = mainMenu->add_tab("femboy.Orianna.misc", "Misc");
		{
			settings::misc::interrupter = misctab->add_checkbox("femboy.Orianna.inter", "Enable Interrupter", true);
		}

		const auto hitchancetab = mainMenu->add_tab("femboy.Orianna.hitchance", "Hitchance");
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

		settings::hitchance::qHitChance = hitchancetab->add_combobox("femboy.Orianna.qhitchance", "Q Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("femboy.Orianna.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			float defaultcolorW[] = { 0.f, 0.f, 1.f, 1.f };
			float defaultcolorE[] = { 0.f, 1.f, 0.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };
			const auto qdrawtab = drawTab->add_tab("femboy.Orianna.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("femboy.Orianna.qrange", "Draw Q Range", true);
				settings::draws::qColor = qdrawtab->add_colorpick("femboy.Orianna.qcolor", "Q Draw Color", defaultcolorQ);
			}

			const auto wdrawtab = drawTab->add_tab("femboy.Orianna.draww", "W Draw Settings");
			{
				settings::draws::wRange = wdrawtab->add_checkbox("femboy.Orianna.wrange", "Draw W Range", true);
				settings::draws::wColor = wdrawtab->add_colorpick("femboy.Orianna.wcolor", "W Draw Color", defaultcolorW);

			}

			const auto edrawtab = drawTab->add_tab("femboy.Orianna.drawe", "E Draw Settings");
			{
				settings::draws::eRange = edrawtab->add_checkbox("femboy.Orianna.erange", "Draw E Range", true);
				settings::draws::eColor = edrawtab->add_colorpick("femboy.Orianna.ecolor", "E Draw Color", defaultcolorE);
			}

			const auto rdrawtab = drawTab->add_tab("femboy.Orianna.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("femboy.Orianna.rrange", "Draw R Range", true);
				settings::draws::rColor = rdrawtab->add_colorpick("femboy.Orianna.rcolor", "R Draw Color", defaultcolorR);
			}

			const auto miscdrawTab = drawTab->add_tab("femboy.Orianna.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("femboy.Orianna.damage", "Draw Damage Indicator", true);
				settings::draws::legCircles = miscdrawTab->add_checkbox("femboy.Orianna.3dcircle", "Enable 3D Circle", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("femboy.Orianna.aaindicator", "Draw AA Counter", true);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Femboy] Orianna");
		Permashow::Instance.AddElement("Farm", settings::clear::farmtoggle);
		Permashow::Instance.AddElement("Semi E", settings::esettings::SemiE);
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

	void WLogic()
	{
		if (settings::wsettings::wCombo->get_bool())
		{
			if (w->is_ready())
			{
				if (oriball->is_valid())
				{
					if (ballpos.count_enemies_in_range(w->range()) >= 1)
					{
						w->cast();
					}
				}
			}
		}
	}

	void ELogic()
	{
		if (settings::esettings::AutoE->get_bool())
		{
			if (e->is_ready())
			{
				if (health_prediction->get_incoming_damage(myhero, 1.f, true) >= settings::esettings::incommingmindamage->get_int() || health_prediction->get_incoming_damage(myhero, 0.5f, true) >= myhero->get_real_health())
				{
					e->cast(myhero);
				}
			}
		}
	}

	void SemiE()
	{
		if (e->is_ready())
		{
			e->cast(myhero);
		}
	}

	void RLogic()
	{
		if (settings::rsettings::rCombo->get_bool())
		{
			if (r->is_ready())
			{
				if (oriball->is_valid())
				{
					if (ballpos.count_enemies_in_range(r->range()) >= settings::rsettings::minEneinR->get_int())
					{
						r->cast();
					}

					for (const auto& target : entitylist->get_enemy_heroes())
					{
						if (target != nullptr)
						{
							if (ballpos.distance(target->get_position()) <= r->range())
							{
								if (target->get_health_percent() <= settings::rsettings::rHP->get_int())
								{
									r->cast();
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
			auto qpred = q->get_prediction(enemy);
			if (qpred.hitchance >= hit_chance::high)
			{
				q->cast(qpred.get_cast_position());
			}
		}
	}

	void killstealW(const game_object_script& enemy)
	{
		if (!w->is_ready()) return;
		if (!settings::killsteal::ksW->get_bool()) return;
		if (ballpos.distance(enemy->get_position()) >= w->range()) return;

		if (enemy->get_real_health() <= wDamage(enemy))
		{
			w->cast();
		}
	}

	void killstealR(const game_object_script& enemy)
	{
		if (!r->is_ready()) return;
		if (!settings::killsteal::ksR->get_bool()) return;
		if (ballpos.distance(enemy->get_position()) > r->range()) return;
		if (q->is_ready() && w->is_ready()) return;

		if (enemy->get_real_health() <= rDamage(enemy))
		{
			r->cast();
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

		for (auto& minionW : minions_list)
		{
			if (ballpos.distance(minionW->get_position()) > w->range())
				continue;


			if (settings::clear::LcW->get_bool() && w->is_ready())
			{
				w->cast();
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
			if (ballpos.distance(Jungleminionw->get_position()) > w->range())
				continue;

			if (settings::clear::JcW->get_bool() && w->is_ready())
			{
				w->cast();
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
			killstealR(enemy);
		}
	}

	void on_env_draw()
	{
		if (settings::draws::qRange->get_bool())
		{
			if (q->level() >= 1)
			{
				drawCircle(myhero->get_position(), q->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::qColor->get_color(), 2);
			}
		}

		if (settings::draws::wRange->get_bool())
		{
			if (oriball != nullptr)
			{
				if (w->level() >= 1)
				{
					drawCircle(ballpos, w->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::wColor->get_color(), 2);
				}
			}
		}

		if (settings::draws::eRange->get_bool())
		{
			if (e->level() >= 1)
			{
				drawCircle(myhero->get_position(), e->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::eColor->get_color(), 2);
			}
		}

		if (settings::draws::rRange->get_bool())
		{
			if (oriball != nullptr)
			{
				if (r->level() >= 1)
				{
					drawCircle(ballpos, r->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::rColor->get_color(), 2);
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
	}

	void update()
	{
		if (orbwalker->combo_mode())
		{
			RLogic();
			WLogic();
			QLogic();
		}

		if (orbwalker->lane_clear_mode())
		{
			laneclear();
			jungleclear();
		}

		if (settings::esettings::SemiE->get_bool())
		{
			SemiE();
		}

		ELogic();
		killstealloop();
		interrupter();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 825.f);
		q->set_skillshot(0.25f + 0.066f, 80.f, 1400.f, {}, skillshot_type::skillshot_circle);
		q->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 225.f);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 1120.f);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 415.f);
		r->set_spell_lock(false);

		createmenu();

		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_create_object>::add_callback(oncreate, event_prority::high);
		event_handler<events::on_env_draw>::add_callback(on_env_draw, event_prority::high);
		event_handler<events::on_draw>::add_callback(on_draw_real, event_prority::high);
	}

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_create_object>::remove_handler(oncreate);
		event_handler<events::on_draw>::remove_handler(on_draw_real);
		event_handler<events::on_env_draw>::remove_handler(on_env_draw);
	}

	void oncreate(game_object_script obj)
	{
		if (obj->get_base_skin_name() == "TheDoomBall" ||obj->get_name() == "TheDoomBall")
		{
			oriball = obj;
			ballpos = obj->get_position();
			console->print("Ori Ball Position Updated!");
			return;
		}
	}

	float qrawdamage[] = { 60.f, 90.f, 120.f, 150.f, 180.f };
	float qDamage(const game_object_script& target)
	{
		if (q->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = qrawdamage[q->level() - 1] + myhero->get_total_ability_power() * 0.5f;
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float wrawdamage[] = { 70.f, 120.f, 170.f, 220.f, 270.f };
	float wDamage(const game_object_script& target)
	{
		if (w->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = wrawdamage[w->level() - 1] + myhero->get_total_ability_power() * 0.7f;
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float rrawdamage[] = { 250.f, 400.f, 550.f };
	float rDamage(const game_object_script& target)
	{
		if (r->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = rrawdamage[r->level() - 1] + myhero->get_total_ability_power() * 0.95f;
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
			Damage += e->get_damage(target);
		}

		if (r->is_ready())
		{
			Damage += rDamage(target);
		}

		return Damage;
	}

	void interrupter()
	{
		if (settings::misc::interrupter->get_bool())
		{
			for (auto&& enemy : entitylist->get_enemy_heroes())
			{
				if (r->is_ready())
				{
					if (enemy->is_enemy() && enemy->is_casting_interruptible_spell() == 2)
					{
						if (ballpos.distance(enemy->get_position()) <= r->range())
						{
							r->cast();
						}
					}
				}
			}
		}
	}
}