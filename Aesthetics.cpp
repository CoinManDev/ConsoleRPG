#include "Aesthetics.h"

std::ostream& operator<<(std::ostream& out, Aesthetics::Color color)
{
	return out << Aesthetics::colors[color];
}
