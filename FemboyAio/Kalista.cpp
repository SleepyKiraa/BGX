#include "../plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "femboyutils.h"
#include "Kalista.h"

namespace kalista
{
	script_spell* q;
	script_spell* w;
	script_spell* e;
	script_spell* r;
	game_object_script ally = nullptr;

	TreeTab* mainMenu;
	namespace settings {
		namespace qsettings
		{
			TreeEntry* qCombo;
		}

		namespace esettings
		{
			TreeEntry* EBeforeDeath;
			TreeEntry* EOnStacks;
			TreeEntry* EStackscount;
		}

		namespace rsettings
		{
			TreeEntry* AutoRSave;
			TreeEntry* Ballista;
			TreeEntry* Ballistarange;
		}

		namespace killsteal
		{
			TreeEntry* ksQ;
			TreeEntry* ksE;
		}

		namespace hitchance
		{
			TreeEntry* qHitChance;
		}

		namespace draws
		{
			TreeEntry* qRange;
			TreeEntry* eRange;
			TreeEntry* rRange;
			TreeEntry* Qcolor;
			TreeEntry* Ecolor;
			TreeEntry* Rcolor;
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

	void createmenu()
	{
		mainMenu = menu->create_tab("femboy.Kalista", "[Femboy] Kalista");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("femboy.Kalista.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("femboy.Kalista.qsettings", "Pierce - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qCombo = qsettings->add_checkbox("femboy.Kalista.useq", "Enable Q in Combo", true);
			}

			const auto esettings = combotab->add_tab("femboy.Kalista.esettings", "Rend - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::EOnStacks = esettings->add_checkbox("femboy.Kalista.eonstacks", "Enable E in Combo", false);
				settings::esettings::EBeforeDeath = esettings->add_checkbox("femboy.Kalista.eondeat", "Auto E Before Death", true);				
				settings::esettings::EStackscount = esettings->add_slider("femboy.Kalista.estackcount", "Min Stacks on Target to use E", 10, 1, 50);
			}

			const auto rsettings = combotab->add_tab("femboy.Kalista.rsettings", "Fate's Call - [R]");
			rsettings->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			{
				settings::rsettings::AutoRSave = rsettings->add_checkbox("femboy.Kalista.autosave", "Auto Save Ally if Incomming Damage is Higher then his HP", true);
				settings::rsettings::AutoRSave->set_tooltip("DONT RELOAD AFTER CONNECTING WITH YOUR ALLY OTHER WISE THE R LOGICS IS BROKEN FOR THE WHOLE ROUND!");
				for (auto&& allys : entitylist->get_ally_heroes())
				{
					if (allys->get_base_skin_name() == "Blitzcrank")
					{
						settings::rsettings::Ballista = rsettings->add_checkbox("femboy.Kalista.ballista", "Enable Ballista", true);
						settings::rsettings::Ballistarange = rsettings->add_slider("femoby.Kalista.ballistarange", "Min Range to Ally To Ballista", 150, 0, 600);
					}
				}
			}
		}

		const auto cleartab = mainMenu->add_tab("femboy.Kalista.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("femboy.Kalista.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcQ = cleartab->add_checkbox("femboy.Kalista.qclear", "Enable Q Lane Clear", true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcQ = cleartab->add_checkbox("femboy.Kalista.qjclear", "Enable Q Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("femboy.Kalista.ks", "Killsteal");
		{
			settings::killsteal::ksQ = killstealtab->add_checkbox("femboy.Kalista.ksq", "Enable Q Killsteal", true);
			settings::killsteal::ksE = killstealtab->add_checkbox("femboy.Kalista.kse", "Enable E Killsteal", true);
		}

		const auto hitchancetab = mainMenu->add_tab("femboy.Kalista.hitchance", "Hitchance");
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

		settings::hitchance::qHitChance = hitchancetab->add_combobox("femboy.Kalista.qhitchance", "Q Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("femboy.Kalista.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			float defaultcolorE[] = { 0.f, 1.f, 0.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };

			const auto qdrawtab = drawTab->add_tab("femboy.Kalista.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("femboy.Kalista.qrange", "Draw Q Range", true);
				settings::draws::Qcolor = qdrawtab->add_colorpick("femboy.Kalista.qcolor", "Q Draw Color", defaultcolorQ);
			}

			const auto edrawtab = drawTab->add_tab("femboy.Kalista.drawe", "E Draw Settings");
			{
				settings::draws::eRange = edrawtab->add_checkbox("femboy.Kalista.erange", "Draw E Range", true);
				settings::draws::Ecolor = edrawtab->add_colorpick("femboy.Kalista.ecolor", "E Draw Color", defaultcolorE);
			}

			const auto rdrawtab = drawTab->add_tab("femboy.Kalista.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("femboy.Kalista.rrange", "Draw R Range", true);
				settings::draws::Rcolor = rdrawtab->add_colorpick("femboy.Kalista.rcolor", "R Draw Color", defaultcolorR);
			}

			const auto miscdrawTab = drawTab->add_tab("femboy.Kalista.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("femboy.Kalista.damage", "Draw Damage Indicator", true);
				settings::draws::legCircles = miscdrawTab->add_checkbox("femboy.Kalista.3dcircle", "Enable 3D Circle", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("femboy.Kalista.aaindicator", "Draw AA Counter", true);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Femboy] Kalista");
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
		if (myhero->is_winding_up())
		{
			return;
		}

		if (settings::qsettings::qCombo->get_bool())
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

	void ELogic()
	{
		if (settings::esettings::EOnStacks->get_bool())
		{
			if (orbwalker->combo_mode())
			{
				if (e->is_ready())
				{
					auto etarget = target_selector->get_target(e->range(), damage_type::physical);
					if (etarget != nullptr)
					{
						if (etarget->is_valid_target())
						{
							if (etarget->get_buff_count(buff_hash("kalistaexpungemarker")) >= settings::esettings::EStackscount->get_int())
							{
								e->cast();
							}
						}
					}
				}
			}
		}

		if (settings::esettings::EBeforeDeath->get_bool())
		{
			if (e->is_ready())
			{
				for (auto&& enemy : entitylist->get_enemy_heroes())
				{
					if (enemy->is_valid_target(e->range()))
					{
						if (enemy != nullptr)
						{
							if (enemy->has_buff(buff_hash("kalistaexpungemarker")))
							{
								if (health_prediction->get_incoming_damage(myhero, 0.5f, true) >= myhero->get_health())
								{
									e->cast();
								}
							}
						}
					}
				}
			}
		}
	}

	void Rlogic()
	{
		if (settings::rsettings::AutoRSave->get_bool())
		{
			if (r->is_ready())
			{
				if (ally != nullptr)
				{
					if (ally->get_distance(myhero) <= r->range())
					{
						if (health_prediction->get_incoming_damage(ally, 0.5f, true) >= ally->get_real_health())
						{
							r->cast();
							console->print("Saving %s", ally->get_name_cstr());
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

		if (enemy->get_health() <= qDamage(enemy))
		{
			auto qpred = q->get_prediction(enemy);
			if (qpred.hitchance >= hit_chance::high)
			{
				q->cast(qpred.get_cast_position());
			}
		}
	}

	void killstealE(const game_object_script& enemy)
	{
		if (!e->is_ready()) return;
		if (!settings::killsteal::ksE->get_bool()) return;
		if (!enemy->is_valid_target(e->range())) return;

		if (enemy->get_health() <= eDamage(enemy))
		{
			e->cast();
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
	}

	void on_draw_env()
	{
		if (settings::draws::qRange->get_bool())
		{
			if (q->level() >= 1)
			{
				drawCircle(myhero->get_position(), q->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::Qcolor->get_color());
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
					femboyutils::AATracker();
				}
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

		Rlogic();
		ELogic();
		killstealloop();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 1200.f);
		q->set_skillshot(0.25f + 0.066f, 40.f, 2400.f, { collisionable_objects::yasuo_wall, collisionable_objects::minions }, skillshot_type::skillshot_line);
		q->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 5000.f);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 1000.f);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 1200.f);
		r->set_spell_lock(false);

		createmenu();
		
		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_buff_gain>::add_callback(onbuffgain, event_prority::high);
		event_handler<events::on_buff_gain>::add_callback(onbuffgain2, event_prority::high);
		event_handler<events::on_draw>::add_callback(on_draw_real, event_prority::high);
		event_handler<events::on_env_draw>::add_callback(on_draw_env, event_prority::high);
	}

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_buff_gain>::remove_handler(onbuffgain);
		event_handler<events::on_buff_gain>::remove_handler(onbuffgain2);
		event_handler<events::on_draw>::remove_handler(on_draw_real);
		event_handler<events::on_env_draw>::remove_handler(on_draw_env);
	}

	float qrawdamage[] = { 20.f, 85.f, 150.f, 215.f, 280.f };
	float qDamage(const game_object_script& target)
	{
		if (q->level() == 0) return 0.f;

		damage_input input;
		input.raw_physical_damage = qrawdamage[q->level() - 1] + myhero->get_total_attack_damage() * 1.05f;
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float erawdamage[] = { 20.f, 30.f, 40.f, 50.f, 60.f };
	float estackdamage[] = { 8.f, 12.f, 16.f, 20.f, 24.f };
	float eadscaling[] = { 0.25f, 0.30f, 0.35f, 0.40f, 0.45f };
	float eDamage(const game_object_script& target)
	{
		if (e->level() == 0) return 0.f;

		auto ebuff = target->get_buff_count(buff_hash("kalistaexpungemarker"));
		if (ebuff == 0)
		{
			return 0.f;
		}

		auto eBaseDamage = erawdamage[e->level() - 1] + myhero->get_total_attack_damage() * 0.70f + myhero->get_total_ability_power() * 0.20f;
		auto EStackingDamage = estackdamage[e->level() - 1] + eadscaling[e->level() - 1] * myhero->get_total_attack_damage() + myhero->get_total_ability_power() * 0.20f;

		damage_input input;
		input.raw_physical_damage = eBaseDamage + EStackingDamage * (ebuff - 1);
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

		if (e->is_ready())
		{
			Damage += eDamage(target);
		}

		if (r->is_ready())
		{
			Damage += r->get_damage(target);
		}

		return Damage;
	}

	void onbuffgain(game_object_script sender, buff_instance_script buff)
	{
		if (sender->is_ally() && !sender->is_me())
		{
			if (buff->get_hash_name() != buff_hash("kalistavobindally"))
			{
				return;
				console->print("Not Found");
			}
			else
			{
				console->print("Connected to %s", sender->get_name_cstr());
				ally = sender;
			}
		}
	}

	void onbuffgain2(game_object_script sender, buff_instance_script buff)
	{
		if (ally != nullptr)
		{
			if (ally->get_base_skin_name() == "Blitzcrank")
			{
				if (settings::rsettings::Ballista->get_bool())
				{
					if (myhero->get_distance(ally) >= settings::rsettings::Ballistarange->get_int() && ally->get_distance(myhero) <= r->range())
					{
						if (sender->is_enemy() && sender->has_buff(buff_hash("rocketgrab2")) && sender->is_ai_hero())
						{
							r->cast();
							myhero->print_chat(0, "Casting Balista Combo!");
						}
					}
				}
			}
		}
	}
}