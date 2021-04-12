#include <iostream>
#include <string>
#include <sstream>

#include "position.h"
#include "types.h"

namespace Epimedes
{
	const BitBoard Position::RANK_MASK[8] = {
		0xFFULL, 0xFF00ULL, 0xFF0000ULL, 0xFF000000ULL, 0xFF00000000ULL,
		0xFF0000000000ULL, 0xFF000000000000ULL, 0xFF00000000000000ULL
	};
	const BitBoard Position::FILE_MASK[8] = {
		0x101010101010101ULL, 0x202020202020202ULL, 0x404040404040404ULL,
		0x808080808080808ULL, 0x1010101010101010ULL, 0x2020202020202020ULL,
		0x4040404040404040ULL, 0x8080808080808080ULL
	};
	const BitBoard Position::DIAGONAL_MASK[15] = {
		0x1ULL, 0x102ULL, 0x10204ULL, 0x1020408ULL, 0x102040810ULL, 0x10204081020ULL, 0x1020408102040ULL,
		0x102040810204080ULL, 0x204081020408000ULL, 0x408102040800000ULL, 0x810204080000000ULL,
		0x1020408000000000ULL, 0x2040800000000000ULL, 0x4080000000000000ULL, 0x8000000000000000ULL
	};
	const BitBoard Position::ANTIDIAGONAL_MASK[15] = {
		0x80ULL, 0x8040ULL, 0x804020ULL, 0x80402010ULL, 0x8040201008ULL, 0x804020100804ULL, 0x80402010080402ULL,
		0x8040201008040201ULL, 0x4020100804020100ULL, 0x2010080402010000ULL, 0x1008040201000000ULL,
		0x804020100000000ULL, 0x402010000000000ULL, 0x201000000000000ULL, 0x100000000000000ULL
	};

	const BitBoard Position::KNIGHT_SPAN = 0xA1100110AULL;
	const BitBoard Position::KING_SPAN = 0x70507ULL;

	const int Position::INITIAL_ROOK_SQUARES[4] = { 7, 0, 63, 56 };
	const int Position::CASTLED_ROOK_SQUARES[4] = { 5, 3, 61, 59 };

	std::ostream& operator<<(std::ostream& os, const Position& pos)
	{
		os << "+---+---+---+---+---+---+---+---+\n";
		for (Rank r = Rank::RANK_8; r >= Rank::RANK_1; --r)
		{
			for (File f = File::FILE_A; f <= File::FILE_H; ++f)
			{
				os << "| " << pos.pieceChar(pos.pieceAt(r, f)) << " ";
			}
			os << "| " << static_cast<int>(r) + 1 << "\n+---+---+---+---+---+---+---+---+\n";
		}

		os << "  a   b   c   d   e   f   g   h\n";

		os << "FEN: " << pos.getFen();

		os << "\nActive Color: " << pos.getSideToMove();

		os << "\nEP Square: " << static_cast<int>(pos.getEPSquare());

		char castleChars[4] = { 'K', 'Q', 'k', 'q' };

		os << "\nCastling Rights: ";
		for (int i = 0; i < 4; i++)
		{
			if (pos.canCastle(i)) os << castleChars[i];
		}

		return os;
	}

	Position::Position()
	{
		for (Piece& p : board) p = Piece::PIECE_NULL;
		for (BitBoard& b : pieceBoards) b = 0ULL;

		sideToMove = Color::WHITE;
		enPassantSquare = Square::SQUARE_NULL;
		for (bool& b : castleRights) b = false;
	}

	Position::Position(const Position& pos)
	{
		for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			board[i] = pos.pieceAt(i);
		for (int i = 0; i < static_cast<int>(Piece::PIECE_NUM); i++)
			pieceBoards[i] = pos.getBoard(static_cast<Piece>(i));
		
		sideToMove = pos.getSideToMove();
		enPassantSquare = pos.getEPSquare();
		for (int i = 0; i < 4; i++)
			castleRights[i] = pos.canCastle(i);
	}

	Position& Position::set(const std::string& fenStr)
	{
		clear();
		Square square = Square::A8;
		Piece piece = Piece::PIECE_NULL;

		fen = fenStr;

		std::istringstream stringStream(fenStr);
		stringStream >> std::noskipws;
		char token;

		// Adding pieces to the board
		while (stringStream >> token && !isspace(token))
		{
			if (isdigit(token))
			{
				for (int i = token - '0'; i > 0; i--)
				{
					board[static_cast<int>(square)] = Piece::PIECE_NULL;
					++square;
				}
			}
			else if (getPiece(token, piece) != Piece::PIECE_NULL)
			{
				setPiece(square, piece);
				++square;
			}
			else if (token == '/')
			{
				square -= 16;
			}
		}
		
		// Player to move
		stringStream.get(token);
		sideToMove = token == 'w' ? Color::WHITE : Color::BLACK;
		stringStream >> token;

		// Castling Rights
		while (stringStream >> token && !isspace(token))
		{
			Color color = islower(token) ? Color::BLACK : Color::WHITE;
			char relSide = toupper(token);

			if (relSide == 'K')
			{
				if (color == Color::WHITE) setCastleRights(Piece::WHITE_KING, true);
				else setCastleRights(Piece::BLACK_KING, true);
			}
			else if(relSide == 'Q')
			{
				if (color == Color::WHITE) setCastleRights(Piece::WHITE_QUEEN, true);
				else setCastleRights(Piece::BLACK_QUEEN, true);
			}
		}

		char file, rank;
		if ((stringStream >> file) && (stringStream >> rank) &&
			(file >= 'a' && file <= 'h') && rank == (sideToMove == Color::WHITE ? '6' : '3'))
		{
			enPassantSquare = asSquare(rank - '1', file - 'a');
		}
		else enPassantSquare = Square::SQUARE_NULL;

		return *this;
	}

	Position& Position::updateBoard()
	{
		for (int square = static_cast<int>(Square::A1); square < static_cast<int>(Square::SQUARE_NUM); ++square)
		{
			board[square] = Piece::PIECE_NULL;
			int arraySize = sizeof(pieceBoards) / sizeof(pieceBoards[0]);
			for (int i = 0; i < arraySize; i++)
			{
				if (((pieceBoards[i] >> square) & 1) == 1) board[square] = static_cast<Piece>(i);
			}
		}

		return *this;
	}

	Position& Position::makeMove(const Move& move)
	{
		int from = static_cast<int>(fromSquare(move));
		int to = static_cast<int>(toSquare(move));

		if (typeof(move) == MoveType::EN_PASSANT)
		{
			for (BitBoard& bitBoard : pieceBoards)
			{
				if (((bitBoard >> from) & 1) == 1) bitBoard |= 1ULL << to;

				bitBoard &= ~(1ULL << from);
			}

			int captureSquare = (sideToMove == Color::WHITE) ? static_cast<int>(enPassantSquare) - 8 : static_cast<int>(enPassantSquare) + 8;

			pieceBoards[static_cast<int>(Piece::WHITE_PAWN)] &= ~(1ULL << captureSquare);
			pieceBoards[static_cast<int>(Piece::BLACK_PAWN)] &= ~(1ULL << captureSquare);

			enPassantSquare = Square::SQUARE_NULL;
		}
		else if (typeof(move) == MoveType::CASTLING)
		{
			enPassantSquare = Square::SQUARE_NULL;

			int rookFrom, rookTo;
			if (to > from)
			{
				rookFrom = to + 1;
				rookTo = to - 1;
			}
			else
			{
				rookFrom = to - 2;
				rookTo = to + 1;
			}

			if (sideToMove == Color::WHITE)
			{
				pieceBoards[static_cast<int>(Piece::WHITE_KING)] &= ~(1ULL << from);
				pieceBoards[static_cast<int>(Piece::WHITE_ROOK)] &= ~(1ULL << rookFrom);

				pieceBoards[static_cast<int>(Piece::WHITE_KING)] |= (1ULL << to);
				pieceBoards[static_cast<int>(Piece::WHITE_ROOK)] |= (1ULL << rookTo);

				setCastleRights(Piece::WHITE_KING, false);
				setCastleRights(Piece::WHITE_QUEEN, false);
			}
			else
			{
				pieceBoards[static_cast<int>(Piece::BLACK_KING)] &= ~(1ULL << from);
				pieceBoards[static_cast<int>(Piece::BLACK_ROOK)] &= ~(1ULL << rookFrom);

				pieceBoards[static_cast<int>(Piece::BLACK_KING)] |= (1ULL << to);
				pieceBoards[static_cast<int>(Piece::BLACK_ROOK)] |= (1ULL << rookTo);

				setCastleRights(Piece::BLACK_KING, false);
				setCastleRights(Piece::BLACK_QUEEN, false);
			}
		}
		else if (typeof(move) == MoveType::PROMOTION)
		{
			enPassantSquare = Square::SQUARE_NULL;
			
			for (BitBoard& bitBoard : pieceBoards)
				bitBoard &= ~((1ULL << from) | (1ULL << to));

			Piece promotion = getPromotionPiece(move, sideToMove);
			pieceBoards[static_cast<int>(promotion)] |= (1ULL << to);

			if (static_cast<Square>(to) == Square::A1)
				setCastleRights(Piece::WHITE_QUEEN, false);
			else if (static_cast<Square>(to) == Square::H1)
				setCastleRights(Piece::WHITE_KING, false);
			else if (static_cast<Square>(to) == Square::A8)
				setCastleRights(Piece::BLACK_QUEEN, false);
			else if (static_cast<Square>(to) == Square::H8)
				setCastleRights(Piece::BLACK_KING, false);
		}
		else
		{
			if (pieceAt(from) == Piece::WHITE_PAWN && to - from == 16) enPassantSquare = static_cast<Square>(from + 8);
			else if (pieceAt(from) == Piece::BLACK_PAWN && from - to == 16) enPassantSquare = static_cast<Square>(from - 8);
			else enPassantSquare = Square::SQUARE_NULL;

			for (BitBoard& bitBoard : pieceBoards)
			{
				if (((bitBoard >> from) & 1) == 1) bitBoard |= 1ULL << to;
				else bitBoard &= ~(1ULL << to);

				bitBoard &= ~(1ULL << from);
			}

			if (static_cast<Square>(from) == Square::A1 || static_cast<Square>(to) == Square::A1) 
				setCastleRights(Piece::WHITE_QUEEN, false);
			else if (static_cast<Square>(from) == Square::H1 || static_cast<Square>(to) == Square::H1) 
				setCastleRights(Piece::WHITE_KING, false);
			else if (static_cast<Square>(from) == Square::A8 || static_cast<Square>(to) == Square::A8)
				setCastleRights(Piece::BLACK_QUEEN, false);
			else if (static_cast<Square>(from) == Square::H8 || static_cast<Square>(to) == Square::H8)
				setCastleRights(Piece::BLACK_KING, false);
			else if (static_cast<Square>(from) == Square::E1)
			{
				setCastleRights(Piece::WHITE_KING, false);
				setCastleRights(Piece::WHITE_QUEEN, false);
			}
			else if (static_cast<Square>(from) == Square::E8)
			{
				setCastleRights(Piece::BLACK_KING, false);
				setCastleRights(Piece::BLACK_QUEEN, false);
			}
		}

		sideToMove = (sideToMove == Color::WHITE) ? Color::BLACK : Color::WHITE;

		return *this;
	}

	BitBoard Position::getAttacked(Color color) const
	{
		BitBoard attacked = 0ULL;
		if (color == Color::BLACK)
		{
			attacked |= (getBoard(Piece::BLACK_PAWN) >> 7) & ~fileBoard(File::FILE_A);
			attacked |= (getBoard(Piece::BLACK_PAWN) >> 9) & ~fileBoard(File::FILE_H);
		}
		else
		{
			attacked |= (getBoard(Piece::WHITE_PAWN) << 7) & ~fileBoard(File::FILE_H);
			attacked |= (getBoard(Piece::WHITE_PAWN) << 9) & ~fileBoard(File::FILE_A);
		}

		BitBoard pieceBoard = (color == Color::BLACK) ? getBoard(Piece::BLACK_QUEEN) | getBoard(Piece::BLACK_ROOK):
														getBoard(Piece::WHITE_QUEEN) | getBoard(Piece::WHITE_ROOK);
		while(pieceBoard != 0)
		{
			int mIndex = bitScanForward(pieceBoard);
			pieceBoard &= ~(1ULL << mIndex);

			Rank rank = toRank(mIndex / 8);
			File file = toFile(mIndex % 8);

			BitBoard pPos = 1ULL << mIndex;
			BitBoard horizontalMoves = ((getOccupied() - 2 * pPos) ^ reverse(reverse(getOccupied()) - 2 * reverse(pPos))) & Position::rankBoard(rank);
			BitBoard verticalMoves = (((getOccupied() & Position::fileBoard(file)) - 2 * pPos) ^ reverse(reverse((getOccupied() & Position::fileBoard(file))) - 2 * reverse(pPos))) & Position::fileBoard(file);

			BitBoard possibleMoves = (horizontalMoves | verticalMoves);

			attacked |= possibleMoves;
		}

		pieceBoard = (color == Color::BLACK) ? getBoard(Piece::BLACK_QUEEN) | getBoard(Piece::BLACK_BISHOP):
											   getBoard(Piece::WHITE_QUEEN) | getBoard(Piece::WHITE_BISHOP);
		while(pieceBoard != 0)
		{
			int mIndex = bitScanForward(pieceBoard);
			pieceBoard &= ~(1ULL << mIndex);

			Rank rank = toRank(mIndex / 8);
			File file = toFile(mIndex % 8);

			BitBoard pPos = 1ULL << mIndex;
			BitBoard diagonalMoves = (((getOccupied() & Position::diagonalBoard(rank, file)) - 2 * pPos)
				^ reverse(reverse((getOccupied() & Position::diagonalBoard(rank, file))) - 2 * reverse(pPos))) & Position::diagonalBoard(rank, file);
			BitBoard antiDagionalMoves = (((getOccupied() & Position::antiDiagonalBoard(rank, file)) - 2 * pPos)
				^ reverse(reverse((getOccupied() & Position::antiDiagonalBoard(rank, file))) - 2 * reverse(pPos))) & Position::antiDiagonalBoard(rank, file);

			BitBoard possibleMoves = (diagonalMoves | antiDagionalMoves);

			attacked |= possibleMoves;
		}

		pieceBoard = (color == Color::BLACK) ? getBoard(Piece::BLACK_KNIGHT):
											   getBoard(Piece::WHITE_KNIGHT);
		while(pieceBoard != 0)
		{
			int mIndex = bitScanForward(pieceBoard);
			pieceBoard &= ~(1ULL << mIndex);

			BitBoard knightMoves;
			if (mIndex > 18) knightMoves = Position::KNIGHT_SPAN << (mIndex - 18);
			else knightMoves = Position::KNIGHT_SPAN >> (18 - mIndex);

			File file = toFile(mIndex % 8);
			switch (file)
			{
				case File::FILE_A:
					knightMoves &= ~(Position::fileBoard(File::FILE_G) | Position::fileBoard(File::FILE_H));
					break;
				case File::FILE_B:
					knightMoves &= ~Position::fileBoard(File::FILE_H);
					break;
				case File::FILE_G:
					knightMoves &= ~Position::fileBoard(File::FILE_A);
					break;
				case File::FILE_H:
					knightMoves &= ~(Position::fileBoard(File::FILE_A) | Position::fileBoard(File::FILE_B));
					break;
			}

			attacked |= knightMoves;
		}

		pieceBoard = (color == Color::BLACK) ? getBoard(Piece::BLACK_KING):
											   getBoard(Piece::WHITE_KING);
		int mIndex = bitScanForward(pieceBoard);

		BitBoard kingMoves;
		if (mIndex > 9) kingMoves = Position::KING_SPAN << (mIndex - 9);
		else kingMoves = Position::KING_SPAN >> (9 - mIndex);

		File file = toFile(mIndex % 8);
		if (file == File::FILE_A) kingMoves &= ~Position::fileBoard(File::FILE_H);
		else if (file == File::FILE_H) kingMoves &= ~Position::fileBoard(File::FILE_A);

		attacked |= kingMoves;

		return attacked;
	}

	bool Position::isLegal(const Move& move) const
	{
		BitBoard attacked = 0ULL;
		if (sideToMove == Color::BLACK)
		{
			return (getBoard(Piece::WHITE_KING) & getAttacked(Color::BLACK)) == 0;
		}
		else
		{
			return (getBoard(Piece::BLACK_KING) & getAttacked(Color::WHITE)) == 0;
		}
	}
} //namespace Epimedes