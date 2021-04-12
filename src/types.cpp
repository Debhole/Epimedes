#include "types.h"

const int bitScanTable[64] = {
	0,  1, 48,  2, 57, 49, 28,  3,
   61, 58, 50, 42, 38, 29, 17,  4,
   62, 55, 59, 36, 53, 51, 43, 22,
   45, 39, 33, 30, 24, 18, 12,  5,
   63, 47, 56, 27, 60, 41, 37, 16,
   54, 35, 52, 21, 44, 32, 23, 11,
   46, 26, 40, 15, 34, 20, 31, 10,
   25, 14, 19,  9, 13,  8,  7,  6
};

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