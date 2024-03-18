#include "../plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "femboyutils.h"
#include "Jhin.h"

namespace jhin
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
			TreeEntry* wOnlyMarked;
			TreeEntry* wOnlyifnotinaa;
			TreeEntry* SemiW;
		}

		namespace esettings
		{
			TreeEntry* autoEonCC;		
		}

		namespace rsettings
		{
			TreeEntry* rCombo;			
			TreeEntry* rMode;
			TreeEntry* mouseRange;
			TreeEntry* rTapKey;
		}

		namespace harass
		{
			TreeEntry* qHarass;
		}

		namespace killsteal
		{
			TreeEntry* ksQ;
			TreeEntry* ksW;
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
			TreeEntry* drawcursorrange;
			TreeEntry* Damage;
			TreeEntry* legCircles;
			TreeEntry* NeededAA;
		}

		namespace clear
		{
			TreeEntry* farmtoggle;
			TreeEntry* LcQ;
			TreeEntry* LcE;
			TreeEntry* JcQ;
			TreeEntry* JcE;
		}

		namespace misc
		{
			TreeEntry* antigapcloser;
		}
	}

	void createmenu()
	{
		mainMenu = menu->create_tab("femboy.Jhin", "[Femboy] Jhin");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("femboy.Jhin.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("femboy.Jhin.qsettings", "Dancing Grenade - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qCombo = qsettings->add_checkbox("femboy Jhin.useq", "Enable Q in Combo", true);
			}

			const auto wsettings = combotab->add_tab("femboy.Jhin.wsettings", "Deadly Flourish - [W]");
			wsettings->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			{
				settings::wsettings::wCombo = wsettings->add_checkbox("femboy.Jhin.usew", "Enable W in Combo", true);
				settings::wsettings::wOnlyMarked = wsettings->add_checkbox("femboy.Jhin.marked", "Only W if Target is Marked", true);
				settings::wsettings::wOnlyMarked->set_tooltip("This Setting Also Carrys over to Semi W!!!");
				settings::wsettings::wOnlyifnotinaa = wsettings->add_checkbox("femboy.Jhin.aa", "Only W if Target is not in AA Range", true);
				settings::wsettings::SemiW = wsettings->add_hotkey("femboy.Jhin.semiw", "Semi W", TreeHotkeyMode::Hold, 0x54, false, true);
			}

			const auto esettings = combotab->add_tab("femboy.Jhin.esettings", "Captive Audience - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::autoEonCC = esettings->add_checkbox("femboy.Jhin.autoe", "Auto E on CC Targets", true);
			}

			const auto rsettings = combotab->add_tab("femboy.Jhin.rsettings", "Curtain Call - [R]");
			rsettings->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			{
				settings::rsettings::rMode = rsettings->add_combobox("femboy.Jhin.rmode", "R Mode", { {"Auto", nullptr}, {"Tap Key", nullptr} }, 1, true);
				settings::rsettings::rCombo = rsettings->add_checkbox("femboy.Jhin.user", "Enable R in Combo", true);
				settings::rsettings::mouseRange = rsettings->add_slider("femboy.Jhin.mouserange", "Cursor Range to get Target", 1000, 500, 1500, true);
				settings::rsettings::rTapKey = rsettings->add_hotkey("femboy.Jhin.tapkey", "R Tap Key", TreeHotkeyMode::Hold, 0x55, false, true);
			}
		}

		const auto harrastab = mainMenu->add_tab("femboy.Jhin.harass", "Harass");
		{
			settings::harass::qHarass = harrastab->add_checkbox("femboy.Jhin.qharass", "Enable Q Harass", true);
		}

		const auto cleartab = mainMenu->add_tab("femboy.Jhin.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("femboy.Jhin.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcQ = cleartab->add_checkbox("femboy.Jhin.qclear", "Enable Q Lane Clear", true);
			settings::clear::LcE = cleartab->add_checkbox("femboy.Jhin.eclear", "Enable E Lane Clear", true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcQ = cleartab->add_checkbox("femboy.Jhin.qjclear", "Enable Q Jungle Clear", true);
			settings::clear::JcE = cleartab->add_checkbox("femboy.Jhin.ejclear", "Enable E Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("femboy.Jhin.ks", "Killsteal");
		{
			settings::killsteal::ksQ = killstealtab->add_checkbox("femboy.Jhin.ksq", "Enable Q Killsteal", true);
			settings::killsteal::ksW = killstealtab->add_checkbox("femboy.Jhin.ksw", "Enable W Killsteal", true);
		}

		const auto misctab = mainMenu->add_tab("femboy.Jhin.misc", "Misc");
		{
			settings::misc::antigapcloser = misctab->add_checkbox("femboy.Jhin.antigap", "Enable Antigapcloser", true);
		}

		const auto hitchancetab = mainMenu->add_tab("femboy.Jhin.hitchance", "Hitchance");
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

		settings::hitchance::wHitChance = hitchancetab->add_combobox("femboy.Jhin.whitchance", "W Hitchance", combo_elemts, 2);
		settings::hitchance::rHitChance = hitchancetab->add_combobox("femboy.Jhin.rhitchance", "R Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("femboy.Jhin.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			float defaultcolorW[] = { 0.f, 0.f, 1.f, 1.f };
			float defaultcolorE[] = { 0.f, 1.f, 0.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };
			const auto qdrawtab = drawTab->add_tab("femboy.Jhin.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("femboy.Jhin.qrange", "Draw Q Range", true);
				settings::draws::qColor = qdrawtab->add_colorpick("femboy.Jhin.qcolor", "Q Draw Color", defaultcolorQ);
			}

			const auto wdrawtab = drawTab->add_tab("femboy.Jhin.draww", "W Draw Settings");
			{
				settings::draws::wRange = wdrawtab->add_checkbox("femboy.Jhin.wrange", "Draw W Range", true);
				settings::draws::wColor = wdrawtab->add_colorpick("femboy.Jhin.wcolor", "W Draw Color", defaultcolorW);

			}

			const auto edrawtab = drawTab->add_tab("femboy.Jhin.drawe", "E Draw Settings");
			{
				settings::draws::eRange = edrawtab->add_checkbox("femboy.Jhin.erange", "Draw E Range", true);
				settings::draws::eColor = edrawtab->add_colorpick("femboy.Jhin.ecolor", "E Draw Color", defaultcolorE);
			}

			const auto rdrawtab = drawTab->add_tab("femboy.Jhin.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("femboy.Jhin.rrange", "Draw R Range", true);
				settings::draws::drawcursorrange = rdrawtab->add_checkbox("femboy.Jhin.cursordraw", "Draw Cursor Range", true);
				settings::draws::rColor = rdrawtab->add_colorpick("femboy.Jhin.rcolor", "R Draw Color", defaultcolorR);
			}

			const auto miscdrawTab = drawTab->add_tab("femboy.Jhin.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("femboy.Jhin.damage", "Draw Damage Indicator", true);
				settings::draws::legCircles = miscdrawTab->add_checkbox("femboy.Jhin.3dcircle", "Enable 3D Circle", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("femboy.Jhin.aaindicator", "Draw AA Counter", true);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Femboy] Jhin");
		Permashow::Instance.AddElement("Semi W", settings::wsettings::SemiW);
		Permashow::Instance.AddElement("R Tap Key", settings::rsettings::rTapKey);
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

	void afterAA(game_object_script target)
	{
		if (settings::qsettings::qCombo->get_bool())
		{
			if (q->is_ready())
			{
				if (target != nullptr)
				{
					if (target->is_ai_hero() && target->is_enemy() && target->is_valid_target(q->range()) && q->is_in_range(target))
					{
						q->cast(target);
					}
				}
			}
		}
	}

	void WLogic()
	{
		auto wtarget = target_selector->get_target(w->range(), damage_type::physical);

		if (r->name() == "JhinRShot")
		{
			return;
		}

		if (settings::wsettings::wOnlyMarked->get_bool())
		{
			if (wtarget != nullptr)
			{
				if (!wtarget->has_buff(buff_hash("jhinespotteddebuff")))
				{
					return;
				}
			}
		}

		if (settings::wsettings::wOnlyifnotinaa->get_bool())
		{
			if (wtarget != nullptr)
			{
				if (myhero->is_in_auto_attack_range(wtarget))
				{
					return;
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
		auto wtarget = target_selector->get_target(w->range(), damage_type::physical);

		if (settings::wsettings::wOnlyMarked->get_bool())
		{
			if (wtarget != nullptr)
			{
				if (!wtarget->has_buff(buff_hash("jhinespotteddebuff")))
				{
					return;
				}
			}
		}

		if (r->name() == "JhinRShot")
		{
			return;
		}

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

	void ELogic()
	{
		if (r->name() == "JhinRShot")
		{
			return;
		}

		auto etarget = target_selector->get_target(e->range(), damage_type::magical);

		if (settings::esettings::autoEonCC->get_bool())
		{
			if (e->is_ready())
			{
				if (etarget != nullptr)
				{
					if (etarget->is_valid_target())
					{
						if (etarget->has_buff_type(buff_type::Stun) || etarget->has_buff_type(buff_type::Snare))
						{
							e->cast(etarget->get_position());
						}
					}
				}
			}
		}
	}

	void qHarass()
	{
		if (settings::harass::qHarass->get_bool())
		{
			if (q->is_ready())
			{
				auto target = target_selector->get_target(q->range(), damage_type::physical);
				if (target != nullptr)
				{
					if (target->is_valid_target())
					{
						q->cast(target);
					}
				}
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

		if (settings::draws::eRange->get_bool())
		{
			if (e->level() >= 1)
			{
				drawCircle(myhero->get_position(), e->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::eColor->get_color());
			}			
		}

		if (settings::draws::drawcursorrange->get_bool())
		{
			if (r->name() == "JhinRShot")
			{
				drawCircle(hud->get_hud_input_logic()->get_game_cursor_position(), settings::rsettings::mouseRange->get_int(), 100, settings::draws::legCircles->get_bool(), settings::draws::rColor->get_color(), 1);
			}
		}
	}

	void on_draw_real()
	{
		if (settings::draws::rRange->get_bool())
		{
			if (r->level() >= 1)
			{
				draw_manager->draw_circle_on_minimap(myhero->get_position(), r->range(), MAKE_COLOR(255, 0, 0, 255), 2, 100);
			}
		}

		if (settings::draws::wRange->get_bool())
		{
			if (w->level() >= 1)
			{
				draw_manager->draw_circle_on_minimap(myhero->get_position(), w->range(), MAKE_COLOR(0, 0, 255, 255), 2, 100);
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

	void killstealW(const game_object_script& enemy)
	{
		if (r->name() == "JhinRShot") return;
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

	void killstealQ(const game_object_script& enemy)
	{
		if (r->name() == "JhinRShot") return;
		if (!q->is_ready()) return;
		if (!settings::killsteal::ksQ->get_bool()) return;
		if (!enemy->is_valid_target(q->range())) return;

		if (enemy->get_real_health() <= q->get_damage(enemy))
		{
			q->cast(enemy);
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

			if (settings::clear::LcQ->get_bool() && q->is_ready() && q->get_damage(minionQ) >= minionQ->get_health())
			{
				q->cast(minionQ);
			}
		}

		for (auto& minionE : minions_list)
		{
			if (minionE->get_distance(myhero) > e->range())
				continue;

			if (settings::clear::LcE->get_bool() && e->is_ready())
			{
				e->cast(minionE->get_position());
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
				q->cast(Jungleminionq);
			}
		}

		for (auto& Jungleminione : minions_list)
		{
			if (Jungleminione->get_distance(myhero) > e->range())
				continue;

			if (settings::clear::JcE->get_bool() && e->is_ready())
			{
				e->cast(Jungleminione->get_position());
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

	void update()
	{
		if (r->name() == "JhinRShot")
		{
			evade->disable_evade();
		}
		else
		{
			evade->enable_evade();
		}

		if (r->name() == "JhinRShot")
		{
			orbwalker->set_movement(false);
			orbwalker->set_attack(false);

			if (r->is_ready())
			{
				for (const auto& target : entitylist->get_enemy_heroes())
				{
					if (target->get_distance(myhero) <= r->range())
					{
						if (target != nullptr)
						{
							if (hud->get_hud_input_logic()->get_game_cursor_position().distance(target->get_position()) <= settings::rsettings::mouseRange->get_int())
							{
								auto rpred = r->get_prediction(target);
								if (rpred.hitchance >= getPredIntFromSettings(settings::hitchance::rHitChance->get_int()))
								{
									if (settings::rsettings::rMode->get_int() == 0)
									{
										r->cast(rpred.get_cast_position());
									}
									else if (settings::rsettings::rMode->get_int() == 1)
									{
										if (settings::rsettings::rTapKey->get_bool())
										{
											r->cast(rpred.get_cast_position());
										}
									}
								}
							}
						}
					}
				}
			}
		}

		if (r->name() != "JhinRShot")
		{
			orbwalker->set_movement(true);
			orbwalker->set_attack(true);
		}

		if (settings::wsettings::SemiW->get_bool())
		{
			if (r->name() == "JhinRShot")
			{
				return;
			}

			myhero->issue_order(hud->get_hud_input_logic()->get_game_cursor_position());

			SemiW();
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
			qHarass();
		}

		killstealloop();
		ELogic();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 550.f + myhero->get_bounding_radius());
		q->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 2500.f);
		w->set_skillshot(0.75f + 0.066f, 45.f, FLT_MAX, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 750.f);
		e->set_skillshot(0.25f + 0.066f, 80.f, FLT_MAX, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_circle);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 3500.f);
		r->set_skillshot(0.25f + 0.066f, 80.f, 5000.f, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		r->set_spell_lock(false);

		createmenu();

		antigapcloser::add_event_handler(antigap);
		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_after_attack_orbwalker>::add_callback(afterAA, event_prority::high);
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
		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_after_attack_orbwalker>::remove_handler(afterAA);
		event_handler<events::on_draw>::remove_handler(on_draw_real);
		event_handler<events::on_env_draw>::remove_handler(on_draw);
	}

	static constexpr float WRawDamage[]{0, 60, 95, 130, 165, 200};
	static constexpr float ERawDamage[]{0, 20, 80, 140, 200, 260};
	static constexpr float RRawDamage[]{0, 256, 616, 976};

	static float WDamage(const game_object_script& target)
	{
		auto wlevel = w->level();
		auto wdamage = WRawDamage[wlevel] + 0.5f * myhero->get_total_attack_damage();
		return damagelib->calculate_damage_on_unit(myhero, target, damage_type::physical, wdamage);
	}

	static float EDamage(const game_object_script& target)
	{
		auto elevel = e->level();
		auto edamage = ERawDamage[elevel] + 1.20f * myhero->get_total_attack_damage() + 1.f * myhero->get_total_ability_power();
		return damagelib->calculate_damage_on_unit(myhero, target, damage_type::magical, edamage);
	}

	static float RDamage(const game_object_script& target)
	{
		auto rlevel = r->level();
		auto rdamage = RRawDamage[rlevel] + 1.f * myhero->get_total_attack_damage();
		return damagelib->calculate_damage_on_unit(myhero, target, damage_type::physical, rdamage);
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
			Damage += EDamage(target);
		}

		if (r->is_ready())
		{
			Damage += RDamage(target);
		}

		return Damage;
	}

	void antigap(game_object_script sender, antigapcloser::antigapcloser_args* args)
	{
		if (settings::misc::antigapcloser->get_bool())
		{
			if (w->is_ready())
			{
				if (sender->is_enemy() && sender->is_valid())
				{
					if (args->end_position.distance(myhero) < 350)
					{
						if (sender->has_buff(buff_hash("jhinespotteddebuff")))
						{
							auto wpred = w->get_prediction(sender);
							if (wpred.hitchance >= hit_chance::high)
							{
								w->cast(args->end_position);
							}
						}
					}
				}
			}
		}
	}
}