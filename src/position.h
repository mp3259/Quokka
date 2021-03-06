#ifndef __POSITION_H__
#define __POSITION_H__

#include <string>
#include "types.h"

using namespace std;

extern Square sq120_to_64[120];
extern Square sq64_to_120[120];

extern PieceType piece_type[13];

class Position {

public:
	Piece board[120]; // Board array
	Value material[2]; // Material count for both black and white players
	MoveScore cutoff_moves[120][120]; // Array which holds moves that caused an alpha cutoff
	Piece piece_num[13]; // the number of pieces to help index the piece lists
	Piece piece_list[13][10]; // Piece lists to speed up move generation
	Byte castling_perms; // Byte which holds castling permissions for current position
	Square en_passant_target; // En-Passant target square if it exists
	Color to_move; // side to move
	int rule50; // Halfmoves since the last capture or pawn advance (50 move draw)
	int game_ply; // total ply since the start of the game
	Value mobility[2]; // mobility score for white and black
	Snapshot history_stack[MAX_GAME_MOVES]; // History stack used to undo moves
	Key pos_key; // Key of the previous position to modify

	Position();
	Position(const string fen);
	static void init();
	void print_board();
	void parse_fen(const string& fen);
	void make_move(Move m, bool save = true);
	void undo_move();
	Piece piece_at(Square s);
	Key generate_position_key();

private:
	// Helper functions
	void clear();
	void add_piece(Square s, Piece p);
	void remove_piece(Square s, Piece p);
	void handle_en_passant(Piece p, Move m);
	void parse_castling(Piece p, Move m);
	void do_castling(Move m);
	void undo_castling(Move m);
	void take_snapshot(Move m);

};

inline Piece create_piece(Color side, PieceType ptype) {
	return (6 * side + ptype);
}

extern void parse_UCI_move(Position& pos, string& str);

// Takes a file and rank and returns their 120 based square index
inline Square FR2SQ(File f, Rank r) {
	return ((f + 21) + (r * 10));
}

// Converts a 120 based square to a 64 based square index
inline Square to64(Square s) {
	return sq120_to_64[s];
}

// Converts a 64 based square to a 120 based square index
inline Square to120(Square s) {
	return sq64_to_120[s];
}

// Convert a 64 based square to a file
inline File file_of(Square s) {
	return s & 7;
}

// Convert a 64 based square to a rank
inline Rank rank_of(Square s) {
	return s >> 3;
}

// Get the color of a piece
inline Color color_of(Piece p) {
	return (p <= W_KING) ? WHITE : BLACK;
}

// Get the type of a piece
inline PieceType type_of(Piece p) {
	return piece_type[p];
}

// Set a bit to zero (for disabling castling perms)
inline void clear_bit(Byte& i, int bit) {
	i &= ~(bit);
}
#endif // !__POSITION_H__
