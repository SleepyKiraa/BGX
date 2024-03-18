#include "plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "Vayne.h"
#include "Spaceutils.h"

namespace vayne
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
			TreeEntry* DontGapTurret;
			TreeEntry* wstackstoq;
			TreeEntry* qGapcloser;
		}

		namespace esettings
		{
			TreeEntry* eCombo;
		}

		namespace rsettings
		{
			TreeEntry* rCombo;
			TreeEntry* minene;
			TreeEntry* rrange;
		}

		namespace killsteal
		{
			TreeEntry* ksE;
		}

		namespace draws
		{
			TreeEntry* qRange;
			TreeEntry* eRange;
			TreeEntry* qColor;
			TreeEntry* q2Color;
			TreeEntry* qThicness;
			TreeEntry* eColor;
			TreeEntry* e2Color;
			TreeEntry* eThicness;
			TreeEntry* DrawECollision;
			TreeEntry* Damage;
			TreeEntry* NeededAA;
			TreeEntry* BuffTime;
		}

		namespace clear
		{
			TreeEntry* farmtoggle;
			TreeEntry* JcQ;
		}

		namespace misc
		{
			TreeEntry* antigapcloser;
			TreeEntry* antigapcloserHP;
			TreeEntry* Interrupter;
		}
	}

	void createmenu()
	{
		mainMenu = menu->create_tab("space.vayne", "[Spaceglide] Vayne");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("space.vayne.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("space.vayne.qsettings", "Tumble - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qCombo = qsettings->add_checkbox("space.vayne.useq", "Enable Q in Combo", true);
				settings::qsettings::qGapcloser = qsettings->add_checkbox("space.vayne.qgap", "Use Q to Gap Close", true);
				settings::qsettings::DontGapTurret = qsettings->add_hotkey("space.vayne.qgapturret", "Enable Tower Dive with Gap closer", TreeHotkeyMode::Toggle, 0x54, true);
				settings::qsettings::wstackstoq = qsettings->add_slider("space.vayne.wstacks", "W Stacks to Q", 0, 0, 2, true);
			}

			const auto esettings = combotab->add_tab("space.vayne.esettings", "Condemn - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::eCombo = esettings->add_checkbox("space.vayne.usee", "Enable E in Combo", true);
			}

			const auto rsettings = combotab->add_tab("space.vayne.rsettings", "Final Hour - [R]");
			rsettings->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			{
				settings::rsettings::rCombo = rsettings->add_checkbox("space.vayne.user", "Enable R in Combo", true);
				settings::rsettings::minene = rsettings->add_slider("space.vayne.minEne", "Min Enemies in Range to use R", 2, 1, 5, true);
				settings::rsettings::rrange = rsettings->add_slider("space.vayne.rrange", "Target Scan Range for Enemies", 750, 0, 1500, true);
			}
		}

		const auto cleartab = mainMenu->add_tab("space.vayne.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("space.vayne.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcQ = cleartab->add_checkbox("space.varus.jcq", "Enable Q Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("space.vayne.ks", "Killsteal");
		{
			settings::killsteal::ksE = killstealtab->add_checkbox("space.vayne.kse", "Enable E Killsteal", true);
		}

		const auto misctab = mainMenu->add_tab("space.vayne.misc", "Misc");
		{
			settings::misc::antigapcloser = misctab->add_checkbox("space.vayne.anti", "Enable Anti Gapcloser", true);
			settings::misc::antigapcloserHP = misctab->add_slider("space.vayne.hp", "Min HP % to Anti Gapclose", 50, 1, 100, true);
			settings::misc::Interrupter = misctab->add_checkbox("space.vayne.inter", "Enable Interrupter", true);
		}		

		const auto drawTab = mainMenu->add_tab("space.vayne.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			float defaultcolorQ2[] = { 1.f, 0.f, 1.f, 1.f };
			float defaultcolorE[] = { 0.f, 1.f, 0.f, 1.f };
			float defaultcolorE2[] = { 1.f, 0.f, 1.f, 1.f };

			const auto qdrawtab = drawTab->add_tab("space.vayne.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("space.vayne.qrange", "Draw Q Range", true);
				settings::draws::qColor = qdrawtab->add_colorpick("space.vayne.qcolor", "Q Draw Color", defaultcolorQ);
				settings::draws::q2Color = qdrawtab->add_colorpick("space.vayne.q2color", "Q2 Draw Color", defaultcolorQ2);
				settings::draws::qThicness = qdrawtab->add_slider("space.vayne.thicknessq", "Q circle Thickness", 2, 1, 6, true);
			}

			const auto edrawtab = drawTab->add_tab("space.vayne.drawe", "E Draw Settings");
			{
				settings::draws::eRange = edrawtab->add_checkbox("space.vayne.erange", "Draw E Range", true);
				settings::draws::DrawECollision = edrawtab->add_checkbox("space.vayne.ecol", "Draw E Collision", true);
				settings::draws::eColor = edrawtab->add_colorpick("space.vayne.ecolor", "E Draw Color", defaultcolorE);
				settings::draws::e2Color = edrawtab->add_colorpick("space.vayne.e2color", "E2 Draw Color", defaultcolorE2);
				settings::draws::eThicness = edrawtab->add_slider("space.vayne.thicknesse", "E circle Thickness", 2, 1, 6, true);
			}

			const auto miscdrawTab = drawTab->add_tab("space.vayne.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("space.vayne.damage", "Draw Damage Indicator", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("space.vayne.aaindicator", "Draw AA Counter", true);
				settings::draws::BuffTime = miscdrawTab->add_checkbox("space.vayne.buff", "Draw Buff Times", true);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Spaceglide] Vayne");
		Permashow::Instance.AddElement("Farm", settings::clear::farmtoggle);
		Permashow::Instance.AddElement("Gap Closer Tower Dive", settings::qsettings::DontGapTurret);
	}

	hit_chance getPredIntFromSettings(int hitchance)
	{
		// Get hitchance from settings value
		return static_cast<hit_chance>(hitchance + 3);
	}

	void QLogic()
	{
		auto qtarget = target_selector->get_target(myhero->get_attackRange() + myhero->get_bounding_radius() + q->range(), damage_type::physical);

		if (!settings::qsettings::DontGapTurret->get_bool())
		{
			if (qtarget != nullptr)
			{
				if (myhero->get_position().extend(hud->get_hud_input_logic()->get_game_cursor_position(), q->range()).is_under_enemy_turret())
				{
					return;
				}
			}
		}

		if (settings::qsettings::qGapcloser->get_bool())
		{
			if (q->is_ready())
			{
				if (qtarget != nullptr)
				{
					if (qtarget->is_valid())
					{
						q->cast(hud->get_hud_input_logic()->get_game_cursor_position());
					}
				}
			}
		}
	}

	void afteraa(game_object_script target)
	{
		if (settings::qsettings::qCombo->get_bool())
		{
			if (orbwalker->combo_mode())
			{
				if (q->is_ready())
				{
					if (target != nullptr)
					{
						if (target->is_ai_hero() && target->is_enemy() && myhero->is_in_auto_attack_range(target))
						{
							auto getwbuffcount = target->get_buff_count(buff_hash("VayneSilveredDebuff"));
							if (getwbuffcount >= settings::qsettings::wstackstoq->get_int())
							{
								if (evade->is_dangerous_path({ myhero->get_position(), hud->get_hud_input_logic()->get_game_cursor_position()}, 0.f)) return;

								q->cast(hud->get_hud_input_logic()->get_game_cursor_position());
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
						if (condemcheck(myhero->get_position(), etarget))
						{
							e->cast(etarget);
						}
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
				if (myhero->count_enemies_in_range(settings::rsettings::rrange->get_int()) >= settings::rsettings::minene->get_int())
				{
					r->cast();
				}
			}
		}
	}

	void KillstealE(const game_object_script& enemy)
	{
		if (!e->is_ready()) return;
		if (!settings::killsteal::ksE->get_bool()) return;
		if (!enemy->is_valid_target(e->range())) return;

		if (enemy->get_real_health() <= EDamage(enemy))
		{
			e->cast(enemy);
		}
	}

	void killstealloop()
	{
		for (auto&& enemy : entitylist->get_enemy_heroes())
		{
			if (enemy == nullptr || !enemy->is_valid_target()) continue;

			KillstealE(enemy);
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
			if (Jungleminionq->get_distance(myhero) > myhero->get_attackRange() + myhero->get_bounding_radius())
				continue;


			if (settings::clear::JcQ->get_bool() && q->is_ready())
			{
				q->cast(hud->get_hud_input_logic()->get_game_cursor_position());
			}
		}
	}

	void on_env_draw()
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

		if (settings::draws::DrawECollision->get_bool())
		{
			for (const auto& target : entitylist->get_enemy_heroes())
			{

				if (target != nullptr)
				{
					if (target->is_valid_target(e->range()))
					{
						auto pred = e->get_prediction(target);
						for (auto i = 0; i <= 450; i += 20)
						{
							auto loc = pred.get_unit_position().extend(myhero->get_position(), -i);
							if (loc.is_wall() || loc.is_building())
							{
								draw_manager->add_circle(loc, 50, MAKE_COLOR(0, 255, 0, 255), 2);
							}
						}
					}
				}
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

		if (settings::draws::BuffTime->get_bool())
		{
			auto buff = myhero->get_buff(buff_hash("VayneInquisition"));
			if (buff != nullptr)
			{
				int num = (int)buff->get_end() - (int)gametime->get_time();
				std::string numstr = std::to_string(num);
				int firstdigit = std::stoi(numstr.substr(0, 2));
				const auto pos = vector(800.f, 590.f);
				draw_manager->add_text_on_screen(pos, MAKE_COLOR(255, 255, 255, 255), 22, "R Time: %i", firstdigit);
			}
		}
	}

	void update()
	{
		if (orbwalker->combo_mode())
		{
			RLogic();
			ELogic();
			QLogic();
		}

		if (orbwalker->lane_clear_mode())
		{
			jungleclear();
		}

		interrupter();
		killstealloop();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 300.f);
		q->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 0.f);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 550.f + myhero->get_bounding_radius());
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 0.f);
		r->set_spell_lock(false);

		createmenu();

		event_handler<events::on_after_attack_orbwalker>::add_callback(afteraa, event_prority::high);
		event_handler<events::on_process_spell_cast>::add_callback(ondocast, event_prority::high);
		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_env_draw>::add_callback(on_env_draw, event_prority::high);
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

		event_handler<events::on_process_spell_cast>::remove_handler(ondocast);
		event_handler<events::on_after_attack_orbwalker>::remove_handler(afteraa);
		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_env_draw>::remove_handler(on_env_draw);
		event_handler<events::on_draw>::remove_handler(on_draw);
		antigapcloser::remove_event_handler(antigap);
	}

	bool condemcheck(vector start, const game_object_script& target)
	{
		auto targetpos = target->get_position();
		auto pushdistance = start == myhero->get_position() ? 450 : 440;

		for (auto i = 0; i <= pushdistance; i += 20)
		{
			auto targetpoint = targetpos.extend(start, -i);
			auto predpoint = targetpos.extend(start, -i);

			if (predpoint.is_wall() && targetpoint.is_wall())
			{
				return true;
			}

			if (predpoint.is_building() && targetpoint.is_building())
			{
				return true;
			}
		}

		return false;
	}

	float eDamage[] = { 50.f, 85.f, 120.f, 155.f, 190.f };

	float EDamage(const game_object_script& target)
	{
		if (e->level() == 0) return 0.f;

		damage_input input;
		input.raw_physical_damage = eDamage[e->level() - 1] + 0.50f * (myhero->get_total_attack_damage() - myhero->get_base_attack_damage());
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
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
			Damage += w->get_damage(target);
		}

		if (e->is_ready())
		{
			Damage += EDamage(target);
		}

		if (r->is_ready())
		{
			Damage += r->get_damage(target);
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
			if (e->is_ready())
			{
				if (sender->is_enemy() && sender->is_valid())
				{
					if (args->end_position.distance(myhero) <= 450.f)
					{
						e->cast(sender);
						console->print("[Spaceglide] Casting E on Gap Closer Sender: %s", sender->get_base_skin_name().c_str());
					}
				}
			}
		}
	}

	void interrupter()
	{
		if (settings::misc::Interrupter->get_bool())
		{
			for (auto& enemy : entitylist->get_enemy_heroes())
			{
				if (e->is_ready())
				{
					if (enemy->is_valid() && enemy->is_casting_interruptible_spell() == 2)
					{
						if (enemy->get_distance(myhero) <= e->range())
						{
							e->cast(enemy);
							console->print("[Spaceglide] Interrupting %s", enemy->get_base_skin_name().c_str());
						}
					}
				}
			}
		}
	}

	void ondocast(game_object_script sender, spell_instance_script spell)
	{
		if (settings::misc::antigapcloser->get_bool())
		{
			if (e->is_ready())
			{
				if (sender != nullptr)
				{
					if (sender->get_base_skin_name() == "Alistar")
					{
						if (spell->get_spellslot() == spellslot::w)
						{
							if (spell->get_last_target_id() == myhero->get_id())
							{
								e->cast(sender);
								console->print("[Spaceglide] Casting E on Alistar W");
							}
						}
					}

					if (sender->get_base_skin_name() == "Jax")
					{
						if (spell->get_spellslot() == spellslot::q)
						{
							if (spell->get_last_target_id() == myhero->get_id())
							{
								e->cast(sender);
								console->print("[Spaceglide] Casting E on Jax Q");
							}
						}
					}

					if (sender->get_base_skin_name() == "Pantheon")
					{
						if (spell->get_spellslot() == spellslot::w)
						{
							if (spell->get_last_target_id() == myhero->get_id())
							{
								e->cast(sender);
								console->print("[Spaceglide] Casting E on Pantheon W");
							}
						}
					}

					if (sender->get_base_skin_name() == "Briar")
					{
						if (spell->get_spellslot() == spellslot::q)
						{
							if (spell->get_last_target_id() == myhero->get_id())
							{
								e->cast(sender);
								console->print("[Spaceglide] Casting E on Briar Q");
							}
						}
					}
				}
			}
		}
	}
}