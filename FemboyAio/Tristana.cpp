#include "../plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "femboyutils.h"
#include "Tristana.h"

namespace tristana
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

		namespace esettings
		{
			TreeEntry* eCombo;
		}

		namespace killsteal
		{
			TreeEntry* ksW;
			TreeEntry* ksER;
		}

		namespace hitchance
		{
			TreeEntry* wHitChance;
		}

		namespace draws
		{
			TreeEntry* wRange;
			TreeEntry* rRange;
			TreeEntry* wColor;
			TreeEntry* rColor;
			TreeEntry* drawcursorrange;
			TreeEntry* Damage;
			TreeEntry* legCircles;
			TreeEntry* NeededAA;
		}

		namespace misc
		{
			TreeEntry* antigapcloser;
			TreeEntry* antigaphp;
			TreeEntry* Interrupter;
		}
	}

	void createmenu()
	{
		mainMenu = menu->create_tab("femboy.Tristana", "[Femboy] Tristana");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("femboy.Tristana.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("femboy.Tristana.qsettings", "Rapid Fire - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qCombo = qsettings->add_checkbox("femboy.Tristana.useq", "Enable Q in Combo", true);
			}

			const auto esettings = combotab->add_tab("femboy.Tristana.esettings", "Explosive Charge - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::eCombo = esettings->add_checkbox("femboy.Tristana.autoe", "Auto E on CC Targets", true);
			}
		}

		const auto killstealtab = mainMenu->add_tab("femboy.Tristana.ks", "Killsteal");
		{
			settings::killsteal::ksW = killstealtab->add_checkbox("femboy.Tristana.ksw", "Enable W Killsteal", true);
			settings::killsteal::ksER = killstealtab->add_checkbox("femboy.Tristana.kser", "Enable ER Killsteal", true);
		}

		const auto misctab = mainMenu->add_tab("femboy.Tristana.misc", "Misc");
		{
			misctab->add_separator("sep1", "Anti Gapcloser");
			settings::misc::antigapcloser = misctab->add_checkbox("femboy.Tristana.antigap", "Enable Anti Gapcloser", true);
			settings::misc::antigaphp = misctab->add_slider("femboy.Tristana.antigaphp", "HP % To Anti Gapclose", 70, 1, 100);
			misctab->add_separator("sep2", "Interrupter");
			settings::misc::Interrupter = misctab->add_checkbox("femboy.Tristana.inter", "Enable Interrupter", true);
		}

		const auto hitchancetab = mainMenu->add_tab("femboy.Tristana.hitchance", "Hitchance");
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

		settings::hitchance::wHitChance = hitchancetab->add_combobox("femboy.Tristana.whitchance", "W Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("femboy.Tristana.draw", "Draws");
		{
			float defaultcolorW[] = { 0.f, 0.f, 1.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };			

			const auto wdrawtab = drawTab->add_tab("femboy.Tristana.draww", "W Draw Settings");
			{
				settings::draws::wRange = wdrawtab->add_checkbox("femboy.Tristana.wrange", "Draw W Range", true);
				settings::draws::wColor = wdrawtab->add_colorpick("femboy.Tristana.wcolor", "W Draw Color", defaultcolorW);

			}			

			const auto rdrawtab = drawTab->add_tab("femboy.Tristana.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("femboy.Tristana.rrange", "Draw R Range", true);
				settings::draws::rColor = rdrawtab->add_colorpick("femboy.Tristana.rcolor", "R Draw Color", defaultcolorR);
			}

			const auto miscdrawTab = drawTab->add_tab("femboy.Tristana.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("femboy.Tristana.damage", "Draw Damage Indicator", true);
				settings::draws::legCircles = miscdrawTab->add_checkbox("femboy.Tristana.3dcircle", "Enable 3D Circle", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("femboy.Tristana.aaindicator", "Draw AA Counter", true);
			}
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

	void beforeaa(game_object_script target, bool* process)
	{
		if (!orbwalker->combo_mode())
		{
			return;
		}

		if (settings::qsettings::qCombo->get_bool())
		{
			if (q->is_ready())
			{
				if (target->is_valid_target(q->range() - 50))
				{
					if (target != nullptr)
					{
						q->cast();
					}
				}
			}
		}

		if (settings::esettings::eCombo->get_bool())
		{
			if (e->is_ready())
			{
				if (target->is_valid_target(e->range() - 50))
				{
					if (target != nullptr)
					{
						e->cast(target);
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

		if (enemy->get_real_health() <= wDamage(enemy))
		{
			auto wpred = w->get_prediction(enemy);
			if (wpred.hitchance >= hit_chance::high)
			{
				w->cast(wpred.get_cast_position());
			}
		}
	}

	void killstealER(const game_object_script& enemy)
	{
		if (!r->is_ready()) return;
		if (!enemy->is_valid_target(r->range())) return;
		if (!settings::killsteal::ksER->get_bool()) return;

		if (enemy->get_real_health() <= EDamage(enemy) + RDamage(enemy))
		{
			r->cast(enemy);
		}
	}

	void on_draw()
	{
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

	void killstealloop()
	{
		for (auto&& enemy : entitylist->get_enemy_heroes())
		{
			if (enemy == nullptr || !enemy->is_valid_target()) continue;

			killstealER(enemy);
			killstealW(enemy);
		}
	}

	void update()
	{
		q->set_range(525 + myhero->get_bounding_radius() + 8 * myhero->get_level() - 1);
		e->set_range(525 + myhero->get_bounding_radius() + 8 * myhero->get_level() - 1);
		r->set_range(525 + myhero->get_bounding_radius() + 8 * myhero->get_level() - 1);

		killstealloop();

		interrupter();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 525.f + myhero->get_bounding_radius());
		q->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 900.f);
		w->set_skillshot(0.25f + 0.066f, 140.f, 1100.f, {}, skillshot_type::skillshot_circle);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 525);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 525);
		r->set_spell_lock(false);

		createmenu();
		
		antigapcloser::add_event_handler(antigap);
		event_handler<events::on_before_attack_orbwalker>::add_callback(beforeaa, event_prority::high);
		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_draw>::add_callback(on_draw_real, event_prority::high);
		event_handler<events::on_env_draw>::add_callback(on_draw, event_prority::high);
	}

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		antigapcloser::remove_event_handler(antigap);
		event_handler<events::on_before_attack_orbwalker>::remove_handler(beforeaa);
		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_draw>::remove_handler(on_draw_real);
		event_handler<events::on_env_draw>::remove_handler(on_draw);
	}

	float wrawdamage[] = { 95.f, 145.f, 195.f, 245.f, 295.f };

	float wDamage(const game_object_script& target)
	{
		if (w->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = wrawdamage[w->level() - 1] + myhero->get_total_ability_power() * 0.5f;
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float ERawDamage[]{ 0, 70, 80, 90, 100, 110 };
	float EBonusDamage[]{ 0, 0.5, 0.75, 1.0, 1.25, 1.5 };
	float EStack[]{ 0, 21, 24, 27, 30, 33 };
	float EstackBonusDamage[]{ 0, 0.15, 0.22, 0.30, 0.37, 0.45 };
	float RRawDamage[]{ 0, 300, 400, 500 };

	float EDamage(const game_object_script& target)
	{
		if (e->level() == 0) return 0.f;

		if (!target->has_buff(buff_hash("TristanaECharge")))
		{
			return 0.f;
		}

		damage_input input;
		auto eDamage = ERawDamage[e->level() - 1] + EBonusDamage[e->level() - 1] * myhero->mPercentBonusPhysicalDamageMod() + 0.5f * myhero->get_total_ability_power();
		auto stackDamage = EStack[e->level() - 1] + EstackBonusDamage[e->level() - 1] + 0.15f * myhero->get_total_ability_power();
		input.raw_physical_damage = eDamage + stackDamage * target->get_buff_count(buff_hash("TristanaECharge"));
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float RDamage(const game_object_script& target)
	{
		if (r->level() == 0) return 0.f;

		damage_input input;
		input.raw_physical_damage = RRawDamage[r->level() - 1] + myhero->get_total_ability_power() * 1.f;
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float GetDamage(const game_object_script& target)
	{
		auto Damage = 0;

		if (w->is_ready())
		{
			Damage += wDamage(target);
		}

		if (e->is_ready() || target->has_buff(buff_hash("TristanaECharge")))
		{
			Damage += EDamage(target);
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
							r->cast(enemy);
						}
					}
				}
			}
		}
	}

	void antigap(game_object_script sender, antigapcloser::antigapcloser_args* args)
	{
		if (myhero->get_health_percent() >= settings::misc::antigaphp->get_int())
		{
			return;
		}

		if (settings::misc::antigapcloser->get_bool())
		{
			if (r->is_ready())
			{
				if (sender->is_enemy() && sender->is_valid())
				{
					if (args->end_position.distance(myhero) < 350)
					{
						r->cast(sender);
					}
				}
			}
		}
	}
}