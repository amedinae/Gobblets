#include "utils.h"

int main(int args, char **argv) {
	board game = new_game();
    play(game, PLAYER_1);
    print_winner(game);
	return 0;
}