#include "Keybinds.h"

std::ostream& operator<<(std::ostream& out, const Keybinds::Keybind& keybind)
{
	return out << keybind.keys[0];
}

std::ostream& operator<<(std::ostream& out, const std::vector<Keybinds::Keybind>& keybindCombination)
{
	for (const auto& keybind : keybindCombination)
		out << keybind;
	return out;
}

bool operator==(char c, const Keybinds::Keybind& keybind)
{
	return std::find(keybind.keys.begin(), keybind.keys.end(), c) != keybind.keys.end();
}
