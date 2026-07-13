#pragma once

#include <iostream>
#include <vector>

namespace Keybinds
{
	struct Keybind
	{
		std::vector<char> keys{};
	};

	inline const Keybind up{ { 'W', 'w' } };
	inline const Keybind down{ { 'S', 's' } };
	inline const Keybind left{ { 'A', 'a' } };
	inline const Keybind right{ { 'D', 'd' } };
	inline const Keybind inventory{ { 'I', 'i' } };
	inline const Keybind select{ { 'E', 'e' } };
	inline const Keybind agree{ { 'Y', 'y' } };
	inline const Keybind disagree{ { 'N', 'n' } };
	inline const Keybind quit{ { 'Q', 'q' } };

	inline const std::vector<Keybind> movementKeybinds{ up, left, down, right };
}

std::ostream& operator<<(std::ostream& out, const Keybinds::Keybind& keybind);
std::ostream& operator<<(std::ostream& out, const std::vector<Keybinds::Keybind>& keybindCombination);
bool operator==(char c, const Keybinds::Keybind& keybind);
