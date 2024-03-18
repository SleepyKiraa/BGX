#include "../plugin_sdk/plugin_sdk.hpp"
#include <unordered_set>
#include <unordered_map>
#include <stdarg.h>
#include <iostream>
#include "Permashow.h"
#include "femboyutils.h"
#include "Yasuo.h"

namespace yasuo
{
	script_spell* q;
	script_spell* q3;
	script_spell* w;
	script_spell* e;
	script_spell* r;
	script_spell* flash;

	TreeTab* mainMenu;
	namespace settings {
		namespace qsettings
		{
			TreeEntry* qCombo;
			TreeEntry* q3Combo;
			TreeEntry* q3AutoDashM;
			TreeEntry* eqCombo;
		}

		namespace wsettings
		{
			TreeEntry* Wevade;
			TreeEntry* AnnieQ;
			TreeEntry* VayneE;
			TreeEntry* LuLuW;
			TreeEntry* FiddleQ;
			TreeEntry* MFQ;
			TreeEntry* MFR;
			TreeEntry* VeigarR;
			TreeEntry* TeemoQ;
			TreeEntry* TristR;
			TreeEntry* SyndraR;
		}

		namespace esettings
		{
			TreeEntry* eCombo;
			TreeEntry* eGapclose;
			TreeEntry* eGapCloseTargetScanRange;
			TreeEntry* ETowerDive;
		}

		namespace rsettings
		{
			TreeEntry* rCombo;
			TreeEntry* rmode;
			TreeEntry* rHP;
			TreeEntry* RTowerDive;
		}

		namespace killsteal
		{
			TreeEntry* ksQ;
			TreeEntry* ksQ3;
			TreeEntry* ksQFlash;
			TreeEntry* ksE;
		}

		namespace hitchance
		{
			TreeEntry* qHitChance;
			TreeEntry* q3HitChance;
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
			TreeEntry* BuffTime;
		}

		namespace clear
		{
			TreeEntry* farmtoggle;
			TreeEntry* LcQ;
			TreeEntry* JcQ;
		}

		namespace misc
		{
			TreeEntry* antigap;
			TreeEntry* antigaphp;
			TreeEntry* interrupter;
		}
	}

	void createmenu()
	{
		mainMenu = menu->create_tab("femboy.Yasuo", "[Femboy] Yasuo");
		mainMenu->set_texture(myhero->get_square_icon_portrait());

		const auto combotab = mainMenu->add_tab("femboy.Yasuo.combo", "Combo");
		{
			const auto qsettings = combotab->add_tab("femboy.Yasuo.qsettings", "Steel Tempest - [Q]");
			qsettings->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			{
				settings::qsettings::qCombo = qsettings->add_checkbox("femboy.Yasuo.useq", "Enable Q in Combo", true);
				settings::qsettings::q3Combo = qsettings->add_checkbox("femboy.Yasuo.useq3", "Enable Q3 in Combo", true);
				settings::qsettings::eqCombo = qsettings->add_checkbox("femboy.Yasuo.useeq", "Enable EQ in Combo", true);
				settings::qsettings::q3AutoDashM = qsettings->add_checkbox("femboy.Yasuo.autoq", "Enable Auto Q3 on Dashing Target", true);
			}

			const auto wsettings = combotab->add_tab("femboy.Yasuo.wsettings", "Wind Wall - [W]");
			wsettings->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			{
				settings::wsettings::Wevade = wsettings->add_checkbox("femboy.Yasuo.wevade", "Use W to Evade Spells", true);
				wsettings->add_separator("sep345", "Target Spells");
				for (const auto& enemys : entitylist->get_enemy_heroes())
				{
					if (enemys->get_base_skin_name() == "Annie")
					{
						settings::wsettings::AnnieQ = wsettings->add_checkbox("femboy.Yasuo.annieq", "Block Annie Q", true);
					}

					if (enemys->get_base_skin_name() == "Fiddlesticks")
					{
						settings::wsettings::FiddleQ = wsettings->add_checkbox("femboy.Yasuo.fiddleq", "Block Fiddle Q", true);
					}

					if (enemys->get_base_skin_name() == "Lulu")
					{
						settings::wsettings::LuLuW = wsettings->add_checkbox("femboy.Yasuo.luluw", "Block Lulu W", true);
					}

					if (enemys->get_base_skin_name() == "MissFortune")
					{
						settings::wsettings::MFQ = wsettings->add_checkbox("femboy.Yasuo.mfq", "Block Miss Fortune Q", true);
						settings::wsettings::MFR = wsettings->add_checkbox("femboy.Yasuo.mfr", "Block Miss Fortune R", true);
					}

					if (enemys->get_base_skin_name() == "Syndra")
					{
						settings::wsettings::SyndraR = wsettings->add_checkbox("femboy.Yasuo.syndrar", "Block Syndra R", true);
					}

					if (enemys->get_base_skin_name() == "Teemo")
					{
						settings::wsettings::TeemoQ = wsettings->add_checkbox("femboy.Yasuo.teemoq", "Block Teemo Q", true);
					}

					if (enemys->get_base_skin_name() == "Tristana")
					{
						settings::wsettings::TristR = wsettings->add_checkbox("femboy.Yasuo.tristanar", "Block Tristana R", true);
					}

					if (enemys->get_base_skin_name() == "Vayne")
					{
						settings::wsettings::VayneE = wsettings->add_checkbox("femboy.Yasuo.vaynee", "Block Vayne E", true);
					}

					if (enemys->get_base_skin_name() == "Veigar")
					{
						settings::wsettings::VeigarR = wsettings->add_checkbox("femboy.Yasuo.veigarr", "Block Veigar R", true);
					}
				}
			}

			const auto esettings = combotab->add_tab("femboy.Yasuo.esettings", "Sweeping Blade - [E]");
			esettings->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			{
				settings::esettings::eCombo = esettings->add_checkbox("femboy.Yasuo.usee", "Enable E in Combo", true);
				settings::esettings::eGapclose = esettings->add_checkbox("femboy.Yasuo.useegap", "Enable E to Gap Close", true);
				settings::esettings::eGapCloseTargetScanRange = esettings->add_slider("femboy.Yasuo.escan", "Gap Close Target Scan Range", 1000, 500, 1500, true);
				settings::esettings::ETowerDive = esettings->add_hotkey("femboy.Yasuo.divee", "Enable Tower Dive", TreeHotkeyMode::Toggle, 0x47, false, true);
			}

			const auto rsettings = combotab->add_tab("femboy.Yasuo.rsettings", "Last Breath - [R]");
			rsettings->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
			{
				settings::rsettings::rCombo = rsettings->add_checkbox("femboy.Yasuo.user", "Enable R in Combo", true);
				settings::rsettings::rmode = rsettings->add_combobox("femboy.Yasuo.rmode", "R Mode", { {"HP %", nullptr}, {"Combo Killable", nullptr} }, 1, true);
				settings::rsettings::rHP = rsettings->add_slider("femboy.Yasuo.minhp", "Min HP % to R in HP Mode", 50, 1, 100, true);
				settings::rsettings::RTowerDive = rsettings->add_hotkey("femboy.Yasuo.diver", "Enable Tower Dive", TreeHotkeyMode::Toggle, 0x47, false, true);
			}
		}

		const auto cleartab = mainMenu->add_tab("femboy.Yasuo.clear", "Clear");
		{
			settings::clear::farmtoggle = cleartab->add_hotkey("femboy.Yasuo.toggle", "Farm Toggle", TreeHotkeyMode::Toggle, 0x41, true, true);
			cleartab->add_separator("seperator1", "Lane Clear");
			settings::clear::LcQ = cleartab->add_checkbox("femboy.Yasuo.qclear", "Enable Q Lane Clear", true);
			cleartab->add_separator("seperator2", "Jungle Clear");
			settings::clear::JcQ = cleartab->add_checkbox("femboy.Yasuo.qjclear", "Enable Q Jungle Clear", true);
		}

		const auto killstealtab = mainMenu->add_tab("femboy.Yasuo.ks", "Killsteal");
		{
			settings::killsteal::ksQ = killstealtab->add_checkbox("femboy.Yasuo.ksq", "Enable Q Killsteal", true);
			settings::killsteal::ksQ3 = killstealtab->add_checkbox("femboy.Yasuo.ksq3", "Enable Q3 Killsteal", true);
			settings::killsteal::ksQFlash = killstealtab->add_checkbox("femboy.Yasuo.ksqf", "Enable Flash Q Killsteal", true);
			settings::killsteal::ksQFlash->set_tooltip("For Ks Q Flash Please Enable Ks Q Too otherwise it wont work!");
			settings::killsteal::ksE = killstealtab->add_checkbox("femboy.Yasuo.kse", "Enable E Killsteal", true);
		}

		const auto misctab = mainMenu->add_tab("femboy.Yasuo.misc", "Misc");
		{
			settings::misc::antigap = misctab->add_checkbox("femboy.Yasuo.anti", "Enable Anti Gapcloser", true);
			settings::misc::antigaphp = misctab->add_slider("femboy.Yasuo.hp", "Min HP % to Anti Gapclose", 50, 1, 100, true);
			settings::misc::interrupter = misctab->add_checkbox("femboy.Yasuo.inter", "Enable Interrupter", true);
		}

		const auto hitchancetab = mainMenu->add_tab("femboy.Yasuo.hitchance", "Hitchance");
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

		settings::hitchance::qHitChance = hitchancetab->add_combobox("femboy.Yasuo.qhitchance", "Q Hitchance", combo_elemts, 2);
		settings::hitchance::q3HitChance = hitchancetab->add_combobox("femboy.Yasuo.q3hitchance", "Q3 Hitchance", combo_elemts, 2);

		const auto drawTab = mainMenu->add_tab("femboy.Yasuo.draw", "Draws");
		{
			float defaultcolorQ[] = { 1.f, 1.f, 1.f, 1.f };
			float defaultcolorW[] = { 0.f, 0.f, 1.f, 1.f };
			float defaultcolorE[] = { 0.f, 1.f, 0.f, 1.f };
			float defaultcolorR[] = { 1.f, 0.f, 0.f, 1.f };
			const auto qdrawtab = drawTab->add_tab("femboy.Yasuo.drawq", "Q Draw Settings");
			{
				settings::draws::qRange = qdrawtab->add_checkbox("femboy.Yasuo.qrange", "Draw Q Range", true);
				settings::draws::Qcolor = qdrawtab->add_colorpick("femboy.Yasuo.qcolor", "Q Draw Color", defaultcolorQ);
			}

			const auto wdrawtab = drawTab->add_tab("femboy.Yasuo.draww", "W Draw Settings");
			{
				settings::draws::wRange = wdrawtab->add_checkbox("femboy.Yasuo.wrange", "Draw W Range", true);
				settings::draws::Wcolor = wdrawtab->add_colorpick("femboy.Yasuo.wcolor", "W Draw Color", defaultcolorW);

			}

			const auto edrawtab = drawTab->add_tab("femboy.Yasuo.drawe", "E Draw Settings");
			{
				settings::draws::eRange = edrawtab->add_checkbox("femboy.Yasuo.erange", "Draw E Range", true);
				settings::draws::Ecolor = edrawtab->add_colorpick("femboy.Yasuo.ecolor", "E Draw Color", defaultcolorE);
			}

			const auto rdrawtab = drawTab->add_tab("femboy.Yasuo.drawr", "R Draw Settings");
			{
				settings::draws::rRange = rdrawtab->add_checkbox("femboy.Yasuo.rrange", "Draw R Range", true);
				settings::draws::Rcolor = rdrawtab->add_colorpick("femboy.Yasuo.rcolor", "R Draw Color", defaultcolorR);
			}

			const auto miscdrawTab = drawTab->add_tab("femboy.Yasuo.draw.misc", "Misc Draw Settings");
			{
				settings::draws::Damage = miscdrawTab->add_checkbox("femboy.Yasuo.damage", "Draw Damage Indicator", true);
				settings::draws::legCircles = miscdrawTab->add_checkbox("femboy.Yasuo.3dcircle", "Enable 3D Circle", true);
				settings::draws::NeededAA = miscdrawTab->add_checkbox("femboy.Yasuo.aaindicator", "Draw AA Counter", true);
				settings::draws::BuffTime = miscdrawTab->add_checkbox("femboy.Yasuo.bufftime", "Draw Buff Time", true);
			}
		}

		Permashow::Instance.Init(mainMenu, "[Femboy] Yasuo");
		Permashow::Instance.AddElement("Farm", settings::clear::farmtoggle);
		Permashow::Instance.AddElement("E Tower Dive", settings::esettings::ETowerDive);
		Permashow::Instance.AddElement("R Tower Dive", settings::rsettings::RTowerDive);
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
		auto qtarget = target_selector->get_target(q->range(), damage_type::physical);

		if (myhero->is_dashing())
		{
			if (qtarget != nullptr)
			{
				if (qtarget->get_distance(myhero) > 215)
				{
					return;
				}
			}
		}

		if (q->is_ready())
		{
			if (settings::qsettings::qCombo->get_bool())
			{
				if (q->name() != "YasuoQ3Wrapper")
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

			if (settings::qsettings::q3Combo->get_bool())
			{
				if (q->name() == "YasuoQ3Wrapper")
				{
					auto q3target = target_selector->get_target(q3->range(), damage_type::physical);
					if (q3target != nullptr)
					{
						if (q3target->is_valid_target())
						{
							auto q3pred = q3->get_prediction(q3target);
							if (q3pred.hitchance >= getPredIntFromSettings(settings::hitchance::q3HitChance->get_int()))
							{
								q3->cast(q3pred.get_cast_position());
							}
						}
					}
				}
			}			
		}
	}

	void eqlogic()
	{
		auto eqtarget = target_selector->get_target(e->range(), damage_type::physical);

		if (eqtarget != nullptr)
		{
			if (eqtarget->has_buff(buff_hash("YasuoE")))
			{
				return;
			}
		}

		if (!settings::esettings::ETowerDive->get_bool())
		{
			if (eqtarget != nullptr)
			{
				auto myheroendpos = myhero->get_position().extend(eqtarget->get_position(), 475);
				if (myheroendpos.is_under_enemy_turret())
				{
					return;
				}
			}
		}

		if (!q->is_ready())
		{
			return;
		}

		if (settings::qsettings::eqCombo->get_bool())
		{
			if (eqtarget != nullptr)
			{
				if (eqtarget->is_valid_target())
				{
					if (eqtarget->get_distance(myhero) > 175 && eqtarget->get_distance(myhero) < 475)
					{
						e->cast(eqtarget);						
					}

					if (myhero->is_dashing())
					{
						if (eqtarget->get_distance(myhero) < 280)
						{
							q->cast(eqtarget);
						}
					}					
				}
			}
		}
	}

	void AutoQDash()
	{
		auto target = target_selector->get_target(q3->range(), damage_type::physical);
		if (target == nullptr)
		{
			return;
		}

		if (q->name() != "YasuoQ3Wrapper")
		{
			return;
		}

		if (settings::qsettings::q3AutoDashM->get_bool())
		{
			if (q->is_ready())
			{			
				if (target->is_valid_target())
				{
					auto q3pred = q3->get_prediction(target);
					if (q3pred.hitchance >= hit_chance::dashing)
					{
						q3->cast(q3pred.get_cast_position());
						console->print("[FemboyAio] Casting Q3 on Dashing/Immobile Target");
					}
				}
			}
		}
	}

	void ELogic()
	{
		if (settings::esettings::eCombo->get_bool())
		{
			if (settings::esettings::eGapclose->get_bool())
			{
				if (e->is_ready())
				{
					auto target = target_selector->get_target(settings::esettings::eGapCloseTargetScanRange->get_int(), damage_type::magical);
					if (target != nullptr)
					{
						for (const auto& min : entitylist->get_enemy_minions())
						{
							if (min != nullptr)
							{
								if (target != nullptr)
								{
									if (target->get_distance(myhero) > 300)
									{
										if (min->is_valid_target(e->range()) && !min->is_dead() && !min->has_buff(buff_hash("YasuoE")))
										{
											if (min->get_distance(target) < myhero->get_distance(target))
											{
												auto posaftergap = myhero->get_position().extend(min->get_position(), 475);
												if (!settings::esettings::ETowerDive->get_bool())
												{
													if (posaftergap.is_under_enemy_turret())
													{
														return;
													}
													else
													{
														e->cast(min);
													}
												}
												else
												{
													e->cast(min);
												}
											}
										}
									}
								}
							}
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
			for (const auto& target : entitylist->get_enemy_heroes())
			{
				if (target != nullptr)
				{
					if (target->is_valid_target(r->range()))
					{
						if (!settings::rsettings::RTowerDive->get_bool())
						{
							if (target->get_position().is_under_enemy_turret())
							{
								return;
							}
						}

						if (target->has_buff_type(buff_type::Knockup))
						{
							if (settings::rsettings::rmode->get_int() == 0)
							{
								if (target->get_health_percent() <= settings::rsettings::rHP->get_int())
								{
									r->cast(target);
								}
							}

							if (settings::rsettings::rmode->get_int() == 1)
							{
								if (target->get_real_health() <= GetDamage(target))
								{
									r->cast(target);
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
		if (q->name() == "YasuoQ3Wrapper") return;
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

	void killstealQ3(const game_object_script& enemy)
	{
		if (!q->is_ready()) return;
		if (!settings::killsteal::ksQ3->get_bool()) return;
		if (q->name() != "YasuoQ3Wrapper") return;
		if (!enemy->is_valid_target(q3->range())) return;

		if (enemy->get_real_health() <= qDamage(enemy))
		{
			auto q3pred = q3->get_prediction(enemy);
			if (q3pred.hitchance >= hit_chance::high)
			{
				q3->cast(q3pred.get_cast_position());
			}
		}
	}

	void killstealFQ(const game_object_script& enemy)
	{
		if (!q->is_ready()) return;
		if (!flash->is_ready()) return;
		if (!settings::killsteal::ksQFlash->get_bool()) return;
		if (q->name() == "YasuoQ3Wrapper") return;
		if (!enemy->is_valid_target(q->range() + flash->range())) return;
		if (enemy->get_distance(myhero) < q->range()) return;

		if (enemy->get_real_health() <= qDamage(enemy))
		{
			flash->cast(enemy->get_position());
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

		if (enemy->get_real_health() <= eDamage(enemy))
		{
			e->cast(enemy);
		}
	}

	void killstealloop()
	{
		for (auto&& enemy : entitylist->get_enemy_heroes())
		{
			if (enemy == nullptr || !enemy->is_valid_target()) continue;

			killstealQ(enemy);
			killstealQ3(enemy);
			killstealFQ(enemy);
			killstealE(enemy);
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
	}

	void on_draw_env()
	{
		if (settings::draws::qRange->get_bool())
		{
			if (q->level() >= 1)
			{
				if (q->name() == "YasuoQ1Wrapper" || q->name() == "YasuoQ2Wrapper")
				{
					drawCircle(myhero->get_position(), q->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::Qcolor->get_color());
				}

				if (q->name() == "YasuoQ3Wrapper")
				{
					drawCircle(myhero->get_position(), q3->range(), 100, settings::draws::legCircles->get_bool(), settings::draws::Qcolor->get_color());
				}
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
					femboyutils::AATracker();
				}
			}
		}

		if (settings::draws::BuffTime->get_bool())
		{
			auto buff = myhero->get_buff(buff_hash("YasuoQ2"));
			if (buff != nullptr)
			{
				int num = (int)buff->get_end() - (int)gametime->get_time();
				std::string numstr = std::to_string(num);
				int firstdigit = std::stoi(numstr.substr(0, 2));
				const auto position = vector(800.f, 651.f);
				draw_manager->add_text_on_screen(position, MAKE_COLOR(255, 255, 255, 255), 22, "Q3 Time: %i", firstdigit);
			}
		}
	}

	void update()
	{
		if (orbwalker->combo_mode())
		{
			eqlogic();
			ELogic();			
			QLogic();
			RLogic();
		}

		if (orbwalker->lane_clear_mode())
		{
			laneclear();
			jungleclear();
		}

		AutoQDash();
		killstealloop();
		interrupter();
	}

	void load()
	{
		q = plugin_sdk->register_spell(spellslot::q, 450.f);
		q->set_skillshot(0.25f + 0.066f, 40.f, 1200.f, {}, skillshot_type::skillshot_line);
		q->set_spell_lock(false);

		q3 = plugin_sdk->register_spell(spellslot::q, 1150.f);
		q3->set_skillshot(0.25f + 0.066f, 90.f, 1200.f, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		q3->set_spell_lock(false);

		w = plugin_sdk->register_spell(spellslot::w, 350.f);
		w->set_spell_lock(false);

		e = plugin_sdk->register_spell(spellslot::e, 475.f);
		e->set_spell_lock(false);

		r = plugin_sdk->register_spell(spellslot::r, 1400.f);
		r->set_spell_lock(false);

		if (myhero->get_spell(spellslot::summoner1)->get_spell_data()->get_name_hash() == spell_hash("SummonerFlash"))
			flash = plugin_sdk->register_spell(spellslot::summoner1, 400.f);
		else if (myhero->get_spell(spellslot::summoner2)->get_spell_data()->get_name_hash() == spell_hash("SummonerFlash"))
			flash = plugin_sdk->register_spell(spellslot::summoner2, 400.f);

		createmenu();

		antigapcloser::add_event_handler(antigap);
		event_handler<events::on_process_spell_cast>::add_callback(blockspellswithW, event_prority::high);
		event_handler<events::on_process_spell_cast>::add_callback(onprocess, event_prority::high);
		event_handler<events::on_update>::add_callback(update, event_prority::highest);
		event_handler<events::on_env_draw>::add_callback(on_draw_env, event_prority::high);
		event_handler<events::on_draw>::add_callback(on_draw_real, event_prority::high);
	}

	void unload()
	{
		plugin_sdk->remove_spell(q);
		plugin_sdk->remove_spell(q3);
		plugin_sdk->remove_spell(w);
		plugin_sdk->remove_spell(e);
		plugin_sdk->remove_spell(r);

		if (flash)
			plugin_sdk->remove_spell(flash);

		antigapcloser::remove_event_handler(antigap);
		event_handler<events::on_process_spell_cast>::remove_handler(onprocess);
		event_handler<events::on_update>::remove_handler(update);
		event_handler<events::on_env_draw>::remove_handler(on_draw_env);
		event_handler<events::on_draw>::remove_handler(on_draw_real);
		event_handler<events::on_process_spell_cast>::remove_handler(blockspellswithW);
	}

	float qrawdamage[] = { 20.f, 45.f, 70.f, 95.f, 120.f };
	float qDamage(const game_object_script& target)
	{
		if (q->level() == 0) return 0.f;

		damage_input input;
		input.raw_physical_damage = qrawdamage[q->level() - 1] + myhero->get_total_attack_damage() * 1.05f;
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float erawdamage[] = { 60.f, 70.f, 80.f, 90.f, 100.f };
	float eDamage(const game_object_script& target)
	{
		if (e->level() == 0) return 0.f;

		damage_input input;
		input.raw_magical_damage = erawdamage[e->level() - 1] + myhero->mPercentBonusPhysicalDamageMod() * 0.20f + myhero->get_total_ability_power() * 0.60f;
		const float damage = damagelib->calculate_damage_on_unit(myhero, target, &input);
		return damage;
	}

	float rrawdamage[] = { 200.f, 350.f, 500.f };
	float rDamage(const game_object_script& target)
	{
		if (r->level() == 0) return 0.f;

		damage_input input;
		input.raw_physical_damage = rrawdamage[r->level() - 1] + myhero->get_total_attack_damage() * 1.50f;
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
			Damage += rDamage(target);
		}

		return Damage;
	}

	void antigap(game_object_script sender, antigapcloser::antigapcloser_args* args)
	{
		if (myhero->get_health_percent() > settings::misc::antigaphp->get_int())
		{
			return;
		}

		if (q->name() != "YasuoQ3Wrapper")
		{
			return;
		}

		if (settings::misc::antigap->get_bool())
		{
			if (q->is_ready())
			{
				if (sender != nullptr)
				{
					if (sender->is_enemy() && sender->is_valid_target(q3->range()))
					{
						if (args->end_position.distance(myhero) < 450)
						{
							q3->cast(args->end_position);
							console->print("[FemboyAio] Casting on Gap Closer End Position");
						}
					}
				}
			}
		}
	}

	void interrupter()
	{
		if (q->name() != "YasuoQ3Wrapper")
		{
			return;
		}

		if (settings::misc::interrupter->get_bool())
		{
			for (auto&& enemy : entitylist->get_enemy_heroes())
			{
				if (q->is_ready())
				{
					if (enemy != nullptr)
					{
						if (enemy->is_enemy() && enemy->is_casting_interruptible_spell() == 2 && enemy->is_valid_target(q3->range()))
						{
							if (enemy->get_distance(myhero) <= q3->range())
							{
								auto q3pred = q3->get_prediction(enemy);
								if (q3pred.hitchance >= hit_chance::high)
								{
									q3->cast(enemy);
									console->print("[FemboyAio] Casting Q3 on %s", enemy->get_name_cstr());
								}
							}
						}
					}
				}
			}
		}
	}

	void blockspellswithW(game_object_script sender, spell_instance_script spell)
	{
		if (!settings::wsettings::Wevade->get_bool())
		{
			return;
		}		

		if (w->is_ready())
		{
			for (const auto& enemy : entitylist->get_enemy_heroes())
			{
				if (enemy == nullptr)
				{
					return;
				}

				if (enemy->get_base_skin_name() == "Annie" || sender->get_base_skin_name() == "Annie")
				{
					if (settings::wsettings::AnnieQ->get_bool())
					{
						if (enemy != nullptr)
						{
							if (enemy->get_base_skin_name() == "Annie" || sender->get_base_skin_name() == "Annie")
							{
								if (enemy->has_buff(buff_hash("anniepassiveprimed")))
								{
									if (spell->get_spellslot() == spellslot::q)
									{
										if (spell->get_last_target_id() == myhero->get_id())
										{
											console->print("Blocking Annie Q");
											w->cast(enemy->get_position());
										}
									}
								}
							}
						}
					}
				}			

				if (enemy->get_base_skin_name() == "Fiddlesticks" || sender->get_base_skin_name() == "Fiddlesticks")
				{
					if (settings::wsettings::FiddleQ->get_bool())
					{
						if (enemy != nullptr)
						{
							if (enemy->get_base_skin_name() == "Fiddlesticks" || sender->get_base_skin_name() == "Fiddlesticks")
							{
								if (spell->get_spellslot() == spellslot::q)
								{
									if (spell->get_last_target_id() == myhero->get_id())
									{
										console->print("Blocking Fiddle Q");
										w->cast(enemy->get_position());
									}
								}
							}
						}
					}
				}				
			
				if (enemy->get_base_skin_name() == "Lulu" || sender->get_base_skin_name() == "Lulu")
				{
					if (settings::wsettings::LuLuW->get_bool())
					{
						if (enemy != nullptr)
						{
							if (enemy->get_base_skin_name() == "Lulu" || sender->get_base_skin_name() == "Lulu")
							{
								if (spell->get_spellslot() == spellslot::w)
								{
									if (spell->get_last_target_id() == myhero->get_id())
									{
										console->print("Blocking Lulu W");
										w->cast(enemy->get_position());
									}
								}
							}
						}
					}
				}
				
				if (enemy->get_base_skin_name() == "MissFortune" || sender->get_base_skin_name() == "MissFortune")
				{
					if (settings::wsettings::MFQ->get_bool())
					{
						if (enemy != nullptr)
						{
							if (enemy->get_base_skin_name() == "MissFortune" || sender->get_base_skin_name() == "MissFortune")
							{
								if (spell->get_spellslot() == spellslot::q)
								{
									if (spell->get_last_target_id() == myhero->get_id())
									{
										console->print("Blocking Miss Fortune Q");
										w->cast(enemy->get_position());
									}
								}
							}
						}
					}
				}

				if (enemy->get_base_skin_name() == "Teemo" || sender->get_base_skin_name() == "Teemo")
				{
					if (settings::wsettings::TeemoQ->get_bool())
					{
						if (enemy != nullptr)
						{
							if (enemy->get_base_skin_name() == "Teemo" || sender->get_base_skin_name() == "Teemo")
							{
								if (spell->get_spellslot() == spellslot::q)
								{
									if (spell->get_last_target_id() == myhero->get_id())
									{
										console->print("Blocking Teemo Q");
										w->cast(enemy->get_position());
									}
								}
							}
						}
					}
				}

				if (enemy->get_base_skin_name() == "Tristana" || sender->get_base_skin_name() == "Tristana")
				{
					if (settings::wsettings::TristR->get_bool())
					{
						if (enemy != nullptr)
						{
							if (enemy->get_base_skin_name() == "Tristana" || sender->get_base_skin_name() == "Tristana")
							{
								if (spell->get_spellslot() == spellslot::r)
								{
									if (spell->get_last_target_id() == myhero->get_id())
									{
										console->print("Blocking Tristana R");
										w->cast(enemy->get_position());
									}
								}
							}
						}
					}
				}

				if (enemy->get_base_skin_name() == "Vayne" || sender->get_base_skin_name() == "Vayne")
				{
					if (settings::wsettings::VayneE->get_bool())
					{
						if (enemy != nullptr)
						{
							if (enemy->get_base_skin_name() == "Vayne" || sender->get_base_skin_name() == "Vayne")
							{
								if (spell->get_spellslot() == spellslot::e)
								{
									if (spell->get_last_target_id() == myhero->get_id())
									{
										console->print("Blocking Vayne E");
										w->cast(enemy->get_position());
									}
								}
							}
						}
					}
				}

				if (enemy->get_base_skin_name() == "Veigar" || sender->get_base_skin_name() == "Veigar")
				{
					if (settings::wsettings::VeigarR->get_bool())
					{
						if (enemy != nullptr)
						{
							if (enemy->get_base_skin_name() == "Veigar" || sender->get_base_skin_name() == "Veigar")
							{
								if (spell->get_spellslot() == spellslot::r)
								{
									if (spell->get_last_target_id() == myhero->get_id())
									{
										console->print("Blocking Veigar R");
										w->cast(enemy->get_position());
									}
								}
							}
						}
					}
				}

				if (enemy->get_base_skin_name() == "Syndra" || sender->get_base_skin_name() == "Syndra")
				{
					if (settings::wsettings::SyndraR->get_bool())
					{
						if (enemy != nullptr)
						{
							if (enemy->get_base_skin_name() == "Syndra" || sender->get_base_skin_name() == "Syndra")
							{
								if (spell->get_spellslot() == spellslot::r)
								{
									if (spell->get_last_target_id() == myhero->get_id())
									{
										console->print("Blocking Syndra R");
										w->cast(enemy->get_position());
									}
								}
							}
						}
					}
				}
			}
		}
	}

	void onprocess(game_object_script sender, spell_instance_script spell)
	{
		for (auto&& enemys : entitylist->get_enemy_heroes())
		{
			if (enemys != nullptr)
			{
				if (enemys->get_base_skin_name() == "MissFortune")
				{
					if (settings::wsettings::MFR->get_bool())
					{												
						if (enemys->is_valid())
						{							
							if (enemys->get_base_skin_name() == "MissFortune")
							{								
								for (auto spells : evade->get_skillshots())
								{									
									if (spells.spell_data_name == "MissFortuneBulletTime")
									{																	
										if (spells.polygon.is_inside(myhero->get_position()))
										{
											console->print("Blocking MF R");
											w->cast(enemys->get_position());
										}										
									}								
								}							
							}						
						}						
					}										
				}
			}						
		}
	}
}