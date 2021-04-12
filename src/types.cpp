#include "types.h"

Move fromSquareMask = 0b111111;
Move toSquareMask = 0b111111000000;
Move promotionMask = 0b11000000000000;
Move typeMask = 0b1100000000000000;

char* charSwitch[16] = {
	"a", "b", "c", "d", "e", "f", "g", "h",
	"1", "2", "3", "4", "5", "6", "7", "8"
};

char* promotionType[4] = {
	"n", "b", "r", "q"
};

std::ostream& operator<<(std::ostream& os, const Color& color)
{
	if (color == Color::WHITE) os << "White";
	else if (color == Color::BLACK) os << "Black";
	else os << "ERROR: Color neither White nor Black!";
	return os;
}