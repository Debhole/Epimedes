#pragma once

#include <iostream>
#include <string>
#include <stdint.h>

#include "types.h"

namespace Epimedes
{
	class Position
	{
	public:
		Position();
		Position(const Position& pos);

		Position& set(const std::string& fenStr);
		Position& makeMove(const Move& move);
		Position& updateBoard();
		void clear();

		void setCastleRights(Piece side, bool should);

		std::string getFen() const;
		BitBoard getBoard(Piece piece) const;
		BitBoard getBoard(Color color) const;
		BitBoard getOccupied() const;
		BitBoard getAttacked(Color color) const;

		static BitBoard rankBoard(Rank rank);
		static BitBoard fileBoard(File file);
		static BitBoard diagonalBoard(Rank rank, File file);
		static BitBoard antiDiagonalBoard(Rank rank, File file);

		Color getSideToMove() const;
		Square getEPSquare() const;
		bool canCastle(int index) const;

		bool isLegal(const Move& move) const;

		Piece& getPiece(char token, Piece& piece) const;
		
		Piece pieceAt(Rank r, File f) const;
		Piece pieceAt(int square) const;
		char pieceChar(Piece piece) const;

		void setPiece(const Square& square, const Piece& piece);

		static const BitBoard KNIGHT_SPAN;
		static const BitBoard KING_SPAN;

		static const int INITIAL_ROOK_SQUARES[4];
		static const int CASTLED_ROOK_SQUARES[4];
	private:
		Piece board[static_cast<int>(Square::SQUARE_NUM)];

		BitBoard pieceBoards[static_cast<int>(Piece::PIECE_NUM)];

		static const BitBoard RANK_MASK[8];
		static const BitBoard FILE_MASK[8];
		static const BitBoard DIAGONAL_MASK[15];
		static const BitBoard ANTIDIAGONAL_MASK[15];

		Color sideToMove;
		Square enPassantSquare;

		bool castleRights[4];

		std::string fen;
	};

	extern std::ostream& operator<<(std::ostream& os, const Position& pos);

	inline void Position::clear()
	{
		for (Piece& p : board) p = Piece::PIECE_NULL;
		for (BitBoard& b : pieceBoards) b = 0ULL;

		sideToMove = Color::WHITE;
		for (bool& b : castleRights) b = false;
	}

	inline void Position::setCastleRights(Piece side, bool should)
	{
		switch (side)
		{
			case Piece::WHITE_KING:
				castleRights[0] = should;
				break;
			case Piece::WHITE_QUEEN:
				castleRights[1] = should;
				break;
			case Piece::BLACK_KING:
				castleRights[2] = should;
				break;
			case Piece::BLACK_QUEEN:
				castleRights[3] = should;
				break;
		}
	}

	inline std::string Position::getFen() const { return fen; }

	inline BitBoard Position::getBoard(Piece piece) const 
	{ 
		return pieceBoards[static_cast<int>(piece)]; 
	}
	inline BitBoard Position::getBoard(Color color) const 
	{ 
		int start = 6 * static_cast<int>(color);
		return pieceBoards[0 + start] | pieceBoards[1 + start]
			 | pieceBoards[2 + start] | pieceBoards[3 + start]
			 | pieceBoards[4 + start] | pieceBoards[5 + start];
	}

	inline BitBoard Position::getOccupied() const
	{
		return pieceBoards[0] | pieceBoards[1] | pieceBoards[2] | pieceBoards[3]
			 | pieceBoards[4] | pieceBoards[5] | pieceBoards[6] | pieceBoards[7]
			 | pieceBoards[8] | pieceBoards[9] | pieceBoards[10] | pieceBoards[11];
	}
	inline BitBoard Position::rankBoard(Rank rank)
	{
		return RANK_MASK[static_cast<int>(rank)];
	}
	inline BitBoard Position::fileBoard(File file)
	{
		return FILE_MASK[static_cast<int>(file)];
	}
	inline BitBoard Position::diagonalBoard(Rank rank, File file)
	{
		return DIAGONAL_MASK[static_cast<int>(rank) + static_cast<int>(file)];
	}
	inline BitBoard Position::antiDiagonalBoard(Rank rank, File file)
	{
		return ANTIDIAGONAL_MASK[static_cast<int>(rank) - static_cast<int>(file) + 7];
	}

	inline Color Position::getSideToMove() const
	{
		return sideToMove;
	}
	inline Square Position::getEPSquare() const
	{
		return enPassantSquare;
	}
	inline bool Position::canCastle(int index) const
	{
		return castleRights[index];
	}

	inline Piece& Position::getPiece(char token, Piece& piece) const
	{
		switch (token)
		{
			case 'P':
				return piece = Piece::WHITE_PAWN;
			case 'N':
				return piece = Piece::WHITE_KNIGHT;
			case 'B':
				return piece = Piece::WHITE_BISHOP;
			case 'R':
				return piece = Piece::WHITE_ROOK;
			case 'Q':
				return piece = Piece::WHITE_QUEEN;
			case 'K':
				return piece = Piece::WHITE_KING;
			case 'p':
				return piece = Piece::BLACK_PAWN;
			case 'n':
				return piece = Piece::BLACK_KNIGHT;
			case 'b':
				return piece = Piece::BLACK_BISHOP;
			case 'r':
				return piece = Piece::BLACK_ROOK;
			case 'q':
				return piece = Piece::BLACK_QUEEN;
			case 'k':
				return piece = Piece::BLACK_KING;
		}
		return piece = Piece::PIECE_NULL;
	}

	inline Piece Position::pieceAt(Rank r, File f) const
	{
		return board[static_cast<int>(r) * 8 + static_cast<int>(f)];
	}
	inline Piece Position::pieceAt(int square) const
	{
		return board[static_cast<int>(square)];
	}

	inline char Position::pieceChar(Piece piece) const
	{
		switch (piece)
		{
			case Piece::WHITE_PAWN:
				return 'P';
			case Piece::WHITE_KNIGHT:
				return 'N';
			case Piece::WHITE_BISHOP:
				return 'B';
			case Piece::WHITE_ROOK:
				return 'R';
			case Piece::WHITE_QUEEN:
				return 'Q';
			case Piece::WHITE_KING:
				return 'K';
			case Piece::BLACK_PAWN:
				return 'p';
			case Piece::BLACK_KNIGHT:
				return 'n';
			case Piece::BLACK_BISHOP:
				return 'b';
			case Piece::BLACK_ROOK:
				return 'r';
			case Piece::BLACK_QUEEN:
				return 'q';
			case Piece::BLACK_KING:
				return 'k';
			default:
				return ' ';
		}
	}

	inline void Position::setPiece(const Square& square, const Piece& piece)
	{
		board[static_cast<int>(square)] = piece;
		if (piece != Piece::PIECE_NULL)
		{
			pieceBoards[static_cast<int>(piece)] |= 1ULL << static_cast<int>(square);
		}
	}

} //namespace Epimedes