#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <vector>

namespace skin_changer
{
	inline std::vector< std::pair< std::string, int > > paint_kits{};

	extern void init_parser();
	extern bool check_skin_compatibility(int weapon_index, int paint_kit_id);
	extern int correct_sequence(const short& index, const int seq);
	extern void on_postdataupdate_start(int stage);
	extern void on_game_events(c_game_event* event);
	extern void on_frame_render_end(int stage);
}