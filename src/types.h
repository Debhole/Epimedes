#pragma once
#include <iostream>
#include <string>
#include <stdio.h>

typedef uint64_t BitBoard;

inline BitBoard reverse(BitBoard b)
{
	b = (b & 0x5555555555555555ULL) << 1 | (b >> 1) & 0x5555555555555555ULL;
	b = (b & 0x3333333333333333ULL) << 2 | (b >> 2) & 0x3333333333333333ULL;
	b = (b & 0x0f0f0f0f0f0f0f0fULL) << 4 | (b >> 4) & 0x0f0f0f0f0f0f0f0fULL;
	b = (b & 0x00ff00ff00ff00ffULL) << 8 | (b >> 8) & 0x00ff00ff00ff00ffULL;
	b = (b << 48) | ((b & 0xffff0000ULL) << 16) |
		((b >> 16) & 0xffff0000ULL) | (b >> 48);
	return b;
}

enum class Piece : int
{
	WHITE_PAWN, WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, WHITE_KING,
	BLACK_PAWN, BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, BLACK_KING,
	PIECE_NUM = 12, PIECE_NULL
};

enum class Color : int
{
	WHITE, BLACK, COLOR_NUM = 2
};

extern std::ostream& operator<<(std::ostream& os, const Color& color);

inline Color colorOf(Piece piece)
{
	return static_cast<Color>(static_cast<int>(piece) / 6);
}

enum class File : int
{
	FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H
};

inline File toFile(int f)
{
	return static_cast<File>(f);
}

enum class Rank : int
{
	RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8
};

inline Rank toRank(int r)
{
	return static_cast<Rank>(r);
}

enum class Square : int
{
	A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8,
	SQUARE_NUM = 64, SQUARE_NULL = 65
};

inline int fileOf(const Square& square) { return static_cast<int>(square) % 8; }
inline int rankOf(const Square& square) { return static_cast<int>(square) / 8; }
inline int numOf(const Square& square) { return static_cast<int>(square); }

inline Square asSquare(int rank, int file)
{
	return static_cast<Square>(rank * 8 + file);
}

#define DEFINE_REGULAR_OPERATORS(T)															 \
inline T& operator+=(T& d, int n) { return d = static_cast<T>(static_cast<int>(d) + n); }	 \
inline T& operator-=(T& d, int n) { return d = static_cast<T>(static_cast<int>(d) - n); }    \
inline T& operator++(T& d) { return d = static_cast<T>(static_cast<int>(d) + 1); }           \
inline T& operator--(T& d) { return d = static_cast<T>(static_cast<int>(d) - 1); }			 \
inline T operator+(T d, int n) { return static_cast<T>(static_cast<int>(d) + n); }			 \
inline T operator-(T d, int n) { return static_cast<T>(static_cast<int>(d) - n); }


DEFINE_REGULAR_OPERATORS(Square)
DEFINE_REGULAR_OPERATORS(Rank)
DEFINE_REGULAR_OPERATORS(File)
DEFINE_REGULAR_OPERATORS(Piece)
DEFINE_REGULAR_OPERATORS(Color)

#undef DEFINE_REGULAR_OPERATORS

// Bits 0 - 5: Square the piece is moving from
// Bits 6 - 11: Square the piece is moving too
// Bit 12 - 13: Promotion (00 = knight, 01 = bishop, 10 = rook, 11 = queen)
// Bits 14 - 15: Special move (01 = en passant, 10 = castle, 11 = promotion)
typedef uint16_t Move;

extern Move fromSquareMask;
extern Move toSquareMask;
extern Move promotionMask;
extern Move typeMask;

extern char* charSwitch[16];
extern char* promotionType[4];

enum class MoveType
{
	NORMAL, EN_PASSANT, CASTLING, PROMOTION
};

inline MoveType typeof(const Move& move)
{
	return static_cast<MoveType>((static_cast<int>(move) & typeMask) >> 14);
}

inline Square toSquare(const Move& move)
{
	return static_cast<Square>((toSquareMask & move) >> 6);
}

inline Square fromSquare(const Move& move)
{
	return static_cast<Square>(fromSquareMask & move);
}

inline Piece getPromotionPiece(const Move& move, const Color& sideToMove)
{
	int piece = ((move & promotionMask) >> 12) + 1;
	if (sideToMove == Color::BLACK) piece += 6;

	return static_cast<Piece>(piece);
}

inline std::string moveToAlgebraic(const Move& move)
{
	Square from = Square::A1 + (move & fromSquareMask);
	Square to = Square::A1 + ((move & toSquareMask) >> 6);
	std::string algebraic(charSwitch[fileOf(from)]);
	algebraic.append(charSwitch[rankOf(from) + 8]);
	algebraic.append(charSwitch[fileOf(to)]);
	algebraic.append(charSwitch[rankOf(to) + 8]);

	MoveType type = typeof(move);
	if (type == MoveType::EN_PASSANT) // En passant
	{
		algebraic.append("e");
	}
	else if (type == MoveType::PROMOTION) // Promotion
	{
		algebraic.append(promotionType[((static_cast<int>(move) & promotionMask) >> 12) & 3]);
	}
	else if (type == MoveType::CASTLING) // Castling
	{
		algebraic.append("c");
	}

	return algebraic;
}

inline Move toMove(const Square& from, const Square& to, int special)
{
	return (static_cast<int>(to) << 6) + static_cast<int>(from) + (special << 12);
}

inline Move algebraicToMove(const std::string& algebraic)
{
	Square from = Square::A1 + ((algebraic[0] - 'a') + 8 * (algebraic[1] - '1'));
	Square to = Square::A1 + ((algebraic[2] - 'a') + 8 * (algebraic[3] - '1'));

	int special = 0b0000;

	if (algebraic.size() == 5)
	{
		switch (algebraic.at(4))
		{
			case 'n':
				special = 0b1100;
				break;
			case 'b':
				special = 0b1101;
				break;
			case 'r':
				special = 0b1110;
				break;
			case 'q':
				special = 0b1111;
				break;
			case 'e':
				special = 0b0100;
				break;
			case 'c':
				special = 0b1000;
				break;
		}
	}

	Move move = toMove(from, to, special);
	return move;
}