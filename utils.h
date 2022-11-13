#include "board.h"

const char* get_symbol_by_size_and_player(size s, player p);
void reset_color();
void change_color_by_player(player p);
void change_to_red();
void print_colored_piece(size s, player p);
void print_board_level(board game,int level);
void print_player_house(board game, player p);
void print_house_level(board game,int level);
void print_board_and_house(board game);
void print_input_error_message();
char get_valid_char(char *message,char *valid_chars);
int char_to_int(char c);
char select_play_type();
int select_position(char *message);
int select_size();
void print_separator();
void print_player_turn(player p);
void print_winner(board game);
void print_winner_by_quit();
void print_move_error_message_by_code(int code);
void print_place_error_message_by_code(int code);
bool exit_confirmation();
void play(board game, player currentPlayer);