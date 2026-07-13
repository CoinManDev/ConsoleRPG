#pragma once

#include <array>
#include <iostream>

namespace Aesthetics
{
	enum Color
	{
		reset_color,

		black,
		red,
		green,
		yellow,
		blue,
		magenta,
		cyan,
		white,

		black_bg,
		red_bg,
		green_bg,
		yellow_bg,
		blue_bg,
		magenta_bg,
		cyan_bg,
		white_bg,

		max_colors,
	};

	inline constexpr std::array colors{
		"\033[0m",

		"\033[30m",
		"\033[31m",
		"\033[32m",
		"\033[33m",
		"\033[34m",
		"\033[35m",
		"\033[36m",
		"\033[37m",

		"\033[40m",
		"\033[41m",
		"\033[42m",
		"\033[43m",
		"\033[44m",
		"\033[45m",
		"\033[46m",
		"\033[47m",
	};
	static_assert(colors.size() == max_colors);

	inline constexpr Color bgColor{ green_bg };
	inline constexpr const char* clear{ "\033[H\033[2J" };
}

std::ostream& operator<<(std::ostream& out, Aesthetics::Color color);
